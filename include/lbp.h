#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/face.hpp>
#include <opencv2/plot.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>

using namespace std;
using namespace cv;

// access the directory containing the csv files of the locations of each image for each emotion
int get_dir(const char* path, vector<string>& files);

// reads each csv file and stores the image at each image location in a vector and also the image number
void read_csv(const string& filename, vector<Mat>& images);

// make labels to images map
void get_images(const char* emotions_dir, vector<string>& labels, map<string, vector<Mat> >& images_map);

// plots a desired histogram
void plot_hist(vector<Mat>& hist, int num);

