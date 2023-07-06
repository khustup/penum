#include <penum.hpp>

#include <vector>
#include <iostream>

PENUM(fruit)
    PCASE(apple, std::string)
    PCASE(orange, std::string)
    PCASE(banana, std::string)
    PCASE(mango, std::string)
PENUM_END;

int main()
{
    std::vector<fruit> fruits;
    fruits.push_back(fruit::apple("Green"));
    fruits.push_back(fruit::apple("Red"));
    fruits.push_back(fruit::orange("Orange"));
    fruits.push_back(fruit::banana(std::string("Yellow")));
    fruits.push_back(fruit::mango("Orange"));
    for (const auto& v : fruits) {
        v.pswitch(
            CASE<fruit::apple>([](std::string color) {
                std::cout << color << " Apple" << std::endl;
            }),
            CASE<fruit::orange>([](std::string color) {
                std::cout << color << " Orange" << std::endl;
            }),
            CASE<fruit::banana>([](std::string color) {
                std::cout << color << " Banana" << std::endl;
            }),
            CASE<fruit::mango>([](std::string color) {
                std::cout << color << " Mango" << std::endl;
            })
        );
    }
    std::cout << "===============" << std::endl;
    for (const auto& v : fruits) {
        v.pswitch(
            CASE<fruit::banana>([](std::string color) {
                std::cout << color << " Banana" << std::endl;
            }),
            DEFAULT([](std::string color) {
                std::cout << color << " Not-Banana" << std::endl;
            })
        );
    }
    return 0;
}