#include <opencv2/opencv.hpp>
using namespace cv;

int main(void) {

	// Read image in GrayScale mode
	Mat image = imread("bladee.png", 0);
	Mat qnt_image = Mat::zeros(image.size(), image.type());

	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			if ((int)image.at<uchar>(y, x) < 128) {
				qnt_image.at<uchar>(y, x) = 0;
			}
			else {
				qnt_image.at<uchar>(y, x) = 255;
			}
		}
	}

	imwrite("bladeeQnt.png", qnt_image);

	return 0;
}