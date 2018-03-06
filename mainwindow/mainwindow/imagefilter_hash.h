#ifndef __IMAGEFILTER_HASH_H__
#define __IMAGEFILTER_HASH_H__

#include "imagefilter.h"

class ImageFilterHash: public ImageFilter<std::string>{
public:
	ImageFilterHash(){};
	~ImageFilterHash(){};
public:
	virtual std::string computeFeature(cv::Mat &img);
	virtual double computeFeatureDistance(std::string f1, std::string f2);
	virtual std::string HashValue(cv::Mat &src);

private:
	double HanmingDistance(std::string &str1, std::string &str2);
	void computerHashString();
};

#endif