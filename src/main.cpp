#include "VideoProcessor.h"
#include "SubtitleTrack.h"
#include "ImageLoader.h"
#include "SignLanguageCombiner.h"
#include "OverlayManager.h"
#include <iostream>

int main(int argc, char* argv[]) {
    // Check for required arguments
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <input_video> <subtitles_json> <image_folder> <output_video>" << std::endl;
        return -1;
    }

    // Parse command-line arguments
    std::string inputVideoPath = argv[1];
    std::string subtitlesPath = argv[2];
    std::string imageFolderPath = argv[3];
    std::string outputVideoPath = argv[4];

    // Load the video
    VideoProcessor videoProcessor(inputVideoPath, outputVideoPath);
    if (!videoProcessor.loadVideo()) {
        std::cerr << "Failed to load video: " << inputVideoPath << std::endl;
        return -1;
    }

    // Load the subtitle track
    SubtitleTrack subtitleTrack(subtitlesPath);
    if (!subtitleTrack.loadSubtitles()) {
        std::cerr << "Failed to load subtitles: " << subtitlesPath << std::endl;
        return -1;
    }

    // Load the character images
    ImageLoader imageLoader(imageFolderPath);
    if (!imageLoader.loadImages()) {
        std::cerr << "Failed to load character images from folder: " << imageFolderPath << std::endl;
        return -1;
    }

    // Initialize the combiner
    SignLanguageCombiner combiner(imageLoader);

    // Initialize the overlay manager
    OverlayManager overlayManager(videoProcessor, subtitleTrack, combiner);

    // Process overlays
    if (!overlayManager.processOverlays()) {
        std::cerr << "Failed to process overlays!" << std::endl;
        return -1;
    }

    // Export the final video
    if (videoProcessor.exportVideo()) {
        std::cout << "Video processed and saved successfully to: " << outputVideoPath << std::endl;
    } else {
        std::cerr << "Failed to export video!" << std::endl;
        return -1;
    }

    return 0;
}
