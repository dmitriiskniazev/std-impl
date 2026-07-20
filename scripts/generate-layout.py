#!/usr/bin/env python3
"""Generate expected/optional module layout (interface + impl/<fn> + detail).

Clang rejects out-of-line definitions of constrained member templates on partial
specializations (optional<T&>, expected<void, E>). Those members are defined
in-class in ref_/interface.cppm and void_/interface.cppm — do not regenerate
them as separate impl/*.cppm files.
"""
from __future__ import annotations

import os
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / "src"


def write(path: Path, content: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content, encoding="utf-8")


def expected_detail_files() -> list[Path]:
    files: list[Path] = []

    write(
        SRC / "expected/detail/require_value.cppm",
        """export module std_impl.expected:expected.detail.require_value;
import std;

import :bad_expected_access;

namespace std_impl::expected::detail {
    constexpr auto require_value(bool has_value) -> void {
        if (not has_value) {
            throw bad_expected_access::bad_expected_access{};
        }
    }
}  // namespace std_impl::expected::detail
""",
    )
    files.append(SRC / "expected/detail/require_value.cppm")

    write(
        SRC / "expected/detail/require_error.cppm",
        """export module std_impl.expected:expected.detail.require_error;
import std;

import :bad_expected_access;

namespace std_impl::expected::detail {
    constexpr auto require_error(bool has_value) -> void {
        if (has_value) {
            throw bad_expected_access::bad_expected_access{};
        }
    }
}  // namespace std_impl::expected::detail
""",
    )
    files.append(SRC / "expected/detail/require_error.cppm")

    write(
        SRC / "expected/detail/make_error_result.cppm",
        """export module std_impl.expected:expected.detail.make_error_result;
import std;

import :unexpected;

namespace std_impl::expected::detail {
    template <typename Result, typename Error, typename Err>
    constexpr auto make_error_result(Err&& error) -> Result {
        unexpected::unexpected<Error> unexp{std::forward<Err>(error)};
        return Result{std::move(unexp)};
    }
}  // namespace std_impl::expected::detail
""",
    )
    files.append(SRC / "expected/detail/make_error_result.cppm")

    write(
        SRC / "expected/detail/construct_from_unexpected.cppm",
        """export module std_impl.expected:expected.detail.construct_from_unexpected;
import std;

import :unexpected;

namespace std_impl::expected::detail {
    template <typename Expected, typename G>
        requires std::convertible_to<const G&, typename Expected::error_type>
    constexpr auto construct_from_unexpected(Expected& self, const unexpected::unexpected<G>& unexp) -> void {
        self.storage_.emplace_error(unexp.error());
    }

    template <typename Expected, typename G>
        requires std::convertible_to<G, typename Expected::error_type>
    constexpr auto construct_from_unexpected(Expected& self, unexpected::unexpected<G>&& unexp) -> void {
        self.storage_.emplace_error(std::move(unexp.error()));
    }
}  // namespace std_impl::expected::detail
""",
    )
    files.append(SRC / "expected/detail/construct_from_unexpected.cppm")

    write(
        SRC / "expected/detail/copy_assign.cppm",
        """export module std_impl.expected:expected.detail.copy_assign;
import std;

namespace std_impl::expected::detail {
    template <typename Expected>
    constexpr auto copy_assign(Expected& self, const Expected& other) -> Expected& {
        Expected copy{other};
        self.swap(copy);
        return self;
    }
}  // namespace std_impl::expected::detail
""",
    )
    files.append(SRC / "expected/detail/copy_assign.cppm")

    write(
        SRC / "expected/detail/move_assign.cppm",
        """export module std_impl.expected:expected.detail.move_assign;
import std;

namespace std_impl::expected::detail {
    template <typename Expected>
    constexpr auto move_assign(Expected& self, Expected&& other) noexcept -> Expected& {
        Expected moved{std::move(other)};
        self.swap(moved);
        return self;
    }
}  // namespace std_impl::expected::detail
""",
    )
    files.append(SRC / "expected/detail/move_assign.cppm")

    write(
        SRC / "expected/detail/swap_engaged.cppm",
        """export module std_impl.expected:expected.detail.swap_engaged;
import std;

namespace std_impl::expected::detail {
    template <typename Expected>
    constexpr auto swap_engaged(Expected& lhs, Expected& rhs) noexcept -> void {
        if (lhs.has_value() and rhs.has_value()) {
            using std::swap;
            swap(lhs.storage_.value(), rhs.storage_.value());
        } else if (lhs.has_value()) {
            typename Expected::error_type err = std::move(rhs.storage_.error());
            rhs.storage_.emplace_value(std::move(lhs.storage_.value()));
            lhs.storage_.emplace_error(std::move(err));
        } else if (rhs.has_value()) {
            rhs.swap(lhs);
        } else {
            using std::swap;
            swap(lhs.storage_.error(), rhs.storage_.error());
        }
    }
}  // namespace std_impl::expected::detail
""",
    )
    files.append(SRC / "expected/detail/swap_engaged.cppm")

    return files


