#include "SubtitleTrack.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp> // JSON parsing library

// For simplicity, use a namespace alias for the JSON library
using json = nlohmann::json;

// Constructor
SubtitleTrack::SubtitleTrack(const std::string& jsonPath) : jsonPath(jsonPath) {}

// Destructor
SubtitleTrack::~SubtitleTrack() {}

// Load subtitle data from a JSON file
bool SubtitleTrack::loadSubtitles() {
    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open subtitle JSON file: " << jsonPath << std::endl;
        return false;
    }

    try {
        // Parse the JSON file
        json j;
        file >> j;

        // Populate the subtitles vector
        for (const auto& item : j) {
            auto subtitle = std::make_shared<Subtitle>();
            subtitle->term = item.at("term").get<std::string>();
            subtitle->start_time = item.at("start_time").get<double>();
            subtitle->end_time = item.at("end_time").get<double>();
            subtitle->intensity = item.at("intensity").get<std::string>();
            subtitles.push_back(subtitle);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to parse subtitle JSON file. Exception: " << e.what() << std::endl;
        return false;
    }

    return true;
}

// Retrieve active subtitles at a given timestamp
std::vector<std::shared_ptr<Subtitle>> SubtitleTrack::getSubtitlesForTime(double timestamp) const {
    std::vector<std::shared_ptr<Subtitle>> activeSubtitles;

    for (const auto& subtitle : subtitles) {
        if (timestamp >= subtitle->start_time && timestamp <= subtitle->end_time) {
            activeSubtitles.push_back(subtitle);
        }
    }

    return activeSubtitles;
}
