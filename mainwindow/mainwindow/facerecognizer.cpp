#include "sface_dict.h"
#include <Windows.h>
#include <iostream>


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "facerecognizer.h"

FaceRecognizer::FaceRecognizer(char* modelPath):sFAsample(modelPath){
	std::cout<<std::endl<<modelPath<<std::endl<<std::fflush;
	//initialize the face alignment model;
	try{
		std::cout<<std::endl<<"1"<<std::endl<<std::fflush;
		SFaceAlignment sFAsample(modelPath);
		std::cout<<std::endl<<"2"<<std::endl<<std::fflush;
	}catch (exception e){
		std::cout<<std::endl<<e.what()<<std::endl<<std::fflush;
	}
	std::cout<<std::endl<<"loaded"<<std::endl<<std::fflush;

	//Face detection;
	detectDictPtr = V09_R30P20Y60_S2R2_BM16GR;
}

 std::vector<BoundingBox> FaceRecognizer::getBoundingBox(cv::Mat img){
	 cv::Mat_<uchar> image = img;
	 std::vector<BoundingBox> boundingBoxList;
	
	equalizeHist( image, image);

	int   workSize = sFaceDetectionGetWorkingMemorySize(image.cols,image.rows, image.cols, detectDictPtr);
	void *workPtr  = malloc(workSize);
	int ret = 0;
	SFaceDetectionResult face[12];
	for (int i=0;i<12;i++)
	{
		face[i].faceH = 0;
		face[i].facePitch = 0;
		face[i].faceRoll = 0;
		face[i].faceW = 0;
		face[i].faceX = 0;
		face[i].faceY = 0;
		face[i].faceYaw = 0;
		face[i].score = 0;
	}
	int numFace = 0;


	ret = sFaceDetection(
		image.data, image.cols,image.rows, image.cols,
		detectDictPtr,
		0.4f, 0.841f, 0.0f, 2, 2, 0.50f, S_FACE_DETECT_RESULT_PRECISE,
		face, 12,
		&numFace,
		workPtr, workSize
		);
	if(ret != S_FACE_OK)
	{
		printf("sFaceDetection() failed! %d\n",ret);
		free(workPtr);
		return boundingBoxList;
	}
	if (numFace == 0)
	{
		printf("sFaceDetection() failed! %d\n",0);
		free(workPtr);
		return boundingBoxList;
	}
	if (ret == S_FACE_OK)
	{
		for(int i=0;i<numFace;i++)
		{
			BoundingBox Pubfig_boundingbox;
			Pubfig_boundingbox.start_x = face[i].faceX*image.cols;
			Pubfig_boundingbox.start_y = face[i].faceY*image.rows;
			Pubfig_boundingbox.width = face[i].faceW*image.cols;
			Pubfig_boundingbox.height = face[i].faceH*image.rows;
			Pubfig_boundingbox.score = face[i].score;
			Pubfig_boundingbox.faceRoll = face[i].faceRoll;
			Pubfig_boundingbox.facePitch = face[i].facePitch;
			Pubfig_boundingbox.faceYaw = face[i].faceYaw;
			Pubfig_boundingbox.centroid_x = Pubfig_boundingbox.start_x + Pubfig_boundingbox.width/2.0;
			Pubfig_boundingbox.centroid_y = Pubfig_boundingbox.start_y + Pubfig_boundingbox.height/2.0;
			float boxscale = 0.15;  //0.40 0.35 0.30 0.25 0.0
			Pubfig_boundingbox.start_x = Pubfig_boundingbox.start_x+Pubfig_boundingbox.width*boxscale/(1+2*boxscale);
			Pubfig_boundingbox.start_y = Pubfig_boundingbox.start_y+Pubfig_boundingbox.height*boxscale/(1+2*boxscale);
			Pubfig_boundingbox.width = Pubfig_boundingbox.width - Pubfig_boundingbox.width*boxscale*2/(1+boxscale*2);
			Pubfig_boundingbox.height = Pubfig_boundingbox.height - Pubfig_boundingbox.height*(boxscale)*2/(1+boxscale*2);
			Pubfig_boundingbox.centroid_x = Pubfig_boundingbox.start_x + Pubfig_boundingbox.width/2.0;
			Pubfig_boundingbox.centroid_y = Pubfig_boundingbox.start_y + Pubfig_boundingbox.height/2.0;
			boundingBoxList.push_back(Pubfig_boundingbox);

			std::cout<<"Face detection result: (start_x, start_y, width, height) "<<
				Pubfig_boundingbox.start_x<<" "<<Pubfig_boundingbox.start_y<<" "<<
				Pubfig_boundingbox.width<<" "<<Pubfig_boundingbox.height<<endl;
		}
		free(workPtr);
	}
	return boundingBoxList;
}

cv::Mat FaceRecognizer::getFaceImage(cv::Mat& image, BoundingBox& facebox){
	cv::Rect rect(facebox.start_x, facebox.start_y, facebox.width, facebox.height);
	cv::Mat faceImage = image(rect);
	return faceImage;
}

std::vector<float> FaceRecognizer::GetFaceShapes(cv::Mat& image, BoundingBox& facebox){
	std::vector<float> faceshape_v;
	float *faceshape = new float[sFAsample.global_params.landmark_num*2]();
	sFAsample.sFaceAlignment(image.data,image.cols,image.rows,facebox,faceshape);
	for (int i = 0; i < sFAsample.global_params.landmark_num*2; ++i)
		faceshape_v.push_back(faceshape[i]);
	return faceshape_v;
}