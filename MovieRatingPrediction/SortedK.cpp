#include"SortedK.h"

SortedK::SortedK(unsigned int k) : queue(compare) {
	this->k = k;
}

//insert the data into priority queue in sorted order upto k
void SortedK::insert(std::pair<unsigned short int, float> x) {
	if (queue.size() < k) //if queue size is smaller then k
	{
		queue.push(x); //push the data
		return;
	}

	if (x.second > queue.top().second) //if the top value is less then the current data
	{
		queue.pop(); //get rid of the root
		queue.push(x); //priority queue will automatically restructure
	}
}

//return the vector from the priority queu
const std::vector<std::pair<unsigned short int, float>>& SortedK::getData() {
	return Container(queue);
}