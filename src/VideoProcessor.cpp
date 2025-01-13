#include "VideoProcessor.h"
#include <iostream>
#include <stdexcept>
#include <opencv2/opencv.hpp> // For FFmpeg or OpenCV integration

// Representation of a video frame (using OpenCV Mat)


// Constructor
VideoProcessor::VideoProcessor(const std::string& inputPath, const std::string& outputPath)
    : inputPath(inputPath), outputPath(outputPath), totalFrames(0), frameWidth(0), frameHeight(0), frameRate(0.0) {}

// Destructor
VideoProcessor::~VideoProcessor() {}

// Load video frames
bool VideoProcessor::loadVideo() {
    cv::VideoCapture cap(inputPath); // Open video file

    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open video file: " << inputPath << std::endl;
        return false;
    }

    // Retrieve video properties
    frameRate = cap.get(cv::CAP_PROP_FPS);
    totalFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
    frameWidth = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    frameHeight = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    // Load all frames into memory
    for (int i = 0; i < totalFrames; ++i) {
        cv::Mat frame;
        cap >> frame; // Read the next frame
        if (frame.empty()) break; // End of video

        // Store the frame as a shared pointer
        frames.push_back(std::make_shared<Frame>(Frame{frame}));
    }

    cap.release(); // Release video file
    return true;
}

// Get a specific frame by index
Frame VideoProcessor::getFrame(int index) {
    if (index < 0 || index >= totalFrames) {
        throw std::out_of_range("Frame index out of bounds");
    }
    return *frames[index];
}

// Write a modified frame back into the frame buffer
void VideoProcessor::writeFrame(int index, const Frame& frame) {
    if (index < 0 || index >= totalFrames) {
        throw std::out_of_range("Frame index out of bounds");
    }
    frames[index] = std::make_shared<Frame>(frame);
}

// Export video to output file
bool VideoProcessor::exportVideo() {
    cv::VideoWriter writer(
        outputPath,
        cv::VideoWriter::fourcc('m', 'p', '4', 'v'), // Codec
        frameRate,
        cv::Size(frameWidth, frameHeight)
    );

    if (!writer.isOpened()) {
        std::cerr << "Error: Cannot open output video file: " << outputPath << std::endl;
        return false;
    }

    for (const auto& frame : frames) {
        writer.write(frame->data); // Write each frame
    }

    writer.release(); // Finalize video
    return true;
}

// Getters
int VideoProcessor::getTotalFrames() const {
    return totalFrames;
}

int VideoProcessor::getFrameWidth() const {
    return frameWidth;
}

int VideoProcessor::getFrameHeight() const {
    return frameHeight;
}

double VideoProcessor::getFrameRate() const {
    return frameRate;
}
