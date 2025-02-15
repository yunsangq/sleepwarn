#include "stdafx.h";
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#include <iostream>
#include <queue>
#include <stdio.h>

#include "constants.h"
#include "helpers.h"

#include "findEyeCorner.h"

cv::Mat *leftCornerKernel;
cv::Mat *rightCornerKernel;

float kEyeCornerKernel[4][6] = {
	{ -1,-1,-1, 1, 1, 1 },
	{ -1,-1,-1,-1, 1, 1 },
	{ -1,-1,-1,-1, 0, 3 },
	{ 1, 1, 1, 1, 1, 1 },
};

void createCornerKernels() {
	rightCornerKernel = new cv::Mat(4, 6, CV_32F, kEyeCornerKernel);
	leftCornerKernel = new cv::Mat(4, 6, CV_32F);

	cv::flip(*rightCornerKernel, *leftCornerKernel, 1);
}

void releaseCornerKernels() {
	delete leftCornerKernel;
	delete rightCornerKernel;
}

cv::Mat eyeCornerMap(const cv::Mat &region, bool left, bool left2) {
	cv::Mat cornerMap;

	cv::Size sizeRegion = region.size();
	cv::Range colRange(sizeRegion.width / 4, sizeRegion.width * 3 / 4);
	cv::Range rowRange(sizeRegion.height / 4, sizeRegion.height * 3 / 4);

	cv::Mat miRegion(region, rowRange, colRange);

	cv::filter2D(miRegion, cornerMap, CV_32F,
		(left && !left2) || (!left && !left2) ? *leftCornerKernel : *rightCornerKernel);

	return cornerMap;
}

cv::Point2f findEyeCorner(cv::Mat region, bool left, bool left2) {
	cv::Mat cornerMap = eyeCornerMap(region, left, left2);

	cv::Point maxP;
	cv::minMaxLoc(cornerMap, NULL, NULL, NULL, &maxP);

	cv::Point2f maxP2;
	maxP2 = findSubpixelEyeCorner(cornerMap, maxP);

	return maxP2;
}
cv::Point2f findSubpixelEyeCorner(cv::Mat region, cv::Point maxP) {

	cv::Size sizeRegion = region.size();

	cv::Mat cornerMap(sizeRegion.height * 10, sizeRegion.width * 10, CV_32F);

	cv::resize(region, cornerMap, cornerMap.size(), 0, 0, cv::INTER_CUBIC);

	cv::Point maxP2;
	cv::minMaxLoc(cornerMap, NULL, NULL, NULL, &maxP2);

	return cv::Point2f(sizeRegion.width / 2 + maxP2.x / 10,
		sizeRegion.height / 2 + maxP2.y / 10);
}
