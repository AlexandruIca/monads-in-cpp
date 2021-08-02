#include <coroutine>
#include <iostream>
#include <optional>
#include <utility>

template<typename T>
struct option
{
private:
    std::optional<T> m_data{ std::nullopt };

public:
    option() = default;
    option(option const&) = default;
    option(option&&) noexcept = default;
    ~option() noexcept = default;

    option(T const& value)
        : m_data{ value }
    {
    }

    option(std::nullopt_t)
        : m_data{ std::nullopt }
    {
    }

    auto operator=(option const&) -> option& = default;
    auto operator=(option&&) noexcept -> option& = default;

    auto operator=(T const& value) -> option&
    {
        m_data = value;
        return *this;
    }

    auto operator=(std::nullopt_t) -> option&
    {
        m_data = std::nullopt;
        return *this;
    }

    [[nodiscard]] operator bool() const noexcept
    {
        return m_data.has_value();
    }

    [[nodiscard]] auto value_or(T& fallback) noexcept -> T&
    {
        return m_data.value_or(fallback);
    }

    [[nodiscard]] auto value_or(T const& fallback) const noexcept -> T const&
    {
        return m_data.value_or(fallback);
    }

    [[nodiscard]] auto value() -> T&
    {
        return m_data.value();
    }

    [[nodiscard]] auto value() const -> T const&
    {
        return m_data.value();
    }

    struct promise_type
    {
        option* res = nullptr;

        auto get_return_object() -> option
        {
            return { *this };
        }

        auto return_value(option retval) -> void
        {
            *res = std::move(retval);
        }

        auto initial_suspend() noexcept -> std::suspend_never
        {
            return {};
        }

        auto final_suspend() noexcept -> std::suspend_never
        {
            return {};
        }

        auto unhandled_exception() noexcept -> void
        {
        }
    };

    struct awaiter
    {
        option& res;

        auto await_ready() const noexcept -> bool
        {
            return res;
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
    option(promise_type& promise)
    {
        promise.res = this;
    }
};

template<typename T>
auto operator<<(std::ostream& os, option<T> const& opt) -> std::ostream&
{
    if(opt) {
        os << opt.value();
    }
    else {
        os << "None";
    }
    return os;
}