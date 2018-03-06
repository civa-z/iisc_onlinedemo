#ifndef COMPUTE_H
#define COMPUTE_H
#include "QFileDialog"
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace cv;
using namespace std;

/*store user selected files name*/
QStringList fileNames;

void showPic(std::vector<cv::Mat> picList);
vector<int> getPictures(vector<vector<double>> distance);

vector<vector<double>> computeSimilarity_HASH(std::vector<cv::Mat> &matList);
vector<vector<double>> computeSimilarity_HASH();
string HashValue(Mat &src);
int HanmingDistance(string &str1,string &str2);
double ostuThreshold(const Mat& i1, const Mat& img2);
double computeThreshold(vector<double> similarity);

struct TreeNode {
    int l, r;
    TreeNode* left;
    TreeNode* right;
    double threshold;
    TreeNode(int i): l(i), r(i), left(NULL), right(NULL), threshold(0.0) {}
    TreeNode(TreeNode* left, TreeNode* right, double threshold):
        l(left->l), r(right->r),
        left(left), right(right), threshold(threshold) {}
};

TreeNode* clustering(vector<vector<double>> similarity);


/*deprecated*/
vector<double> computeSimilarity_PSNR();
vector<double> computeSimilarity_OSTU();
double getPSNR(const Mat& I1, const Mat& I2);
#endif // COMPUTE_
