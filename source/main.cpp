#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv) {
    // Check if an image path was provided
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <image_path>" << std::endl;
        return -1;
    }

    // Read the image from the path provided as a command-line argument
    std::string imagePath = argv[1];
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);

    // Check if the image was loaded successfully
    if (image.empty()) {
        std::cerr << "Error: Could not open or find the image at " << imagePath << std::endl;
        return -1;
    }

    // Create a window to display the image
    cv::imshow("Image", image);

    // Wait indefinitely for a key press
    cv::waitKey(0);

    return 0;
}