def expected_storage_files() -> list[Path]:
    files: list[Path] = []

    write(
        SRC / "expected/detail/storage/traits.cppm",
        """export module std_impl.expected:expected.detail.storage.traits;
import std;

namespace std_impl::expected::detail::storage {
    template <typename T> inline constexpr bool nothrow_default_constructible = std::is_nothrow_default_constructible_v<T>;

    template <typename T, typename E> inline constexpr bool nothrow_move_constructible = std::is_nothrow_move_constructible_v<T> and std::is_nothrow_move_constructible_v<E>;

    template <typename T, typename E> inline constexpr bool nothrow_move_assignable = nothrow_move_constructible<T, E> and std::is_nothrow_move_assignable_v<T> and std::is_nothrow_move_assignable_v<E>;
}  // namespace std_impl::expected::detail::storage
""",
    )
    files.append(SRC / "expected/detail/storage/traits.cppm")

    write(
        SRC / "expected/detail/storage/interface.cppm",
        """export module std_impl.expected:expected.detail.storage.interface;
import std;

import :expected.detail.storage.traits;

namespace std_impl::expected::detail::storage {
    export template <typename T, typename E> struct storage {
        using value_type = T;
        using error_type = E;

        union {
            alignas(value_type) std::byte val_data_[sizeof(value_type)];
            alignas(error_type) std::byte err_data_[sizeof(error_type)];
        } data_{};
        [[no_unique_address]] bool has_value_{false};

        constexpr storage() noexcept(nothrow_default_constructible<value_type>)
            requires std::default_initializable<value_type>;
        storage(const storage& other)
            requires std::copy_constructible<value_type> and std::copy_constructible<error_type>;
        storage(storage&& other) noexcept(nothrow_move_constructible<value_type, error_type>)
            requires std::move_constructible<value_type> and std::move_constructible<error_type>;
        auto operator=(const storage& other) -> storage&
            requires std::copy_constructible<value_type> and std::copy_constructible<error_type>;
        auto operator=(storage&& other) noexcept(nothrow_move_assignable<value_type, error_type>)
            requires std::move_constructible<value_type> and std::move_constructible<error_type>;
        ~storage();

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool;
        [[nodiscard]] constexpr auto value(this auto&& self) noexcept -> decltype(auto);
        [[nodiscard]] constexpr auto error(this auto&& self) noexcept -> decltype(auto);
        constexpr auto emplace_value(auto&&... args) -> value_type&;
        constexpr auto emplace_error(auto&&... args) -> error_type&;

    private:
        constexpr auto construct_value(auto&&... args) -> void;
        constexpr auto construct_error(auto&&... args) -> void;
        constexpr auto destroy_active() noexcept -> void;
        constexpr auto copy_from(const storage& other) -> void;
        constexpr auto move_from(storage& other) -> void;
        constexpr auto assign_from(const storage& other) -> void;
        constexpr auto assign_from(storage&& other) -> void;
        constexpr auto leave_moved_from() -> void;
    };
}  // namespace std_impl::expected::detail::storage
""",
    )
    files.append(SRC / "expected/detail/storage/interface.cppm")

    write(
        SRC / "expected/detail/storage/impl.cppm",
        """export module std_impl.expected:expected.detail.storage.impl;
import std;

import :expected.detail.storage.interface;

namespace std_impl::expected::detail::storage {
    template <typename T, typename E>
    constexpr storage<T, E>::storage() noexcept(nothrow_default_constructible<value_type>)
        requires std::default_initializable<value_type> {
        construct_value();
        has_value_ = true;
    }

    template <typename T, typename E>
    storage<T, E>::storage(const storage& other)
        requires std::copy_constructible<value_type> and std::copy_constructible<error_type> {
        copy_from(other);
    }

    template <typename T, typename E>
    storage<T, E>::storage(storage&& other) noexcept(nothrow_move_constructible<value_type, error_type>)
        requires std::move_constructible<value_type> and std::move_constructible<error_type> {
        move_from(other);
        other.leave_moved_from();
    }

    template <typename T, typename E>
    auto storage<T, E>::operator=(const storage& other) -> storage&
        requires std::copy_constructible<value_type> and std::copy_constructible<error_type> {
        if (this != &other) {
            assign_from(other);
        }
        return *this;
    }

    template <typename T, typename E>
    auto storage<T, E>::operator=(storage&& other) noexcept(nothrow_move_assignable<value_type, error_type>)
        requires std::move_constructible<value_type> and std::move_constructible<error_type> {
        if (this != &other) {
            assign_from(std::move(other));
            other.leave_moved_from();
        }
        return *this;
    }

    template <typename T, typename E>
    storage<T, E>::~storage() {
        destroy_active();
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::has_value() const noexcept -> bool {
        return has_value_;
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::value(this auto&& self) noexcept -> decltype(auto) {
        using ptr = std::conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>, const value_type*, value_type*>;
        return *std::launder(reinterpret_cast<ptr>(self.data_.val_data_));
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::error(this auto&& self) noexcept -> decltype(auto) {
        using ptr = std::conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>, const error_type*, error_type*>;
        return *std::launder(reinterpret_cast<ptr>(self.data_.err_data_));
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::emplace_value(auto&&... args) -> value_type& {
        destroy_active();
        construct_value(std::forward<decltype(args)>(args)...);
        has_value_ = true;
        return value();
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::emplace_error(auto&&... args) -> error_type& {
        destroy_active();
        construct_error(std::forward<decltype(args)>(args)...);
        has_value_ = false;
        return error();
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::construct_value(auto&&... args) -> void {
        std::construct_at(reinterpret_cast<value_type*>(data_.val_data_), std::forward<decltype(args)>(args)...);
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::construct_error(auto&&... args) -> void {
        std::construct_at(reinterpret_cast<error_type*>(data_.err_data_), std::forward<decltype(args)>(args)...);
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::destroy_active() noexcept -> void {
        if (has_value_) {
            std::destroy_at(reinterpret_cast<value_type*>(data_.val_data_));
        } else {
            std::destroy_at(reinterpret_cast<error_type*>(data_.err_data_));
        }
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::copy_from(const storage& other) -> void {
        if (other.has_value_) {
            construct_value(other.value());
        } else {
            construct_error(other.error());
        }
        has_value_ = other.has_value_;
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::move_from(storage& other) -> void {
        if (other.has_value_) {
            construct_value(std::move(other.value()));
        } else {
            construct_error(std::move(other.error()));
        }
        has_value_ = other.has_value_;
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::assign_from(const storage& other) -> void {
        if (other.has_value_) {
            if (has_value_) {
                value() = other.value();
            } else {
                destroy_active();
                construct_value(other.value());
                has_value_ = true;
            }
        } else if (has_value_) {
            destroy_active();
            construct_error(other.error());
            has_value_ = false;
        } else {
            error() = other.error();
        }
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::assign_from(storage&& other) -> void {
        if (other.has_value_) {
            if (has_value_) {
                value() = std::move(other.value());
            } else {
                destroy_active();
                construct_value(std::move(other.value()));
                has_value_ = true;
            }
        } else if (has_value_) {
            destroy_active();
            construct_error(std::move(other.error()));
            has_value_ = false;
        } else {
            error() = std::move(other.error());
        }
    }

    template <typename T, typename E>
    constexpr auto storage<T, E>::leave_moved_from() -> void {
        if (has_value_) {
            std::destroy_at(reinterpret_cast<value_type*>(data_.val_data_));
        } else {
            std::destroy_at(reinterpret_cast<error_type*>(data_.err_data_));
        }
        if constexpr (std::is_default_constructible_v<value_type>) {
            construct_value();
            has_value_ = true;
        } else if constexpr (std::is_default_constructible_v<error_type>) {
            construct_error();
            has_value_ = false;
        }
    }
}  // namespace std_impl::expected::detail::storage
""",
    )
    files.append(SRC / "expected/detail/storage/impl.cppm")

    write(
        SRC / "expected/detail/storage/void_/interface.cppm",
        """export module std_impl.expected:expected.detail.storage.void_.interface;
import std;

namespace std_impl::expected::detail::storage {
    export template <typename E> struct storage<void, E> {
        using value_type = void;
        using error_type = E;

        alignas(error_type) std::byte err_data_[sizeof(error_type)]{};
        [[no_unique_address]] bool has_value_{true};

        constexpr storage() noexcept = default;
        storage(const storage& other);
        storage(storage&& other) noexcept(std::is_nothrow_move_constructible_v<error_type>);
        ~storage();

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool;
        [[nodiscard]] constexpr auto has_error() const noexcept -> bool;
        [[nodiscard]] constexpr auto error(this auto&& self) noexcept -> decltype(auto);
        constexpr auto emplace_error(auto&&... args) -> error_type&;
        constexpr auto swap(storage& other) noexcept(std::is_nothrow_move_assignable_v<error_type>) -> void;

    private:
        constexpr auto destroy_error() noexcept -> void;
        constexpr auto copy_from(const storage& other) -> void;
        constexpr auto move_from(storage& other) -> void;
    };
}  // namespace std_impl::expected::detail::storage
""",
    )
    files.append(SRC / "expected/detail/storage/void_/interface.cppm")

    write(
        SRC / "expected/detail/storage/void_/impl.cppm",
        """export module std_impl.expected:expected.detail.storage.void_.impl;
import std;

import :expected.detail.storage.void_.interface;

namespace std_impl::expected::detail::storage {
    template <typename E>
    storage<void, E>::storage(const storage& other) {
        copy_from(other);
    }

    template <typename E>
    storage<void, E>::storage(storage&& other) noexcept(std::is_nothrow_move_constructible_v<error_type>) {
        move_from(other);
    }

    template <typename E>
    storage<void, E>::~storage() {
        destroy_error();
    }

    template <typename E>
    constexpr auto storage<void, E>::has_value() const noexcept -> bool {
        return has_value_;
    }

    template <typename E>
    constexpr auto storage<void, E>::has_error() const noexcept -> bool {
        return not has_value_;
    }

    template <typename E>
    constexpr auto storage<void, E>::error(this auto&& self) noexcept -> decltype(auto) {
        using ptr = std::conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>, const error_type*, error_type*>;
        return *std::launder(reinterpret_cast<ptr>(self.err_data_));
    }

    template <typename E>
    constexpr auto storage<void, E>::emplace_error(auto&&... args) -> error_type& {
        destroy_error();
        std::construct_at(reinterpret_cast<error_type*>(err_data_), std::forward<decltype(args)>(args)...);
        has_value_ = false;
        return error();
    }

    template <typename E>
    constexpr auto storage<void, E>::destroy_error() noexcept -> void {
        if (not has_value_) {
            std::destroy_at(reinterpret_cast<error_type*>(err_data_));
        }
    }

    template <typename E>
    constexpr auto storage<void, E>::swap(storage& other) noexcept(std::is_nothrow_move_assignable_v<error_type>) -> void {
        if (has_value_ and other.has_value_) {
            return;
        }
        if (has_value_) {
            std::construct_at(reinterpret_cast<error_type*>(err_data_), std::move(other.error()));
            other.destroy_error();
            other.has_value_ = true;
            destroy_error();
            has_value_ = false;
        } else if (other.has_value_) {
            other.swap(*this);
        } else {
            using std::swap;
            swap(error(), other.error());
        }
    }

    template <typename E>
    constexpr auto storage<void, E>::copy_from(const storage& other) -> void {
        has_value_ = other.has_value_;
        if (not has_value_) {
            std::construct_at(reinterpret_cast<error_type*>(err_data_), other.error());
        }
    }

    template <typename E>
    constexpr auto storage<void, E>::move_from(storage& other) -> void {
        has_value_ = other.has_value_;
        if (not has_value_) {
            std::construct_at(reinterpret_cast<error_type*>(err_data_), std::move(other.error()));
            other.destroy_error();
            other.has_value_ = true;
        }
    }
}  // namespace std_impl::expected::detail::storage
""",
    )
    files.append(SRC / "expected/detail/storage/void_/impl.cppm")

    write(
        SRC / "expected/detail/storage.cppm",
        """export module std_impl.expected:expected.detail.storage;

export import :expected.detail.storage.traits;
export import :expected.detail.storage.interface;
export import :expected.detail.storage.impl;
export import :expected.detail.storage.void_.interface;
export import :expected.detail.storage.void_.impl;
""",
    )
    files.append(SRC / "expected/detail/storage.cppm")

    return files


