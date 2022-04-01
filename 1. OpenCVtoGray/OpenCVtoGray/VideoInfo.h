#pragma once

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

class VideoInfo 
{
public:
	cv::VideoCapture videoCap;
	cv::Mat input;
	cv::Mat output;

	long frameCounts;
	double fps;
};