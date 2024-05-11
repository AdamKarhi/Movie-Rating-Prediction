#include"UBCF.h"
#include<iomanip>

UBCF::UBCF(Ratings& training) : trainingData(training.totalUsers), KNeighbours(9000) { 
	std::cout << "Initializing the class..." << std::endl;
	unsigned int size = training.ratings.size(); 

	userRatingCount = new std::pair<unsigned short int, unsigned short int>[training.totalUsers]();
	itemRatingCount = new std::pair<unsigned short int, unsigned short int>[training.totalItems]();
	UserRatingSquare = new float[training.totalUsers]();
	UserRatingAvg = new float[training.totalUsers]();
	ItemRatingAvg = new float[training.totalItems]();

	for (unsigned int i = 0; i < size; ++i) {
		trainingData.insert(training.ratings[i].UserId,
			training.ratings[i].ItemId, training.ratings[i].value); 
		userRatingCount[training.ratings[i].UserId].first = training.ratings[i].UserId;  
		userRatingCount[training.ratings[i].UserId].second++; 

		itemRatingCount[training.ratings[i].ItemId].first = training.ratings[i].ItemId; 
		itemRatingCount[training.ratings[i].ItemId].second++; 

		UserRatingSquare[training.ratings[i].UserId] += powf(training.ratings[i].value, 2); 
		UserRatingAvg[training.ratings[i].UserId] += training.ratings[i].value; 
		ItemRatingAvg[training.ratings[i].ItemId] += training.ratings[i].value; 
	}


	size = training.totalUsers;
	float tempSize = 0;
	for (unsigned int i = 1; i < size; ++i) {
		tempSize = userRatingCount[i].second;
		if (tempSize == 0) {
			UserRatingAvg[i] = 0.00f;
		}
		else {
			UserRatingAvg[i] /= tempSize;
		}

	}

	size = training.totalItems;
	for (unsigned int i = 1; i < size; ++i) {
		tempSize = itemRatingCount[i].second;
		if (tempSize == 0) {
			ItemRatingAvg[i] = 0.00f;
		}
		else {
			ItemRatingAvg[i] /= tempSize;
		}

	}

	std::sort(userRatingCount, userRatingCount + training.totalUsers, [](std::pair<unsigned short int, unsigned short int> x, std::pair<unsigned short int, unsigned short int> y) {
		return x.second > y.second;
		});


	std::sort(itemRatingCount, itemRatingCount + training.totalItems, [](std::pair<unsigned short int, unsigned short int> x, std::pair<unsigned short int, unsigned short int> y) {
		return x.second > y.second;
		});

	std::cout << std::endl << "Top 10 Movies" << std::endl;
	std::cout << std::left<<std::setw(4) << "#" << std::setw(12) << "Movie Id"<< std::setw(12) << "Ratings" << std::endl;
	for (int i = 0; i < 10; i++) {
		std::cout << std::left<<std::setw(4) << i + 1 << std::left<<std::setw(12)<< itemRatingCount[i].first << std::setw(12)<<std::setw(12) << itemRatingCount[i].second << std::endl;
	}

	std::cout << std::endl << "Top 10 Users" << std::endl;
	std::cout << std::left << std::setw(4) << "#" << std::setw(12) << "User Id" << std::setw(12) << "Ratings" << std::endl;
	for (int i = 0; i < 10; i++) {
		std::cout << std::left << std::setw(4) << i + 1 << std::left << std::setw(12) << userRatingCount[i].first << std::setw(12) << std::setw(12) << userRatingCount[i].second << std::endl;
	}

	std::cout << std::endl;

	matrix = trainingData.toMatrix();
}


UBCF::~UBCF() {
	delete[] matrix.matrix;
	delete[] userRatingCount;
	delete[] itemRatingCount;
	delete[] UserRatingSquare;
	delete[] UserRatingAvg;
	delete[] ItemRatingAvg;
}

Ratings UBCF::readTrainData(std::string fileName) {
	std::ifstream stream(fileName, std::ios::in);
	std::string data;
	Ratings ratings;
	ratings.totalItems = 0; 
	ratings.totalUsers = 0; 
	Rating rating;

	std::cout << "Reading training data..." << std::endl;
	std::getline(stream, data);

	while (stream.good()) { 
		std::getline(stream, data, ',');
		rating.UserId = std::stoi(data);
		std::getline(stream, data, ',');
		rating.ItemId = std::stoi(data);
		std::getline(stream, data);
		rating.value = std::stof(data);

		if (ratings.totalUsers < rating.UserId) { 
			ratings.totalUsers = rating.UserId; 
		}

		if (ratings.totalItems < rating.ItemId) { 
			ratings.totalItems = rating.ItemId; 
		}

		ratings.ratings.push_back(rating);
	}
	stream.close(); 
	ratings.totalItems += 1; 
	ratings.totalUsers += 1;
	return ratings;
}


