#pragma once
#include<map>
#include<unordered_map>
#include<cmath>
#include<thread>

typedef std::unordered_map<unsigned short int, std::unordered_map<unsigned short int, float>> sparse_t;

class SparseTempMatrix {
private:
	sparse_t matrix;
	unsigned short int rows, cols;
public:
	SparseTempMatrix() {
		rows = 0;
		cols = 0;
	}

	~SparseTempMatrix() {
	}

	void insert(const unsigned short int row, const unsigned short int col, float value) {
		matrix[row][col] = value;
		if (cols < col) {
			cols = col;
		}

		if (rows < row) {
			rows = row;
		}
	}

	float get(const unsigned short int row, const unsigned short int col) {
		return matrix[row][col];
	}

	std::unordered_map<unsigned short int, float>* get(const unsigned short int row) {
		return &matrix[row];
	}

	int getRows() {
		return rows+1;
	}

	int getCols() {
		return cols + 1;
	}

	void threadSubmatrix(SparseTempMatrix &sm, sparse_t::iterator &it_row, sparse_t::iterator& it_row_end) {
		float x = 0;
		for (sparse_t::iterator it_row_col = it_row; it_row_col != it_row_end; ++it_row_col) {
			x = 0;
			for (std::unordered_map<unsigned short int, float>::iterator it_data = it_row->second.begin(); it_data != it_row->second.end(); ++it_data) {
				x += it_data->second * it_row_col->second[it_data->first];
			}
			sm.insert(it_row->first, it_row_col->first, x);
		}
	}

	SparseTempMatrix cosineSimilarity() {
		SparseTempMatrix sm;
		
		std::unordered_map<unsigned short int, float> it_col;
		sparse_t::iterator it_row_end = matrix.end();

		for (sparse_t::iterator it_row = matrix.begin(); it_row != it_row_end; ++it_row) {
			
			
		}

		return sm;
	}

	Matrix toMatrix() {
		Matrix m;
		m.matrix = new float* [rows];
		std::unordered_map<unsigned short int, float>::const_iterator  it;
		for (int i = 0; i < rows; ++i) {
			std::unordered_map<unsigned short int, float>::const_iterator end = matrix[i].end();
			m.matrix[i] = new float[cols + 1]();
			for (it = matrix[i].begin(); it != end; ++it) {
				m.matrix[i][it->first] = it->second;
			}
		}
		m.rows = rows;
		m.cols = cols + 1;

		return m;
	}
};