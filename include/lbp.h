#ifndef LBP_H
#define LBP_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/face.hpp>
#include <opencv2/plot.hpp>
#include <opencv2/ml.hpp>
#include <stdint.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <string.h>
#include <map>`
#include "../include/json.h"

class Image;



// plots a desired histogram
void plot_hist(std::vector<cv::Mat>& hist, int num);

// Orignal LBP
void OLBP(const cv::Mat& src, cv::Mat& dst);

// Eliptical LBP feature extraction
void ELBP(const cv::Mat& src, cv::Mat& dst, int radius = 1, int neighbors = 8);
// storing LBP features in histograms
void getHist(std::vector<std::string>& labels, std::map<std::string, std::vector<cv::Mat> >& images_map, cv::Mat& training_data, cv::Mat& labelMat);

// simply displays an image in a window with a given title
void dispImage(std::string title, cv::Mat& image);

// rescales each image in the dataset to a constant height and width
void rescale(const char* emotions_dir);

// make label to images map from a json file
void get_images_json(const char* json_file, std::vector<std::pair<std::string, Image>>& images_vector);

// get the histograms using the pair method
void get_Hist_json(cv::Mat& training_data, cv::Mat& labelMat,  std::vector<std::pair<std::string, Image>>& images_vector, std::map<std::string, int>& emotions);

#endif