validationData  UBCF::readValidationData(std::string fileName) {
	std::ifstream stream(fileName, std::ios::in);
	std::string data;
	int UserId;
	int ItemId;
	float rating;
	validationData vdata;

	std::getline(stream, data);

	while (stream.good()) {
		std::getline(stream, data, ',');
		UserId = std::stoi(data);
		std::getline(stream, data, ',');
		ItemId = std::stoi(data);
		std::getline(stream, data);
		rating = std::stof(data);
		vdata.x.push_back(std::pair<int, int>(UserId, ItemId)); 
		vdata.y.push_back(rating); 
	}

	stream.close();

	return vdata;
}

testData  UBCF::readTestData(std::string fileName) {
	std::ifstream stream(fileName, std::ios::in);
	std::string data;
	int UserId;
	int ItemId;
	int id;
	testData tData;

	std::getline(stream, data);

	while (stream.good()) {
		std::getline(stream, data, ',');
		id = std::stoi(data);
		std::getline(stream, data, ',');
		UserId = std::stoi(data);
		std::getline(stream, data);
		ItemId = std::stoi(data);
		tData.x.push_back(std::pair<int, int>(UserId, ItemId));
		tData.id.push_back(id);
	}

	stream.close();

	return tData;
}

void  UBCF::writeTestPredictions(std::string fileName, std::vector<int> ids, float* pred) { 
	std::cout << "Writing predictions..." << std::endl;
	std::ofstream stream(fileName, std::ios::out);


	stream << "ID,Predicted" << std::endl;
	unsigned short int size = ids.size();
	for (unsigned short int i = 0; i < size; i++) { 
		stream << ids[i] << "," << pred[i] << std::endl; 
	}

	stream.close();
}

float UBCF::simpleCosineSimilarity(std::vector<std::pair<unsigned short int, float>>* v1, float* v2, unsigned short int trainIndex, unsigned short int testIndex) {
	double x = 0;

	unsigned short int size = v1->size();
	for (unsigned short int k = 0; k < size; ++k) {
		x += (*v1)[k].second * v2[(*v1)[k].first];
	}

	return x / (sqrtf(UserRatingSquare[trainIndex]) * sqrtf(UserRatingSquare[testIndex]));
}

inline float UBCF::advancedCosineSimilarity(std::vector<std::pair<unsigned short int, float>>* v1, float* v2, unsigned short int v1Index, unsigned short int v2Index) {
	double x = 0;
	unsigned int size_v1 = v1->size();
	float v1_avg = UserRatingAvg[v1Index]; 
	float v2_avg = UserRatingAvg[v2Index]; 
	double v1_norm = 0;
	double v2_norm = 0;
	float tempV1, tempV2;

	for (int k = 0; k < size_v1; ++k) {
		if (v2[(*v1)[k].first] == 0) { 
			continue; 
		}
		tempV1 = (*v1)[k].second - v1_avg;
		tempV2 = v2[(*v1)[k].first] - v2_avg;
		x += (tempV1 * tempV2);
		v1_norm += (tempV1 * tempV1);
		v2_norm += (tempV2 * tempV2);
	}

	float temp = (float)sqrtf(v1_norm * v2_norm); 

	if (temp == 0) { 
		return -1.0f; 
	}

	return (x / temp); 
}

inline float  UBCF::ITR(std::vector<std::pair<unsigned short int, float>>* v1, float* v2, unsigned short int v1Index, unsigned short int v2Index) {

	unsigned int size = v1->size();
	double SimTri = 0;
	double r2_v1 = 0, r2_v2 = 0;
	double sigma_v1 = 0, sigma_v2 = 0;
	double avg_v1 = UserRatingAvg[v1Index], avg_v2 = UserRatingAvg[v2Index];
	for (int i = 0; i < size; i++) {
		if (v2[(*v1)[i].first] == 0) {
			continue;
		}

		SimTri += (((*v1)[i].second - v2[(*v1)[i].first]) * ((*v1)[i].second - v2[(*v1)[i].first]));
		r2_v1 += ((*v1)[i].second * (*v1)[i].second);
		r2_v2 += (v2[(*v1)[i].first] * v2[(*v1)[i].first]);
		sigma_v1 += ((*v1)[i].second - avg_v1) * ((*v1)[i].second - avg_v1);
		sigma_v2 += (v2[(*v1)[i].first] - avg_v2) * (v2[(*v1)[i].first] - avg_v2);
	}

	sigma_v1 = sqrt(sigma_v1 / (float)size);
	sigma_v2 = sqrt(sigma_v2 / (float)trainingData.get(v2Index)->size());
	double tempSquare = (sqrt(r2_v1) + sqrt(r2_v2));
	if (tempSquare == 0) {
		return 0.0f;
	}
	SimTri = 1 - (sqrt(SimTri) / tempSquare);
	double SimURP = 1 - (1 / (1 + exp(-1 * (abs(sigma_v1 - sigma_v2) * abs(avg_v1 - avg_v2)))));

	return (SimURP * SimTri);
}

