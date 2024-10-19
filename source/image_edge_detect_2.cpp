#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

/**
 * @brief Apply Gaussian blur to an image manually.
 * 
 * @param src Input grayscale image.
 * @param dst Output image after Gaussian blur.
 * @param kernelSize Size of the Gaussian kernel.
 * @param sigma Standard deviation of the Gaussian function.
 */
void gaussianBlur(const cv::Mat& src, cv::Mat& dst, int kernelSize, double sigma) {
    int radius = kernelSize / 2;
    double sum = 0.0;
    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize));

    /* Generate the Gaussian kernel */
    for (int y = -radius; y <= radius; ++y) {
        for (int x = -radius; x <= radius; ++x) {
            double value = std::exp(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * CV_PI * sigma * sigma);
            kernel[y + radius][x + radius] = value;
            sum += value;
        }
    }

    /* Normalize the kernel */
    for (int y = 0; y < kernelSize; ++y) {
        for (int x = 0; x < kernelSize; ++x) {
            kernel[y][x] /= sum;
        }
    }

    dst = cv::Mat::zeros(src.size(), CV_32F);

    /* Apply Gaussian blur using the kernel */
    for (int y = radius; y < src.rows - radius; ++y) {
        for (int x = radius; x < src.cols - radius; ++x) {
            float blurredPixel = 0.0;
            for (int ky = -radius; ky <= radius; ++ky) {
                for (int kx = -radius; kx <= radius; ++kx) {
                    blurredPixel += src.at<uchar>(y + ky, x + kx) * kernel[ky + radius][kx + radius];
                }
            }
            dst.at<float>(y, x) = blurredPixel;
        }
    }
}

/**
 * @brief Apply Laplacian of Gaussian (LoG) edge detection manually.
 * 
 * @param src Input grayscale image.
 * @param dst Output image after LoG edge detection.
 * @param kernelSize Size of the Gaussian kernel.
 * @param sigma Standard deviation of the Gaussian function.
 */
void laplacianOfGaussian(const cv::Mat& src, cv::Mat& dst, int kernelSize, double sigma) {
    cv::Mat blurred;
    
    /* Apply Gaussian blur */
    gaussianBlur(src, blurred, kernelSize, sigma);

    /* Apply Laplacian filter */
    int laplaceKernel[3][3] = {{0, 1, 0}, {1, -4, 1}, {0, 1, 0}};
    dst = cv::Mat::zeros(blurred.size(), CV_32F);

    for (int y = 1; y < blurred.rows - 1; ++y) {
        for (int x = 1; x < blurred.cols - 1; ++x) {
            float laplacian = 0;
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    laplacian += blurred.at<float>(y + ky, x + kx) * laplaceKernel[ky + 1][kx + 1];
                }
            }
            dst.at<float>(y, x) = std::abs(laplacian);
        }
    }

    /* Normalize the result */
    cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX);
    dst.convertTo(dst, CV_8U);
}

/**
 * @brief Apply Difference of Gaussian (DoG) edge detection manually.
 * 
 * @param src Input grayscale image.
 * @param dst Output image after DoG edge detection.
 * @param kernelSize Size of the Gaussian kernel.
 * @param sigma1 Standard deviation of the first Gaussian blur.
 * @param sigma2 Standard deviation of the second Gaussian blur.
 */
void differenceOfGaussian(const cv::Mat& src, cv::Mat& dst, int kernelSize, double sigma1, double sigma2) {
    cv::Mat blurred1, blurred2;

    /* Apply two Gaussian blurs with different sigmas */
    gaussianBlur(src, blurred1, kernelSize, sigma1);
    gaussianBlur(src, blurred2, kernelSize, sigma2);

    /* Compute the Difference of Gaussian (DoG) */
    cv::Mat dog = blurred1 - blurred2;

    /* Normalize the result */
    cv::normalize(dog, dog, 0, 255, cv::NORM_MINMAX);
    dog.convertTo(dst, CV_8U);
}

int main(int argc, char** argv) {
    /* Check if image path is provided */
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <image_path>" << std::endl;
        return -1;
    }

    /* Load image as grayscale */
    cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Error loading image!" << std::endl;
        return -1;
    }

    cv::Mat logResult, dogResult;

    /* Apply Laplacian of Gaussian (LoG) */
    laplacianOfGaussian(image, logResult, 3, 1.5);

    /* Apply Difference of Gaussian (DoG) */
    differenceOfGaussian(image, dogResult, 3, 1.25, 2.5);

    /* Save the results */
    cv::imwrite("output/EdgeDetection-LoG.jpg", logResult);
    cv::imwrite("output/EdgeDetection-DoG.jpg", dogResult);

    /* Show the results */
    cv::imshow("Original", image);
    cv::imshow("Laplacian of Gaussian (LoG)", logResult);
    cv::imshow("Difference of Gaussian (DoG)", dogResult);
    cv::waitKey(0);

    return 0;
}
