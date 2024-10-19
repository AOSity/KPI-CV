#include <opencv2/opencv.hpp>
#include <iostream>
#include <random>

/**
 * @brief Applies a box filter to the input image for smoothing.
 * 
 * @param src The input grayscale image.
 * @param dst The output image after applying the box filter.
 * @param kernel_size The size of the box filter.
 */
void applyBoxFilter(const cv::Mat& src, cv::Mat& dst, int kernel_size) {
    /* Ensure the input is a grayscale image */
    CV_Assert(src.type() == CV_8UC1);
    
    /* Create the output image with the same size as input */
    dst = cv::Mat::zeros(src.size(), src.type());
    
    /* Pad the image */
    int pad = kernel_size / 2;
    cv::Mat padded;
    cv::copyMakeBorder(src, padded, pad, pad, pad, pad, cv::BORDER_CONSTANT, 0);

    /* Apply the box filter */
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            float sum = 0.0f;
            for (int m = -pad; m <= pad; ++m) {
                for (int n = -pad; n <= pad; ++n) {
                    sum += padded.at<uchar>(i + pad + m, j + pad + n);
                }
            }
            dst.at<uchar>(i, j) = cv::saturate_cast<uchar>(sum / (kernel_size * kernel_size));
        }
    }
}

/**
 * @brief Applies a median filter to the input image for smoothing.
 * 
 * @param src The input grayscale image.
 * @param dst The output image after applying the median filter.
 * @param kernel_size The size of the median filter (should be odd).
 */
void applyMedianFilter(const cv::Mat& src, cv::Mat& dst, int kernel_size) {
    CV_Assert(src.type() == CV_8UC1);

    dst = cv::Mat::zeros(src.size(), src.type());
    int pad = kernel_size / 2;
    cv::Mat padded;
    cv::copyMakeBorder(src, padded, pad, pad, pad, pad, cv::BORDER_CONSTANT, 0);

    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            std::vector<uchar> window;
            for (int m = -pad; m <= pad; ++m) {
                for (int n = -pad; n <= pad; ++n) {
                    window.push_back(padded.at<uchar>(i + pad + m, j + pad + n));
                }
            }
            std::sort(window.begin(), window.end());
            dst.at<uchar>(i, j) = window[window.size() / 2];
        }
    }
}

/**
 * @brief Applies a Gaussian filter to the input image for smoothing.
 * 
 * @param src The input grayscale image.
 * @param dst The output image after applying the Gaussian filter.
 * @param kernel_size The size of the Gaussian kernel.
 * @param sigma The standard deviation for Gaussian kernel.
 */
void applyGaussianFilter(const cv::Mat& src, cv::Mat& dst, int kernel_size, double sigma) {
    CV_Assert(src.type() == CV_8UC1);

    dst = cv::Mat::zeros(src.size(), src.type());
    int pad = kernel_size / 2;

    /* Generate Gaussian kernel */ 
    cv::Mat kernel = cv::getGaussianKernel(kernel_size, sigma, CV_32F);
    kernel = kernel * kernel.t();

    cv::Mat padded;
    cv::copyMakeBorder(src, padded, pad, pad, pad, pad, cv::BORDER_CONSTANT, 0);

    /* Convolution with Gaussian kernel */
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            float sum = 0.0f;
            for (int m = -pad; m <= pad; ++m) {
                for (int n = -pad; n <= pad; ++n) {
                    sum += padded.at<uchar>(i + pad + m, j + pad + n) * kernel.at<float>(m + pad, n + pad);
                }
            }
            dst.at<uchar>(i, j) = cv::saturate_cast<uchar>(sum);
        }
    }
}

/**
 * @brief Applies a sigma filter to the input image for smoothing.
 * 
 * @param src The input grayscale image.
 * @param dst The output image after applying the sigma filter.
 * @param kernel_size The size of the sigma filter (should be odd).
 * @param sigma The standard deviation for the Gaussian distribution.
 */
void applySigmaFilter(const cv::Mat& src, cv::Mat& dst, int kernel_size, double sigma) {
    CV_Assert(src.type() == CV_8UC1);

    dst = cv::Mat::zeros(src.size(), src.type());
    int pad = kernel_size / 2;
    cv::Mat padded;
    cv::copyMakeBorder(src, padded, pad, pad, pad, pad, cv::BORDER_CONSTANT, 0);

    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            std::vector<float> window;
            for (int m = -pad; m <= pad; ++m) {
                for (int n = -pad; n <= pad; ++n) {
                    window.push_back(padded.at<uchar>(i + pad + m, j + pad + n));
                }
            }

            float mean = std::accumulate(window.begin(), window.end(), 0.0f) / window.size();
            float variance = 0.0f;
            for (const auto& value : window) {
                variance += (value - mean) * (value - mean);
            }
            variance /= window.size();

            float threshold = mean + sigma * std::sqrt(variance);
            for (auto& value : window) {
                if (value > threshold) {
                    value = mean;
                }
            }

            dst.at<uchar>(i, j) = cv::saturate_cast<uchar>(std::accumulate(window.begin(), window.end(), 0.0f) / window.size());
        }
    }
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

    /* Add random noise to the image */
    cv::Mat noisy_image = image.clone();
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, 255);
    
    for (int i = 0; i < noisy_image.rows; ++i) {
        for (int j = 0; j < noisy_image.cols; ++j) {
            if (distribution(generator) < 5) {
                /* Add noise */
                noisy_image.at<uchar>(i, j) = static_cast<uchar>(distribution(generator));
            }
        }
    }

    /* Prepare output images */
    cv::Mat box_filtered, median_filtered, gaussian_filtered, sigma_filtered;

    /* Apply smoothing filters */
    applyBoxFilter(noisy_image, box_filtered, 5);
    applyMedianFilter(noisy_image, median_filtered, 5);
    applyGaussianFilter(noisy_image, gaussian_filtered, 5, 1.0);
    applySigmaFilter(noisy_image, sigma_filtered, 5, 1.0);

    /* Save results */
    cv::imwrite("output/Smooth-AdditionalNoise.jpg", noisy_image);
    cv::imwrite("output/Smooth-BoxFiltered.jpg", box_filtered);
    cv::imwrite("output/Smooth-MedianFiltered.jpg", median_filtered);
    cv::imwrite("output/Smooth-GaussianFiltered.jpg", gaussian_filtered);
    cv::imwrite("output/Smooth-SigmaFiltered.jpg", sigma_filtered);

    /* Display results */
    cv::imshow("Original Image", image);
    cv::imshow("Noisy Image", noisy_image);
    cv::imshow("Box Filtered", box_filtered);
    cv::imshow("Median Filtered", median_filtered);
    cv::imshow("Gaussian Filtered", gaussian_filtered);
    cv::imshow("Sigma Filtered", sigma_filtered);

    /* Wait for a key press to close the windows */
    cv::waitKey(0);

    return 0;
}
