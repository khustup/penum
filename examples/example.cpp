#include <penum.hpp>

#include <vector>
#include <iostream>

PENUM_START(vehicle)
    PCASE(motorcycle, std::string, int)
    PCASE(car, std::string, double)
    PCASE(airplane)
    PCASE(boat, std::string, std::string)
    PCASE(bike, int)
PENUM_END

int main()
{
    std::vector<vehicle> vehicles;
    vehicles.push_back(vehicle::motorcycle("Suzuki", 2020));
    vehicles.push_back(vehicle::car("AUDI", 3.6));
    vehicles.push_back(vehicle::airplane());
    vehicles.push_back(vehicle::boat(std::string("Boom"), "Catamaran"));
    vehicles.push_back(vehicle::bike(1));
    for (const auto& v : vehicles) {
        v.pswitch(
            CASE<vehicle::motorcycle>([](std::string name, int year) {
                std::cout << "M " << name << " " << year << std::endl;
            }),
            CASE<vehicle::car>([](std::string name, double engine) {
                std::cout << "C " << name << " " << engine << std::endl;
            }),
            CASE<vehicle::airplane>([]() {
                std::cout << "A " << std::endl;
            }),
            CASE<vehicle::boat>([](std::string name, std::string type) {
                std::cout << "B " << name << " " << type << std::endl;
            }),
            CASE<vehicle::bike>([](int id) {
                std::cout << "V " << id << std::endl;
            })
        );
    }
    return 0;
}