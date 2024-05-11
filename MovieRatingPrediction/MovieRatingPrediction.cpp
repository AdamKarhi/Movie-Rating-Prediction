#include <iostream>
#include"UBCF.h"
#include <chrono>

using namespace std;


int main()
{
	Ratings ratingData = UBCF::readTrainData("train.csv");
	UBCF ubcf(ratingData);
	testData tData = UBCF::readTestData("test.csv");

	auto start = std::chrono::system_clock::now();
	SimilarityMatrix sm = ubcf.train(tData.x);
	float * predictions = ubcf.predict(sm, tData.x);

	UBCF::writeTestPredictions("PredOutput.csv", tData.id, predictions);
	auto end = std::chrono::system_clock::now();
	std::cout << (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / (float)60000 << " minutes" << std::endl;

	return 0;
}