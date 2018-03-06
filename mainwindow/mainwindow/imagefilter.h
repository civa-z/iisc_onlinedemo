#ifndef __IMAGEFILTER_H__
#define __IMAGEFILTER_H__

#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

template<typename FeatureType>
class ImageFilter{
public:
	ImageFilter(){}
	void setFileList(std::vector<std::string> f){
		fileList = f;
		std::cout<<"Begin setFileList"<<std::endl;
		picList.clear();
		for (int i = 0; i < fileList.size(); ++i) {
			std::cout<<"Begin imread"<<std::endl;
			cv::Mat pic = cv::imread(fileList[i], 0);
			std::cout<<"Begin push_back imread"<<std::endl;
			picList.push_back(pic);
		}
	}

	void setPictureList(std::vector<cv::Mat> &piclist){
		picList = piclist;
	}

	std::vector<std::vector<double>> computeSimilarity(){
		computeFeatureList();
		computeSimilarityMatrix();
		return similarity;
	}
	~ImageFilter(){}

protected:
	virtual FeatureType computeFeature(cv::Mat &img) = 0;
	virtual double computeFeatureDistance(FeatureType f1, FeatureType f2) = 0;

private:
	void computeFeatureList(){
		featureList.clear();
		for (int i = 0; i < picList.size(); ++i) {
			featureList.push_back(computeFeature(picList[i]));
		}
	}
	void computeSimilarityMatrix(){
		similarity.clear();
		for (int i = 0; i < featureList.size(); ++i) {
			std::vector<double> tmp;
			for (int j = 0; j < featureList.size(); ++j)
				tmp.push_back(computeFeatureDistance(featureList[i], featureList[j]) / 256);
			similarity.push_back(tmp);
		}
	}

private:
	std::vector<std::string> fileList;
	std::vector<cv::Mat> picList;
	std::vector<FeatureType> featureList;
	std::vector<std::vector<double>> similarity;
};

#endif
