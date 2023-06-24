#include <penum.hpp>

#include <cmath>
#include <iostream>

PENUM(result)
    PCASE(success, double)
    PCASE(error, std::string)
PENUM_END

result square_root(double v)
{
    if (v < 0.0) {
        return result::error("Can't calculate square root of negative value.");
    }
    return result::success(sqrt(v));
}

int main()
{
    double x = 0;
    std::cout << "ENTER Value ";
    std::cin >> x;
    auto r = square_root(x);
    r.pswitch(
        CASE<result::success>([](double v) {
            std::cout << "SQRT = " << v << std::endl;
        }),
        CASE<result::error>([](std::string err) {
            std::cout << "ERROR: " << err << std::endl;
        })
    );
    return 0;
}