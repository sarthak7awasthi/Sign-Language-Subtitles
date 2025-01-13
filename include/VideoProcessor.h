#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <opencv2/opencv.hpp> // Include OpenCV for cv::Mat

// Representation of a video frame (using OpenCV Mat)
struct Frame {
    cv::Mat data; // The actual pixel data of the frame
};

class VideoProcessor {
public:
    VideoProcessor(const std::string& inputPath, const std::string& outputPath);
    ~VideoProcessor();

    bool loadVideo();
    Frame getFrame(int index);
    void writeFrame(int index, const Frame& frame);
    bool exportVideo();

    int getTotalFrames() const;
    int getFrameWidth() const;
    int getFrameHeight() const;
    double getFrameRate() const;

private:
    std::string inputPath;
    std::string outputPath;
    int totalFrames;
    int frameWidth;
    int frameHeight;
    double frameRate;

    std::vector<std::shared_ptr<Frame>> frames;
};

#endif // VIDEOPROCESSOR_H
