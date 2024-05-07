#include <opencv2/opencv.hpp>
#include "lbp.h"

int main(int argc, const char *argv[]) {
	std::cout << "hello world" << std::endl;
	if (argc != 2){
		std::cout << "usage: " << argv[0] << " <emotions dir> " << std::endl;
		return 1;
	}

	// I want a map of histograms by emotion label eventually

	
	const char* emotions_dir = argv[1];
	std::cout << emotions_dir << std::endl;

	// later do a matrix of vectors? one vector for each emotion
	//vector<Mat> images;
	
	std::vector<std::string> labels =  {"anger", "contempt", "disgust", "fear", "happy", "neutral", "sadness", "surprise"};
	std::map<string, int> emotions;
	

	int i = 0;
	
	

	std::map<std::string, std::vector<cv::Mat>> images_map;							            
	//std::map<std::string, std::vector<cv::Mat>> images_map;
	
	// loop through all images for each emotion, resize the image, then save to a new directory.
	// then write a csv containing the new locations of the resized images.
	get_images(emotions_dir, labels, images_map);
	// define desired size and aspect ratio
	int newWidth = 640;
	int newHeight = 490;
	double aspectRatio = (double)newWidth / (double)newHeight;

	

	for (int i = 0; i < labels.size(); i++){
		for (int j = 0; j < images_map[labels[i]].size; j++){
			std::cout << "Old size: " << "(" << images_map[labels[i]][j].rows << ", " << images_map[labels[i]][j].cols << ")" std::endl;
			cv::Mat resizedImage;
			if (images_map[labels[i]][j].rows != newHeight || images_map[labels[i]][j].cols != newWidth){
				int cropX = (images_map[labels[i]][j].cols - (images_map[labels[i]][j].rows * aspectRatio)) / 2;
				cv::Rect cropRect(cropX, 0, images_map[labels[i]][j].rows * aspectRatio, images_map[labels[i]][j].rows);
				// crop the image
				cv::Mat croppedImage = images_map[labels[i]][j](cropRect);
				// Resize the image to the new size
				cv::resize(croppedImage, resizedImage, cv::Size(newWidth, newHeight));
			} else {
				resizedImage = images_map[labels[i]][j].clone();
			}
			// make the new path and write the resized image to that path
			std::ostringstream builder;
			builder << labels[i] << "/" << j << .png;
			cv::imwrite(builder, resizedImage);
			

		}
	}
}