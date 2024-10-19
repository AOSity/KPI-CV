# Build directory
BUILD_DIR = build
OpenCV_BUILD_DIR = third-party/opencv/build

# For cmake
OpenCV_DIR = ../third-party/opencv/build/install/lib/cmake/opencv4

# Default image path (can be overridden by passing IMAGE when running make run)
IMAGE ?= input/uni.jpg input/uni-in-the-box.jpg

# Build project
default:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DOpenCV_DIR=$(OpenCV_DIR) ..
	$(MAKE) -C $(BUILD_DIR)

# Install dependancies (opencv)
dependencies:
	git submodule update --init --recursive
	mkdir -p $(OpenCV_BUILD_DIR) && cd $(OpenCV_BUILD_DIR) && cmake .. && make install

# Opens image
open: default
	$(BUILD_DIR)/ImageOpen $(IMAGE)

# Histogram equilation
equalize_histogram: default
	$(BUILD_DIR)/ImageEqualizeHistogram $(IMAGE)

# Conditional scaling
scale_conditional: default
	$(BUILD_DIR)/ImageScaleConditional $(IMAGE)

# Apply kernels
apply_kernel: default
	$(BUILD_DIR)/ImageApplyKernel $(IMAGE)

# Smooth
smooth: default
	$(BUILD_DIR)/ImageSmooth $(IMAGE)

# Sharpen
sharp: default
	$(BUILD_DIR)/ImageSharpen $(IMAGE)

# Edge detection (Sobel, Shcarr, Laplace, Canny)
detect_edges: default
	$(BUILD_DIR)/ImageDetectEdges $(IMAGE)

# Edge detection (LoG, DoG)
detect_edges2: default
	$(BUILD_DIR)/ImageDetectEdges2 $(IMAGE)

# Clean build directory
clean:
	rm -rf $(BUILD_DIR)

.PHONY: default run clean
