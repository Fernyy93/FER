#include <lbp.h>

int get_dir(const char* path, std::vector<std::string>& files) {

	// THIS IS A REALLY BAD WAY TO DO IT IN C++

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

void read_csv(const std::string& filename, std::vector<cv::Mat>& images) {

	std::ifstream file(filename.c_str(), std::ifstream::in);
	if(!file) {
		std::string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
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

void get_images(const char* emotions_dir, std::vector<std::string>& labels, std::map<std::string, std::vector<cv::Mat> >& images_map){
	// vector of filenames in emotions dir, contains 8 files
	// this is probably an extremely inefficient way of doing this until I 
	// figure out how to overwrite vectors properly
	std::vector<std::string> files = {};
	get_dir(emotions_dir, files);
	
	for (int i = 0; i < labels.size(); i++){
		std::vector<cv::Mat> images;
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

void plot_hist(std::vector<cv::Mat>& hist, int num) {

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
	
	neighbors = std::max(std::min(neighbors,31),1); // set bounds to between 1 and 31
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

void getHist(std::vector<std::string>& labels, std::map<std::string, std::vector<cv::Mat> >& images_map, std::map<std::string, std::vector<cv::Mat> >& histogram_map){
	
	for (int i = 0; i < labels.size(); i++){
		cv::Mat dst;
		std::vector<cv::Mat> histograms;
		//std::vector<cv::Mat> src = images_map[labels[i]];
		for (int j = 0; j < images_map[labels[i]].size(); j++){
			ELBP(images_map[labels[i]][j], dst, 1, 8);
			//std::cout << "(" << i << "," << j << "): " << images_map[labels[i]][j].rows << std::endl;
			histograms.push_back(dst);
		}
		histogram_map.insert({labels[i], histograms});
	}
}

void dispImage(std::string title, cv::Mat& image){
	
	cv::namedWindow(title, CV_WINDOW_AUTOSIZE);
	cv::imshow(title, image);
	cv::waitKey(0);
}