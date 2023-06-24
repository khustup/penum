#include <penum.hpp>

#include <vector>
#include <iostream>

PENUM(fruit)
    PCASE(apple, int, float, std::string)
    PCASE(orange, double, std::string)
    PCASE(banana, std::string)
    PCASE(mango)
PENUM_END

int main()
{
    std::vector<fruit> fruits;
    fruits.push_back(fruit::apple(1, 2.5f, "A"));
    fruits.push_back(fruit::apple(2, 3.1f, "B"));
    fruits.push_back(fruit::orange(5.5, "C"));
    fruits.push_back(fruit::banana(std::string("Yellow")));
    fruits.push_back(fruit::mango());
    for (const auto& v : fruits) {
        v.pswitch(
            CASE<fruit::apple>([](int rank, float weight, const std::string& tag) {
                std::cout << weight << "kg Apple of rank " << rank << " and tag " << tag << std::endl;
            }),
            CASE<fruit::orange>([](double weight, const std::string& tag) {
                std::cout << weight << "kg Orange of tag " << tag << std::endl;
            }),
            CASE<fruit::banana>([](const std::string& color) {
                std::cout << color << " Banana" << std::endl;
            }),
            CASE<fruit::mango>([]() {
                std::cout << "Mango" << std::endl;
            })
        );
    }
    std::cout << "===============" << std::endl;
    for (const auto& v : fruits) {
        v.pswitch(
            CASE<fruit::banana>([](const std::string& color) {
                std::cout << color << " Banana" << std::endl;
            }),
            DEFAULT([]() {
                std::cout << "Not-Banana" << std::endl;
            })
        );
    }
    return 0;
}