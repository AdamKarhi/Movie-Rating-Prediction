#ifndef USERBASED_H
#define USERBASED_H
#include"SparseMatrix.h"
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<algorithm>
#include<thread>
#include<cmath>
#include"SortedK.h"

typedef std::vector<SortedK> SimilarityMatrix;

struct Rating {
	unsigned short int UserId;
	unsigned short int ItemId;
	float value;
};

struct Ratings {
	std::vector<Rating> ratings;
	unsigned short int totalUsers;
	unsigned short int totalItems;
};

struct validationData {
	std::vector<std::pair<int, int>> x;
	std::vector<float> y;
};

struct testData {
	std::vector<std::pair<int, int>> x;
	std::vector<int> id;
};

class UBCF {
private:
	SparseMatrix trainingData; //sparse matrix for training data
	Matrix matrix; //dense matrix for fast access
	std::pair<unsigned short int, unsigned short int>* userRatingCount; //array to store the counts for user ratings
	std::pair<unsigned short int, unsigned short int>* itemRatingCount; //array to store the counts for Item Ratings
	float* UserRatingSquare; //array to store the square of users ratings
	float* UserRatingAvg; //array to store the average of users ratings
	float* ItemRatingAvg; //array to store the average of items ratings
	const unsigned short int KNeighbours; //number of nearerst neighbours to select



	//Simple cosine simlarity, RMSE > 1.0
	inline float simpleCosineSimilarity(std::vector<std::pair<unsigned short int, float>>* v1, float* v2, unsigned short int trainIndex, unsigned short int testIndex);

	//Modified cosine similarity, RMSE = 0.94
	inline float advancedCosineSimilarity(std::vector<std::pair<unsigned short int, float>>* v1, float* v2, unsigned short int v1Index, unsigned short int v2Index);

	//Triangle similarity, RMSE = 0.96, Limited approach
	inline float ITR(std::vector<std::pair<unsigned short int, float>>* v1, float* v2, unsigned short int v1Index, unsigned short int v2Index);

	//training process for each thread
	void inline trainThread(std::vector<std::pair<int, int>>& testingData, unsigned short startTestingSize, unsigned short endTestingSize, unsigned short int trainingSize
		, SimilarityMatrix& sMatrix);

	//calculate the predictions according to the similarity matrix for each threads
	void predictions(int startTestingData, int endTestingData, std::vector<std::pair<int, int>>& testingData, SimilarityMatrix& sm, int k, float* predictions);
public:

	UBCF(Ratings& training);

	//destructor to free the memmory
	~UBCF();

	//static function to read the training data independently
	static Ratings readTrainData(std::string fileName);

	//reading the validaion data i.e. test data and ground truth
	static validationData readValidationData(std::string fileName);

	//read only the test data
	static testData readTestData(std::string fileName);

	//writng the predictions to the file
	static void writeTestPredictions(std::string fileName, std::vector<int> ids, float* pred);

	//actual train function, it equaly divide the training data into small chunks for each thread
	SimilarityMatrix train(std::vector<std::pair<int, int>>& testingData);

	//return the predictions for each test users
	float* predict(SimilarityMatrix sm, std::vector<std::pair<int, int>>& testingData);

	//root mean square error
	static float RMSE(float* pred, std::vector<float> groundTruth);
		
};

#endif // !USERBASED_H
