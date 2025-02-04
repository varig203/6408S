#include "Master-Selector/auton.hpp"

namespace ms {

Auton::Auton(std::string name, const std::function<void()>& callback)
    : name(std::move(name)), callback(callback) {}

}