def expected_interface() -> None:
    write(
        SRC / "expected/interface.cppm",
        """export module std_impl.expected:expected.interface;
import std;

import :bad_expected_access;
import :detail.concepts;
import :detail.storage;
import :unexpected;

namespace std_impl::expected {
    export template <typename T, typename E>
        requires distinct<T, E>
    class expected {
    public:
        using value_type = T;
        using error_type = E;
        using unexpected_type = unexpected::unexpected<error_type>;

        constexpr expected() noexcept(detail::storage::nothrow_default_constructible<value_type>)
            requires std::default_initializable<value_type> = default;
        expected(const expected&) = default;
        expected(expected&&) = default;
        auto operator=(const expected&) -> expected& = default;
        auto operator=(expected&&) -> expected& = default;
        ~expected() = default;

        constexpr expected(const value_type& value);
        constexpr expected(value_type&& value);
        constexpr expected(convertible_value<expected, value_type, unexpect_t> auto&& value);

        template <typename G>
            requires std::convertible_to<const G&, error_type>
        constexpr expected(const unexpected::unexpected<G>& unexp);

        template <typename G>
            requires std::convertible_to<G, error_type>
        constexpr expected(unexpected::unexpected<G>&& unexp);

        constexpr expected(std::in_place_t, auto&&... args)
            requires std::constructible_from<value_type, decltype(args)...>;

        constexpr expected(unexpect_t, auto&&... args)
            requires std::constructible_from<error_type, decltype(args)...>;

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool;
        [[nodiscard]] constexpr auto has_error() const noexcept -> bool;
        [[nodiscard]] constexpr explicit operator bool() const noexcept;

        [[nodiscard]] constexpr auto value(this auto&& self) -> decltype(auto);
        [[nodiscard]] constexpr auto error(this auto&& self) -> decltype(auto);
        [[nodiscard]] constexpr auto operator*(this auto&& self) -> decltype(auto);
        [[nodiscard]] constexpr auto operator->(this auto& self) -> decltype(auto);
        [[nodiscard]] constexpr auto value_or(this auto&& self, auto&& default_value) -> auto;
        [[nodiscard]] constexpr auto error_or(this auto&& self, auto&& default_error) -> auto;

        [[nodiscard]] constexpr auto and_then(this auto&& self, auto&& func) -> auto;
        [[nodiscard]] constexpr auto transform(this auto&& self, auto&& func) -> auto;
        [[nodiscard]] constexpr auto transform_error(this auto&& self, auto&& func) -> auto;
        [[nodiscard]] constexpr auto or_else(this const auto& self, auto&& func) -> auto;

        constexpr auto swap(expected& other) noexcept(detail::storage::nothrow_move_assignable<value_type, error_type>) -> void;

    private:
        detail::storage::storage<value_type, error_type> storage_;
    };

    export template <typename T, typename E>
    auto swap(expected<T, E>& lhs, expected<T, E>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void;
}  // namespace std_impl::expected
""",
    )


def expected_void_interface() -> None:
    write(
        SRC / "expected/void_/interface.cppm",
        """export module std_impl.expected:expected.void_.interface;
import std;

import :bad_expected_access;
import :detail.concepts;
import :detail.storage;
import :expected.interface;
import :unexpected;

namespace std_impl::expected::detail {
    template <typename Expected, typename G>
        requires std::convertible_to<const G&, typename Expected::error_type>
    constexpr auto construct_from_unexpected(Expected& self, const unexpected::unexpected<G>& unexp) -> void;

    template <typename Expected, typename G>
        requires std::convertible_to<G, typename Expected::error_type>
    constexpr auto construct_from_unexpected(Expected& self, unexpected::unexpected<G>&& unexp) -> void;
}  // namespace std_impl::expected::detail

namespace std_impl::expected {
    // Constrained member templates of this partial specialization are defined
    // in-class: Clang rejects matching out-of-line definitions with requires.
    export template <typename E> class expected<void, E> {
    public:
        using value_type = void;
        using error_type = E;

        constexpr expected() noexcept = default;

        expected(const expected& other);
        expected(expected&& other) noexcept(std::is_nothrow_move_constructible_v<error_type>);
        auto operator=(const expected& other) -> expected&;
        auto operator=(expected&& other) noexcept(std::is_nothrow_move_assignable_v<error_type>) -> expected&;
        ~expected() = default;

        template <typename G>
            requires std::convertible_to<const G&, error_type>
        constexpr expected(const unexpected::unexpected<G>& unexp) {
            storage_.emplace_error(unexp.error());
        }

        template <typename G>
            requires std::convertible_to<G, error_type>
        constexpr expected(unexpected::unexpected<G>&& unexp) {
            storage_.emplace_error(std::move(unexp.error()));
        }

        constexpr explicit expected(unexpect_t, auto&&... args)
            requires std::constructible_from<error_type, decltype(args)...>;

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool;
        [[nodiscard]] constexpr auto has_error() const noexcept -> bool;
        [[nodiscard]] constexpr explicit operator bool() const noexcept;

        constexpr auto value() const -> void;
        [[nodiscard]] constexpr auto error(this auto&& self) -> decltype(auto);

        [[nodiscard]] constexpr auto and_then(this auto&& self, auto&& func) -> auto;
        [[nodiscard]] constexpr auto transform(this auto&& self, auto&& func) -> auto;
        [[nodiscard]] constexpr auto transform_error(this auto&& self, auto&& func) -> auto;
        [[nodiscard]] constexpr auto or_else(this const auto& self, auto&& func) -> auto;

        constexpr auto swap(expected& other) noexcept(std::is_nothrow_move_assignable_v<error_type>) -> void;

    private:
        detail::storage::storage<value_type, error_type> storage_;

        template <typename Expected, typename G>
            requires std::convertible_to<const G&, typename Expected::error_type>
        friend constexpr auto detail::construct_from_unexpected(Expected& self, const unexpected::unexpected<G>& unexp) -> void;

        template <typename Expected, typename G>
            requires std::convertible_to<G, typename Expected::error_type>
        friend constexpr auto detail::construct_from_unexpected(Expected& self, unexpected::unexpected<G>&& unexp) -> void;
    };

    export template <typename E> auto swap(expected<void, E>& lhs, expected<void, E>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void;
}  // namespace std_impl::expected
""",
    )


def _impl_header(module: str, imports: list[str]) -> str:
    lines = [f"export module std_impl.expected:{module};", "import std;"]
    for imp in imports:
        lines.append(f"import {imp};")
    lines.append("")
    lines.append("namespace std_impl::expected {")
    return "\n".join(lines)


def _impl_footer() -> str:
    return "}  // namespace std_impl::expected\n"


