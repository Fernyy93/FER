// predict the emotion with a captured image

#include "fer.h"

using namespace cv;
using namespace cv::ml;

int main(int argc, const char* argv[]){
	int testWidth = 400;
	int testHeight = 400;
	double test_aspectRatio = (double(testWidth))/(double(testHeight));
	cv::Mat coloured_image;
	cv::Mat test_resizedImage;

	cv::VideoCapture camera(0);
	if (!camera.isOpened()){
		std::cout << "Error: Could not open camera" << std::endl;
		return 1;
	}

	// create a window to display the images from the webcam
	cv::namedWindow("Webcam", cv::WINDOW_NORMAL);

	// this will contain the image from the webcame
	cv::Mat frame;
	cv::Mat resized;
	cv::Mat gray;
	
	while(1){
		// capture frame by frame
		camera >> frame;

		if (frame.empty())
			break;

		// display the resulting frame
		imshow("Frame", frame);

		// press esc on keyboard to exit
		char c = (char)waitKey(0);
		if (c==27){
			
			cv::imwrite("../test/test_image.png", frame);
			
			cv::resize(frame, resized, cv::Size(400,400));
			
			cv::cvtColor(resized, gray, cv::COLOR_BGR2GRAY);
			//cv::Mat reshaped = gray.reshape(1,1);
			//std::cout << "Reshaped matrix: " << reshaped << std::endl;
			break;
			
		}
	}

	// release the video capture object
	camera.release();

	
	// close all the frames
	cv::destroyAllWindows();
	cv::Mat testHist;
	ELBP(gray, testHist, 1, 8);
	testHist = testHist.reshape(1,1);
	std::cout << "Width before reshape: " << gray.cols << std::endl;
	std::cout << "Height before reshape: " << gray.rows << std::endl;

	if (testHist.type() != CV_32F){
		testHist.convertTo(testHist, CV_32F);
	}	
	
	std::cout << "Width after reshape: " << testHist.cols << std::endl;
	std::cout << "Height after reshape: " << testHist.rows << std::endl;	
	

	Ptr<SVM> svm = SVM::load("../svms/svm.xml");

	int var_count = svm->getVarCount();
	std::cout << "number of cols in test image: " << testHist.cols << std::endl;
	std::cout << "number of expected variables: " << var_count << std::endl;
	std::cout << "Histogram type: " << testHist.type() << std::endl;
	if ((var_count != testHist.cols)){
		std::cout << "test image number of features does not match expected number of features" << std::endl;
		
	} else {
		std::cout << "model is ready to be predicted" << std::endl;	
	}
	
	int emotion = svm->predict(testHist);

	std::map<int, std::string> emotions;
	emotions[0] = "anger";
	emotions[1] = "contempt";
	emotions[2] = "disgust";
	emotions[3] = "fear";
	emotions[4] = "happy";
	emotions[5] = "neutral";
	emotions[6] = "sadness";
	emotions[7] = "surprise";
	
	std::cout << "Predicted emotion is: " << emotions[emotion] << std::endl;

	return 0;

}