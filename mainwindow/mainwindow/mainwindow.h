#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QListWidgetItem>
#include <QtGui/QButtonGroup>
#include <QTimer>
#include <iostream>
#include <windows.h>

#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp> 

#include "imagefilter.h"
#include "imagefilter_hash.h"
#include "imagefilter_hash_face.h"
#include "imagefilter_alignment_face.h"


extern QStringList fileNames;
using namespace std;
extern vector<double> computeSimilarity_PSNR();
extern vector<vector<double>> computeSimilarity_HASH(std::vector<cv::Mat> &matList);
extern vector<double> computeSimilarity_OSTU();
extern double computeThreshold(vector<double> similarity);
extern vector<int> getPictures(vector<vector<double>> distance);
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	void computerAndShowPictures();

private:
    Ui::MainWindow *ui;

private:
    void deleteAllItems();
    void showOneRes(cv::Mat& mat, int nIndex);
	void showOneVideoFrame(cv::Mat& mat);
	QButtonGroup* qButtonGroup;
    int iconWidth;
    int iconHeight;

	std::string getFilterType();
	ImageFilterHash imagefilter_hash;
	FaceImageFilter imagefilter_hash_face;
	ImageFilterAlignmentFace imagefilter_alignment;

public slots: 
	void OnButtonClicked();
	void OnTypeButtonClicked();
	void ReadFrame();

private:
	QTimer* timer; 
	cv::VideoCapture capture;
	std::vector<cv::Mat> matList;
	string filterType;
};

#endif // MAINWINDOW_H
