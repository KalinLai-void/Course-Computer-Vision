#pragma once

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

#define DIVIATION 50 // for GetDiff() function
#define SMALLEST_DISPLAY_SELECT_AREA 10 // for DrawArea() function, throwing some small area out.

class VideoInfo 
{
public: // video info
	cv::VideoCapture videoCap;
	cv::Mat input;
	cv::Mat process;

	long frameCounts;
	double fps;

public: // using for process
	cv::Mat bg;
	cv::Mat preFrame;
};