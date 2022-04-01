#include <opencv2/opencv.hpp>
#include <vector>

int get_max_height(std::vector<int> b_pxls, std::vector<int> g_pxls, std::vector<int> r_pxls) {
	int max(0);

	for (int i(0); i < 256; i++) {
		if (b_pxls.at(i) > max) {
			max = b_pxls.at(i);
		}

		if (g_pxls.at(i) > max) {
			max = g_pxls.at(i);
		}

		if (r_pxls.at(i) > max) {
			max = r_pxls.at(i);
		}
	}

	return max;
}

int main() {
	// reading img from directory and writing it to jpeg
	cv::Mat png_img = cv::imread("../../../data/cross_0256x0256.png");
	cv::imwrite("cross_0256x0256_025.jpg", png_img, { cv::IMWRITE_JPEG_QUALITY, 25 });

	// splitting png_img to 3 single channels - blue, red, green
	cv::Mat single_channels_png[3];
	cv::split(png_img, single_channels_png);

	// create empty channel with black color
	cv::Mat empty_channel(256, 256, CV_8UC1);
	empty_channel = 0;

	// creating monochromes png_s
	cv::Mat blue_channel_png[3] = { single_channels_png[0], empty_channel, empty_channel };
	cv::Mat green_channel_png[3] = { empty_channel, single_channels_png[1], empty_channel };
	cv::Mat red_channel_png[3] = { empty_channel, empty_channel, single_channels_png[2] };
	cv::Mat monochromes_vis_png[3];
	cv::merge(blue_channel_png, 3, monochromes_vis_png[0]);
	cv::merge(green_channel_png, 3, monochromes_vis_png[1]);
	cv::merge(red_channel_png, 3, monochromes_vis_png[2]);

	// creating mosaic by concatening obtaibed png_s
	cv::Mat mosaic_png_top;
	cv::Mat mosaic_png_bot;
	cv::Mat mosaic_png;

	cv::hconcat(png_img, monochromes_vis_png[2], mosaic_png_top);
	cv::hconcat(monochromes_vis_png[1], monochromes_vis_png[0], mosaic_png_bot);
	cv::vconcat(mosaic_png_top, mosaic_png_bot, mosaic_png);
	cv::imwrite("cross_0256x0256_png_channels.png", mosaic_png);


	cv::Mat jpeg_img = cv::imread("cross_0256x0256_025.jpg");

	// splitting jpeg_img to 3 single channels - blue, red, green
	cv::Mat single_channels_jpeg[3];
	cv::split(jpeg_img, single_channels_jpeg);

	// creating monochromes jpeg_s
	cv::Mat blue_channel_jpeg[3] = { single_channels_jpeg[0], empty_channel, empty_channel };
	cv::Mat green_channel_jpeg[3] = { empty_channel, single_channels_jpeg[1], empty_channel };
	cv::Mat red_channel_jpeg[3] = { empty_channel, empty_channel, single_channels_jpeg[2] };
	cv::Mat monochromes_vis_jpeg[3];
	cv::merge(blue_channel_jpeg, 3, monochromes_vis_jpeg[0]);
	cv::merge(green_channel_jpeg, 3, monochromes_vis_jpeg[1]);
	cv::merge(red_channel_jpeg, 3, monochromes_vis_jpeg[2]);

	// creating mosaic by concatening obtaibed jpegs_s
	cv::Mat mosaic_jpeg_top;
	cv::Mat mosaic_jpeg_bot;
	cv::Mat mosaic_jpeg;

	cv::hconcat(jpeg_img, monochromes_vis_jpeg[2], mosaic_jpeg_top);
	cv::hconcat(monochromes_vis_jpeg[1], monochromes_vis_jpeg[0], mosaic_jpeg_bot);
	cv::vconcat(mosaic_jpeg_top, mosaic_jpeg_bot, mosaic_jpeg);
	cv::imwrite("cross_0256x0256_jpeg_channels.png", mosaic_jpeg);

	// creating png histogram

	cv::Mat png_hist(256, 256, CV_8UC3, cv::Scalar(240, 240, 240));

	std::vector<int> blue_pixels(256);
	std::vector<int> green_pixels(256);
	std::vector<int> red_pixels(256);

	for (int i(0); i < 256; i++) {
		for (int j(0); j < 256; j++) {
			blue_pixels[single_channels_png[0].at<uchar>(i, j)]++;
			green_pixels[single_channels_png[1].at<uchar>(i, j)]++;
			red_pixels[single_channels_png[2].at<uchar>(i, j)]++;
		}
	}
	 
	//getting max_height
	int max_height(get_max_height(blue_pixels, green_pixels, red_pixels));

	// downscaling considering max height
	for (int i(0); i < 256; i++) {
		blue_pixels[i] = ((double) blue_pixels[i] / max_height) * png_hist.rows;
		green_pixels[i] = ((double) green_pixels[i] / max_height) * png_hist.rows;
		red_pixels[i] = ((double) red_pixels[i] / max_height) * png_hist.rows;
	}

	// drawing lines in histogram
	for (int i(0); i < 255; i++) {
		cv::line(png_hist, cv::Point(i, png_hist.rows - blue_pixels[i]),
			cv::Point(i + 1, png_hist.rows - blue_pixels[i + 1]),
			cv::Scalar(255, 0, 0), 1, 8, 0);

		cv::line(png_hist, cv::Point(i, png_hist.rows - green_pixels[i]),
			cv::Point(i + 1, png_hist.rows - green_pixels[i + 1]),
			cv::Scalar(0, 255, 0), 1, 8, 0);

		cv::line(png_hist, cv::Point(i, png_hist.rows - red_pixels[i]),
			cv::Point(i + 1, png_hist.rows - red_pixels[i + 1]),
			cv::Scalar(0, 0, 255), 1, 8, 0);
	}

	// creating jpeg histogram
	cv::Mat jpeg_hist(256, 256, CV_8UC3, cv::Scalar(240, 240, 240));

	// clearing vectors
	for (int i(0); i < 256; i++) {
		blue_pixels[i] = 0;
		green_pixels[i] = 0;
		red_pixels[i] = 0;
	}

	for (int i(0); i < 256; i++) {
		for (int j(0); j < 256; j++) {
			blue_pixels[single_channels_jpeg[0].at<uchar>(i, j)]++;
			green_pixels[single_channels_jpeg[1].at<uchar>(i, j)]++;
			red_pixels[single_channels_jpeg[2].at<uchar>(i, j)]++;
		}
	}

	//getting max_height
	max_height = get_max_height(blue_pixels, green_pixels, red_pixels);
	
	// downscaling considering max height
	for (int i(0); i < 256; i++) {
		blue_pixels[i] = ((double)blue_pixels[i] / max_height) * jpeg_hist.rows;
		green_pixels[i] = ((double)green_pixels[i] / max_height) * jpeg_hist.rows;
		red_pixels[i] = ((double)red_pixels[i] / max_height) * jpeg_hist.rows;
	}

	// drawing lines in histogram
	for (int i(0); i < 255; i++) {
		cv::line(jpeg_hist, cv::Point(i, jpeg_hist.rows - blue_pixels[i]),
			cv::Point(i + 1, jpeg_hist.rows - blue_pixels[i + 1]),
			cv::Scalar(255, 0, 0), 1, 8, 0);

		cv::line(jpeg_hist, cv::Point(i, jpeg_hist.rows - green_pixels[i]),
			cv::Point(i + 1, jpeg_hist.rows - green_pixels[i + 1]),
			cv::Scalar(0, 255, 0), 1, 8, 0);

		cv::line(jpeg_hist, cv::Point(i, jpeg_hist.rows - red_pixels[i]),
			cv::Point(i + 1, jpeg_hist.rows - red_pixels[i + 1]),
			cv::Scalar(0, 0, 255), 1, 8, 0);
	}

	//creating hisogram mosaic
	cv::Mat mosaic_jpeg_left;
	cv::Mat mosaic_jpeg_right;
	cv::Mat mosaic_jpeg_hist;

	cv::vconcat(png_img, png_hist, mosaic_jpeg_left);
	cv::vconcat(jpeg_img, jpeg_hist, mosaic_jpeg_right);
	cv::hconcat(mosaic_jpeg_left, mosaic_jpeg_right, mosaic_jpeg_hist);
	cv::imwrite("cross_0256x0256_hists.png", mosaic_jpeg_hist);

	cv::imshow("dasdasdas", mosaic_jpeg_hist);
	cv::waitKey();
}