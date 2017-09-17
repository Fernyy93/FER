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

int get_dir( const char* path, vector<string>& files );

void read_csv(const string& filename, vector<Mat>& images, 
	vector<int>& nums);