def expected_impl_files() -> list[Path]:
    files: list[Path] = []
    base = "expected.impl"
    iface = ":expected.interface"
    detail_req_val = ":expected.detail.require_value"
    detail_req_err = ":expected.detail.require_error"
    detail_make = ":expected.detail.make_error_result"
    detail_unexp = ":expected.detail.construct_from_unexpected"
    detail_swap = ":expected.detail.swap_engaged"

    impls = {
        "ctor.value": (
            [iface],
            """
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr expected<T, E>::expected(const value_type& value) {
        storage_.emplace_value(value);
    }

    template <typename T, typename E>
        requires distinct<T, E>
    constexpr expected<T, E>::expected(value_type&& value) {
        storage_.emplace_value(std::move(value));
    }

    template <typename T, typename E>
        requires distinct<T, E>
    constexpr expected<T, E>::expected(convertible_value<expected, value_type, unexpect_t> auto&& value) {
        storage_.emplace_value(std::forward<decltype(value)>(value));
    }
""",
        ),
        "ctor.in_place": (
            [iface],
            """
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr expected<T, E>::expected(std::in_place_t, auto&&... args)
        requires std::constructible_from<value_type, decltype(args)...> {
        storage_.emplace_value(std::forward<decltype(args)>(args)...);
    }
""",
        ),
        "ctor.unexpected": (
            [iface, detail_unexp],
            """
    template <typename T, typename E>
        requires distinct<T, E>
    template <typename G>
        requires std::convertible_to<const G&, typename expected<T, E>::error_type>
    constexpr expected<T, E>::expected(const unexpected::unexpected<G>& unexp) {
        detail::construct_from_unexpected(*this, unexp);
    }

    template <typename T, typename E>
        requires distinct<T, E>
    template <typename G>
        requires std::convertible_to<G, typename expected<T, E>::error_type>
    constexpr expected<T, E>::expected(unexpected::unexpected<G>&& unexp) {
        detail::construct_from_unexpected(*this, std::move(unexp));
    }
""",
        ),
        "ctor.unexpect": (
            [iface],
            """
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr expected<T, E>::expected(unexpect_t, auto&&... args)
        requires std::constructible_from<error_type, decltype(args)...> {
        storage_.emplace_error(std::forward<decltype(args)>(args)...);
    }
""",
        ),
        "has_value": (
            [iface],
            """
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::has_value() const noexcept -> bool {
        return storage_.has_value();
    }
""",
        ),
        "has_error": (
            [iface],
            """
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::has_error() const noexcept -> bool {
        return not has_value();
    }
""",
        ),
        "operator_bool": (
            [iface],
            """
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr expected<T, E>::operator bool() const noexcept {
        return has_value();
    }
""",
        ),
        "value": (
            [iface, detail_req_val],
            """
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::value(this auto&& self) -> decltype(auto) {
        detail::require_value(self.has_value());
        return std::forward_like<decltype(self)>(self.storage_.value());
    }
""",
        ),
        "error": (
            [iface, detail_req_err],
            """
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::error(this auto&& self) -> decltype(auto) {
        detail::require_error(self.has_value());
        return std::forward_like<decltype(self)>(self.storage_.error());
    }
""",
        ),
        "operator_star": (
            [iface],
            """
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::operator*(this auto&& self) -> decltype(auto) {
        return self.value();
    }
""",
        ),
        "operator_arrow": (
            [iface],
            """
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::operator->(this auto& self) -> decltype(auto) {
        return std::addressof(self.value());
    }
""",
        ),
        "value_or": (
            [iface],
            """
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::value_or(this auto&& self, auto&& default_value) -> auto {
        if (self.has_value()) {
            return std::forward_like<decltype(self)>(self.storage_.value());
        }
        return static_cast<value_type>(std::forward<decltype(default_value)>(default_value));
    }
""",
        ),
        "error_or": (
            [iface],
            """
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::error_or(this auto&& self, auto&& default_error) -> auto {
        if (self.has_value()) {
            return static_cast<error_type>(std::forward<decltype(default_error)>(default_error));
        }
        return std::forward_like<decltype(self)>(self.storage_.error());
    }
""",
        ),
        "and_then": (
            [iface, detail_make],
            """
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::and_then(this auto&& self, auto&& func) -> auto {
        using result = std::invoke_result_t<decltype(func), decltype(self.value())>;

        if (self.has_value()) {
            return std::invoke(std::forward<decltype(func)>(func), self.value());
        }
        return detail::make_error_result<result, error_type>(self.error());
    }
""",
        ),
        "transform": (
            [iface, detail_make],
            """
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::transform(this auto&& self, auto&& func) -> auto {
        using U = std::remove_cvref_t<std::invoke_result_t<decltype(func), decltype(self.value())>>;

        if (self.has_value()) {
            const auto transformed = std::invoke(std::forward<decltype(func)>(func), self.value());
            return expected<U, error_type>{transformed};
        }
        return detail::make_error_result<expected<U, error_type>, error_type>(self.error());
    }
""",
        ),
        "transform_error": (
            [iface, detail_make],
            """
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::transform_error(this auto&& self, auto&& func) -> auto {
        using G = std::remove_cvref_t<std::invoke_result_t<decltype(func), decltype(self.error())>>;

        if (self.has_value()) {
            return expected<value_type, G>{self.value()};
        }

        auto mapped = std::invoke(std::forward<decltype(func)>(func), self.error());
        return detail::make_error_result<expected<value_type, G>, G>(std::move(mapped));
    }
""",
        ),
        "or_else": (
            [iface],
            """
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::or_else(this const auto& self, auto&& func) -> auto {
        if (self.has_value()) {
            return self;
        }
        return std::invoke(std::forward<decltype(func)>(func));
    }
""",
        ),
        "swap": (
            [iface, detail_swap],
            """
    template <typename T, typename E>
        requires distinct<T, E>
    constexpr auto expected<T, E>::swap(expected& other) noexcept(detail::storage::nothrow_move_assignable<value_type, error_type>) -> void {
        detail::swap_engaged(*this, other);
    }

    template <typename T, typename E>
    auto swap(expected<T, E>& lhs, expected<T, E>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void {
        lhs.swap(rhs);
    }
""",
        ),
    }

    for name, (imports, body) in impls.items():
        path = SRC / "expected/impl" / f"{name}.cppm"
        content = _impl_header(f"{base}.{name}", imports) + body + "\n" + _impl_footer()
        write(path, content)
        files.append(path)

    return files


def expected_void_impl_files() -> list[Path]:
    files: list[Path] = []
    base = "expected.void_.impl"
    iface = ":expected.void_.interface"
    detail_req_val = ":expected.detail.require_value"
    detail_req_err = ":expected.detail.require_error"
    detail_make = ":expected.detail.make_error_result"
    detail_unexp = ":expected.detail.construct_from_unexpected"
    detail_copy = ":expected.detail.copy_assign"
    detail_move = ":expected.detail.move_assign"

    impls = {
        "ctor.copy": (
            [iface],
            """
    template <typename E>
    expected<void, E>::expected(const expected& other) :
        storage_{other.storage_} {}
""",
        ),
        "ctor.move": (
            [iface],
            """
    template <typename E>
    expected<void, E>::expected(expected&& other) noexcept(std::is_nothrow_move_constructible_v<error_type>) :
        storage_{std::move(other.storage_)} {}
""",
        ),
        "assign.copy": (
            [iface, detail_copy],
            """
    template <typename E>
    auto expected<void, E>::operator=(const expected& other) -> expected& {
        return detail::copy_assign(*this, other);
    }
""",
        ),
        "assign.move": (
            [iface, detail_move],
            """
    template <typename E>
    auto expected<void, E>::operator=(expected&& other) noexcept(std::is_nothrow_move_assignable_v<error_type>) -> expected& {
        return detail::move_assign(*this, std::move(other));
    }
""",
        ),
        "ctor.unexpect": (
            [iface],
            """
    template <typename E>
    constexpr expected<void, E>::expected(unexpect_t, auto&&... args)
        requires std::constructible_from<error_type, decltype(args)...> {
        storage_.emplace_error(std::forward<decltype(args)>(args)...);
    }
""",
        ),
        "has_value": (
            [iface],
            """
    template <typename E>
    constexpr auto expected<void, E>::has_value() const noexcept -> bool {
        return storage_.has_value();
    }
""",
        ),
        "has_error": (
            [iface],
            """
    template <typename E>
    constexpr auto expected<void, E>::has_error() const noexcept -> bool {
        return storage_.has_error();
    }
""",
        ),
        "operator_bool": (
            [iface],
            """
    template <typename E>
    constexpr expected<void, E>::operator bool() const noexcept {
        return storage_.has_value();
    }
""",
        ),
        "value": (
            [iface, detail_req_val],
            """
    template <typename E>
    constexpr auto expected<void, E>::value() const -> void {
        detail::require_value(storage_.has_value());
    }
""",
        ),
        "error": (
            [iface, detail_req_err],
            """
    template <typename E>
    constexpr auto expected<void, E>::error(this auto&& self) -> decltype(auto) {
        detail::require_error(self.storage_.has_value());
        return std::forward_like<decltype(self)>(self.storage_.error());
    }
""",
        ),
        "and_then": (
            [iface, detail_make],
            """
    template <typename E>
    constexpr auto expected<void, E>::and_then(this auto&& self, auto&& func) -> auto {
        using result = std::invoke_result_t<decltype(func)>;

        if (self.has_value()) {
            return std::invoke(std::forward<decltype(func)>(func));
        }
        return detail::make_error_result<result, error_type>(self.error());
    }
""",
        ),
        "transform": (
            [iface, detail_make],
            """
    template <typename E>
    constexpr auto expected<void, E>::transform(this auto&& self, auto&& func) -> auto {
        using U = std::remove_cvref_t<std::invoke_result_t<decltype(func)>>;

        if (self.has_value()) {
            const auto transformed = std::invoke(std::forward<decltype(func)>(func));
            return expected<U, error_type>{transformed};
        }
        return detail::make_error_result<expected<U, error_type>, error_type>(self.error());
    }
""",
        ),
        "transform_error": (
            [iface, detail_make],
            """
    template <typename E>
    constexpr auto expected<void, E>::transform_error(this auto&& self, auto&& func) -> auto {
        using G = std::remove_cvref_t<std::invoke_result_t<decltype(func), decltype(self.error())>>;

        if (self.has_value()) {
            return expected<void, G>{};
        }

        auto mapped = std::invoke(std::forward<decltype(func)>(func), self.error());
        return detail::make_error_result<expected<void, G>, G>(std::move(mapped));
    }
""",
        ),
        "or_else": (
            [iface],
            """
    template <typename E>
    constexpr auto expected<void, E>::or_else(this const auto& self, auto&& func) -> auto {
        if (self.has_value()) {
            return self;
        }
        return std::invoke(std::forward<decltype(func)>(func));
    }
""",
        ),
        "swap": (
            [iface],
            """
    template <typename E>
    constexpr auto expected<void, E>::swap(expected& other) noexcept(std::is_nothrow_move_assignable_v<error_type>) -> void {
        storage_.swap(other.storage_);
    }

    template <typename E>
    auto swap(expected<void, E>& lhs, expected<void, E>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void {
        lhs.swap(rhs);
    }
""",
        ),
    }

    for name, (imports, body) in impls.items():
        path = SRC / "expected/void_/impl" / f"{name}.cppm"
        content = _impl_header(f"{base}.{name}", imports) + body + "\n" + _impl_footer()
        write(path, content)
        files.append(path)

    return files


