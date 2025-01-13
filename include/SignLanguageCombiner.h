#ifndef SIGNLANGUAGECOMBINER_H
#define SIGNLANGUAGECOMBINER_H

#include <string>
#include <opencv2/opencv.hpp>
#include "ImageLoader.h"

class SignLanguageCombiner {
public:
    // Constructor
    SignLanguageCombiner(const ImageLoader& imageLoader);

    // Combine character images into a single composite image
    cv::Mat combine(const std::string& term, int maxWidth, int maxHeight) const;

private:
    const ImageLoader& imageLoader; // Reference to the ImageLoader
};

#endif // SIGNLANGUAGECOMBINER_H
