#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

int main() {
	cv::Mat I_1(60, 768, CV_8UC1);
	int color(0);

	for (size_t i(0); i < 768; i += 3) {
		for (size_t j(0); j < 60; j++) {
			I_1.at<uint8_t>(j, i) = color;
			I_1.at<uint8_t>(j, i + 1) = color;
			I_1.at<uint8_t>(j, i + 2) = color;
		}
		color++;
	}

	cv::Mat G_1 = I_1.clone(); 
	//I_1.copyTo(G_1);

	auto start_time_1 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	G_1.convertTo(G_1, CV_32F, 1.0 / 255);
	cv::pow(G_1, 2.2, G_1);
	G_1.convertTo(G_1, CV_8UC1, 255);

	auto end_time_1 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	auto diff_time_1 = end_time_1 - start_time_1;

	std::cout << diff_time_1 << std::endl;

	cv::Mat G_2 = I_1.clone();
	auto start_time_2 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	for (size_t i(0); i < 768; i += 3) {
		for (size_t j(0); j < 60; j++) {
			double color = std::pow(G_2.at<uint8_t>(j, i) / 255.0, 2.8) * 255;
			G_2.at<uint8_t>(j, i) = color;
			G_2.at<uint8_t>(j, i + 1) = color;
			G_2.at<uint8_t>(j, i + 2) = color;
		}
	}

	auto end_time_2 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	int diff_time_2 = end_time_2 - start_time_2;
	std::cout << diff_time_2 << std::endl;

	cv::Mat final_img(180, 768, CV_8UC1);
	final_img = 0;

	for (size_t i(0); i < 768; i++) {
		for (size_t j(0); j < 60; j++) {
			final_img.at<uint8_t>(j, i) = I_1.at<uint8_t>(j, i);
			final_img.at<uint8_t>(j + 60, i) = G_1.at<uint8_t>(j, i);
			final_img.at<uint8_t>(j + 120, i) = G_2.at<uint8_t>(j, i);
		  }
	}

	cv::Rect2d rc = { 0, 0, 768, 60 };
	cv::rectangle(final_img, rc, { 100 }, 1);
	rc.y += rc.height;
	cv::rectangle(final_img, rc, { 255 }, 1);
	rc.y += rc.height;
	cv::rectangle(final_img, rc, { 150 }, 1);

	cv::imwrite("lab01.png", final_img);
	cv::imshow("lab01.png", final_img);
	cv::waitKey();
}
