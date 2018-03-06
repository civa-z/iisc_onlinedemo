#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QAction>
#include <iostream>
#include <algorithm>



using namespace std;
MainWindow::MainWindow(QWidget *parent, HANDLE hMutex, std::vector<cv::Mat> *matList_buf) :
    QMainWindow(parent),
	hMutex(hMutex),
	matList_buf(matList_buf),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    iconWidth = ui->resWidget->width() / 2;
    iconHeight = ui->resWidget->height() / 2;
    iconWidth = 192;
    iconHeight = 192;
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
	connect(this, SIGNAL(startHandleFrame()),this, SLOT(ReadFram())); 
}

MainWindow::~MainWindow()
{
	delete qButtonGroup;
    delete ui;
}


void MainWindow::ReadFram(){
	WaitForSingleObject(hMutex, INFINITE);//临界区开始  30ms
	matList = *matList_buf;
	matList_buf->clear();
	ReleaseMutex(hMutex);//临界区结束 

	computerAndShowPictures();
	matList.clear();
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
    int itemWidth = img.width() / 2;
    int itemHeight = img.height() / 2;

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
	string filterType = getFilterType();
	vector<vector<double>> similarity;
	std::cout<<"filterType: "<<filterType<<std::endl<<std::fflush;

	if (filterType.compare("Face") == 0){
		std::cout<<std::endl<<"Start Face"<<std::endl<<std::fflush;
		imagefilter_hash_face.loadFaceRecognizer("E:\\Work\\IISC_2017\\LBF_Face_Alignment_for_MJ\\data\\FA\\");
		imagefilter_hash_face.setPictureList(matList);
		similarity = imagefilter_hash_face.computeSimilarity();
	} else if (filterType.compare("Alignment") == 0){
		imagefilter_alignment.loadFaceRecognizer("E:\\Work\\IISC_2017\\LBF_Face_Alignment_for_MJ\\data\\FA\\");
		imagefilter_alignment.setPictureList(matList);
		similarity = imagefilter_alignment.computeSimilarity();
	} else {
		std::cout<<std::endl<<"Start Sence"<<std::endl<<std::fflush;
		similarity = computeSimilarity_HASH(matList);
	}

    int nIndex = 0;
    vector<int> result = getPictures(similarity);
    for (int i = 0; i < result.size(); ++i) {
		std::cout<<std::endl<<"Start Face:"<<result[i]<<std::endl<<std::fflush;
        showOneRes(matList[result[i]], nIndex);
        nIndex++;
    }
}
