#include "imagefilter_hash.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

std::string ImageFilterHash::computeFeature(cv::Mat &img){
	return HashValue(img);
}

double ImageFilterHash::computeFeatureDistance(std::string f1, std::string f2){
	return (double)HanmingDistance(f1, f2) / 256;
}

std::string ImageFilterHash::HashValue(cv::Mat &src){
	std::string rst(256,'\0');
	cv::Mat img;
	if(src.channels()==3)
		cvtColor(src,img,CV_BGR2GRAY);
	else
		img=src.clone();

	resize(img,img,cv::Size(16,16));

	uchar *pData;
	for(int i=0;i<img.rows;i++)
	{
		pData = img.ptr<uchar>(i);
		for(int j=0;j<img.cols;j++)
		{
			pData[j]=pData[j]/4;
		}
	}

	int average = mean(img).val[0];

	cv::Mat mask= (img>=(uchar)average);

	int index = 0;
	for(int i=0;i<mask.rows;i++)
	{
		pData = mask.ptr<uchar>(i);
		for(int j=0;j<mask.cols;j++)
		{
			if(pData[j]==0)
				rst[index++]='0';
			else
				rst[index++]='1';
		}
	}
	return rst;
}

double ImageFilterHash::HanmingDistance(std::string &str1, std::string &str2){
	if((str1.size()!=256)||(str2.size()!=256))
		return -1;
	int difference = 0;
	for(int i=0;i<256;i++)
	{
		if(str1[i]!=str2[i])
			difference++;
	}
	return difference;
}