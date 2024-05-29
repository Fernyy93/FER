#define _USE_MATH_DEFINES
#include <cmath>
#include "lbp.h"
#include "image.h"

using namespace cv;
using namespace std;

int get_dir(const char* path, std::vector<std::string>& files){
	// start from basics.... I had a bunch of images in unorganised folders, and I want them organised by emotions
	// need to work out why this function is necessary
	struct dirent *entry;
	DIR *dp;
	
	if ((dp = opendir(path)) != NULL){
		while((entry = readdir(dp)) != NULL){
			if (std::string(entry->d_name)[0] != '.'){
				std::cout << std::string(entry->d_name) << std::endl;
				files.push_back(std::string(entry->d_name));
			}
			
		}
		closedir (dp);
	} else {
		perror("");
		return EXIT_FAILURE;
	}

	return 0;
}

void read_csv(const std::string& filename, std::vector<cv::Mat>& images){
// function reads a path to an image from a csv and adds the images at that path to images vector
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if(!file) {
		std::string error_message = "No valid input file was given, please check the given filename.";
		cv::Error::StsBadArg;
	}

	std::string line;
	std::string path;
	

	while(std::getline(file, line)) {
		std::stringstream liness(line);
		std::getline(liness, path, ';');
		//getline(liness, classlabel);
		if (!path.empty()){
			images.push_back(cv::imread(path,0));
		 	//nums.push_back(atoi(classlabel.c_str()));
		}
	}
}


void get_images_json(const char* json_file, std::vector<std::pair<std::string, Image>>& images_vector){
// objective is to read the json file, add the image to the map according to its emotion
	// 1. read the json file and check it exists
	std::cout << "Input json file: " << json_file << std::endl;
	std::ifstream file(json_file);
	if (!file){
		std::string error_message = "No valid input file was given, please check the given filename.";
		cv::Error::StsBadArg;
	}
	//std::vector<std::pair<std::string, Image> images_vector; // return this by passing by reference
	json data = json::parse(file);
	for (const auto& element : data){
		std::string emotion = element.at("Emotion");
		std::string filename = element.at("Location");
		Image image(filename, emotion);
		// big architectural change... make a map std::map<std::string, Image> or just a vector of Images
		// the SVM needs two 1 dimensional data structures so it makes sense to dump the images into a 2 dimensional ordered data structure
		// this means the keys are the labels and the values can be processed into histograms
		// No, maps can only have 1 value associated with the key.
		// but I can push a pair into a vector
		images_vector.push_back(std::pair<std::string, Image>(emotion, image));
	}



}



void get_images(const char* emotions_dir, std::vector<std::string>& labels, std::map<std::string, std::vector<cv::Mat>>& images_map){
	// vector of filenames in emotions dir, contains 8 files
	// this is probably an extremely inefficient way of doing this until I 
	// figure out how to overwrite vectors properly
	std::cout << emotions_dir << std::endl;
	
	std::vector<std::string> files = {};
	// I want to get the filename of each .csv file in the directory
	get_dir(emotions_dir, files);
	
	// for each emotion
	for (int i = 0; i < labels.size(); ++i){
		std::vector<cv::Mat> images;
		// the directory contains the emotion
		std::string fn_csv = std::string(emotions_dir) + "/" + files[i];
		std::cout << "file containing image paths for " << labels[i] << ": " << files[i] << std::endl;
		std::cout << "the file name is: " << fn_csv << std::endl;

		try {
			read_csv(fn_csv, images);
		} catch (cv::Exception& e){
			std::cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << std::endl;
			exit(1);
		}

		images_map.insert({labels[i], images});
		
	}
	

}

void plot_hist(std::vector<cv::Mat>& hist, int num){

	cv::Mat hist_plot;
	cv::Mat plot_result;
	hist[num].convertTo(hist_plot, CV_64F);
	std::cout << "Histogram type is: " << hist_plot.type() << std::endl;

	cv::Ptr<cv::plot::Plot2d> plot = cv::plot::Plot2d::create(hist_plot*-1);
	plot->setPlotBackgroundColor(cv::Scalar(50, 50, 50));
    plot->setPlotLineColor (cv::Scalar(50, 50, 255));
    plot->render(plot_result);

    cv::imshow("plot", plot_result);
    cv::waitKey();
}

void OLBP(const cv::Mat& src, cv::Mat& dst){
	dst = cv::Mat::zeros(src.rows-2, src.cols-2, CV_8UC1);
	for (int i = 1; i < src.rows-1; i++){
		for (int j = 1; j < src.cols-1; j++){
			unsigned char c = src.at<unsigned char>(i,j);
			unsigned char code = 0;
			code |= (src.at<unsigned char>(i-1, j-1) > c) << 7;
			code |= (src.at<unsigned char>(i-1, j) > c) << 6;
			code |= (src.at<unsigned char>(i-1, j+1) > c) << 5;
			code |= (src.at<unsigned char>(i, j+1) > c) << 4;
			code |= (src.at<unsigned char>(i+1, j+1) > c) << 3;
			code |= (src.at<unsigned char>(i+1, j) > c) << 2;
			code |= (src.at<unsigned char>(i+1, j-1) > c) << 1;
			code |= (src.at<unsigned char>(i, j-1) > c) << 0;
			dst.at<unsigned char>(i-1, j-1) = code;
		}
	}
}

