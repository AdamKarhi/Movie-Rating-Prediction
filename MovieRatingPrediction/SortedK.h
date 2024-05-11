#ifndef SORTEDK_H
#define SORTEDK_H

#include<queue>
#include<vector>

auto compare = [](std::pair<unsigned short int, float> a, std::pair<unsigned short int, float> b) { return a.second > b.second; };

template <class T, class S, class C>
S& Container(std::priority_queue<T, S, C>& q) {
	struct HackedQueue : private std::priority_queue<T, S, C> {
		static S& Container(std::priority_queue<T, S, C>& q) {
			return q.* & HackedQueue::c; 
		}
	};
	return HackedQueue::Container(q); 
}

class SortedK {
private:
	
	std::priority_queue<std::pair<unsigned short int, float>, std::vector<std::pair<unsigned short int, float>>, decltype(compare)> queue;
	unsigned int k;

public:
	SortedK(unsigned int k);

	
	void insert(std::pair<unsigned short int, float> x);

	
	const std::vector<std::pair<unsigned short int, float>>& getData();
};
#endif