def expected_facades(all_impl: list[Path], void_impl: list[Path]) -> None:
    primary_impls = sorted({p.stem for p in all_impl})
    void_impls = sorted({p.stem for p in void_impl})

    primary_imports = "\n".join(f"import :expected.impl.{n};" for n in primary_impls)
    void_imports = "\n".join(f"import :expected.void_.impl.{n};" for n in void_impls)

    write(
        SRC / "expected/expected.cppm",
        f"""export module std_impl.expected:expected;

export import :expected.interface;
{primary_imports}
""",
    )

    write(
        SRC / "expected/void_.cppm",
        f"""export module std_impl.expected:void_;

export import :expected.void_.interface;
{void_imports}
""",
    )

    write(
        SRC / "expected.cppm",
        """export module std_impl.expected;

export import :detail.concepts;
export import :expected;
export import :void_;
export import :unexpected;
export import :bad_expected_access;
""",
    )


def optional_detail_and_storage() -> list[Path]:
    files: list[Path] = []

    write(
        SRC / "optional/detail/require_value.cppm",
        """export module std_impl.optional:optional.detail.require_value;
import std;

import :bad_optional_access;

namespace std_impl::optional::detail {
    constexpr auto require_value(bool has_value) -> void {
        if (not has_value) {
            throw bad_optional_access::bad_optional_access{};
        }
    }
}  // namespace std_impl::optional::detail
""",
    )
    files.append(SRC / "optional/detail/require_value.cppm")

    write(
        SRC / "optional/detail/storage/traits.cppm",
        """export module std_impl.optional:optional.detail.storage.traits;
import std;

namespace std_impl::optional::detail::storage {
    template <typename T> inline constexpr bool nothrow_move_constructible = std::is_nothrow_move_constructible_v<T>;
    template <typename T> inline constexpr bool nothrow_move_assignable = std::is_nothrow_move_constructible_v<T> and std::is_nothrow_move_assignable_v<T>;
    template <typename T> inline constexpr bool nothrow_swappable = std::is_nothrow_move_constructible_v<T> and std::is_nothrow_swappable_v<T>;
}  // namespace std_impl::optional::detail::storage
""",
    )
    files.append(SRC / "optional/detail/storage/traits.cppm")

    write(
        SRC / "optional/detail/storage/interface.cppm",
        """export module std_impl.optional:optional.detail.storage.interface;
import std;

import :optional.detail.storage.traits;

namespace std_impl::optional::detail::storage {
    export template <typename T> struct storage {
        alignas(T) std::byte data_[sizeof(T)];
        bool engaged_{false};

        constexpr storage() noexcept = default;
        storage(const storage& other)
            requires std::copy_constructible<T>;
        storage(storage&& other) noexcept(nothrow_move_constructible<T>)
            requires std::move_constructible<T>;
        auto operator=(const storage& other) -> storage&
            requires std::copy_constructible<T>;
        auto operator=(storage&& other) noexcept(nothrow_move_assignable<T>)
            requires std::move_constructible<T>;
        ~storage();

        [[nodiscard]] constexpr auto engaged() const noexcept -> bool;
        [[nodiscard]] constexpr auto value(this auto&& self) noexcept -> decltype(auto);
        constexpr auto emplace(auto&&... args) -> T&;
        constexpr auto reset() noexcept -> void;
    };
}  // namespace std_impl::optional::detail::storage
""",
    )
    files.append(SRC / "optional/detail/storage/interface.cppm")

    write(
        SRC / "optional/detail/storage/impl.cppm",
        """export module std_impl.optional:optional.detail.storage.impl;
import std;

import :optional.detail.storage.interface;

namespace std_impl::optional::detail::storage {
    template <typename T>
    storage<T>::storage(const storage& other)
        requires std::copy_constructible<T> {
        if (other.engaged_) {
            emplace(other.value());
        }
    }

    template <typename T>
    storage<T>::storage(storage&& other) noexcept(nothrow_move_constructible<T>)
        requires std::move_constructible<T> {
        if (other.engaged_) {
            emplace(std::move(other.value()));
            other.reset();
        }
    }

    template <typename T>
    auto storage<T>::operator=(const storage& other) -> storage&
        requires std::copy_constructible<T> {
        if (this != &other) {
            if (other.engaged_) {
                if (engaged_) {
                    value() = other.value();
                } else {
                    emplace(other.value());
                }
            } else {
                reset();
            }
        }
        return *this;
    }

    template <typename T>
    auto storage<T>::operator=(storage&& other) noexcept(nothrow_move_assignable<T>)
        requires std::move_constructible<T> {
        if (this != &other) {
            if (other.engaged_) {
                if (engaged_) {
                    value() = std::move(other.value());
                } else {
                    emplace(std::move(other.value()));
                }
                other.reset();
            } else {
                reset();
            }
        }
        return *this;
    }

    template <typename T>
    storage<T>::~storage() {
        reset();
    }

    template <typename T>
    constexpr auto storage<T>::engaged() const noexcept -> bool {
        return engaged_;
    }

    template <typename T>
    constexpr auto storage<T>::value(this auto&& self) noexcept -> decltype(auto) {
        using ptr = std::conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>, const T*, T*>;
        return *std::launder(reinterpret_cast<ptr>(self.data_));
    }

    template <typename T>
    constexpr auto storage<T>::emplace(auto&&... args) -> T& {
        reset();
        ::new (data_) T(std::forward<decltype(args)>(args)...);
        engaged_ = true;
        return value();
    }

    template <typename T>
    constexpr auto storage<T>::reset() noexcept -> void {
        if (engaged_) {
            value().~T();
            engaged_ = false;
        }
    }
}  // namespace std_impl::optional::detail::storage
""",
    )
    files.append(SRC / "optional/detail/storage/impl.cppm")

    write(
        SRC / "optional/detail/storage.cppm",
        """export module std_impl.optional:optional.detail.storage;

export import :optional.detail.storage.traits;
export import :optional.detail.storage.interface;
export import :optional.detail.storage.impl;
""",
    )
    files.append(SRC / "optional/detail/storage.cppm")

    return files


