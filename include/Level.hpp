#pragma once

#include <filesystem>
#include <vector>

#include "Block.hpp"
#include "Portal.hpp"
#include "Spike.hpp"

class Level {
private:
    std::vector<Block> blocks;
    std::vector<Spike> spikes;
    std::vector<Portal> portals;

    // Mode at the start of every attempt; a "start ship" line changes it.
    PlayerMode startingMode = PlayerMode::Cube;

public:
    // Read grid coordinates and convert them into world-space rectangles.
    // Throws std::runtime_error if the file cannot be read or is invalid.
    static Level loadFromFile(const std::filesystem::path& path,
                              float blockSize, float floorY);

    const std::vector<Block>& getBlocks() const {
        return blocks;
    }

    const std::vector<Spike>& getSpikes() const {
        return spikes;
    }

    const std::vector<Portal>& getPortals() const {
        return portals;
    }

    PlayerMode getStartingMode() const {
        return startingMode;
    }
};
