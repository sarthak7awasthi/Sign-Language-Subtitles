cmake_minimum_required(VERSION 3.10)
project(VideoOverlay)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

# Include directories
include_directories(include ${OpenCV_INCLUDE_DIRS})

set(SOURCES
    src/main.cpp
    src/VideoProcessor.cpp
    src/SubtitleTrack.cpp
    src/ImageLoader.cpp
    src/SignLanguageCombiner.cpp
    src/OverlayManager.cpp
)


# Define executable
add_executable(video_overlay ${SOURCES})

# Link OpenCV libraries
target_link_libraries(video_overlay ${OpenCV_LIBS})
