// Feature Extraction.cpp. Reads the images_yaml file and extracts features then writes to the histogram_yaml file
#include "lbp.h"
#include "image.h"

void read_yaml(std::vector<std::pair<std::string, cv::Mat>>& images, const char* filename){
	cv::FileStorage fs(filename, cv::FileStorage::READ);
	if(!fs.isOpened()){
		std::cerr << "failed to open" << filename << std::endl;		
	}
	cv::FileNode n = fs["images"];
	if (n.type() != cv::FileNode::SEQ){
		std::cerr << "Images is not a sequence! FAIL" << std::endl;		
	}

	cout << "reading images" << std::endl;

	for (cv::FileNodeIterator fit = n.begin(); fit != n.end(); ++fit){
		cv::FileNode item = *fit; //dereference the node pointer
		//Image image((string)item["Image_path"], (string)item["Emotion"]);		
		string emotion = (string)item["Emotion"];
		cv::Mat mat;
		item["Matrix"] >> mat;
		images.push_back(std::pair<std::string, cv::Mat>(emotion, mat));
	}
}

void get_Hist(std::vector<std::pair<std::string, cv::Mat>>& images_vector, std::vector<std::pair<std::string, cv::Mat>>& histograms){
	for (const auto& im : images_vector){
		cv::Mat hist;
		ELBP(im.second, hist, 1, 8);
		if (hist.type() != CV_32F){
			hist.convertTo(hist, CV_32F);
		}
		hist = hist.reshape(1,1);
		histograms.push_back(std::pair<std::string, cv::Mat>(im.first, hist));
	}
}

void write_yaml(std::vector<std::pair<std::string, cv::Mat>>& histograms, const char* filename){
	cv::FileStorage fs(filename, cv::FileStorage::WRITE);
	if (!fs.isOpened()){
		std::cerr << "failed to open" << filename << std::endl;
	}
	fs << "histograms" << "[";
	for (const auto& hist : histograms){
		fs << "{" << "emotion" << hist.first << "matrix" << hist.second << "}";
	}
	fs << "]";
	fs.release();
}

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
// why is my code returning empty images and histogram vectors?