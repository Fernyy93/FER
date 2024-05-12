// File: Image.cpp
#include "image.h"


// lessons learned. class definitions end with a semicolon. Need to have different variable names for constructor definition.
Image::Image(int width, int height, std::string img_name, int type, std::string img_emotion) : 
	width(width), height(height), image_name(img_name), type(type), emotion(img_emotion), image(cv::Mat::eye(width, height, type)){
	// add additional constructor logic
}

void Image::setWidth(int width){
	this->width = width;
}

void Image::setHeight(int height){
	this->height = height;
}

void Image::setType(int type){
	this->type = type;
}

void Image::set_emotion(std::string emotion){
	this->emotion = emotion;
}

int Image::getWidth(){
	return this->width;
}

int Image::getHeight(){
	return this->height;
}

int Image::getType(){
	return this->type;
}

std::string Image::getEmotion(){
	return this->emotion;
}

void Image::print(){
	std::cout << image << std::endl;
}

int main(){
	Image my_image(5, 5, "image.png", CV_32F, "happy");
	my_image.print();
	return 0; 
}