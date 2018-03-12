
#include "opencv2/highgui/highgui.hpp"
#include "cv.h"
#include <Windows.h>
#include "sface.h"
#include "sface_dict.h"

#include "sfacealignment.h"

#include <opencv2/core/core.hpp>
#include <Windows.h>
#include "sface.h"
#include "sface_dict.h"

#include "imagefilter_hash_face.h"

std::string FaceImageFilter::HashValue(cv::Mat &src){
	if (nullptr == faceRecognizer)
		return "";
	boundingBoxList = faceRecognizer->getBoundingBox(src);
	if (0 == boundingBoxList.size()){
		return "";
	}
	cv::Mat faceImage = faceRecognizer->getFaceImage(src, boundingBoxList[0]);
	
	return ImageFilterHash::HashValue(faceImage);
}

void FaceImageFilter::renderImage(cv::Mat &img){
	for (int i = 0; i < boundingBoxList.size(); ++i)
	cv::rectangle(img,
		cv::Point(boundingBoxList[i].start_x,boundingBoxList[i].start_y),
		cv::Point(boundingBoxList[i].start_x + boundingBoxList[i].width,boundingBoxList[i].start_y + boundingBoxList[i].height),
		cv::Scalar(0,255,0), 2, 8, 0);  
}