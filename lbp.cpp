#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
//#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/face.hpp"
#include <opencv2/plot.hpp>

#include <iostream>
#include <fstream>
#include <sstream>


using namespace cv;
using namespace cv::face;
using namespace cv::plot;
using namespace std;

static void read_csv(const string& filename, vector<Mat>& images, 
	vector<int>&labels, char separator = ';') {

	std::ifstream file(filename.c_str(), ifstream::in);
	if(!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line;
	string path;
	string classlabel;

	while(getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty()){
			images.push_back(imread(path,0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}

int main(int argc, const char *argv[]){
	if (argc != 2){
		cout << "usage: " << argv[0] << " <csv.ext>" << endl;
	}

	string fn_csv = string(argv[1]);
	cout << "filename: " << fn_csv << endl;
	// later do a matrix of vectors? one vector for each emotion
	vector<Mat> images;
	vector<int> labels;

	try {
		read_csv(fn_csv, images, labels);
	} catch (cv::Exception& e) {
		cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
		exit(1); 
	}

	if (images.size() <= 1){
		string error_message = "This program requires at least 2 images to work. Plaease add more images to your dataset!";
		CV_Error(CV_StsError, error_message);
	}		

	const Size size = images[0].size();
	cout << size.width << " " << size.height << endl;
	
	
	cout << "images size: " << images.size() << endl;
	for (vector<int>::const_iterator it = labels.begin(); it!=labels.end(); ++it){
		cout << "labels: " <<  *it << endl;
	}
	for (vector<Mat>::const_iterator it = images.begin(); it!=images.end(); ++it){
		const Size size = (*it).size();
		cout << size.width << " " << size.height << endl;
	}

	// display first image for testing
	/*
	namedWindow("First image", WINDOW_AUTOSIZE);
	imshow("First image", images[0]);
	waitKey(0);
	*/
	// get height from first image, will need this later to reshape images 
	// to their original size

	int height = images[0].rows;

	// divide dataset into training and testing sets for the face detector
	// later remove 20% of the images (maybe 10 or 5 since my set is small)
	Mat testSample = images[images.size() -1];
	int testLabel = labels[labels.size() - 1];
	images.pop_back(); // removes last element of vector
	labels.pop_back();

	//****** Shamelessly copy pasted from opencv.org ******//
	// The following lines create an LBPH model for
    // face recognition and train it with the images and
    // labels read from the given CSV file.
    //
    // The LBPHFaceRecognizer uses Extended Local Binary Patterns
    // (it's probably configurable with other operators at a later
    // point), and has the following default values
    // (Should look into ^^^, just want to see it work for now)
    //      radius = 1
    //      neighbors = 8
    //      grid_x = 8
    //      grid_y = 8
    //
    // So if you want a LBPH FaceRecognizer using a radius of
    // 2 and 16 neighbors, call the factory method with:
    //
    //      cv::createLBPHFaceRecognizer(2, 16);
    //
    // And if you want a threshold (e.g. 123.0) call it with its default values:
    //
    //      cv::createLBPHFaceRecognizer(1,8,8,8,123.0)
    //***** End copy paste ******************************//
	Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();	
	// the following line predicts the label of a given test image
	model->train(images, labels);
	int predictedLabel = model->predict(testSample);
	// To get the confidence of a prediction call the model with:
    //
    //      int predictedLabel = -1;
    //      double confidence = 0.0;
    //      model->predict(testSample, predictedLabel, confidence);
    //
    string result_message = format("Predicted class = %d / Actual class = %d",
    	predictedLabel, testLabel);
    cout << result_message << endl;
    // Sometimes you'll need to get/set internal model data,
    // which isn't exposed by the public cv::FaceRecognizer.
    // Since each cv::FaceRecognizer is derived from a
    // cv::Algorithm, you can query the data.
    //
    // First we'll use it to set the threshold of the FaceRecognizer
    // to 0.0 without retraining the model. This can be useful if
    // you are evaluating the model:
    //
    model->setThreshold(0.0);
    // Now the threshold of this model is set to 0.0. A prediction
    // now returns -1, as it's impossible to have a distance below
    // it
    predictedLabel = model->predict(testSample);
    cout << "Predicted class = " << predictedLabel << endl;
    // Show some informations about the model, as there's no cool
    // Model data to display as in Eigenfaces/Fisherfaces.
    // Due to efficiency reasons the LBP images are not stored
    // within the model:
    cout << "Model Information:" << endl;
    String model_info = format("\tLBPH(radius=%i, neighbors=%i, grid_x=%i, grid_y=%i, threshold=%.2f)",
    	model->getRadius(),
    	model->getNeighbors(),
    	model->getGridX(),
    	model->getGridY(),
    	model->getThreshold());
	cout << model_info << endl;
	// We could get the histograms for example
	vector<Mat> histograms = model->getHistograms();
	// But should I really visualize it? YES but just do length for now:
	cout << "Size of histograms: " << histograms[0].size << endl; // .size is of type MatSize, .size() is a function that prints the size as a Mat
	cout << "Number of rows of histogram: " << histograms[0].rows << endl; // .rows returns an int
	cout << "Number of cols of histogram: " << histograms[0].cols << endl; // .cols returns an int

	/*  I need to turn the below into a function
	//  probably like:
	//  void plotStuff(vector<mat>& hist) {
			Mat hist_plot;
			Mat plot_result;
			hist.convertTo(hist_plot, CV_64F);
			bla bla bla
			plot->render()
			imshow
			waitKey
	//  }
	*/

	Mat hist_plot;
	Mat plot_result;
	histograms[0].convertTo(hist_plot, CV_64F);
	cout << "histogram type == " << hist_plot.type() << endl;

    
    Ptr<Plot2d> plot = Plot2d::create( hist_plot*-1 ); // for some reason they're inverted
    plot->setPlotBackgroundColor( Scalar( 50, 50, 50) );
    plot->setPlotLineColor (Scalar (50, 50, 255 ) );
    plot->render ( plot_result );

    imshow("plot", plot_result);
    waitKey();

	

	return 0;
}
