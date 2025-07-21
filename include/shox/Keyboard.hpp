#include <iostream>
#include <string>

namespace shox {

/**
 * @brief Waits for keyboard input from the user.
 *
 * This function blocks until the user provides input via the keyboard.
 * It reads a line from standard input and clears any error flags to ensure
 * that it can be called repeatedly without issues.
 */
void wait_for_keyboard_input() {
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        if (std::cin.good()) {
            break; // Only exit if we actually got valid input
        }
        std::cin.clear(); // Clear any error flags
    }
}

} // namespace shox