def optional_interface() -> None:
    write(
        SRC / "optional/interface.cppm",
        """export module std_impl.optional:optional.interface;
import std;

import :bad_optional_access;
import :detail.concepts;
import :detail.storage;

namespace std_impl::optional {
    export struct nullopt_t {
        struct tag {};

        explicit constexpr nullopt_t(tag) {}
    };

    export inline constexpr nullopt_t nullopt{nullopt_t::tag{}};

    export template <typename T> class optional {
    public:
        using value_type = T;

        constexpr optional() noexcept;
        constexpr optional(nullopt_t) noexcept;

        optional(const optional& other)
            requires std::copy_constructible<value_type>;
        optional(optional&& other) noexcept(detail::storage::nothrow_move_constructible<value_type>)
            requires std::move_constructible<value_type>;

        constexpr optional(value_type value);

        template <typename... Args>
            requires std::constructible_from<value_type, Args...>
        constexpr explicit optional(std::in_place_t, Args&&... args);

        template <typename U, typename... Args>
            requires std::constructible_from<value_type, std::initializer_list<U>&, Args...>
        constexpr explicit optional(std::in_place_t, std::initializer_list<U> list, Args&&... args);

        template <typename U>
            requires detail::constructible_from_external_value<value_type, U>
        constexpr explicit(not std::is_convertible_v<U, value_type>) optional(U&& value) noexcept(std::is_nothrow_constructible_v<value_type, U>);

        template <typename U>
            requires detail::constructible_from_other_optional<value_type, U, const U&>
        constexpr explicit(not std::is_convertible_v<const U&, value_type>) optional(const optional<U>& other);

        template <typename U>
            requires detail::constructible_from_other_optional<value_type, U, U>
        constexpr explicit(not std::is_convertible_v<U, value_type>) optional(optional<U>&& other);

        constexpr auto operator=(nullopt_t) noexcept -> optional&;
        auto operator=(const optional& other) -> optional&
            requires std::copy_constructible<value_type>;
        auto operator=(optional&& other) noexcept(detail::storage::nothrow_move_assignable<value_type>) -> optional&
            requires std::move_constructible<value_type>;
        constexpr auto operator=(value_type value) -> optional&;
        template <typename U>
            requires detail::assignable_from_converted_value<value_type, U>
        constexpr auto operator=(U&& value) -> optional&;
        template <typename U>
            requires detail::assignable_from_optional_value<value_type, U, const U&>
        constexpr auto operator=(const optional<U>& other) -> optional&;
        template <typename U>
            requires detail::assignable_from_optional_value<value_type, U, U>
        constexpr auto operator=(optional<U>&& other) -> optional&;

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool;
        [[nodiscard]] constexpr explicit operator bool() const noexcept;
        [[nodiscard]] constexpr auto value(this auto&& self) -> decltype(auto);
        [[nodiscard]] constexpr auto operator*(this auto&& self) -> decltype(auto);
        [[nodiscard]] constexpr auto operator->(this auto& self) -> decltype(auto);
        [[nodiscard]] constexpr auto value_or(this auto&& self, auto&& default_value) -> auto;
        constexpr auto emplace(auto&&... args) -> value_type&;
        constexpr auto reset() noexcept -> void;
        constexpr auto swap(optional& other) noexcept(detail::storage::nothrow_swappable<value_type>) -> void;

        [[nodiscard]] constexpr auto and_then(this auto&& self, auto&& func) -> auto;
        [[nodiscard]] constexpr auto transform(this auto&& self, auto&& func) -> auto;
        [[nodiscard]] constexpr auto or_else(this const auto& self, auto&& func) -> auto;

    private:
        detail::storage::storage<value_type> storage_;
    };

    export template <typename T> auto swap(optional<T>& lhs, optional<T>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void;

    export template <typename T, typename U>
        requires detail::optional_values_equal_comparable<T, U>
    constexpr auto operator==(const optional<T>& lhs, const optional<U>& rhs) -> bool;

    export template <typename T, typename U>
        requires std::three_way_comparable_with<T, U>
    constexpr auto operator<=>(const optional<T>& lhs, const optional<U>& rhs) -> std::compare_three_way_result_t<T, U>;

    export template <typename T>
    constexpr auto operator==(const optional<T>& opt, nullopt_t) noexcept -> bool;

    export template <typename T>
    constexpr auto operator==(nullopt_t, const optional<T>& opt) noexcept -> bool;

    export template <typename T>
    constexpr auto operator<=>(const optional<T>& opt, nullopt_t) noexcept -> std::strong_ordering;

    export template <typename T>
    constexpr auto operator<=>(nullopt_t, const optional<T>& opt) noexcept -> std::strong_ordering;

    export template <typename T, typename U>
        requires detail::optional_value_equal_comparable<T, U>
    constexpr auto operator==(const optional<T>& opt, const U& value) -> bool;

    export template <typename T, typename U>
        requires detail::optional_value_equal_comparable<T, U>
    constexpr auto operator==(const U& value, const optional<T>& opt) -> bool;

    export template <typename T, typename U>
        requires detail::optional_value_three_way_comparable<T, U>
    constexpr auto operator<=>(const optional<T>& opt, const U& value) -> std::compare_three_way_result_t<T, U>;

    export template <typename T, typename U>
        requires detail::optional_value_three_way_comparable<T, U>
    constexpr auto operator<=>(const U& value, const optional<T>& opt) -> std::compare_three_way_result_t<T, U>;
}  // namespace std_impl::optional
""",
    )


def optional_ref_interface() -> None:
    write(
        SRC / "optional/ref_/interface.cppm",
        """export module std_impl.optional:optional.ref_.interface;
import std;

import :bad_optional_access;
import :detail.concepts;
import :optional.interface;

namespace std_impl::optional {
    // Constrained member templates of this partial specialization are defined
    // in-class: Clang rejects matching out-of-line definitions with requires.
    export template <typename T> class optional<T&> {
    public:
        using value_type = T;

        constexpr optional() noexcept = default;
        constexpr optional(nullopt_t) noexcept;
        constexpr optional(optional&) noexcept = default;
        constexpr optional(const optional&) noexcept = default;
        constexpr optional(optional&&) noexcept = default;

        template <typename Arg>
            requires detail::binds_ref_without_temporary<value_type&, Arg>
        constexpr explicit optional(std::in_place_t, Arg&& arg) {
            convert_ref_init_val(std::forward<Arg>(arg));
        }

        template <typename U>
            requires detail::constructible_ref_from_external_value<value_type, U>
        constexpr explicit(not std::is_convertible_v<U, value_type&>) optional(U&& value) noexcept(std::is_nothrow_constructible_v<value_type&, U>) :
            optional{std::in_place, std::forward<U>(value)} {}

        template <typename U>
            requires detail::constructible_ref_from_other_optional<value_type, U, U&>
        constexpr explicit(not std::is_convertible_v<U&, value_type&>) optional(optional<U>& rhs) noexcept(std::is_nothrow_constructible_v<value_type&, U&>) {
            if (rhs.has_value()) {
                convert_ref_init_val(rhs.value());
            }
        }

        template <typename U>
            requires detail::constructible_ref_from_other_optional<value_type, U, const U&>
        constexpr explicit(not std::is_convertible_v<const U&, value_type&>) optional(const optional<U>& rhs) noexcept(std::is_nothrow_constructible_v<value_type&, const U&>) {
            if (rhs.has_value()) {
                convert_ref_init_val(rhs.value());
            }
        }

        template <typename U>
            requires detail::constructible_ref_from_other_optional<value_type, U, U>
        constexpr explicit(not std::is_convertible_v<U, value_type&>) optional(optional<U>&& rhs) noexcept(std::is_nothrow_constructible_v<value_type&, U>) {
            if (rhs.has_value()) {
                convert_ref_init_val(std::move(rhs.value()));
            }
        }

        template <typename U>
            requires detail::constructible_ref_from_other_optional<value_type, U, const U>
        constexpr explicit(not std::is_convertible_v<const U, value_type&>) optional(const optional<U>&& rhs) noexcept(std::is_nothrow_constructible_v<value_type&, const U>) {
            if (rhs.has_value()) {
                convert_ref_init_val(std::move(rhs.value()));
            }
        }

        constexpr auto operator=(nullopt_t) noexcept -> optional&;
        constexpr auto operator=(optional& rhs) noexcept -> optional&;
        constexpr auto operator=(const optional& rhs) noexcept -> optional&;
        constexpr auto operator=(optional&& rhs) noexcept -> optional&;

        template <typename U>
            requires detail::assignable_ref_from_external_value<value_type, U>
        constexpr auto operator=(U&& value) noexcept(std::is_nothrow_constructible_v<value_type&, U>) -> optional& {
            convert_ref_init_val(std::forward<U>(value));
            return *this;
        }

        [[nodiscard]] constexpr auto has_value() const noexcept -> bool;
        [[nodiscard]] constexpr explicit operator bool() const noexcept;
        [[nodiscard]] constexpr auto value() const -> value_type&;
        [[nodiscard]] constexpr auto operator*() const noexcept -> value_type&;
        [[nodiscard]] constexpr auto operator->() const noexcept -> value_type*;
        template <typename U = value_type>
        [[nodiscard]] constexpr auto value_or(U&& default_value) const -> value_type;

        template <typename U>
            requires detail::binds_ref_without_temporary<value_type&, U>
        constexpr auto emplace(U&& value) noexcept(std::is_nothrow_constructible_v<value_type&, U>) -> value_type& {
            convert_ref_init_val(std::forward<U>(value));
            return *value_;
        }

        constexpr auto reset() noexcept -> void;
        constexpr auto swap(optional& other) noexcept -> void;

        [[nodiscard]] constexpr auto and_then(this auto&& self, auto&& function) -> auto;
        [[nodiscard]] constexpr auto transform(this auto&& self, auto&& function) -> auto;
        [[nodiscard]] constexpr auto or_else(this const auto& self, auto&& function) -> auto;

    private:
        template <typename U>
        constexpr auto convert_ref_init_val(U&& value) -> void;

        value_type* value_{nullptr};
    };

    export template <typename T> auto swap(optional<T&>& lhs, optional<T&>& rhs) noexcept -> void;
}  // namespace std_impl::optional
""",
    )


def _opt_impl_header(module: str, imports: list[str]) -> str:
    lines = [f"export module std_impl.optional:{module};", "import std;"]
    for imp in imports:
        lines.append(f"import {imp};")
    lines.append("")
    lines.append("namespace std_impl::optional {")
    return "\n".join(lines)


