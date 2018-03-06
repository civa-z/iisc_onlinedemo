#include "compute.h"

using namespace cv;
using namespace std;

vector<double> computeSimilarity_PSNR() {
	vector<Mat> picList;
	for (int i = 0; i < fileNames.size(); ++i) {
		Mat pic = imread(fileNames[i].toStdString());
		picList.push_back(pic);
	}
	vector<double> similarity;
	for (int i = 1; i < picList.size(); ++i) {
		similarity.push_back(getPSNR(picList[i - 1], picList[i]));
	}
	return similarity;
};

vector<double> computeSimilarity_OSTU() {
	vector<Mat> picList;
	for (int i = 0; i < fileNames.size(); ++i) {
		Mat pic = imread(fileNames[i].toStdString());
		picList.push_back(pic);
	}
	vector<double> similarity;
	for (int i = 1; i < picList.size(); ++i) {
		similarity.push_back(ostuThreshold(picList[i - 1], picList[i]));
	}
	return similarity;
};
double getPSNR(const Mat& I1, const Mat& I2)
{
	Mat s1;
	absdiff(I1, I2, s1);       // |I1 - I2|
	s1.convertTo(s1, CV_32F);  // 不能在8位矩阵上做平方运算
	s1 = s1.mul(s1);           // |I1 - I2|^2

	Scalar s = sum(s1);         // 叠加每个通道的元素

	double sse = s.val[0] + s.val[1] + s.val[2]; // 叠加所有通道

	if( sse <= 1e-10) // 如果值太小就直接等于0
		return 0;
	else
	{
		double  mse =sse /(double)(I1.channels() * I1.total());
		double psnr = 10.0*log10((255*255)/mse);
		return psnr;
	}
}


void showPic(vector<Mat> picList) {
	int i = 0;
	for (int j = 0; j < picList.size(); ++j) {
		const string winname = "src" + i;
		namedWindow(winname, CV_WINDOW_NORMAL);
		imshow(winname, picList[j]);
		waitKey(0);
		destroyWindow(winname);
		i++;
	}
}

double ostuThreshold(const Mat& i1, const Mat& i2) {
	Mat img1gray;
	cvtColor(i1, img1gray, CV_BGR2GRAY);
	Mat img2gray;
	cvtColor(i2, img2gray, CV_BGR2GRAY);

	Mat img1thre;
	threshold(img1gray, img1thre, 0, 255, CV_THRESH_OTSU);

	Mat img2thre;
	threshold(img2gray, img2thre, 0, 255, CV_THRESH_OTSU);

	//assert has same width and height


	int height = i1.rows;
	int width = i1.cols;

	Mat cmpDst;
	bitwise_xor(img1thre, img2thre, cmpDst);

	return countNonZero(cmpDst) / (double)(height * width);
}

/*Here starts the final solution*/
vector<vector<double>> computeSimilarity_HASH(std::vector<cv::Mat> &matList){
	std::cout<<"Start computeSimilarity_HASH=========2:"<<matList.size()<<"\n"<<std::fflush;
	vector<string> hashstring;
	for (int i = 0; i < matList.size(); ++i) {
		hashstring.push_back(HashValue(matList[i]));
	}
	vector<vector<double>> similarity;
	for (int i = 0; i < hashstring.size(); ++i) {
		vector<double> tmp;
		for (int j = 0; j < hashstring.size(); ++j){
			
			tmp.push_back((double)HanmingDistance(hashstring[i], hashstring[j]) / 256);
		}
		similarity.push_back(tmp);
	}
	return similarity;
}

/*Here starts the final solution*/
vector<vector<double>> computeSimilarity_HASH() {
	vector<Mat> picList;
	for (int i = 0; i < fileNames.size(); ++i) {
		std::string name = fileNames[i].toStdString();
		Mat pic = imread(name);
		picList.push_back(pic);
	}
	vector<string> hashstring;
	for (int i = 0; i < picList.size(); ++i) {
		hashstring.push_back(HashValue(picList[i]));
	}

	vector<vector<double>> similarity;
	for (int i = 0; i < hashstring.size(); ++i) {
		vector<double> tmp;
		for (int j = 0; j < hashstring.size(); ++j)
			tmp.push_back((double)HanmingDistance(hashstring[i], hashstring[j]) / 256);
		similarity.push_back(tmp);
	}

	return similarity;
};

//均值Hash算法
string HashValue(Mat &src)
{
	string rst(256,'\0');
	Mat img;
	if(src.channels()==3)
		cvtColor(src,img,CV_BGR2GRAY);
	else
		img=src.clone();

	resize(img,img,Size(16,16));

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

	Mat mask= (img>=(uchar)average);

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

//汉明距离计算
int HanmingDistance(string &str1,string &str2)
{
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


double clusterDistance(vector<vector<double>> distance, int Ll, int Lr, int Rl, int Rr) {
	double result = 0.0;
	for (int i = Ll; i <= Lr; i++)
		for (int j = Rl; j <= Rr; j++)
			result = std::max(result, distance[i][j]);
	return result;
	//return result / (Lr - Ll + 1) / (Rr - Rl + 1);
}

TreeNode* clustering(vector<vector<double>> distance) {
	int n = distance.size();
	vector<TreeNode*> tree(n, NULL);
	for (int i = 0; i < n; ++i)
		tree[i] = new TreeNode(i);
	for (int i = 0; i < n - 1; ++i) {
		int m = tree.size();
		int p = 1;
		double dis = 1000000.0;
		for (int j = 1; j < m; ++j) {
			TreeNode* left = tree[j - 1];
			TreeNode* right = tree[j];
			double cur = clusterDistance(distance, left->l, left->r, right->l, right->r);
			if (cur < dis) {
				dis = cur;
				p = j;
			}
		}
		TreeNode* cluster = new TreeNode(tree[p - 1], tree[p], dis);
		tree.erase(tree.begin() + p);
		tree[p - 1] = cluster;
	}
	return tree[0];
}

void getDistance(TreeNode* root, vector<double>& distance) {
	if (root->left && root->right) {
		cout << root->l << ' ' << root->r << ' ' << root->threshold << endl;
		distance.push_back(root->threshold);
		getDistance(root->left, distance);
		getDistance(root->right, distance);
	}
}
void getPictures(TreeNode* root, double dis, vector<int>& P) {
	if (root->left && root->right && root->threshold > dis) {
		getPictures(root->left, dis, P);
		getPictures(root->right, dis, P);
	} else {
		P.push_back(root->l);
	}
}

double computeThreshold(vector<double> similarity) {
	double tmp;
	double opt;
	vector<double> copy = similarity;
	sort(copy.begin(), copy.end());
	double u0 = 0;
	double u1 = 0;
	for (int i = 0; i < similarity.size(); ++i) u1 += similarity[i];
	int n = similarity.size();
	double maxg = 0;
	int i = 0;
	while (i < n) {
		double th = copy[i];
		while (i < n && copy[i] == th) {
			u0 += th;
			u1 -= th;
			++i;
		}
		if (i == 0 || i == n) continue;
		double u00 = u0 / i;
		double u10 = u1 / (n - i);
		double w00 = (double)i / n;
		double w10 = (double)(n - i) / n;
		double g = w00 * w10 * (u00 - u10) * (u00 - u10);
		if (g >= maxg) {
			maxg = g;
			opt = th;
		}
	}
	return opt;
}

vector<int> getPictures(vector<vector<double>> distance) {
	TreeNode* root = clustering(distance);
	vector<double> dis;
	getDistance(root, dis);
	double th = computeThreshold(dis);
	vector<int> result;
	getPictures(root, th, result);
	return result;
}
