#include"SparseMatrix.h"

//constructor to create the sparse matrix
SparseMatrix::SparseMatrix(unsigned short int rows) {
	this->rows = rows;
	matrix = new std::vector<std::pair<unsigned short int, float>>[rows];
	cols = 0;
}

//delete the array of the data
SparseMatrix::~SparseMatrix() {
	delete[] matrix;
}

//insert the data in to the matrix
void SparseMatrix::insert(const unsigned short int row, const unsigned short int col, float value) {
	matrix[row].push_back({ col, value }); //geting the row and push the data into the vector
	if (cols < col) { //if the added column is less then the previous
		cols = col; //replace the previous to new one
	}
}


//return the vector of particular row
std::vector<std::pair<unsigned short int, float>>* SparseMatrix::get(const unsigned short int row) {
	return &matrix[row];
}

//get the number of rows
int SparseMatrix::getRows() {
	return rows;
}

//get the numbers of columns
int SparseMatrix::getCols() {
	return cols + 1; //this is the maximum column present in the sparse matrix
}

//convert sparse to dense matrix
Matrix SparseMatrix::toMatrix() {
	Matrix m;
	m.matrix = new float* [rows]; //create the array of the matrix rows
	int j = 0;
	int size = 0;
	for (int i = 0; i < rows; ++i) {
		size = matrix[i].size();
		m.matrix[i] = new float[cols + 1](); //create the array of matrix columns, it initialize all values to zero
		for (j = 0; j < size; ++j) {
			m.matrix[i][matrix[i][j].first] = matrix[i][j].second; //add the data to the columns
		}
	}
	m.rows = rows;
	m.cols = cols + 1;

	return m; //return the matrix
}