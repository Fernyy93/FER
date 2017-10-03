#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/face.hpp>
#include <opencv2/plot.hpp>
#include <stdint.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <string.h>

// access the directory containing the csv files of the locations of each image for each emotion
int get_dir(const char* path, std::vector<std::string>& files);

// reads each csv file and stores the image at each image location in a vector and also the image number
void read_csv(const std::string& filename, std::vector<cv::Mat>& images);

// make labels to images map
void get_images(const char* emotions_dir, std::vector<std::string>& labels, std::map<std::string, std::vector<cv::Mat> >& images_map);

// plots a desired histogram
void plot_hist(std::vector<cv::Mat>& hist, int num);

// Orignal LBP
void OLBP(const cv::Mat& src, cv::Mat& dst);

// Eliptical LBP feature extraction
void ELBP(const cv::Mat& src, cv::Mat& dst, int radius = 1, int neighbors = 8);

// storing LBP features in histograms
void getHist(std::vector<std::string>& labels, std::map<std::string, std::vector<cv::Mat> >& images_map, std::map<std::string, std::vector<cv::Mat> >& histogram_map);

// simply displays an image in a window with a given title
void dispImage(std::string title, cv::Mat& image);

