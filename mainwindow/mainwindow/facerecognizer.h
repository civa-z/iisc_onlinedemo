#ifndef __FACERECOGNIZER_H__
#define __FACERECOGNIZER_H__

#include <vector>
#include "sfacealignment.h"
#include <opencv2/core/core.hpp>
#include "sface.h"



class FaceRecognizer{
public:
	FaceRecognizer(char* modelPath);
	std::vector<BoundingBox> getBoundingBox(cv::Mat image);
	cv::Mat getFaceImage(cv::Mat& image, BoundingBox& faceboxe);
	std::vector<float> GetFaceShapes(cv::Mat& image, BoundingBox& faceboxe);

private:
	const char* modelPath;
	SFaceDetectionDictPtr detectDictPtr;
	SFaceAlignment sFAsample;
};

#endif