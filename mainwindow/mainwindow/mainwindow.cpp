#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QAction>
#include <QMessageBox>
#include <iostream>
#include <algorithm>



using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    iconWidth = ui->resWidget->width() / 2;
    iconHeight = ui->resWidget->height() / 2;
    iconWidth = 96;
    iconHeight = 96;
    ui->resWidget->setIconSize(QSize(iconWidth, iconHeight));
    ui->resWidget->setResizeMode(QListView::Adjust);
    ui->resWidget->setViewMode(QListView::IconMode);
    ui->resWidget->setMovement(QListView::Static);
    ui->resWidget->setSpacing(10);

	qButtonGroup = new QButtonGroup(this);
	qButtonGroup->addButton(ui->radioButton_Scene, 1);
    qButtonGroup->addButton(ui->radioButton_Face, 2);
    qButtonGroup->addButton(ui->radioButton_Alignment, 3);

	timer =new QTimer(this);
	connect(timer, SIGNAL(timeout()),this, SLOT(ReadFrame()));
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(OnButtonClicked()));
	connect(ui->radioButton_Scene, SIGNAL(clicked()), this, SLOT(OnTypeButtonClicked()));
	connect(ui->radioButton_Face, SIGNAL(clicked()), this, SLOT(OnTypeButtonClicked()));
	connect(ui->radioButton_Alignment, SIGNAL(clicked()), this, SLOT(OnTypeButtonClicked()));
	filterType = "sence";	
}

MainWindow::~MainWindow()
{
	delete qButtonGroup;
    delete ui;
}

void MainWindow::OnButtonClicked(){
	imagefilter_hash_face.loadFaceRecognizer("./data/FA/");
	imagefilter_alignment.loadFaceRecognizer("./data/FA/");
	capture =  cv::VideoCapture(0);
	if (!capture.isOpened()) {
		QMessageBox::warning(this,tr("error"),tr("capture is not opened"),QMessageBox::Ok);
		return;
	}
	timer->start(100);
}

void MainWindow::OnTypeButtonClicked(){
	filterType = getFilterType();
	imagefilter_hash_face.reset();
	imagefilter_alignment.reset();
	matList.clear();
	std::cout<<std::endl<<"Start "<<filterType<<std::endl<<std::fflush;
}

void MainWindow::ReadFrame(){
	if (capture.isOpened()){
		cv::Mat frame;
		capture >> frame;
		matList.push_back(frame.clone());

		if(filterType == "Face"){
			imagefilter_hash_face.prepareFeature(frame.clone());
			imagefilter_hash_face.renderImage(frame);
		} else if (filterType == "Alignment"){
			imagefilter_alignment.prepareFeature(frame.clone());
			imagefilter_alignment.renderImage(frame);
		}
		showOneVideoFrame(frame);
	} 
	if (matList.size() == 10){
		computerAndShowPictures();
		matList.clear();
	}
}

void MainWindow::showOneVideoFrame(cv::Mat& mat){
	QImage qimg;
	cv::Mat rgb;
	if(mat.channels() == 3)    // RGB image  
	{  
		cv::cvtColor(mat,rgb,CV_BGR2RGB);  
		qimg = QImage((const uchar*)(rgb.data),  //(const unsigned char*)  
						rgb.cols,rgb.rows,  
						rgb.cols*rgb.channels(),   //new add  
						QImage::Format_RGB888);  
	}else                     // gray image  
	{  
		qimg = QImage((const uchar*)(mat.data),  
						mat.cols, mat.rows,  
						mat.cols * mat.channels(),    //new add  
						QImage::Format_Indexed8);  
	}
	QPixmap img = QPixmap::fromImage(qimg);
	double ratio = (double)max((double)img.width() / ui->videoWidget->width(),
		(double)img.height() / ui->videoWidget->height());

	ui->videoWidget->setPixmap(img.scaled(QSize(img.width() / ratio, img.height() /ratio)));  
	ui->videoWidget->show(); 
}

/*Display one result to front-end*/
void MainWindow::showOneRes(cv::Mat& mat, int nIndex) {
	QImage qimg;
	cv::Mat rgb;
	if(mat.channels() == 3)    // RGB image  
	{  
		cv::cvtColor(mat,rgb,CV_BGR2RGB);  
		qimg = QImage((const uchar*)(rgb.data),  //(const unsigned char*)  
						rgb.cols,rgb.rows,  
						rgb.cols*rgb.channels(),   //new add  
						QImage::Format_RGB888);  
	}else                     // gray image  
	{  
		qimg = QImage((const uchar*)(mat.data),  
						mat.cols, mat.rows,  
						mat.cols * mat.channels(),    //new add  
						QImage::Format_Indexed8);  
	}
    QPixmap img = QPixmap::fromImage(qimg);
    double ratio = (double)max(img.width(), img.height()) / (iconWidth);
    int itemWidth = img.width() / ratio;
    int itemHeight = img.height() / ratio;

    QListWidgetItem *pItem = new QListWidgetItem(ui->resWidget);
    pItem->setIcon(QIcon(img.scaled(QSize(itemWidth,itemHeight))));
    //string spath = path.toStdString();
    //pItem->setText(QString::fromStdString(name));
    pItem->setTextAlignment(Qt::AlignHCenter);
    pItem->setSizeHint(QSize(iconWidth,iconHeight));
    ui->resWidget->insertItem(nIndex, pItem);
}

/*delete all loaded image files each time 'open' gets clicked*/
void MainWindow::deleteAllItems() {
    ui->resWidget->clear();
}


std::string MainWindow::getFilterType(){
	if (ui->radioButton_Scene->isChecked())
		return "Scene";
	if (ui->radioButton_Face->isChecked())
		return "Face";
	if (ui->radioButton_Alignment->isChecked())
		return "Alignment";
	return "Other";
}

void MainWindow::computerAndShowPictures(){
	ui->resWidget->clear();
	
	vector<vector<double>> similarity;
	std::cout<<std::endl<<"Computer "<<filterType<<std::endl<<std::fflush;
	if (filterType.compare("Face") == 0){
		similarity = imagefilter_hash_face.computeSimilarity();
	} else if (filterType.compare("Alignment") == 0){
		similarity = imagefilter_alignment.computeSimilarity();
	} else {
		similarity = computeSimilarity_HASH(matList);
	}

    int nIndex = 0;
	std::cout<<"ShowPictures similarity.size():"<<similarity.size()<<std::endl<<std::flush;
    vector<int> result = getPictures(similarity);
    for (int i = 0; i < result.size(); ++i) {
		std::cout<<"ShowPictures:"<<result[i]<<std::endl<<std::flush;
        showOneRes(matList[result[i]], nIndex);
        nIndex++;
    }
}
