// File: Image.cpp
#include "image.h"
<<<<<<< HEAD

=======
>>>>>>> feature_extraction

// lessons learned. class definitions end with a semicolon. Need to have different variable names for constructor definition.
Image::Image(std::string img_location, std::string img_emotion){
	// read the image in the constructor or read it outside and save the properties?
	this->image = cv::imread(img_location, 0);
	this->emotion = img_emotion;
	this->image_location = img_location;
}

void Image::set_emotion(std::string emotion){
	this->emotion = emotion;
}

int Image::getWidth(){
	return this->image.rows;
}

int Image::getHeight(){
	return this->image.cols;
}

int Image::getType(){
	return this->image.type();
}

std::string Image::getEmotion(){
	return this->emotion;
}

void Image::print(){
	std::stringstream ss;
	ss << "Image location: " << this->image_location << "\n";
	ss << "Image width: " << this->getWidth() << "\n";
	ss << "Image Height: " << this->getHeight() << "\n";
	ss << "Image emotion: " << this->getEmotion() << "\n";
	std::cout << ss.str() << std::endl;
}
// don't forget Image::
void Image::write(cv::FileStorage& fs, const std::string filename, std::string label){
	fs << "{" << "filename" << filename << "emotion" << label << "data" << image << "}";
}

void Image::read(const cv::FileNode& node){
	node["emotion"] >> emotion;
	node["filename"] >> image_location;
	node["data"] >> image;
}



/*
json Image::create_json_object(){
 	json j = {
 		{"Location", this->image_location},
 		{"Width", this->getWidth()},
 		{"Height", this->getHeight()},
 		{"Emotion", this->getEmotion()}
 	};
 	/*
 	std::ofstream file("image_json_test.json");
 	if (file.is_open()){
 		file << std::setw(4) << j << std::endl;
 		file.close();
 	} else {
 		std::cerr << "Could not open file for writing" << std::endl;
 	}
	
	return j;
}
*/
const cv::Mat Image::getImage() const{
	return this->image;
}