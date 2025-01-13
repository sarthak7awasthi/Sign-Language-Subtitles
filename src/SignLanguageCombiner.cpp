#include "SignLanguageCombiner.h"
#include <iostream>
#include <sstream>   // For std::istringstream
#include <iterator>  // For std::istream_iterator

// Constructor
SignLanguageCombiner::SignLanguageCombiner(const ImageLoader& imageLoader) : imageLoader(imageLoader) {}

// Combine character images into a single composite image
cv::Mat SignLanguageCombiner::combine(const std::string& term, int maxWidth, int maxHeight) const {
    // Split the term into individual words
    std::istringstream iss(term);
    std::vector<std::string> words{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

    // Store combined images for each word
    std::vector<cv::Mat> wordImages;

    for (const auto& word : words) {
        // List to hold character images for the word
        std::vector<cv::Mat> charImages;

        // Load images for each character in the word
        for (char c : word) {
            if (std::isalpha(c)) { // Only process alphabetic characters
                c = std::tolower(c); // Convert to lowercase
                cv::Mat charImage = imageLoader.getCharacterImage(c);
                if (!charImage.empty()) {
                    charImages.push_back(charImage);
                } else {
                    std::cerr << "Warning: Image for character '" << c << "' not found!" << std::endl;
                }
            }
        }

        if (charImages.empty()) {
            std::cerr << "Error: No valid character images found for word: " << word << std::endl;
            continue;
        }

        // Combine character images for the word
        int spacing = 10; // Space between characters
        int totalWidth = spacing * (charImages.size() - 1); // Start with spacing between images
        int maxHeightChar = 0;

        for (const auto& img : charImages) {
            totalWidth += img.cols;
            maxHeightChar = std::max(maxHeightChar, img.rows);
        }

        // Apply a scaling factor to reduce original size
        double scale = 0.8; // Scale factor to slightly reduce the size
        if (totalWidth > maxWidth) {
            scale = std::min(scale, static_cast<double>(maxWidth) / totalWidth);
            totalWidth = maxWidth;
        }

        maxHeightChar = static_cast<int>(maxHeightChar * scale);

        cv::Mat wordImage = cv::Mat::zeros(maxHeightChar, totalWidth, charImages[0].type());

        int xOffset = 0;
        for (const auto& img : charImages) {
            cv::Mat resizedImg;
            cv::resize(img, resizedImg, cv::Size(), scale, scale);
            resizedImg.copyTo(wordImage(cv::Rect(xOffset, 0, resizedImg.cols, resizedImg.rows)));
            xOffset += resizedImg.cols + spacing;
        }

        wordImages.push_back(wordImage);
    }

    if (wordImages.empty()) {
        std::cerr << "Error: No valid word images found for term: " << term << std::endl;
        return cv::Mat();
    }

    // Combine word images vertically to create the final multi-line subtitle
    int spacing = 20; // Space between lines
    int totalHeight = spacing * (wordImages.size() - 1);
    int maxWidthWord = 0;

    for (const auto& img : wordImages) {
        totalHeight += img.rows;
        maxWidthWord = std::max(maxWidthWord, img.cols);
    }

    cv::Mat combinedImage = cv::Mat::zeros(totalHeight, maxWidthWord, wordImages[0].type());

    int yOffset = 0;
    for (const auto& img : wordImages) {
        img.copyTo(combinedImage(cv::Rect((maxWidthWord - img.cols) / 2, yOffset, img.cols, img.rows)));
        yOffset += img.rows + spacing;
    }

    return combinedImage;
}
