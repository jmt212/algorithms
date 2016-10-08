/*
CS590A - Group 50 - James Trollo, Zeyu Ni, Shuo Zhang
Huffman Coding Assignment
*/

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <map>
#include <algorithm>
#include <string>
#include <functional>
using namespace std;

//class to encapsulate functionality of huffman encoding for a file of text that is stored at ./infile.dat and output to ./outfile.dat
class HuffmanCoding {

public:
	struct tree_node {
		int leftchild;
		int rightchild;
		int parent;
	};

	struct symbol_info {
		char symbol;
		int frequency;
		int leaf;
	};

	struct forest_roots {
		int weight;
		int root;
		bool operator<(const forest_roots &a) const {
			return weight < a.weight;
		}
		bool operator>(const forest_roots &a) const {
			return weight > a.weight;
		}
	};

	//Function that reads text from file and generates a map of the frequency of each character in the text
	map<char, int> readFile() {
		ifstream input("infile.dat");
		string line;
		while (getline(input, line)) {
			for (char c : line) {
				if (isalpha(c) || isdigit(c)) {
					if (frequency.find(c) != frequency.end()) {
						frequency[c] = frequency[c] + 1;
					}
					else {
						frequency[c] = 1;
					}
				}
			}
		}
		return frequency;
	}

	//Initializes the FOREST, TREE, and ALPHABET to their initial state after reading in data from file
	void initializeTreeForestAlphabet() {
		tree_node node = { 0, 0, 0 };
		TREES.push_back(node);
		for (auto it = frequency.cbegin(); it != frequency.cend(); it++) {
			tree_node node = { 0, 0, 0 };
			TREES.push_back(node);

			symbol_info symbol = { it->first, it->second, static_cast<int>(TREES.size() - 1) };
			ALPHABET.push_back(symbol);

			forest_roots root = { it->second, symbol.leaf };
			FOREST.push(root);
		}

	}

	//Function to produce a Binary tree that represents the huffman coding for a given set of text
	//This function is from the algorithm defined in CS590 lecture notes on HuffmanCoding
	void Huffman() {
		while (FOREST.size() > 1) {
			forest_roots least = FOREST.top();
			FOREST.pop();
			forest_roots second = FOREST.top();
			FOREST.pop();

			TREES.push_back(tree_node());
			(TREES.back()).leftchild = least.root;
			(TREES.back()).rightchild = second.root;
			(TREES.back()).parent = 0;
			TREES[least.root].parent = static_cast<int>(TREES.size()) - 1;
			TREES[second.root].parent = static_cast<int>(TREES.size()) - 1;

			forest_roots newroot;
			newroot.weight = least.weight + second.weight;
			newroot.root = static_cast<int>(TREES.size()) - 1;
			FOREST.push(newroot);
		}
	}

	//recursivley walks a given huffman tree and appends the appropriate huffman "code" at each node based on weather the previous node was a left or right node
	void walkTree(tree_node& node, int previousNode, string& code) {
		if (node.parent != 0) {
			tree_node parent = TREES[node.parent];
			if (parent.leftchild == previousNode) {
				code += '0';
			}
			else {
				code += '1';
			}
			walkTree(TREES[node.parent], node.parent, code);
		}
	}

	//iterates all letters in the alphabet to get their huffman code by walking the huffman tree from leaf to root
	map<char, string> getCodes() {
		for (symbol_info sym : ALPHABET) {
			string code;
			walkTree(TREES[sym.leaf], sym.leaf, code);
			reverse(code.begin(), code.end());
			codes[sym.symbol] = code;
		}
		return codes;
	}

	//prints each characters frequency to console
	void printFrequency() {
		cout << "Symbol" << "\t" << "Frequency" << endl;
		for (auto it = frequency.cbegin(); it != frequency.cend(); ++it) {
			cout << it->first << "\t\t" << it->second << "\n";
		}
	}

	//prints huffman codes to console
	void printCodes() {
		map<char, string> codes = getCodes();
		for (auto it = codes.cbegin(); it != codes.cend(); ++it) {
			cout << it->first << "\t\t" << it->second << endl;
		}
	}

	//calculates and prints the total number of bits after running text through huffman coding algorithm and prints them to console
	void printNumberOfBits() {
		int numBits = 0;
		for (auto it = frequency.cbegin(); it != frequency.cend(); ++it) {
			numBits += codes[it->first].size() * it->second;
		}
		cout << "Number of bits after compression: " << numBits << endl;
	}

	//prints huffman codes, frequency, and total bits of text after huffman encoding to outfile.dat
	void printDetailsToFile() {
		ofstream outFile;
		outFile.open("outfile.dat");

		outFile << "Symbol" << "\t" << "Frequency" << endl;
		for (auto it = frequency.cbegin(); it != frequency.cend(); ++it) {
			outFile << it->first << "\t\t" << it->second << endl;
		}

		outFile << "===============" << endl;
		outFile << "Symbol" << "\t" << "Code" << endl;
		map<char, string> codes = getCodes();
		for (auto it = codes.cbegin(); it != codes.cend(); ++it) {
			outFile << it->first << "\t\t" << it->second << endl;
		}

		outFile << "===============" << endl;
		int numBits = 0;
		for (auto it = frequency.cbegin(); it != frequency.cend(); ++it) {
			numBits += codes[it->first].size() * it->second;
		}
		outFile << "Number of bits after compression: " << numBits << endl;

		outFile.close();
	}

private:
	vector<tree_node> TREES;
	vector<symbol_info> ALPHABET;
	priority_queue<forest_roots, vector<forest_roots>, std::greater<forest_roots> > FOREST;
	map<char, int> frequency;
	map<char, string> codes;

};

//Main method - entry point into program
int main(int argc, const char * argv[]) {
	HuffmanCoding coding;
	cout << "Applying huffman coding to ./infile.dat ....." << endl;
	cout << "Outputing huffman coding results to ./outfile.dat" << endl;
	coding.readFile();
	coding.initializeTreeForestAlphabet();
	coding.Huffman();
	coding.printDetailsToFile();
}
