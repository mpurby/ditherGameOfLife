#include <opencv2/opencv.hpp>
using namespace cv;

int qntVal(int input);

int main(void) {

	// Read image in GrayScale mode
	Mat image = imread("bladee.png", 0);
	Mat qnt_image = Mat::zeros(image.size(), image.type());
	Mat dith_image = image;

	// quantization code below

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

	// dithering code below

	int oldPixel{}, newPixel{}, qntError{};

	for (int y = 1; y < image.rows - 1; y++) {
		for (int x = 1; x < image.cols - 1; x++) {
			oldPixel = (int)image.at<uchar>(y, x);
			newPixel = qntVal(oldPixel);
			dith_image.at<uchar>(y, x) = newPixel;
			qntError = oldPixel - newPixel;
			dith_image.at<uchar>(y    , x + 1) = (int)dith_image.at<uchar>(y    , x + 1)
												+ (qntError * 7 / 16);
			dith_image.at<uchar>(y + 1, x - 1) = (int)dith_image.at<uchar>(y + 1, x - 1)
												+ (qntError * 3 / 16);
			dith_image.at<uchar>(y + 1, x    ) = (int)dith_image.at<uchar>(y + 1, x    )
												+ (qntError * 5 / 16);
			dith_image.at<uchar>(y + 1, x + 1) = (int)dith_image.at<uchar>(y + 1, x + 1)
												+ (qntError * 1 / 16);
		}
	}

	// ^ still need to sort out borders and overflow

	imwrite("bladeeDith.png", dith_image);

	Mat prev_image = dith_image;
	Mat gol_image = dith_image;
	int nbrs{};
	bool live{};
	std::string fileName{};

	for (int i = 0; i < 48; i++) {
		for (int y = 1; y < prev_image.rows - 1; y++) {
			for (int x = 1; x < prev_image.cols - 1; x++) {
				nbrs = 0;
				live = false;
				if ((int)prev_image.at<uchar>(y, x) < 128) {
					live = true;
				}
				if ((int)prev_image.at<uchar>(y - 1, x - 1) < 128) {
					nbrs++;
				}
				if ((int)prev_image.at<uchar>(y - 1, x) < 128) {
					nbrs++;
				}
				if ((int)prev_image.at<uchar>(y - 1, x + 1) < 128) {
					nbrs++;
				}
				if ((int)prev_image.at<uchar>(y, x - 1) < 128) {
					nbrs++;
				}
				if ((int)prev_image.at<uchar>(y, x + 1) < 128) {
					nbrs++;
				}
				if ((int)prev_image.at<uchar>(y + 1, x - 1) < 128) {
					nbrs++;
				}
				if ((int)prev_image.at<uchar>(y + 1, x    ) < 128) {
					nbrs++;
				}
				if ((int)prev_image.at<uchar>(y + 1, x + 1) < 128) {
					nbrs++;
				}
				if (nbrs == 3) {
					gol_image.at<uchar>(y, x) = 0;
				}
				else if (nbrs == 2 && live) {
					gol_image.at<uchar>(y, x) = 0;
				}
				else {
					gol_image.at<uchar>(y, x) = 255;
				}
			}
		}
		fileName = "BladeeGol";
		fileName += std::to_string(i);
		fileName += ".png";
		imwrite(fileName, gol_image);
		prev_image = gol_image;
	}

	return 0;
}

int qntVal(int input) {
	if (input < 128) {
		return 0;
	}
	else {
		return 255;
	}
}