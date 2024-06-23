// read yaml features into a histogram and hopefully they're already in the format of 1,X array.
#include "fer.h"

using namespace cv;
using namespace cv::ml;

int main(int argc, const char *argv[]){
	if (argc != 2){
		std::cerr << "Usage: " << argv[0] << "./classification <features yaml path>" << std::endl;
		return 1;
	}
	
	std::vector<std::pair<std::string, cv::Mat>> histograms;
	read_yaml(histograms, argv[1]);
	std::cout << "Number of features: " << histograms.size() << std::endl;

	cv::Mat training_data;
	cv::Mat labels;

	std::map<std::string, int> emotions;
	emotions["anger"] = 0;
	emotions["contempt"] = 1;
	emotions["disgust"] = 2;
	emotions["fear"] = 3;
	emotions["happy"] = 4;
	emotions["neutral"] = 5;
	emotions["sadness"] = 6;
	emotions["surprise"] = 7;

	for (const auto& hist : histograms){
		cv::Mat h = hist.second;
		//std::cout << h << std::endl;
		if (h.type() != CV_32F){
			h.convertTo(h, CV_32F);
		}
		//h = h.reshape(1,1);
		if (training_data.empty()){
			training_data = h.clone();
		} else {
			cv::vconcat(training_data, h, training_data);
		}

		if (labels.empty()){
			labels = cv::Mat(1,1, CV_32SC1, emotions[hist.first]);
		} else {
			cv::Mat label(1, 1, CV_32SC1, emotions[hist.first]);
			cv::vconcat(labels, label, labels);
		}
	}
	if (training_data.empty()){
		std::cout << "no training data, something went wrong" << std::endl;
		return 1;
	}
	std::cout << "Number of labels: " << labels.rows << std::endl;
	std::cout << "training_data width: " << training_data.cols << std::endl;
	std::cout << "training_data height: " << training_data.rows << std::endl;
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	svm->train(training_data, ROW_SAMPLE, labels);
	svm->save("../svms/svm.xml");
	return 0;
}