#ifndef SUBTITLETRACK_H
#define SUBTITLETRACK_H

#include <string>
#include <vector>
#include <memory>

// Subtitle structure to hold individual subtitle details
struct Subtitle {
    std::string term;       // The word/phrase to overlay
    double start_time;      // Start time of the subtitle (seconds)
    double end_time;        // End time of the subtitle (seconds)
    std::string intensity;  // Intensity: "Low", "Medium", "High"
};

class SubtitleTrack {
public:
    // Constructor and Destructor
    SubtitleTrack(const std::string& jsonPath);
    ~SubtitleTrack();

    // Load subtitle data from a JSON file
    bool loadSubtitles();

    // Retrieve active subtitles at a given timestamp
    std::vector<std::shared_ptr<Subtitle>> getSubtitlesForTime(double timestamp) const;

private:
    std::string jsonPath;  // Path to the JSON file
    std::vector<std::shared_ptr<Subtitle>> subtitles; // Parsed subtitles
};

#endif // SUBTITLETRACK_H
