#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

int main() {
	int numToRead;
	std::cout << "Please enter how many numbers you would like to enter: ";
	std::cin >> numToRead;
	cout << "Please enter your numbers followed by enter:" << endl;

	vector<int>vec(numToRead + 1);

	auto readLambda = []()->int {
		int entry;
		cin >> entry;
		return entry;
	};

	vector<int> countVec(numToRead + 1);
	int i = 0;
	auto countLambda = [&vec, &i]()->int {
		int currentElm = vec[i];
		i++;
		return count(vec.begin(), vec.end(), currentElm);
	};

	generate_n(vec.begin(), numToRead, readLambda);

	generate_n(countVec.begin(), numToRead, countLambda);

	auto maxFn = [&countVec](int i, int j) {
		return i < j;
	};

	int max = *(max_element(countVec.begin(), countVec.end(), maxFn));
	vector<int> indexes(numToRead + 1);

	int n = 0;
	auto getIndex = [&countVec, &max, &n]()->int {
		int currentElm = countVec[n];
		n++;
		if (currentElm == max) {
			return n - 1;
		}
		else {
			return -1;
		}
	};
	
	generate_n(indexes.begin(), numToRead, getIndex);

	int z = 0;
	set<int> modes;
	auto printIndex = [&vec, &indexes, &modes, &z]()->int {
		int currIndex = indexes[z];
		z++;
		if (currIndex != -1) {
			if (modes.find(vec[currIndex]) == modes.end()) {
				cout << vec[currIndex] << endl;
				modes.insert(vec[currIndex]);
			}
		}
		return 0;
	};

	cout << "The mode(s) are: " << endl;
	generate_n(indexes.begin(), numToRead, printIndex);
}