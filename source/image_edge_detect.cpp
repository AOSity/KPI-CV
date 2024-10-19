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
 * @brief Apply Sobel edge detection manually.
 * 
 * @param src Input grayscale image.
 * @param dst Output image after Sobel edge detection.
 */
void sobelEdgeDetection(const cv::Mat& src, cv::Mat& dst) {
    int sobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int sobelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    dst = cv::Mat::zeros(src.size(), CV_32F);

    for (int y = 1; y < src.rows - 1; ++y) {
        for (int x = 1; x < src.cols - 1; ++x) {
            float gradX = 0, gradY = 0;

            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    int pixel = src.at<uchar>(y + ky, x + kx);
                    gradX += pixel * sobelX[ky + 1][kx + 1];
                    gradY += pixel * sobelY[ky + 1][kx + 1];
                }
            }
            dst.at<float>(y, x) = std::sqrt(gradX * gradX + gradY * gradY);
        }
    }

    cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX);
    dst.convertTo(dst, CV_8U);
}

/**
 * @brief Apply Scharr edge detection manually.
 * 
 * @param src Input grayscale image.
 * @param dst Output image after Scharr edge detection.
 */
void scharrEdgeDetection(const cv::Mat& src, cv::Mat& dst) {
    int scharrX[3][3] = {{-3, 0, 3}, {-10, 0, 10}, {-3, 0, 3}};
    int scharrY[3][3] = {{-3, -10, -3}, {0, 0, 0}, {3, 10, 3}};

    dst = cv::Mat::zeros(src.size(), CV_32F);

    for (int y = 1; y < src.rows - 1; ++y) {
        for (int x = 1; x < src.cols - 1; ++x) {
            float gradX = 0, gradY = 0;

            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    int pixel = src.at<uchar>(y + ky, x + kx);
                    gradX += pixel * scharrX[ky + 1][kx + 1];
                    gradY += pixel * scharrY[ky + 1][kx + 1];
                }
            }
            dst.at<float>(y, x) = std::sqrt(gradX * gradX + gradY * gradY);
        }
    }

    cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX);
    dst.convertTo(dst, CV_8U);
}

/**
 * @brief Apply Laplace edge detection manually.
 * 
 * @param src Input grayscale image.
 * @param dst Output image after Laplace edge detection.
 */
void laplaceEdgeDetection(const cv::Mat& src, cv::Mat& dst) {
    int laplaceKernel[3][3] = {{0, 1, 0}, {1, -4, 1}, {0, 1, 0}};

    dst = cv::Mat::zeros(src.size(), CV_32F);

    for (int y = 1; y < src.rows - 1; ++y) {
        for (int x = 1; x < src.cols - 1; ++x) {
            float laplace = 0;
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    laplace += src.at<uchar>(y + ky, x + kx) * laplaceKernel[ky + 1][kx + 1];
                }
            }
            dst.at<float>(y, x) = std::abs(laplace);
        }
    }

    cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX);
    dst.convertTo(dst, CV_8U);
}

/**
 * @brief Manually apply Sobel operator to compute gradients.
 * 
 * @param src Input grayscale image.
 * @param gradX Output gradient in the x-direction.
 * @param gradY Output gradient in the y-direction.
 */
void manualSobel(const cv::Mat& src, cv::Mat& gradX, cv::Mat& gradY) {
    int sobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int sobelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    
    gradX = cv::Mat::zeros(src.size(), CV_32F);
    gradY = cv::Mat::zeros(src.size(), CV_32F);

    for (int y = 1; y < src.rows - 1; ++y) {
        for (int x = 1; x < src.cols - 1; ++x) {
            float gx = 0.0, gy = 0.0;
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    int pixel = src.at<uchar>(y + ky, x + kx);
                    gx += pixel * sobelX[ky + 1][kx + 1];
                    gy += pixel * sobelY[ky + 1][kx + 1];
                }
            }
            gradX.at<float>(y, x) = gx;
            gradY.at<float>(y, x) = gy;
        }
    }
}

/**
 * @brief Apply Canny edge detection manually.
 * 
 * @param src Input grayscale image.
 * @param dst Output image after Canny edge detection.
 * @param lowThreshold Lower threshold for edge detection.
 * @param highThreshold Upper threshold for edge detection.
 */
