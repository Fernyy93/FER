// Feature Extraction.cpp. Reads the images_yaml file and extracts features then writes to the histogram_yaml file
#include "image.h"
#include "fer.h"

int main(int argc, const char* argv[]){
	if (argc != 3){
        std::cerr << "Usage: " << argv[0] << "<Images Yaml File path>" << "<desired histogram Yaml File path>" << std::endl;        
        return 1;
    }
	std::vector<std::pair<std::string, cv::Mat>> images;
	std::vector<std::pair<std::string, cv::Mat>> histograms;
	read_yaml(images, argv[1]);
	std::cout << "Number of images: " << images.size() << std::endl;
	get_Hist(images, histograms);
	std::cout << "Number of histograms: " << histograms.size() << std::endl;
	write_yaml(histograms, argv[2]);
	return 0;
}