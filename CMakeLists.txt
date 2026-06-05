cmake_minimum_required(VERSION 3.31)
project(Web_cam_detect)

add_compile_options(-Wall -Wextra)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
set(CMAKE_DEFAULT_BUILD_TYPE "Debug" CACHE STRING "" FORCE)

message("Using system level libraries")
find_package(OpenCV REQUIRED)
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
        MultimediaWidgets
        REQUIRED
)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Web_cam_detect
        main.cpp
)

message("Using UNIX target link")
target_link_libraries(Web_cam_detect
        PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Multimedia
        Qt6::MultimediaWidgets

        opencv_core
        opencv_imgproc
        opencv_highgui
        opencv_tracking
        opencv_objdetect
)