void cannyEdgeDetection(const cv::Mat& src, cv::Mat& dst, int lowThreshold, int highThreshold) {
    cv::Mat blurred, gradX, gradY, gradMag, nonMaxSuppressed, edges;
    
    /* Apply Gaussian blur to reduce noise */
    gaussianBlur(src, blurred, 5, 1.4);

    /* Compute gradients using Sobel filters */
    cv::Sobel(blurred, gradX, CV_32F, 1, 0, 3);
    cv::Sobel(blurred, gradY, CV_32F, 0, 1, 3);

    /* Compute gradient magnitude */
    cv::magnitude(gradX, gradY, gradMag);

    /* Non-maximum suppression */
    nonMaxSuppressed = cv::Mat::zeros(gradMag.size(), gradMag.type());
    for (int y = 1; y < gradMag.rows - 1; ++y) {
        for (int x = 1; x < gradMag.cols - 1; ++x) {
            float angle = std::atan2(gradY.at<float>(y, x), gradX.at<float>(y, x)) * 180.0 / CV_PI;
            angle = angle < 0 ? angle + 180 : angle;

            /* Check for non-maximum suppression based on gradient direction */
            if (((angle >= 0 && angle < 22.5) || (angle >= 157.5 && angle < 180)) && 
                gradMag.at<float>(y, x) > gradMag.at<float>(y, x - 1) && gradMag.at<float>(y, x) > gradMag.at<float>(y, x + 1)) {
                nonMaxSuppressed.at<float>(y, x) = gradMag.at<float>(y, x);
            } else if ((angle >= 22.5 && angle < 67.5) && 
                gradMag.at<float>(y, x) > gradMag.at<float>(y - 1, x + 1) && gradMag.at<float>(y, x) > gradMag.at<float>(y + 1, x - 1)) {
                nonMaxSuppressed.at<float>(y, x) = gradMag.at<float>(y, x);
            } else if ((angle >= 67.5 && angle < 112.5) && 
                gradMag.at<float>(y, x) > gradMag.at<float>(y - 1, x) && gradMag.at<float>(y, x) > gradMag.at<float>(y + 1, x)) {
                nonMaxSuppressed.at<float>(y, x) = gradMag.at<float>(y, x);
            } else if ((angle >= 112.5 && angle < 157.5) && 
                gradMag.at<float>(y, x) > gradMag.at<float>(y - 1, x - 1) && gradMag.at<float>(y, x) > gradMag.at<float>(y + 1, x + 1)) {
                nonMaxSuppressed.at<float>(y, x) = gradMag.at<float>(y, x);
            }
        }
    }

    /* Apply double thresholding and edge tracking */
    edges = cv::Mat::zeros(src.size(), CV_8U);
    for (int y = 0; y < nonMaxSuppressed.rows; ++y) {
        for (int x = 0; x < nonMaxSuppressed.cols; ++x) {
            if (nonMaxSuppressed.at<float>(y, x) >= highThreshold) {
                edges.at<uchar>(y, x) = 255;
            } else if (nonMaxSuppressed.at<float>(y, x) >= lowThreshold) {
                edges.at<uchar>(y, x) = 128;
            }
        }
    }

    /* Hysteresis: Link weak edges (128) to strong edges (255) */
    for (int y = 1; y < edges.rows - 1; ++y) {
        for (int x = 1; x < edges.cols - 1; ++x) {
            if (edges.at<uchar>(y, x) == 128) {
                if (edges.at<uchar>(y - 1, x - 1) == 255 || edges.at<uchar>(y - 1, x) == 255 || edges.at<uchar>(y - 1, x + 1) == 255 ||
                    edges.at<uchar>(y, x - 1) == 255 || edges.at<uchar>(y, x + 1) == 255 ||
                    edges.at<uchar>(y + 1, x - 1) == 255 || edges.at<uchar>(y + 1, x) == 255 || edges.at<uchar>(y + 1, x + 1) == 255) {
                    edges.at<uchar>(y, x) = 255;
                } else {
                    edges.at<uchar>(y, x) = 0;
                }
            }
        }
    }

    dst = edges;
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

    cv::Mat sobelResult, scharrResult, laplaceResult, cannyResult;

    /* Apply Sobel, Scharr, Laplace, and Canny edge detection */
    sobelEdgeDetection(image, sobelResult);
    scharrEdgeDetection(image, scharrResult);
    laplaceEdgeDetection(image, laplaceResult);
    cannyEdgeDetection(image, cannyResult, 75, 150);

    /* Save the results */
    cv::imwrite("output/EdgeDetection-Sobel.jpg", sobelResult);
    cv::imwrite("output/EdgeDetection-Scharr.jpg", scharrResult);
    cv::imwrite("output/EdgeDetection-Laplace.jpg", laplaceResult);
    cv::imwrite("output/EdgeDetection-Canny.jpg", cannyResult);

    /* Show the results */
    cv::imshow("Original", image);
    cv::imshow("Sobel Edge Detection", sobelResult);
    cv::imshow("Scharr Edge Detection", scharrResult);
    cv::imshow("Laplace Edge Detection", laplaceResult);
    cv::imshow("Canny Edge Detection", cannyResult);
    cv::waitKey(0);

    return 0;
}