def optional_impl_files() -> list[Path]:
    files: list[Path] = []
    iface = ":optional.interface"
    req = ":optional.detail.require_value"

    impls = {
        "ctor.default": (
            [iface],
            """
    template <typename T>
    constexpr optional<T>::optional() noexcept :
        storage_{} {}
""",
        ),
        "ctor.nullopt": (
            [iface],
            """
    template <typename T>
    constexpr optional<T>::optional(nullopt_t) noexcept {}
""",
        ),
        "ctor.copy": (
            [iface],
            """
    template <typename T>
    optional<T>::optional(const optional& other)
        requires std::copy_constructible<value_type> :
        storage_(other.storage_) {}
""",
        ),
        "ctor.move": (
            [iface],
            """
    template <typename T>
    optional<T>::optional(optional&& other) noexcept(detail::storage::nothrow_move_constructible<value_type>)
        requires std::move_constructible<value_type> :
        storage_(std::move(other.storage_)) {}
""",
        ),
        "ctor.value": (
            [iface],
            """
    template <typename T>
    constexpr optional<T>::optional(value_type value) {
        storage_.emplace(std::move(value));
    }
""",
        ),
        "ctor.in_place": (
            [iface],
            """
    template <typename T>
    template <typename... Args>
        requires std::constructible_from<value_type, Args...>
    constexpr optional<T>::optional(std::in_place_t, Args&&... args) {
        storage_.emplace(std::forward<Args>(args)...);
    }

    template <typename T>
    template <typename U, typename... Args>
        requires std::constructible_from<value_type, std::initializer_list<U>&, Args...>
    constexpr optional<T>::optional(std::in_place_t, std::initializer_list<U> list, Args&&... args) {
        storage_.emplace(list, std::forward<Args>(args)...);
    }
""",
        ),
        "ctor.convert": (
            [iface],
            """
    template <typename T>
    template <typename U>
        requires detail::constructible_from_external_value<value_type, U>
    constexpr optional<T>::optional(U&& value) noexcept(std::is_nothrow_constructible_v<value_type, U>) {
        storage_.emplace(std::forward<U>(value));
    }
""",
        ),
        "ctor.from_optional": (
            [iface],
            """
    template <typename T>
    template <typename U>
        requires detail::constructible_from_other_optional<value_type, U, const U&>
    constexpr optional<T>::optional(const optional<U>& other) {
        if (other.has_value()) {
            storage_.emplace(*other);
        }
    }

    template <typename T>
    template <typename U>
        requires detail::constructible_from_other_optional<value_type, U, U>
    constexpr optional<T>::optional(optional<U>&& other) {
        if (other.has_value()) {
            storage_.emplace(std::move(*other));
        }
    }
""",
        ),
        "assign.nullopt": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T>::operator=(nullopt_t) noexcept -> optional& {
        storage_.reset();
        return *this;
    }
""",
        ),
        "assign.copy": (
            [iface],
            """
    template <typename T>
    auto optional<T>::operator=(const optional& other) -> optional&
        requires std::copy_constructible<value_type> {
        storage_ = other.storage_;
        return *this;
    }
""",
        ),
        "assign.move": (
            [iface],
            """
    template <typename T>
    auto optional<T>::operator=(optional&& other) noexcept(detail::storage::nothrow_move_assignable<value_type>) -> optional&
        requires std::move_constructible<value_type> {
        storage_ = std::move(other.storage_);
        return *this;
    }
""",
        ),
        "assign.value": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T>::operator=(value_type value) -> optional& {
        if (storage_.engaged()) {
            storage_.value() = std::move(value);
        } else {
            storage_.emplace(std::move(value));
        }
        return *this;
    }
""",
        ),
        "assign.convert": (
            [iface],
            """
    template <typename T>
    template <typename U>
        requires detail::assignable_from_converted_value<value_type, U>
    constexpr auto optional<T>::operator=(U&& value) -> optional& {
        if (storage_.engaged()) {
            storage_.value() = std::forward<U>(value);
        } else {
            storage_.emplace(std::forward<U>(value));
        }
        return *this;
    }
""",
        ),
        "assign.from_optional": (
            [iface],
            """
    template <typename T>
    template <typename U>
        requires detail::assignable_from_optional_value<value_type, U, const U&>
    constexpr auto optional<T>::operator=(const optional<U>& other) -> optional& {
        if (other.has_value()) {
            if (storage_.engaged()) {
                storage_.value() = *other;
            } else {
                storage_.emplace(*other);
            }
        } else if (storage_.engaged()) {
            storage_.reset();
        }
        return *this;
    }

    template <typename T>
    template <typename U>
        requires detail::assignable_from_optional_value<value_type, U, U>
    constexpr auto optional<T>::operator=(optional<U>&& other) -> optional& {
        if (other.has_value()) {
            if (storage_.engaged()) {
                storage_.value() = std::move(*other);
            } else {
                storage_.emplace(std::move(*other));
            }
        } else if (storage_.engaged()) {
            storage_.reset();
        }
        return *this;
    }
""",
        ),
        "has_value": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T>::has_value() const noexcept -> bool {
        return storage_.engaged();
    }
""",
        ),
        "operator_bool": (
            [iface],
            """
    template <typename T>
    constexpr optional<T>::operator bool() const noexcept {
        return has_value();
    }
""",
        ),
        "value": (
            [iface, req],
            """
    template <typename T>
    constexpr auto optional<T>::value(this auto&& self) -> decltype(auto) {
        detail::require_value(self.has_value());
        return std::forward_like<decltype(self)>(self.storage_.value());
    }
""",
        ),
        "operator_star": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T>::operator*(this auto&& self) -> decltype(auto) {
        return self.value();
    }
""",
        ),
        "operator_arrow": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T>::operator->(this auto& self) -> decltype(auto) {
        return std::addressof(self.value());
    }
""",
        ),
        "value_or": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T>::value_or(this auto&& self, auto&& default_value) -> auto {
        if (self.has_value()) {
            return std::forward_like<decltype(self)>(self.storage_.value());
        }
        return static_cast<value_type>(std::forward<decltype(default_value)>(default_value));
    }
""",
        ),
        "emplace": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T>::emplace(auto&&... args) -> value_type& {
        return storage_.emplace(std::forward<decltype(args)>(args)...);
    }
""",
        ),
        "reset": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T>::reset() noexcept -> void {
        storage_.reset();
    }
""",
        ),
        "swap": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T>::swap(optional& other) noexcept(detail::storage::nothrow_swappable<value_type>) -> void {
        if (has_value() and other.has_value()) {
            using std::swap;
            swap(storage_.value(), other.storage_.value());
        } else if (has_value()) {
            other.storage_.emplace(std::move(storage_.value()));
            storage_.reset();
        } else if (other.has_value()) {
            storage_.emplace(std::move(other.storage_.value()));
            other.storage_.reset();
        }
    }

    template <typename T>
    auto swap(optional<T>& lhs, optional<T>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void {
        lhs.swap(rhs);
    }
""",
        ),
        "and_then": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T>::and_then(this auto&& self, auto&& func) -> auto {
        using result = std::invoke_result_t<decltype(func), decltype(self.value())>;

        if (self.has_value()) {
            return std::invoke(std::forward<decltype(func)>(func), self.value());
        }
        return result{};
    }
""",
        ),
        "transform": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T>::transform(this auto&& self, auto&& func) -> auto {
        using U = std::remove_cvref_t<std::invoke_result_t<decltype(func), decltype(self.value())>>;

        if (self.has_value()) {
            const auto transformed = std::invoke(std::forward<decltype(func)>(func), self.value());
            return optional<U>{transformed};
        }
        return optional<U>{};
    }
