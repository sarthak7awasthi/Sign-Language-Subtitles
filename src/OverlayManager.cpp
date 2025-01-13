
#include "VideoProcessor.h" // Include to bring in the full definition of Frame

#include "OverlayManager.h"
#include <iostream>

// Constructor
OverlayManager::OverlayManager(VideoProcessor& videoProcessor, SubtitleTrack& subtitleTrack, SignLanguageCombiner& combiner)
    : videoProcessor(videoProcessor), subtitleTrack(subtitleTrack), combiner(combiner) {}

// Process and overlay subtitles onto the video





bool OverlayManager::processOverlays() {
    int totalFrames = videoProcessor.getTotalFrames();
    double frameRate = videoProcessor.getFrameRate();

    for (int frameIndex = 0; frameIndex < totalFrames; ++frameIndex) {
        // Calculate the timestamp for the current frame
        double timestamp = frameIndex / frameRate;

        // Get active subtitles for this timestamp
        auto activeSubtitles = subtitleTrack.getSubtitlesForTime(timestamp);

        // Get the current frame
        Frame frame = videoProcessor.getFrame(frameIndex);

        // Process each active subtitle
        for (const auto& subtitle : activeSubtitles) {
            // Generate the combined sign language image
            cv::Mat overlayImage = combiner.combine(subtitle->term, frame.data.cols * 0.8, frame.data.rows * 0.2);

            if (overlayImage.empty()) {
                std::cerr << "Warning: Failed to create overlay for term: " << subtitle->term << std::endl;
                continue;
            }

            // Store original size of the overlay
            cv::Size originalSize = overlayImage.size();

            // Apply effects based on intensity
            if (subtitle->intensity == "High") {
                // Progressive scaling effect
                double scaleFactor = 1.0 + (0.1 * frameIndex / frameRate); // Grow larger over time
                cv::resize(overlayImage, overlayImage, cv::Size(), scaleFactor, scaleFactor);
            } else if (subtitle->intensity == "Medium") {
                // Rotation effect (±15 degrees)
                double angle = 1.0 * sin(2 * CV_PI * frameIndex / frameRate); // Oscillation angle

                // Dynamically expand the canvas to accommodate rotation
                int padding = overlayImage.rows / 2; // Add enough padding for rotation
                cv::Mat paddedImage = cv::Mat::zeros(overlayImage.rows + 2 * padding, overlayImage.cols + 2 * padding, overlayImage.type());
                overlayImage.copyTo(paddedImage(cv::Rect(padding, padding, overlayImage.cols, overlayImage.rows)));

                // Rotate the padded image
                cv::Point2f center(paddedImage.cols / 2.0, paddedImage.rows / 2.0);
                cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle, 1.0);
                cv::Mat rotatedImage;
                cv::warpAffine(paddedImage, rotatedImage, rotationMatrix, paddedImage.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));

                // Adjust position to ensure it stays fixed relative to the original location
                cv::Rect roi((rotatedImage.cols - overlayImage.cols) / 2, (rotatedImage.rows - overlayImage.rows) / 2, overlayImage.cols, overlayImage.rows);
                overlayImage = rotatedImage(roi);
            }
            else if (subtitle->intensity == "Low") {
                // Restore to original size (if modified by previous effects)
                if (overlayImage.size() != originalSize) {
                    cv::resize(overlayImage, overlayImage, originalSize);
                }
            }

            // Calculate the position for the overlay
            cv::Point position = calculatePosition(overlayImage, frame.data.cols, frame.data.rows);

            // Define the ROI for placing the overlay
            cv::Rect roi(position.x, position.y, overlayImage.cols, overlayImage.rows);

            // Ensure ROI is within frame bounds
            if (roi.x + roi.width > frame.data.cols || roi.y + roi.height > frame.data.rows) {
                std::cerr << "Warning: Overlay ROI exceeds frame bounds for term: " << subtitle->term << std::endl;
                continue;
            }

            // Extract the ROI from the frame
            cv::Mat frameROI = frame.data(roi);

            // Resize overlay image to match ROI dimensions
            if (frameROI.size() != overlayImage.size()) {
                cv::resize(overlayImage, overlayImage, frameROI.size());
            }

            // Ensure both images have the same number of channels
            if (frameROI.channels() != overlayImage.channels()) {
                if (overlayImage.channels() == 1) {
                    cv::cvtColor(overlayImage, overlayImage, cv::COLOR_GRAY2BGR);
                } else if (overlayImage.channels() == 4) {
                    cv::cvtColor(overlayImage, overlayImage, cv::COLOR_RGBA2BGR);
                }
            }

            // Blend only the rotated subtitle image onto the frame ROI
            cv::addWeighted(frameROI, 1.0, overlayImage, 0.3, 0.0, frameROI);
        }

        // Write the modified frame back
        videoProcessor.writeFrame(frameIndex, frame);
    }

    return true;
}

// Calculate position for overlay
cv::Point OverlayManager::calculatePosition(const cv::Mat& overlayImage, int frameWidth, int frameHeight) const {
    int x = (frameWidth - overlayImage.cols) / 2; // Center horizontally
    int y = frameHeight - overlayImage.rows - 10; // Position slightly above the bottom
    return cv::Point(x, y);
}
