#include <iostream>
#include <string>

#include "result.hpp"

auto good_result() -> result<int, std::string>
{
    return 5;
}

auto bad_result() -> result<int, std::string>
{
    using namespace std::string_literals;
    return "Error!"s;
}

auto test_result() -> result<int, std::string>
{
    int const a = co_await good_result();
    int const b = co_await bad_result();

    co_return a + b;
}

auto main() -> int
{
    std::cout << test_result() << std::endl;
}