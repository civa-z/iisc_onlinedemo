#ifndef __IMAGEFILTER_ALIGNMENT_FACE_H__
#define __IMAGEFILTER_ALIGNMENT_FACE_H__

#include "imagefilter.h"
#include "facerecognizer.h"

typedef std::vector<std::pair<float, float>> AlignmentFeatureType;
class ImageFilterAlignmentFace:public ImageFilter<AlignmentFeatureType>{
public:
	ImageFilterAlignmentFace():faceRecognizer(nullptr){}
	bool loadFaceRecognizer(char* path){
		if (nullptr == faceRecognizer)
			faceRecognizer = new FaceRecognizer(path);
		return true;
	}
	virtual AlignmentFeatureType computeFeature(cv::Mat &img);
	virtual double computeFeatureDistance(AlignmentFeatureType f1, AlignmentFeatureType f2);

private:
	FaceRecognizer *faceRecognizer;
};

#endif
