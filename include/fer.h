#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/face.hpp>
#include <opencv2/plot.hpp>
#include <opencv2/ml.hpp>
#include <stdint.h>
#include <filesystem>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string.h>

// Extended local binary pattern feature extraction
void ELBP(const cv::Mat& src, cv::Mat& dst, int radius, int neighbors);

// Read yaml files
void read_yaml(std::vector<std::pair<std::string, cv::Mat>>& images, const char* filename);

// convert image to histogram (feature extraction)
void get_Hist(std::vector<std::pair<std::string, cv::Mat>>& images_vector, std::vector<std::pair<std::string, cv::Mat>>& histograms);

// Write yaml files
void write_yaml(std::vector<std::pair<std::string, cv::Mat>>& histograms, const char* filename);

