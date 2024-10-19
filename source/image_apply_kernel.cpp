#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

/**
 * @brief Applies a convolution to a grayscale image using a specified filter kernel.
 * 
 * This function performs convolution by sliding a kernel over the input image and 
 * computing the weighted sum of pixels under the kernel.
 * 
 * @param src The input grayscale image (type CV_8UC1).
 * @param kernel The filter kernel (2D matrix) to apply during convolution.
 * @param dst The output image after applying the filter.
 */
void applyFilter(const cv::Mat& src, const cv::Mat& kernel, cv::Mat& dst) {
    /* Ensure the input is a grayscale image */
    CV_Assert(src.type() == CV_8UC1);
    
    /* Create the output image with the same size as input */
    dst = cv::Mat::zeros(src.size(), src.type());

    /* Get kernel size and calculate padding */
    int kRows = kernel.rows;
    int kCols = kernel.cols;
    int padX = kCols / 2;
    int padY = kRows / 2;

    /* Apply zero padding to the input image */
    cv::Mat padded;
    cv::copyMakeBorder(src, padded, padY, padY, padX, padX, cv::BORDER_CONSTANT, 0);

    /* Convolution operation */
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            /* Apply kernel at position (i, j) */
            float pixel_value = 0.0f;
            for (int m = 0; m < kRows; ++m) {
                for (int n = 0; n < kCols; ++n) {
                    int image_x = i + m;
                    int image_y = j + n;
                    pixel_value += padded.at<uchar>(image_x, image_y) * kernel.at<float>(m, n);
                }
            }
            dst.at<uchar>(i, j) = cv::saturate_cast<uchar>(pixel_value);
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

    /* Define filter kernels */
    std::vector<std::pair<std::string, cv::Mat>> kernels = {
        {"Sharpen", (cv::Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0)},
        {"EdgeDetection", (cv::Mat_<float>(3, 3) << -1, -1, -1, -1, 8, -1, -1, -1, -1)},
        {"GaussianBlur", (cv::Mat_<float>(3, 3) << 1, 2, 1, 2, 4, 2, 1, 2, 1) / 16.0f},
        {"SobelX", (cv::Mat_<float>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1)},
        {"SobelY", (cv::Mat_<float>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1)}
    };

    /* Loop through each kernel and apply it to the image */
    for (const auto& kernel_pair : kernels) {
        cv::Mat filtered_image;
        applyFilter(image, kernel_pair.second, filtered_image);

        /* Display the filtered image */
        cv::imshow(kernel_pair.first, filtered_image);

        /* Save the result */
        std::string output_filename = "output/ApplyKernel-" + kernel_pair.first + ".jpg";
        cv::imwrite(output_filename, filtered_image);
    }

    /* Display the original image for comparison */
    cv::imshow("Original Image", image);

    /* Wait for a key press to close the windows */
    cv::waitKey(0);

    return 0;
}
