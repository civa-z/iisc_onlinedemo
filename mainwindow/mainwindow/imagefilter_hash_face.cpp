
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
	std::cout<<"Begin FaceImageFilter::HashValue"<<std::endl;
	std::vector<BoundingBox> boundingBoxList = faceRecognizer->getBoundingBox(src);
	if (0 == boundingBoxList.size())
		return "";
	cv::Mat faceImage = faceRecognizer->getFaceImage(src, boundingBoxList[0]);
	
	return ImageFilterHash::HashValue(faceImage);
}