#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <string>
#include <unordered_map>
#include <opencv2/opencv.hpp> // For image handling

class ImageLoader {
public:
    // Constructor
    ImageLoader(const std::string& imagesPath);

    // Load all character images (a-z)
    bool loadImages();

    // Retrieve a specific character image
    cv::Mat getCharacterImage(char c) const;

private:
    std::string imagesPath;                                // Path to the images folder
    std::unordered_map<char, cv::Mat> characterImages;     // Map of character to image
};

#endif // IMAGELOADER_H
