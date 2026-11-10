#include "Level.hpp"

#include <cmath>
#include <fstream>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>

Level Level::loadFromFile(const std::filesystem::path& path,
                          float blockSize, float floorY) {
    if (!std::isfinite(blockSize) || blockSize <= 0.0f ||
        !std::isfinite(floorY)) {
        throw std::runtime_error("Invalid block size or floor height");
    }

    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Cannot open level file: " + path.string());
    }

    Level level;
    std::string line;
    std::size_t lineNumber = 0;

    while (std::getline(file, line)) {
        ++lineNumber;

        // Everything after # is a comment; blank lines are allowed.
        const auto comment = line.find('#');

        // If there is a #, erase it from the string
        if (comment != std::string::npos) {
            line.erase(comment);
        }

        // Extract line into a String
        std::istringstream fields(line);

        // Read in line using classic C style rules
        // This means always use a . for decimals
        // No comma separation for numbers
        fields.imbue(std::locale::classic());


        std::string type;
        if (!(fields >> type)) {
            // If extraction failed, continue
            continue;
        }

        // Lambda for if reading a line fails
        const auto fail = [&](const std::string& message) {
            throw std::runtime_error(path.string() + ":" +
                                     std::to_string(lineNumber) + ": " + message);
        };

        // Check if valid type
        if (type != "block" && type != "spike") {
            fail("Unknown object type '" + type + "'; expected block or spike");
        }

        // Check if valid parameters
        float column, row, width, height;
        if (!(fields >> column >> row >> width >> height)) {
            fail("Expected: type column row width height (four numbers)");
        }

        // Check if uneccessary info was added after
        std::string extra;
        if (fields >> extra) {
            fail("Unexpected extra field '" + extra + "'");
        }

        // Check that parameters are actual numbers
        if (!std::isfinite(column) || !std::isfinite(row) ||
            !std::isfinite(width) || !std::isfinite(height) ||
            column < 0.0f || row < 0.0f || width <= 0.0f || height <= 0.0f) {
            fail("Coordinates must be finite and nonnegative; sizes must be finite and positive");
        }

        // Convert file info into coordinates that the program can understand
        // Row 0 rests on the floor; larger rows place objects higher.
        const float x = column * blockSize;
        const float y = floorY - (row + height) * blockSize;
        const float w = width * blockSize;
        const float h = height * blockSize;

        // Check if objects have valid coordinates within range
        if (!std::isfinite(x) || !std::isfinite(y) ||
            !std::isfinite(w) || !std::isfinite(h) ||
            !std::isfinite(x + w) || !std::isfinite(y + h) ||
            x + w <= x || y + h <= y) {
            fail("Object coordinates or dimensions are outside the supported range");
        }

        // Add object in as either block or spike
        if (type == "block") {
            level.blocks.emplace_back(x, y, w, h);
        } else if (type == "spike") {
            level.spikes.emplace_back(x, y, w, h);
        }
    }


    // Check for fatal error
    if (file.bad()) {
        throw std::runtime_error("Error reading level file: " + path.string());
    }

    return level;
}
