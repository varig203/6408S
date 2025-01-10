#pragma once
#include <string>
#include <vector>
#include "auton.hpp"


namespace ms {

class Category {
public:
    /**
     * @brief Construct a new Category in the autonomous selector
     *
     * @param name The name of the category
     * @param autons The autons that the category will contain
     */
    Category(std::string name, const std::vector <Auton> &autons);

    // Default copy/move constructors and assignment operators
    Category(const Category&) = default;
    Category(Category&&) = default;
    Category& operator=(const Category&) = default;
    Category& operator=(Category&&) = default;

    /** @brief The name of the category */
    std::string name;

    /** @brief The autons that the category contains */
    std::vector <Auton> autons;
};

} // namespace ms
