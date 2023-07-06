# penum
Parameterized enums for C++, using `std::variant` and macros

Inspired by Swift and Rust enums.

## Usage
```cpp
PENUM(result)
    PCASE(success, std::string)
    PCASE(error, std::exception_ptr)
PENUM_END;

result request(...)
{
    if (request_succeeded) {
        return result::success(std::string(data));
    }
    return result::error(std::make_exception_ptr(error));
}

```

For usage examples please look at [Examples](examples)