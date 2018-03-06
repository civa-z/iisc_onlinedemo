#ifndef __IMAGEFILTER_HASH_FACE_H__
#define __IMAGEFILTER_HASH_FACE_H__

#include <vector>
#include <opencv2/core/core.hpp>

#include "imagefilter_hash.h"
#include "facerecognizer.h"

class FaceImageFilter: public ImageFilterHash{
public:
	FaceImageFilter():faceRecognizer(nullptr){}
	bool loadFaceRecognizer(char* fath){
		std::cout<<"loadFaceRecognizer"<<std::endl<<std::fflush;
		if (nullptr == faceRecognizer)
			std::cout<<"new loadFaceRecognizer"<<std::endl<<std::fflush;
			faceRecognizer = new FaceRecognizer(fath);
		return true;
	}
	virtual std::string HashValue(cv::Mat &src);

private:
	FaceRecognizer* faceRecognizer;
	std::vector<std::string> hashstring;
	
};

#endif
