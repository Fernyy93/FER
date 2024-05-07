// how do display a single histogram from the histogram map?
#include "lbp.h"

using namespace std;
using namespace cv;
using namespace cv::ml;

int main(int argc, const char *argv[]){
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
	
	for (const auto &label : labels){
		emotions[label] = i;
		i++;
		std::cout << emotions[label] << std::endl;
	}
	

	std::map<std::string, std::vector<cv::Mat>> images_map;							            
	//std::map<std::string, std::vector<cv::Mat>> images_map;
	//rescale(emotions_dir);
	
	// go through the emotions dir and add the images to the images map
	get_images(emotions_dir, labels, images_map);
	
	std::cout << images_map.size() << std::endl;
	std::vector<cv::Mat> thing = images_map[labels[1]];
	std::cout << thing.size() << std::endl;
	std::cout << images_map[labels[1]].size() << std::endl;
	std::cout << "type of image: " << thing[0].type() << std::endl;
	
	

	// this is even more streamlined
	
	
	// cv::SVM takes labels as integers
	std::vector<int> intLabels;

	cv::Mat training_data;
	cv::Mat labelMat;
	//std::map<std::string, std::vector<cv::Mat> > histogram_map;
	getHist(labels, images_map, training_data, labelMat);
	
	std::cout << "Training data size: (" << training_data.rows << ", " << training_data.cols << ")" << std::endl;
	std::cout << "Label Mat size: (" << labelMat.rows << ", " << labelMat.cols << ")" << std::endl;
	
	
	// Visually represent the support vectors
	int width = 512;
	int height = 512;
	cv::Mat sv = cv::Mat::zeros(height, width, CV_8UC3);

	// predeclare variables for cropping captured image to standard size.
	int testWidth = 640;
	int testHeight = 490;
	double test_aspectRatio = (double)testWidth / (double)testHeight;
	cv::Mat coloured_image;
	cv::Mat test_resizedImage;
	

	// open the first webcame plugged into the computer, raise an error if camera undetected
	cv::VideoCapture camera(0);
	if (!camera.isOpened()){
		std::cout << "Error: Could not open camera" << std::endl;
		return 1;
	}

	// create a window to display the images from the webcam
	cv::namedWindow("Webcam", WINDOW_AUTOSIZE);

	// this will contain the image from the webcame
	cv::Mat frame;
	while(1){
		// capture frame by frame
		camera >> frame;

		if (frame.empty())
			break;

		// display the resulting frame
		imshow("Frame", frame);

		// press esc on keyboard to exit
		char c = (char)waitKey(1);
		if (c==27){		
			int cropX = (frame.cols - (frame.rows * test_aspectRatio)) / 2;
			cv::Rect cropRect(cropX, 0, frame.rows * test_aspectRatio, frame.rows);
			cv::Mat test_cropped = frame(cropRect);
			cv::resize(test_cropped, coloured_image, cv::Size(testWidth, testHeight));
			cv::cvtColor(coloured_image, test_resizedImage, cv::COLOR_BGR2GRAY);

			cv::imwrite("../test/test_image.png", test_resizedImage);

			break;
		}
	}

	// release the video capture object
	camera.release();

	
	// close all the frames
	cv::destroyAllWindows();


	cv::Mat testHist;
	ELBP(test_resizedImage, testHist, 1, 8);

	std::cout << "Width before reshape: " << testHist.cols << std::endl;
	std::cout << "Height before reshape: " << testHist.rows << std::endl;



	testHist.convertTo(testHist, CV_32F);
	testHist = testHist.reshape(1,1);

	std::cout << "Width after reshape: " << testHist.cols << std::endl;
	std::cout << "Height after reshape: " << testHist.rows << std::endl;
	
	int var_count = training_data.cols;
	std::cout << "number of cols in test image: " << testHist.cols << std::endl;
	std::cout << "number of expected variables: " << var_count << std::endl;
	std::cout << "Histogram type: " << testHist.type() << std::endl;
	if ((var_count != testHist.cols) || (testHist.type() != CV_32F)){
		std::cout << "test image number of features does not match expected number of features" << std::endl;
		return 1;
	} else {
		std::cout << "model is ready to be trained and predicted" << std::endl;	
	}


	
	
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	svm->train(training_data, ROW_SAMPLE, labelMat);


	cv::Mat supportVectors = svm->getSupportVectors();
	float emotion = svm -> predict(testHist);
	std::cout << "Predicted emotion is: " << emotion << std::endl;
	

	// show the decision regions given by the svm
	// need to get a test image through the camera for this

	// show support vectors
	// still need to get a test imag

	std::cin.get();

	
	return 0;
}
