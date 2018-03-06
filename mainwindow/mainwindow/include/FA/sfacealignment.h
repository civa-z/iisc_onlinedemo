//Created by Kuanhong Xu.

#ifndef FACE_ALIGNMENT
#define FACE_ALIGNMENT

#include <iostream>
#include <fstream>
using namespace std;
/*
#ifndef min
#define min(x,y) (x<y)?x:y;
#endif
#ifndef max
#define max(x,y) (x>y)?x:y;
#endif
*/
typedef unsigned char uchar;

struct Params{

	float bagging_overlap;
	int max_numtrees;
	int max_depth;
	int landmark_num;
	int initial_num;

	int max_numstage;
	float max_radio_radius[10];
	int max_numfeats[10]; 
	int max_numthreshs;
};

class BoundingBox{
public:
	float start_x;
	float start_y;
	float width;
	float height;
	float centroid_x;
	float centroid_y;
	float faceRoll;
	float facePitch;
	float faceYaw;
	float score;
	BoundingBox(){
		start_x = 0;
		start_y = 0;
		width = 0;
		height = 0;
		centroid_x = 0;
		centroid_y = 0;
		faceRoll = 0;
		facePitch = 0;
		faceYaw = 0;
		score = 0;
	}; 
};

struct feature_node 
{
	int index;
	float value;
};

enum { L2R_LR, L2R_L2LOSS_SVC_DUAL, L2R_L2LOSS_SVC, L2R_L1LOSS_SVC_DUAL, MCSVM_CS, L1R_L2LOSS_SVC, L1R_LR, L2R_LR_DUAL, L2R_L2LOSS_SVR = 11, L2R_L2LOSS_SVR_DUAL, L2R_L1LOSS_SVR_DUAL }; /* solver_type */ //baoliu

struct parameter
{
	int solver_type;

	float eps;
	float C;
	int nr_weight;
	int *weight_label;
	float* weight;
	float p;
};

struct model
{
	struct parameter param;
	int nr_class;
	int nr_feature;
	float *w;
	int *label;
	float bias;
};

float predict_values( const struct model *model_,  const struct feature_node *x,  float* dec_values);
float predict( const struct model *model_,  const struct feature_node *x);
struct model *load_model_bin( std::ifstream& fin);
void free_model_content( struct model *model_ptr);
void free_and_destroy_model( struct model **model_ptr_ptr);
int check_regression_model( const struct model *model);

class Node {
public:
	bool issplit;
	int pnode;
	int depth;
	int cnodes[2];
	bool isleafnode;
	float thresh;
	float feat[4];

	Node(){
		issplit = 0;
		pnode = 0;
		depth = 0;
		cnodes[0] = 0;
		cnodes[1] = 0;
		isleafnode = 0;
		thresh = 0;
		feat[0] = 0;
		feat[1] = 0;
		feat[2] = 0;
		feat[3] = 0;
	}
	void Read( std::ifstream& fin);
};

class Tree{
public:
	int landmarkID_;
	int max_depth_;
	int max_numnodes_;
	int num_leafnodes_;
	int num_nodes_;
	int max_numfeats_;
	float max_radio_radius_;
	float overlap_ration_;
	int *id_leafnodes_;
	Node *nodes_;

	Tree(){
	}
	~Tree(){
		delete[] nodes_;
		delete[] id_leafnodes_;
	}
	void Read( std::ifstream& fin);

};

class RandomForest{
public:
	Tree **rfs_;
	int max_numtrees_;
	int num_landmark_;
	int max_depth_;
	int stages_;
	float overlap_ratio_;


	RandomForest(){
		max_numtrees_ = 0;
		num_landmark_ = 0;
		max_depth_    = 0;
		overlap_ratio_ = 0;
		stages_ = 0;
	}
	~RandomForest(){
		for (int i=0;i<num_landmark_;i++)
			delete[] rfs_[i];
		delete[] rfs_;
	}
	void Read( std::ifstream& fin);
};

class SFaceAlignment{
public:
	RandomForest * RandomForest_;
	struct model*** Models_;
	float * mean_shape_;
	Params global_params;
	struct feature_node * binfeature;
public:
	SFaceAlignment(char *modelPath){
		std::cout<<std::endl<<"3:"<<modelPath<<std::endl<<std::fflush;
		Load(modelPath);
		std::cout<<std::endl<<"4"<<std::endl<<std::fflush;
	}
	~SFaceAlignment(){
		for (int i = 0;i < global_params.max_numstage;i++)
			for (int j=0;j<global_params.max_numtrees;j++)
				free_and_destroy_model(&Models_[i][j]);
		delete[] Models_;
		delete[] RandomForest_;
		delete[] mean_shape_;
		delete[] binfeature;
	}
	void Load( char *path);
	void DeriveBinaryFeat( const RandomForest& randf,  const uchar *image,  int imgwidth,  int imgheight,  float *current_shape, const BoundingBox & bounding_box, struct feature_node *binfeature);
	void ResetFeatureSpace( struct feature_node *binfeature);
	void GlobalPrediction( struct feature_node*binfeature,  float *current_shape,  const BoundingBox &bounding_box,  int stage);    
	void sFaceAlignment( const uchar *image,  int imgwidth,  int imgheight,  const BoundingBox& bounding_box,  float *current_shape);
	void ReadRegressor( ifstream& fin,  char *modelPath);
	void ReadGlobalParam( ifstream& fin);
};
void ProjectShape( float *shape,  const BoundingBox& bounding_box,  Params global_params);
void ReProjectShape( float *shape,  const BoundingBox& bounding_box,  Params global_params);
void SimilarityTransform( const float *shape1,  const float *shape2,  float *rotation, float& scale,  Params global_params);
#endif