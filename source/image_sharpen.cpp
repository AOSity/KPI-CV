#include <opencv2/opencv.hpp>
#include <iostream>

/**
 * @brief Applies unsharp masking to sharpen the input image.
 * 
 * @param src The input grayscale image.
 * @param dst The output sharpened image.
 * @param sigma The standard deviation for the Gaussian blur.
 * @param strength The strength of the sharpening effect (how much to boost the edges).
 */
void applyUnsharpMask(const cv::Mat& src, cv::Mat& dst, double sigma, double strength) {
    /* Ensure the input is a grayscale image */
    CV_Assert(src.type() == CV_8UC1);

    /* Create a Gaussian blurred version of the image */
    cv::Mat blurred;
    cv::GaussianBlur(src, blurred, cv::Size(0, 0), sigma);

    /* Create the sharpened image by adding the strength-scaled difference of the original and blurred image */
    cv::Mat sharpened = src + strength * (src - blurred);
    
    /* Clip values to the valid range [0, 255] */
    dst = cv::max(sharpened, 0);
    dst = cv::min(dst, 255);
}

int main(int argc, char** argv) {
    /* Check if the image path is provided */
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <image_path>" << std::endl;
        return -1;
    }

    /* Load the grayscale image */
    cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Error loading image: " << argv[1] << std::endl;
        return -1;
    }

    /* Prepare the output image */
    cv::Mat sharpened_image;

    /* Apply unsharp masking with specified sigma and strength */
    applyUnsharpMask(image, sharpened_image, 1.0, 1.5);

    cv::imwrite("output/UnsharpMasking.jpg", sharpened_image);

    /* Display results */
    cv::imshow("Original Image", image);
    cv::imshow("Sharpened Image", sharpened_image);

    /* Wait for a key press to close the windows */
    cv::waitKey(0);

    return 0;
}
