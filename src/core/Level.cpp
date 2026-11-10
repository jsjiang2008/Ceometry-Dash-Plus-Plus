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
        if (comment != std::string::npos) {
            line.erase(comment);
        }

        std::istringstream fields(line);
        fields.imbue(std::locale::classic());
        std::string type;
        if (!(fields >> type)) {
            continue;
        }

        const auto fail = [&](const std::string& message) {
            throw std::runtime_error(path.string() + ":" +
                                     std::to_string(lineNumber) + ": " + message);
        };

        if (type != "block" && type != "spike") {
            fail("Unknown object type '" + type + "'; expected block or spike");
        }

        float column, row, width, height;
        if (!(fields >> column >> row >> width >> height)) {
            fail("Expected: type column row width height (four numbers)");
        }

        std::string extra;
        if (fields >> extra) {
            fail("Unexpected extra field '" + extra + "'");
        }

        if (!std::isfinite(column) || !std::isfinite(row) ||
            !std::isfinite(width) || !std::isfinite(height) ||
            column < 0.0f || row < 0.0f || width <= 0.0f || height <= 0.0f) {
            fail("Coordinates must be finite and nonnegative; sizes must be finite and positive");
        }

        // Row 0 rests on the floor; larger rows place objects higher.
        const float x = column * blockSize;
        const float y = floorY - (row + height) * blockSize;
        const float w = width * blockSize;
        const float h = height * blockSize;

        if (!std::isfinite(x) || !std::isfinite(y) ||
            !std::isfinite(w) || !std::isfinite(h) ||
            !std::isfinite(x + w) || !std::isfinite(y + h) ||
            x + w <= x || y + h <= y) {
            fail("Object coordinates or dimensions are outside the supported range");
        }

        if (type == "block") {
            level.blocks.emplace_back(x, y, w, h);
        } else {
            level.spikes.emplace_back(x, y, w, h);
        }
    }

    if (file.bad()) {
        throw std::runtime_error("Error reading level file: " + path.string());
    }

    return level;
}
