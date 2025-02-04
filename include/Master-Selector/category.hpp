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
    Category(std::string name, const std::vector<Auton>& autons);

    // Default constructor for empty initialization
    Category() : name(""), autons() {}

    /** @brief The name of the category */
    std::string name;

    /** @brief The autons that the category contains */
    std::vector<Auton> autons;

    /**
     * @brief A vector that holds a copy of the auton names, ensuring the strings persist.
     */
    std::vector<std::string> auton_labels;

    /**
     * @brief A vector of pointers used as the button matrix map for LVGL.
     * The pointers in this vector point to the data in auton_labels.
     */
    std::vector<const char*> auton_map;
};

} // namespace ms
