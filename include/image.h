#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/face.hpp>
#include <opencv2/plot.hpp>
#include <opencv2/ml.hpp>
#include <stdint.h>

using namespace std;

class Image {
private: 
	int width;
	int height;
	std::string image_name;
	int type;
	std::string emotion;
	cv::Mat image;
	

public:
	Image(int width, int height, std::string img_name, int type, std::string img_emotion);
	void print();
	void setWidth(int width);
	void setHeight(int height);
	void setType(int type);
	void set_emotion(std::string emotion);

	int getWidth();
	int getHeight();
	int getType();
	std::string getEmotion();
};