#include <opencv2/opencv.hpp>
#include <iostream>

/**
 * @brief Calculates the mean and variance of a grayscale image.
 * 
 * @param image The input grayscale image (type CV_8UC1).
 * @param mean Output parameter for the mean value of the image.
 * @param variance Output parameter for the variance value of the image.
 */
void calculateMeanVariance(const cv::Mat& image, double& mean, double& variance) {
    /* Ensure the input is a grayscale image */
    CV_Assert(image.type() == CV_8UC1);

    /* Calculate mean */
    mean = cv::mean(image)[0];

    /* Calculate variance */
    variance = 0.0;
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            double diff = image.at<uchar>(i, j) - mean;
            variance += diff * diff;
        }
    }
    variance /= (image.rows * image.cols);
}

/**
 * @brief Adjusts the mean and variance of the source image to match those of the reference image.
 * 
 * @param src The input source grayscale image (type CV_8UC1).
 * @param ref The reference grayscale image (type CV_8UC1) whose mean and variance are used.
 * @param dst The output image with adjusted mean and variance.
 */
void conditionalScaling(const cv::Mat& src, const cv::Mat& ref, cv::Mat& dst) {
    /* Ensure both images are grayscale */
    CV_Assert(src.type() == CV_8UC1 && ref.type() == CV_8UC1);

    /* Calculate mean and variance for both images */
    double mean_src, variance_src;
    calculateMeanVariance(src, mean_src, variance_src);

    double mean_ref, variance_ref;
    calculateMeanVariance(ref, mean_ref, variance_ref);

    /* Standard deviation is the square root of variance */
    double stddev_src = std::sqrt(variance_src);
    double stddev_ref = std::sqrt(variance_ref);

    /* Adjust the source image's mean and variance to match the reference */
    dst = src.clone();
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            /* Apply the conditional scaling formula */
            double adjusted_pixel = ((src.at<uchar>(i, j) - mean_src) * stddev_ref / stddev_src) + mean_ref;
            dst.at<uchar>(i, j) = cv::saturate_cast<uchar>(adjusted_pixel);
        }
    }
}

int main(int argc, char** argv) {
    /* Check if the source and reference image paths are provided */
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <source_image> <reference_image>" << std::endl;
        return -1;
    }

    /* Load the source and reference grayscale images */
    cv::Mat src = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    cv::Mat ref = cv::imread(argv[2], cv::IMREAD_GRAYSCALE);
    if (src.empty() || ref.empty()) {
        std::cerr << "Error loading images!" << std::endl;
        return -1;
    }

    /* Perform conditional scaling */
    cv::Mat adjusted_image;
    conditionalScaling(src, ref, adjusted_image);

    /* Save the result */
    cv::imwrite("output/ConditionalScaling.jpg", adjusted_image);

    /* Display original and adjusted images */
    cv::imshow("Source Image", src);
    cv::imshow("Reference Image", ref);
    cv::imshow("Adjusted Image", adjusted_image);
    cv::waitKey(0);

    return 0;
}
