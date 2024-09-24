# Build directory
BUILD_DIR = build
OpenCV_BUILD_DIR = third-party/opencv/build

# Executable
TARGET = $(BUILD_DIR)/Lab1

# For cmake
OpenCV_DIR = ../third-party/opencv/build/install/lib/cmake/opencv4

# Default image path (can be overridden by passing IMAGE when running make run)
IMAGE ?= example.jpg

default: $(TARGET)

# Install dependancies (opencv)
dependencies:
	git submodule update --init --recursive
	mkdir -p $(OpenCV_BUILD_DIR) && cd $(OpenCV_BUILD_DIR) && cmake .. && make install

# Build project
$(TARGET):
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DOpenCV_DIR=$(OpenCV_DIR) ..
	$(MAKE) -C $(BUILD_DIR)

# Run the program
run: $(TARGET)
	$(TARGET) $(IMAGE)

# Clean build directory
clean:
	rm -rf $(BUILD_DIR)

.PHONY: default run clean