void ELBP(const cv::Mat& src, cv::Mat& dst, int radius, int neighbors){
	
	//neighbors = std::max(std::min(neighbors,31),1); // set bounds to between 1 and 31
	// Might as well convert the resultant histogram to CV_32F and then reshape(1,1)
	dst = cv::Mat::zeros(src.rows-2*radius, src.cols-2*radius, CV_8UC1);
	for (int n = 0; n < neighbors; n++){
		// sample points
		float x = static_cast<float>(radius) * cos(2.0*M_PI*n/static_cast<float>(neighbors));
		float y = static_cast<float>(radius) * sin(2.0*M_PI*n/static_cast<float>(neighbors));
		// relative indices
		int fx = static_cast<int>(floor(x));
		int fy = static_cast<int>(floor(y));
		int cx = static_cast<int>(ceil(x));
		int cy = static_cast<int>(ceil(y));
		// fractional part
		float ty = y - fy;
		float tx = x - fx;
		// set interpolation weights
		float w1 = (1-tx) * (1-ty);
		float w2 =    tx  * (1-ty);
		float w3 = (1-tx) *    ty;
		float w4 =    tx  *    ty;
		// iterate over image
		for (int i = radius; i < src.rows-radius; i++){
			for (int j = radius; j < src.cols-radius; j++){
				// for grayscale images each pixel is of size unsigned char
				float t = w1*src.at<unsigned char>(i+fy, j+fx) + w2*src.at<unsigned char>(i+fy, j+cx) + w3*src.at<unsigned char>(i+cy, j+fx) + w4*src.at<unsigned char>(i+cy, j+cx);
				// we are dealing with floating point precision, so add some little tolerance
				dst.at<unsigned char>(i-radius, j-radius) += ((t > src.at<unsigned char>(i,j)) && (abs(t-src.at<unsigned char>(i,j)) > std::numeric_limits<float>::epsilon())) << n;
			}
		}
	}
}

// I am passing an empty cv::Mat training_data and I get a matrix of 311344 x 1 and also passing an empty cv::Mat labelMat but only getting a matrix of 1x1.
void getHist(std::vector<std::string>& labels, std::map<std::string, std::vector<cv::Mat> >& images_map, cv::Mat& training_data, cv::Mat& labelMat){
	// for each label
	for (int i = 0; i < labels.size(); i++){
		cv::Mat dst;
		std::vector<cv::Mat> histograms;
		//std::vector<cv::Mat> src = images_map[labels[i]];
		// make a histogram of each image associated with that label
		for (int j = 0; j < images_map[labels[i]].size(); j++){
			// I should standardise the image size first.
			//std::cout << "(" << images_map[labels[i]][j].rows << ", " << images_map[labels[i]][j].cols << ")" << std::endl;
			ELBP(images_map[labels[i]][j], dst, 1, 8);
			//std::cout << "(" << i << "," << j << "): " << images_map[labels[i]][j].rows << std::endl;
			// move the below conversion and reshape code to ELBP.
			if (dst.type() != CV_32F){
				dst.convertTo(dst,CV_32F);
			}
			dst = dst.reshape(1,1);
			// COncatenate the reshaped image Mat into training_data
			if (training_data.empty()){
				training_data = dst.clone();
			} else {
				cv::vconcat(training_data, dst, training_data);
			}
			// concatenate the label as an integer into labelMat
			if (labelMat.empty()){
				labelMat = cv::Mat(1, 1, CV_32SC1, cv::Scalar(i));

			} else {
				cv::Mat label(1,1, CV_32SC1, cv::Scalar(i));
				cv::vconcat(labelMat, label, labelMat);
			} 

			
		}
		//histogram_map.insert({labels[i], histograms});
	}
}

void get_Hist_json(cv::Mat& training_data, cv::Mat& labelMat,  std::vector<std::pair<std::string, Image>>& images_vector, std::map<std::string, int>& emotions){
	

	for (const auto& im : images_vector){
		cv::Mat hist;
		ELBP(im.second.getImage(), hist, 1, 8);
		if (hist.type() != CV_32F){
			hist.convertTo(hist, CV_32F);
		}
		hist = hist.reshape(1,1);
		if (training_data.empty()){
			training_data = hist.clone();
		} else {
			cv::vconcat(training_data, hist, training_data);
		}
		if (labelMat.empty()){
			labelMat = cv::Mat(1, 1, CV_32SC1, emotions[im.first]);
		} else {
			cv::Mat label(1, 1, CV_32SC1, emotions[im.first]);
			cv::vconcat(labelMat, label, labelMat);
		}
	}
}

void dispImage(std::string title, cv::Mat& image){
	
	cv::namedWindow(title, cv::WINDOW_AUTOSIZE);
	cv::imshow(title, image);
	cv::waitKey(0);
}

// the image mat should be the input
void rescale(const char* emotions_dir){
	

	// I want a map of histograms by emotion label eventually	
	std::cout << emotions_dir << std::endl;

	// later do a matrix of vectors? one vector for each emotion
	//vector<Mat> images;
	
	std::vector<std::string> labels =  {"anger", "contempt", "disgust", "fear", "happy", "neutral", "sadness", "surprise"};
	
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
		for (int j = 0; j < images_map[labels[i]].size(); j++){
			//std::cout << "Old size: " << "(" << images_map[labels[i]][j].rows << ", " << images_map[labels[i]][j].cols << ")" << std::endl;
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
			//std::cout << "New size: " << "(" << resizedImage.rows << ", " << resizedImage.cols << ")" << std::endl;
			// make the new path and write the resized image to that path
			std::ostringstream builder;
			builder << "../" << "new/" << labels[i] << "/" << j << ".png";
			//std::cout << "Image written to: " << builder.str() << std::endl;
			cv::imwrite(builder.str(), resizedImage);
			

		}
	}
}