""",
        ),
        "or_else": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T>::or_else(this const auto& self, auto&& func) -> auto {
        if (self.has_value()) {
            return self;
        }
        return std::invoke(std::forward<decltype(func)>(func));
    }
""",
        ),
        "operator_eq": (
            [iface],
            """
    template <typename T, typename U>
        requires detail::optional_values_equal_comparable<T, U>
    constexpr auto operator==(const optional<T>& lhs, const optional<U>& rhs) -> bool {
        if (static_cast<bool>(lhs) != static_cast<bool>(rhs)) {
            return false;
        }
        if (not lhs) {
            return true;
        }
        return lhs.value() == rhs.value();
    }
""",
        ),
        "operator_spaceship": (
            [iface],
            """
    template <typename T, typename U>
        requires std::three_way_comparable_with<T, U>
    constexpr auto operator<=>(const optional<T>& lhs, const optional<U>& rhs) -> std::compare_three_way_result_t<T, U> {
        if (lhs.has_value() and rhs.has_value()) {
            return lhs.value() <=> rhs.value();
        }
        return lhs.has_value() <=> rhs.has_value();
    }
""",
        ),
        "operator_eq_nullopt": (
            [iface],
            """
    template <typename T>
    constexpr auto operator==(const optional<T>& opt, nullopt_t) noexcept -> bool {
        return not opt;
    }

    template <typename T>
    constexpr auto operator==(nullopt_t, const optional<T>& opt) noexcept -> bool {
        return not opt;
    }
""",
        ),
        "operator_spaceship_nullopt": (
            [iface],
            """
    template <typename T>
    constexpr auto operator<=>(const optional<T>& opt, nullopt_t) noexcept -> std::strong_ordering {
        return opt.has_value() <=> false;
    }

    template <typename T>
    constexpr auto operator<=>(nullopt_t, const optional<T>& opt) noexcept -> std::strong_ordering {
        return false <=> opt.has_value();
    }
""",
        ),
        "operator_eq_value": (
            [iface],
            """
    template <typename T, typename U>
        requires detail::optional_value_equal_comparable<T, U>
    constexpr auto operator==(const optional<T>& opt, const U& value) -> bool {
        return opt.has_value() ? opt.value() == value : false;
    }

    template <typename T, typename U>
        requires detail::optional_value_equal_comparable<T, U>
    constexpr auto operator==(const U& value, const optional<T>& opt) -> bool {
        return opt == value;
    }
""",
        ),
        "operator_spaceship_value": (
            [iface],
            """
    template <typename T, typename U>
        requires detail::optional_value_three_way_comparable<T, U>
    constexpr auto operator<=>(const optional<T>& opt, const U& value) -> std::compare_three_way_result_t<T, U> {
        if (opt.has_value()) {
            return opt.value() <=> value;
        }
        using result = std::compare_three_way_result_t<T, U>;
        return static_cast<result>(std::strong_ordering::less);
    }

    template <typename T, typename U>
        requires detail::optional_value_three_way_comparable<T, U>
    constexpr auto operator<=>(const U& value, const optional<T>& opt) -> std::compare_three_way_result_t<T, U> {
        if (opt.has_value()) {
            return value <=> opt.value();
        }
        using result = std::compare_three_way_result_t<T, U>;
        return static_cast<result>(std::strong_ordering::greater);
    }
""",
        ),
        "hash": (
            [iface],
            """
}  // namespace std_impl::optional

namespace std {
    export template <typename T>
        requires std_impl::optional::detail::hashable_optional_value<T>
    struct hash<std_impl::optional::optional<T>> {
        [[nodiscard]] auto operator()(const std_impl::optional::optional<T>& opt) const noexcept(noexcept(std::hash<T>{}(opt.value()))) -> std::size_t {
            if (not opt) {
                return 0;
            }
            return std::hash<T>{}(opt.value());
        }
    };

    export template <typename T>
        requires std_impl::optional::detail::hashable_optional_ref_value<T>
    struct hash<std_impl::optional::optional<T&>> {
        [[nodiscard]] auto operator()(const std_impl::optional::optional<T&>& opt) const noexcept(noexcept(std::hash<T>{}(opt.value()))) -> std::size_t {
            if (not opt) {
                return 0;
            }
            return std::hash<T>{}(opt.value());
        }
    };
}  // namespace std
""",
        ),
    }

    for name, (imports, body) in impls.items():
        path = SRC / "optional/impl" / f"{name}.cppm"
        module_name = name.replace(".", "_") if name == "ctor.default" else name
        if name == "hash":
            content = _opt_impl_header(f"optional.impl.{module_name}", imports) + body
        else:
            content = _opt_impl_header(f"optional.impl.{module_name}", imports) + body + "\n}  // namespace std_impl::optional\n"
        write(path, content)
        files.append(path)

    return files


def optional_ref_impl_files() -> list[Path]:
    files: list[Path] = []
    iface = ":optional.ref_.interface"
    req = ":optional.detail.require_value"
    opt_iface = ":optional.interface"

    impls = {
        "ctor.nullopt": (
            [iface],
            """
    template <typename T>
    constexpr optional<T&>::optional(nullopt_t) noexcept :
        value_{nullptr} {}
""",
        ),
        "assign.nullopt": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T&>::operator=(nullopt_t) noexcept -> optional& {
        value_ = nullptr;
        return *this;
    }
""",
        ),
        "assign.copy": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T&>::operator=(optional& rhs) noexcept -> optional& {
        value_ = rhs.value_;
        return *this;
    }

    template <typename T>
    constexpr auto optional<T&>::operator=(const optional& rhs) noexcept -> optional& {
        value_ = rhs.value_;
        return *this;
    }
""",
        ),
        "assign.move": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T&>::operator=(optional&& rhs) noexcept -> optional& {
        value_ = rhs.value_;
        rhs.value_ = nullptr;
        return *this;
    }
""",
        ),
        "has_value": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T&>::has_value() const noexcept -> bool {
        return value_ != nullptr;
    }
""",
        ),
        "operator_bool": (
            [iface],
            """
    template <typename T>
    constexpr optional<T&>::operator bool() const noexcept {
        return has_value();
    }
""",
        ),
        "value": (
            [iface, req],
            """
    template <typename T>
    constexpr auto optional<T&>::value() const -> value_type& {
        detail::require_value(has_value());
        return *value_;
    }
""",
        ),
        "operator_star": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T&>::operator*() const noexcept -> value_type& {
        return *value_;
    }
""",
        ),
        "operator_arrow": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T&>::operator->() const noexcept -> value_type* {
        return value_;
    }
""",
        ),
        "value_or": (
            [iface],
            """
    template <typename T>
    template <typename U>
    constexpr auto optional<T&>::value_or(U&& default_value) const -> value_type {
        if (has_value()) {
            return *value_;
        }
        return static_cast<value_type>(std::forward<U>(default_value));
    }
""",
        ),
        "reset": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T&>::reset() noexcept -> void {
        value_ = nullptr;
    }
""",
        ),
        "swap": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T&>::swap(optional& other) noexcept -> void {
        using std::swap;
        swap(value_, other.value_);
    }

    template <typename T>
    auto swap(optional<T&>& lhs, optional<T&>& rhs) noexcept -> void {
        lhs.swap(rhs);
    }
""",
        ),
        "convert_ref_init_val": (
            [iface],
            """
    template <typename T>
    template <typename U>
    constexpr auto optional<T&>::convert_ref_init_val(U&& value) -> void {
        value_ = std::addressof(static_cast<U&&>(value));
    }
""",
        ),
        "and_then": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T&>::and_then(this auto&& self, auto&& function) -> auto {
        using result = std::invoke_result_t<decltype(function), value_type&>;

        if (self.has_value()) {
            return std::invoke(std::forward<decltype(function)>(function), self.value());
        }
        return result{};
    }
""",
        ),
        "transform": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T&>::transform(this auto&& self, auto&& function) -> auto {
        using U = std::remove_cvref_t<std::invoke_result_t<decltype(function), value_type&>>;

        if (self.has_value()) {
            return optional<U>{std::invoke(std::forward<decltype(function)>(function), self.value())};
        }
        return optional<U>{};
    }
""",
        ),
        "or_else": (
            [iface],
            """
    template <typename T>
    constexpr auto optional<T&>::or_else(this const auto& self, auto&& function) -> auto {
        if (self.has_value()) {
            return self;
        }
        return std::invoke(std::forward<decltype(function)>(function));
    }
""",
        ),
    }

    for name, (imports, body) in impls.items():
        path = SRC / "optional/ref_/impl" / f"{name}.cppm"
        content = _opt_impl_header(f"optional.ref_.impl.{name}", imports) + body + "\n}  // namespace std_impl::optional\n"
        write(path, content)
        files.append(path)

    return files


def optional_facades(opt_impl: list[Path], ref_impl: list[Path]) -> None:
    def partition_name(stem: str) -> str:
        # File ctor.default.cppm → module :optional.impl.ctor_default (default is a keyword).
        return stem.replace(".", "_") if stem == "ctor.default" else stem

    opt_names = sorted({partition_name(p.stem) for p in opt_impl})
    ref_names = sorted({partition_name(p.stem) for p in ref_impl})

    opt_imports = []
    for n in opt_names:
        # Hash specializations must be re-exported for consumers.
        kind = "export import" if n == "hash" else "import"
        opt_imports.append(f"{kind} :optional.impl.{n};")

    write(
        SRC / "optional/optional.cppm",
        "export module std_impl.optional:optional;\n\nexport import :optional.interface;\n"
        + "\n".join(opt_imports)
        + "\n",
    )

    write(
        SRC / "optional/ref_.cppm",
        "export module std_impl.optional:ref_;\n\nexport import :optional.ref_.interface;\n"
        + "\n".join(f"import :optional.ref_.impl.{n};" for n in ref_names)
        + "\n",
    )

    write(
        SRC / "optional.cppm",
        """export module std_impl.optional;

export import :optional;
export import :ref_;
export import :bad_optional_access;
export import :make_optional;
""",
    )


def main() -> None:
    expected_detail_files()
    expected_storage_files()
    expected_interface()
    expected_void_interface()
    exp_impl = expected_impl_files()
    void_impl = expected_void_impl_files()
    expected_facades(exp_impl, void_impl)

    optional_detail_and_storage()
    optional_interface()
    optional_ref_interface()
    opt_impl = optional_impl_files()
    ref_impl = optional_ref_impl_files()
    optional_facades(opt_impl, ref_impl)

    obsolete = [
        SRC / "expected/expected_void.cppm",
        SRC / "expected/detail/monad.cppm",
        SRC / "expected/detail/storage_void.cppm",
        SRC / "optional/optional_ref.cppm",
        SRC / "optional/detail/monad.cppm",
    ]
    for p in obsolete:
        if p.exists():
            p.unlink()

    print("Generated layout.")


if __name__ == "__main__":
    main()
