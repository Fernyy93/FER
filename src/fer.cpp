#include "fer.h"

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

void read_yaml(std::vector<std::pair<std::string, cv::Mat>>& images, const char* filename){
	cv::FileStorage fs(filename, cv::FileStorage::READ);
	if(!fs.isOpened()){
		std::cerr << "failed to open" << filename << std::endl;		
	}
	cv::FileNode n = fs["images"];
	if (n.type() != cv::FileNode::SEQ){
		std::cerr << "Images is not a sequence! FAIL" << std::endl;		
	}

	std::cout << "reading images" << std::endl;

	for (cv::FileNodeIterator fit = n.begin(); fit != n.end(); ++fit){
		cv::FileNode item = *fit; //dereference the node pointer
		//Image image((string)item["Image_path"], (string)item["Emotion"]);		
		std::string emotion = (std::string)item["Emotion"];
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
	fs << "images" << "[";
	for (const auto& hist : histograms){
		fs << "{" << "Emotion" << hist.first << "Matrix" << hist.second << "}";
	}
	fs << "]";
	fs.release();
}