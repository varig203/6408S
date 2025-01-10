#pragma once
#include <functional>
#include <string>
#include <stdlib.h>

namespace ms {

class Auton {
public:
    /**
     * @brief Construct a new Auton
     *
     * @param name The name of the auton
     * @param callback The callback associated with the auton (the function that will be called when auton is selected)
     */
    Auton(std::string name, const std::function<void()> &callback);

    // Explicitly define copy and move constructors and assignment operators
    Auton(const Auton&) = default;  // Copy constructor
    Auton(Auton&&) = default;       // Move constructor
    Auton& operator=(const Auton&) = default; // Copy assignment operator
    Auton& operator=(Auton&&) = default; // Move assignment operator

    /** @brief The name of the auton */
    std::string name;

    /** @brief The callback associated with the auton */
    std::function<void()> callback;
};

} // namespace ms
