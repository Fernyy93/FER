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
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>

using namespace std;


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
	void write(cv::FileStorage& fs, const std::string filename, std::string label);
	void read(const cv::FileNode& node);
	
	const cv::Mat getImage() const;
	std::string getEmotion();
	
};

#endif