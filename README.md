# Monads in C++
Using coroutines we can say something like:
```cpp
auto f() -> result<int, std::string>
{
    return "Error!";
}

auto g() -> result<int, std::string>
{
    return 5;
}

auto compute() -> result<int, std::string>
{
    int const a = co_await f();
    int const b = co_await g();

    co_return a + b;
}
```

# What? Why?
I think there's only two possible reactions to this, no in-between:
 * WTF is this?
 * Such a shame there is no direct support from the language for something like this

# Building
There is a `shell.nix` that gives you an environment with all the stuff needed to run this "project"(though you don't need nix, it's just convenient to use). If you have nix you can:
```sh
nix-shell

./scripts/build.sh
```
And then in `build/src/` you can run `./cpp-monads`.

# Resources
Most of this was inspired from [here](https://www.reddit.com/r/cpp/comments/6ly4rz/it_had_to_be_done_abusing_co_await_for_optionals/) and [here](https://cpp-rendering.io/c-error-handling-lets-abuse-the-co_await-operator/).