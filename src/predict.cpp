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


	if (testHist.type() != CV_32F){
		testHist.convertTo(testHist, CV_32F);
	}	
	testHist = testHist.reshape(1,1);

	std::cout << "Width after reshape: " << testHist.cols << std::endl;
	std::cout << "Height after reshape: " << testHist.rows << std::endl;
	
	
	std::cout << "number of cols in test image: " << testHist.cols << std::endl;

	Ptr<SVM> svm = SVM::create();
	svm->load("../svms/svm.xml");
	
	int emotion = svm->predict(testHist);
	std::cout << "Predicted emotion is: " << emotion << std::endl;

	return 0;

}