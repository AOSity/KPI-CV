#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

/**
 * @brief Equalizes the histogram of a grayscale image.
 * 
 * @param src The input grayscale image (type CV_8UC1).
 * @param dst The output image with an equalized histogram.
 */
void equalizeHistogram(const cv::Mat& src, cv::Mat& dst) {
    /* Ensure the input is a grayscale image */
    CV_Assert(src.type() == CV_8UC1);

    /* Compute the histogram */
    std::vector<int> histogram(256, 0);
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            histogram[src.at<uchar>(i, j)]++;
        }
    }

    /* Compute the cumulative distribution function (CDF) */
    std::vector<int> cdf(256, 0);
    cdf[0] = histogram[0];
    for (int i = 1; i < 256; ++i) {
        cdf[i] = cdf[i - 1] + histogram[i];
    }

    /* Normalize the CDF */
    int total_pixels = src.rows * src.cols;
    std::vector<uchar> equalized_lut(256);
    for (int i = 0; i < 256; ++i) {
        equalized_lut[i] = cv::saturate_cast<uchar>((cdf[i] * 255) / total_pixels);
    }

    /* pply the equalized LUT to the image */
    dst = src.clone();
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            dst.at<uchar>(i, j) = equalized_lut[src.at<uchar>(i, j)];
        }
    }
}

int main(int argc, char** argv) {
    /* Check if the image file path is provided */
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <image_path>" << std::endl;
        return -1;
    }

    /* Load a grayscale image */
    cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Error loading image: " << argv[1] << std::endl;
        return -1;
    }

    /* Create an output image for the equalized result */
    cv::Mat equalized_image;
    equalizeHistogram(image, equalized_image);

    /* Save the result */
    cv::imwrite("output/HistogramEqualization.jpg", equalized_image);

    /* Display original and equalized images */
    cv::imshow("Original Image", image);
    cv::imshow("Equalized Image", equalized_image);
    cv::waitKey(0);

    return 0;
}
