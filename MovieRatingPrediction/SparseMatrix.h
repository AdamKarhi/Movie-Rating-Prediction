#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H
#include<map>
#include<vector>
#include<cmath>

struct Matrix {
	float** matrix;
	unsigned short int rows;
	unsigned short int cols;
};

class SparseMatrix {
private:
	std::vector<std::pair<unsigned short int, float>>* matrix;
	unsigned short int rows, cols;
public:

	//constructor to create the sparse matrix
	SparseMatrix(unsigned short int rows);

	//delete the array of the data
	~SparseMatrix();

	//insert the data in to the matrix
	void insert(const unsigned short int row, const unsigned short int col, float value);

	//return the vector of particular row
	std::vector<std::pair<unsigned short int, float>>* get(const unsigned short int row);

	//get the number of rows
	int getRows();

	//get the numbers of columns
	int getCols();

	//convert sparse to dense matrix
	Matrix toMatrix();
};


#endif