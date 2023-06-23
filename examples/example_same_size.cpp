#include <penum.hpp>

#include <vector>
#include <iostream>

PENUM(fruit)
    PCASE(apple, float)
    PCASE(orange, double)
    PCASE(banana, std::string)
    PCASE(mango, int)
PENUM_END

int main()
{
    std::vector<fruit> fruits;
    fruits.push_back(fruit::apple(2.5f));
    fruits.push_back(fruit::apple(3.1f));
    fruits.push_back(fruit::orange(5.5));
    fruits.push_back(fruit::banana(std::string("Yellow")));
    fruits.push_back(fruit::mango(12));
    for (const auto& v : fruits) {
        v.pswitch(
            CASE<fruit::apple>([](float weight) {
                std::cout << weight << "kg Apple" << std::endl;
            }),
            CASE<fruit::orange>([](double weight) {
                std::cout << weight << "kg Orange" << std::endl;
            }),
            CASE<fruit::banana>([](std::string color) {
                std::cout << color << " Banana" << std::endl;
            }),
            CASE<fruit::mango>([](int count) {
                std::cout << count << " Mango" << std::endl;
            })
        );
    }
    std::cout << "===============" << std::endl;
    for (const auto& v : fruits) {
        v.pswitch(
            CASE<fruit::banana>([](std::string color) {
                std::cout << color << " Banana" << std::endl;
            }),
            DEFAULT([](auto prop) {
                std::cout << "Not-Banana with property - " << prop << std::endl;
            })
        );
    }
    return 0;
}