#include <lbp.h>

int main(int argc, const char *argv[]){
	if (argc != 2){
		std::cout << "usage: " << argv[0] << " <emotions dir> " << std::endl;
	}
	// I want a map of histograms by emotion label eventually

	
	const char* emotions_dir = argv[1];
	
	// later do a matrix of vectors? one vector for each emotion
	//vector<Mat> images;
	
	std::vector<std::string> labels =  {"contempt",
							            "disgust",
							            "surprise",
							            "anger",
							            "fear",
							            "happy",
							            "neutral",
							            "sadness"};

	std::map<std::string, std::vector<cv::Mat> > images_map;
	/*
	try {
		read_csv(fn_csv, images);
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
	

	for (vector<Mat>::const_iterator it = images.begin(); it!=images.end(); ++it){
		const Size size = (*it).size();
		cout << size.width << " " << size.height << endl;
	}
	*/
	//images_map.insert({labels[1], images});
	// display first image for testing
	/*
	namedWindow("First image", WINDOW_AUTOSIZE);
	imshow("First image", images[0]);
	waitKey(0);
	*/
	// get height from first image, will need this later to reshape images 
	// to their original size

	//int height = images[0].rows;
	
	// divide dataset into training and testing sets for the face detector
	// later remove 20% of the images (maybe 10 or 5 since my set is small)
	
	get_images(emotions_dir, labels, images_map);
	std::cout << images_map.size() << std::endl;
	std::vector<cv::Mat> thing = images_map[labels[1]];
	std::cout << thing.size() << std::endl;
	std::cout << images_map[labels[1]].size() << std::endl;
	std::cout << "type of image: " << thing[0].type() << std::endl;
	cv::Mat dst1;
	cv::Mat dst2;
	cv::Mat src = images_map[labels[1]][0];
	OLBP(src, dst1);	
	ELBP(src, dst2, 4, 16);
	dispImage("Original image", src);
	dispImage("OLBP of image", dst1);
	dispImage("ELBP of image", dst2);


	std::map<std::string, std::vector<cv::Mat> > histogram_map;
	getHist(labels, images_map, histogram_map);
	//do I want a map of histograms? Is life fair? No, so I do need one.



	//****** Shamelessly copy pasted from opencv.org ******/
	// The following lines create an LBPH model for
    // face recognition and train it with the images and
    // nums read from the given CSV file.
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
    /*
	Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();	
	// the following line predicts the label of a given test image
	model->train(images, nums);
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

    model->setThreshold(0.1);
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

	//  I need to turn the below into a function
	//  probably like:
	  void plotStuff(vector<mat>& hist) {
			Mat hist_plot;
			Mat plot_result;
			hist.convertTo(hist_plot, CV_64F);
			bla bla bla
			plot->render()
			imshow
			waitKey
	} 
	
	plot_hist(histograms, 1);
	
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
    */

	

	return 0;
}
