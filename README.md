# Sign Language Subtitle Video Processor

A C++ library for overlaying animated sign language subtitles on videos, enhancing accessibility for deaf and hard-of-hearing individuals.

## Overview

This library processes videos by adding sign language subtitles based on provided subtitle tracks. It features dynamic animations that reflect subtitle intensity, supporting three levels:

- **High Intensity**: Subtitles grow progressively larger
- **Medium Intensity**: Subtitles oscillate with ±15 degrees rotation
- **Low Intensity**: Subtitles remain static

## Features

- Overlay sign language subtitles on video frames
- Support for intensity-based animations
- Multi-word subtitle handling with vertical stacking
- Dynamic subtitle track processing via JSON input
- Automatic video processing and subtitle embedding

## System Requirements

- C++ Compiler (GCC 9+ or equivalent)
- CMake (Build system generator)
- OpenCV (Video processing and image manipulation)
- nlohmann/json (JSON subtitle track parsing)

## Installation

### 1. Install Dependencies

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libopencv-dev
```

### 2. Clone the Repository

```bash
git clone https://github.com/your-repo/sign-language-subtitle-processor.git
cd sign-language-subtitle-processor
```

### 3. Build the Project

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

The library is accessed through the `video_overlay` executable:

```bash
./video_overlay <input_video> <subtitles_json> <image_folder> <output_video>
```

Example:
```bash
./video_overlay ../src/input.mp4 ../src/subtitles.json ../images/ ../src/output.mp4
```

## Input File Formats

### Subtitle JSON Format

```json
[
    {
        "term": "Header Goal",
        "start_time": 1.00,
        "end_time": 4.00,
        "intensity": "High"
    },
    {
        "term": "Welcome Goal",
        "start_time": 5.00,
        "end_time": 8.00,
        "intensity": "Medium"
    }
]
```

### Character Images

Place sign language gesture images in the `images/` folder:
- Required format: PNG files named a.png through z.png
- Each image represents a single character's sign language gesture

## Project Structure

```
.
├── src/
│   ├── VideoProcessor.cpp/h
│   ├── SubtitleTrack.cpp/h
│   ├── ImageLoader.cpp/h
│   ├── SignLanguageCombiner.cpp/h
│   ├── OverlayManager.cpp/h
│   └── main.cpp
├── include/
├── images/
├── subtitles.json
├── input.mp4
├── output.mp4
├── CMakeLists.txt
└── README.md
```

## Advanced Features

### Multi-Word Subtitle Display
Words are stacked vertically for improved readability:
```
Header
Goal
```

### Animation Types
- **High Intensity**: Progressive scaling effect
- **Medium Intensity**: Smooth ±15° rotation
- **Low Intensity**: Static display
- Dynamic scaling to fit video frame

## Troubleshooting

### Common Issues

1. Missing Dependencies
   - Verify OpenCV and nlohmann/json installation
   - Check CMakeLists.txt for correct library paths

2. Incorrect File Paths
   - Confirm paths to input.mp4, subtitles.json, and images/

3. Clipping/Missing Subtitles
   - Ensure images/ contains all required character images (a-z)

## Future Development

- Additional language and alphabet support
- Real-time video processing capabilities
- Customizable subtitle animations and styles

## Contributing

We welcome contributions! Please:
1. Fork the repository
2. Create a feature branch
3. Submit a pull request
