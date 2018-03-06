#include "imagefilter.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

template<typename FeatureType>
std::vector<std::vector<double>> ImageFilter<typename FeatureType>::computeSimilarity(){
	computeFeatureList();
	computeSimilarityMatrix();
	return similarity;
}

template<typename FeatureType>
bool ImageFilter<typename FeatureType>::setFileList(std::vector<std::string> fileList){
	this.filelist = fileList;
	for (int i = 0; i < this.fileList.size(); ++i) {
        cv::Mat pic = cv::imread(this.fileList[i]);
        picList.push_back(pic);
    }
}
