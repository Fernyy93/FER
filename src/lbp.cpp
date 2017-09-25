#include <lbp.h>

using namespace cv;
using namespace cv::face;
using namespace cv::plot;
using namespace std;

int get_dir(const char* path, vector<string>& files) {
	struct dirent *entry;
	DIR *dp;
	cout << "input path: " << path << endl;
	dp = opendir(path);
	if (dp == NULL){
		cout << "error" << endl;
		perror("opendir");
		// should throw an exception
		return -1;
	}

	while(( entry = readdir(dp) )){
		cout << entry->d_name << endl;
		files.push_back(string(entry->d_name));		
	}

	closedir(dp);
	return 0;
}

void read_csv(const string& filename, vector<Mat>& images) {


	std::ifstream file(filename.c_str(), ifstream::in);
	if(!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}

	string line;
	string path;
	

	while(getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, ';');
		//getline(liness, classlabel);
		if (!path.empty()){
			images.push_back(imread(path,0));
		 	//nums.push_back(atoi(classlabel.c_str()));
		}
	}


}

void get_images(const char* emotions_dir, vector<string>& labels, map<string, vector<Mat> >& images_map){
	// vector of filenames in emotions dir, contains 7 files
	// this is probably an extremely inefficient way of doing this until I 
	// figure out how to overwrite vectors properly
	vector<string> files;
	vector<Mat> images;
	vector<Mat> temp;
	get_dir(emotions_dir, files);
	int i = 0;
	for (i = 0; i < labels.size(); ++i){
		string fn_csv = string(emotions_dir) + "/" + files[i];
		cout << "file containing image paths for " << labels[i] << ": " << files[i] << endl;
		cout << "the file name is: " << fn_csv << endl;
		read_csv(fn_csv, images);
		temp = images;
		images_map.insert({labels[i], temp});
		images = vector<Mat>();
	}

}

void plot_hist(vector<Mat>& hist, int num) {

	Mat hist_plot;
	Mat plot_result;
	hist[num].convertTo(hist_plot, CV_64F);
	cout << "Histogram type is: " << hist_plot.type() << endl;

	Ptr<Plot2d> plot = Plot2d::create( hist_plot*-1 );
	plot->setPlotBackgroundColor( Scalar( 50, 50, 50) );
    plot->setPlotLineColor (Scalar (50, 50, 255 ) );
    plot->render ( plot_result );

    imshow("plot", plot_result);
    waitKey();
}