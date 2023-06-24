#include <penum.hpp>

#include <cmath>
#include <iostream>

template <typename R>
PENUM(result)
    PCASE(success, R)
    PCASE(error, std::string)
PENUM_END

result<float> square_root(float v)
{
    if (v < 0.0) {
        return result<float>::error("Can't calculate square root of negative value.");
    }
    return result<float>::success(sqrt(v));
}

int main()
{
    double x = 0;
    std::cout << "ENTER Value ";
    std::cin >> x;
    auto r = square_root(x);
    r.pswitch(
        CASE<result<float>::success>([](double v) {
            std::cout << "SQRT = " << v << std::endl;
        }),
        CASE<result<float>::error>([](std::string err) {
            std::cout << "ERROR: " << err << std::endl;
        })
    );
    return 0;
}