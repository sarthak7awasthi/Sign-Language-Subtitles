#ifndef OVERLAYMANAGER_H
#define OVERLAYMANAGER_H

#include <vector>
#include <string>
#include "VideoProcessor.h"
#include "SubtitleTrack.h"
#include "SignLanguageCombiner.h"

class OverlayManager {
public:
    // Constructor
    OverlayManager(VideoProcessor& videoProcessor, SubtitleTrack& subtitleTrack, SignLanguageCombiner& combiner);

    // Process and overlay subtitles onto the video
    bool processOverlays();

private:
    VideoProcessor& videoProcessor;        // Reference to the VideoProcessor
    SubtitleTrack& subtitleTrack;          // Reference to the SubtitleTrack
    SignLanguageCombiner& combiner;        // Reference to the SignLanguageCombiner

    // Helper function to position the overlay
    cv::Point calculatePosition(const cv::Mat& overlayImage, int frameWidth, int frameHeight) const;
};

#endif // OVERLAYMANAGER_H
