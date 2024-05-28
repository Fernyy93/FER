#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/face.hpp>
#include <opencv2/plot.hpp>
#include <opencv2/ml.hpp>
#include <stdint.h>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "../include/json.hpp"
#include <iomanip>

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

class Image {
private: 
	std::string image_location;
	std::string emotion;
	cv::Mat image;
	

public:
	Image(std::string img_location, std::string img_emotion);
	void print();
	
	void set_emotion(std::string emotion);

	int getWidth();
	int getHeight();
	int getType();
	const cv::Mat getImage() const;
	std::string getEmotion();
	json create_json_object();
};

#endif