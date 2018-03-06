#include "mainwindow.h"
#include <QtGui/QApplication>
#include <windows.h>

#include "sfacealignment.h"
#include "sface.h"
#include "sface_dict.h"

std::vector<cv::Mat> matList;
HANDLE hMutex;//互斥锁
const int buffer_length = 10;
MainWindow *w;

DWORD WINAPI  recordVideoThreadBody(LPVOID lpParameter){
	static cv::VideoCapture capture =  cv::VideoCapture(0);
	while (capture.isOpened()){
		std::cout<<"Got one frame\n"<<std::fflush;
		cv::Mat frame; 
		capture >> frame; 
		cv::Mat frame_tmp = frame.clone();
		cv::imshow("frame", frame_tmp);
		cv::waitKey(1);

		WaitForSingleObject(hMutex, INFINITE);//临界区开始  30ms
		if (matList.size() < 30){
			matList.push_back(frame_tmp);
			if (matList.size() == 30)
				w->OnImglistBufFull();
			ReleaseMutex(hMutex);//临界区结束  
		}else {
			std::cout<<"Drop one frame\n"<<std::fflush;
			ReleaseMutex(hMutex);//临界区结束  
		}
		Sleep(100);
	}
	
	return 0;
}


int main(int argc, char *argv[])
{

	hMutex = CreateMutex(NULL, false, NULL);

	HANDLE hThread_1 = CreateThread(NULL, 0, recordVideoThreadBody, "线程1", 0, NULL);  

	QApplication a(argc, argv);
	w = new MainWindow(0, hMutex, &matList);
	w->show();

	return a.exec();
}
