#include "ImageLoader.h"
#include <iostream>
#include <filesystem> // C++17 for directory operations

namespace fs = std::filesystem;

// Constructor
ImageLoader::ImageLoader(const std::string& imagesPath) : imagesPath(imagesPath) {}

// Load all character images (a-z)
bool ImageLoader::loadImages() {
    for (char c = 'a'; c <= 'z'; ++c) {
        std::string imagePath = imagesPath + "/" + c + ".png";
        cv::Mat image = cv::imread(imagePath, cv::IMREAD_UNCHANGED);

        if (image.empty()) {
            std::cerr << "Error: Unable to load image for character: " << c << " at " << imagePath << std::endl;
            return false;
        }

        characterImages[c] = image;
    }
    return true;
}

// Retrieve a specific character image
cv::Mat ImageLoader::getCharacterImage(char c) const {
    auto it = characterImages.find(c);
    if (it != characterImages.end()) {
        return it->second;
    } else {
        std::cerr << "Error: Character image not found for: " << c << std::endl;
        return cv::Mat(); // Return an empty matrix
    }
}