void inline  UBCF::trainThread(std::vector<std::pair<int, int>>& testingData, unsigned short startTestingSize, unsigned short endTestingSize, unsigned short int trainingSize
	, SimilarityMatrix& sMatrix) {
	std::vector<std::pair<unsigned short int, float>>* data;
	double x;
	int j;
	int k = 0;
	for (unsigned int i = startTestingSize; i < endTestingSize; i++) { 
		data = trainingData.get(testingData[i].first);
		for (j = 1; j < trainingSize; ++j) {
			x = 0;
			if (testingData[i].first == j) { 
				continue;
			}
			x = advancedCosineSimilarity(data, matrix.matrix[j], testingData[i].first, j); 
			sMatrix[i].insert({ j,x }); 
		}
	}
}

//actual train function, it equaly divide the training data into small chunks for each thread
SimilarityMatrix  UBCF::train(std::vector<std::pair<int, int>>& testingData) {
	std::cout << "Training the model..." << std::endl;
	unsigned short int const threadsSize = std::thread::hardware_concurrency(); //get the idle number of threads according to CPU
	std::thread* threads = new std::thread[threadsSize]; //thread array

	unsigned short int testingSize = testingData.size();
	unsigned short int trainingSize = trainingData.getRows();
	unsigned short int threadTrainingSize = testingSize / threadsSize; //calculate the size

	SimilarityMatrix smatrix(testingSize, SortedK(KNeighbours)); //create the similarity matrix with sorted k values


	int testingSizeStart, testingSizeEnd;

	for (int i = 0; i < threadsSize; i++) {
		//calculating the size for each thread's test data
		testingSizeStart = i * threadTrainingSize;
		testingSizeEnd = testingSizeStart + threadTrainingSize;

		if (i == threadsSize - 1) {
			testingSizeEnd = testingSize;
		}

		//run the threads
		threads[i] = std::thread([this, &testingData, testingSizeStart, testingSizeEnd, trainingSize, &smatrix]() {
			trainThread(testingData, testingSizeStart, testingSizeEnd, trainingSize, smatrix);
			});
	}

	//wait for all threads keep working
	for (int i = 0; i < threadsSize; i++) {
		threads[i].join();
	}

	delete[] threads;

	return smatrix;
}

//calculate the predictions according to the similarity matrix for each threads
void  UBCF::predictions(int startTestingData, int endTestingData, std::vector<std::pair<int, int>>& testingData, SimilarityMatrix& sm, int k, float* predictions) {
	int itemIndex = 0;
	std::vector<std::pair<unsigned short int, float>> data;
	int size = 0;
	int L = 0;
	float x = 0;
	float weightSum = 0;
	for (int i = startTestingData; i < endTestingData; ++i) {
		itemIndex = testingData[i].second;
		data = sm[i].getData();
		size = data.size();
		L = 0;
		x = 0;
		weightSum = 0;
		for (int j = 0; j < k && L < size; j++, ++L) { //loop until k similar users
			if (matrix.matrix[data[L].first][itemIndex] == 0) { //if the user did not give the rating pass the users
				continue;
			}
			x += ((matrix.matrix[data[L].first][itemIndex] - UserRatingAvg[data[L].first]) * data[L].second); //calculate the weighted sum
			weightSum += abs(data[L].second); //calculate the sum of weights
		}
		if (weightSum != 0) { //if weights sum are zero
			predictions[i] = (UserRatingAvg[testingData[i].first] + (x / (float)weightSum)); //calculate the weighted average
		}
		else {
			predictions[i] = 2.5f; //return the fixed prediction
		}

	}
}

//return the predictions for each test users
float* UBCF::predict(SimilarityMatrix sm, std::vector<std::pair<int, int>>& testingData) {
	std::cout << "Prediction the ratings..." << std::endl;
	
	float* predictionsRes = new float[testingData.size()]; //array to store the predictions

	unsigned short int const threadsSize = std::thread::hardware_concurrency();
	std::thread* threads = new std::thread[threadsSize];

	unsigned short int testingSize = testingData.size();
	unsigned short int threadTrainingSize = testingSize / threadsSize;

	int testingSizeStart, testingSizeEnd;

	for (int i = 0; i < threadsSize; i++) {
		testingSizeStart = i * threadTrainingSize;
		testingSizeEnd = testingSizeStart + threadTrainingSize;

		if (i == threadsSize - 1) {
			testingSizeEnd = testingSize;
		}

		//run the thread to get the predictions
		threads[i] = std::thread([this, &testingData, testingSizeStart, testingSizeEnd, &sm, predictionsRes]() {
			predictions(testingSizeStart, testingSizeEnd, testingData, sm, KNeighbours, predictionsRes);
			});
	}

	for (int i = 0; i < threadsSize; i++) {
		threads[i].join();
	}

	delete[] threads;
	return predictionsRes;
}

//root mean square error
float UBCF::RMSE(float* pred, std::vector<float> groundTruth) {
	int size = groundTruth.size();
	double x = 0;
	for (int i = 0; i < size; i++) {
		x += powf((pred[i] - groundTruth[i]), 2);
	}

	return sqrtf(x / (float)size);
}