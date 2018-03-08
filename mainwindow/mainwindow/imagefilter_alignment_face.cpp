#include "imagefilter_alignment_face.h"
#include <algorithm>

AlignmentFeatureType ImageFilterAlignmentFace::computeFeature(cv::Mat &img){
	if (nullptr == faceRecognizer)
		return alignmentFeature;

	std::vector<BoundingBox> boundingBoxList = faceRecognizer->getBoundingBox(img);
	if (boundingBoxList.size() == 0)
		return alignmentFeature;
	std::vector<float>  faceshape = faceRecognizer->GetFaceShapes(img, boundingBoxList[0]);
	for (int i = 0; i < faceshape.size() / 2; ++i)
		alignmentFeature.push_back(std::pair<float, float>(faceshape[2*i], faceshape[2*i + 1]));
	return alignmentFeature;
}

double ImageFilterAlignmentFace::computeFeatureDistance(AlignmentFeatureType f1, AlignmentFeatureType f2){
	if (f1.size() != f2.size() || f1.size() == 0)
		return -1;
	double euclideanDistance = 0;
	for (int i = 0; i < f1.size(); ++i){
		euclideanDistance += pow(f1[i].first - f2[i].first, 2) + pow(f1[i].second - f2[i].second, 2);
	}
	euclideanDistance = sqrt(euclideanDistance);
	return euclideanDistance;
}

void ImageFilterAlignmentFace::renderImage(cv::Mat &img){
	for (int i = 0; i < alignmentFeature.size(); ++i){
		cv::circle(img, cv::Point2d(alignmentFeature[i].first,alignmentFeature[i].second), ceil((float)5 * img.rows/800), cv::Scalar(255,0,0),-1,8,0);
	}
	alignmentFeature.clear();
}