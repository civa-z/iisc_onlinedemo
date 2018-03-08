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
	bool prepareFeature(std::string file_path){
		cv::Mat mat = cv::imread(file_path, 0);
		feature = computeFeature(mat);
		featureList.push_back(feature);
		return true;
	}

	bool prepareFeature(cv::Mat &mat){
		FeatureType feature = computeFeature(mat);
		featureList.push_back(feature);
		return true;
	}

	std::vector<std::vector<double>> computeSimilarity(){
		std::vector<std::vector<double>> similarity;
		
		for (int i = 0; i < featureList.size(); ++i) {
			std::vector<double> tmp;
			for (int j = 0; j < featureList.size(); ++j){
				tmp.push_back(computeFeatureDistance(featureList[i], featureList[j]) / 256);
			}
			similarity.push_back(tmp);
		}
		featureList.clear();
		return similarity;
	}

	void reset(){
		featureList.clear();
	}

	~ImageFilter(){}

public:
	virtual void renderImage(cv::Mat &img){}
protected:
	virtual double computeFeatureDistance(FeatureType f1, FeatureType f2) = 0;
	virtual FeatureType computeFeature(cv::Mat &img) = 0;
	

private:
	std::vector<FeatureType> featureList;
};

#endif
