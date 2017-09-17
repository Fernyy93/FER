#include <lbp.h>

using namespace cv;
using namespace cv::face;
using namespace cv::plot;
using namespace std;

int get_dir( const char* path, vector<string>& files ) {
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

void read_csv(const string& filename, vector<Mat>& images, 
	vector<int>& nums) {


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
		getline(liness, path, ';');
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty()){
			images.push_back(imread(path,0));
		 	nums.push_back(atoi(classlabel.c_str()));
		}
	}


}

