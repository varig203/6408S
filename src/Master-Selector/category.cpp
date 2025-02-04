#include "Master-Selector/category.hpp"

ms::Category::Category(std::string name, const std::vector<ms::Auton>& autons)
    : name(std::move(name)), autons(autons) {}
