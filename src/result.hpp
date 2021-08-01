#include <coroutine>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <variant>

template<typename T, typename E>
struct result
{
private:
    std::variant<std::monostate, T, E> m_data;

public:
    result() = default;
    result(result const&) = default;
    result(result&&) noexcept = default;
    ~result() noexcept = default;

    result(T const& value)
        : m_data{ value }
    {
    }
    result(E const& err)
        : m_data{ err }
    {
    }

    auto operator=(result const&) -> result& = default;
    auto operator=(result&&) noexcept -> result& = default;

    auto operator=(T const& value)
    {
        m_data = value;
        return *this;
    }

    auto operator=(E const& err) -> result&
    {
        m_data = err;
        return *this;
    }

    [[nodiscard]] auto value_or(T& fallback) -> T&
    {
        if(auto* p = std::get_if<T>(&m_data)) {
            return *p;
        }

        return fallback;
    }

    [[nodiscard]] auto value_or(T const& fallback) const -> T const&
    {
        if(auto* p = std::get_if<T>(&m_data)) {
            return *p;
        }

        return fallback;
    }

    [[nodiscard]] auto value() const -> T const&
    {
        if(auto* p = std::get_if<T>(&m_data)) {
            return *p;
        }

        // Ugly solution, but we don't really care
        std::cout << "Tried to access either value even though it's in error state!" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    [[nodiscard]] auto get_underlying() noexcept -> std::variant<std::monostate, T, E>&
    {
        return m_data;
    }

    [[nodiscard]] auto get_underlying() const noexcept -> std::variant<std::monostate, T, E>
    {
        return m_data;
    }

    [[nodiscard]] inline auto is_error() const noexcept -> bool
    {
        return std::get_if<E>(&m_data);
    }

    [[nodiscard]] auto get_error() noexcept -> E&
    {
        return *std::get_if<E>(&m_data);
    }

    [[nodiscard]] auto get_error() const noexcept -> E const&
    {
        return *std::get_if<E>(&m_data);
    }

    struct promise_type
    {
        result* res = nullptr;

        auto get_return_object() -> result
        {
            return { *this };
        }

        auto return_value(result val) -> void
        {
            *res = std::move(val);
        }

        auto initial_suspend() const noexcept -> std::suspend_never
        {
            return {};
        }

        auto final_suspend() const noexcept -> std::suspend_never
        {
            return {};
        }

        auto unhandled_exception() noexcept -> void
        {
        }
    };

    struct awaiter
    {
        result& res;

        auto await_ready() -> bool
        {
            return !res.is_error();
        }

        auto await_resume() -> T const
        {
            return res.value();
        }

        auto await_suspend(std::coroutine_handle<promise_type> h) -> void
        {
            *(h.promise().res) = res;
            h.destroy();
        }
    };

    auto operator co_await() -> awaiter
    {
        return awaiter{ *this };
    }

private:
    result(promise_type& promise)
    {
        promise.res = this;
    }
};

template<typename T, typename E>
auto operator<<(std::ostream& os, result<T, E> const& res) -> std::ostream&
{
    if(res.is_error()) {
        os << res.get_error();
    }
    else {
        os << res.value();
    }
    return os;
}