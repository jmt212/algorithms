/*
CS590A - Group 50 - James Trollo, Zeyu Ni, Shuo Zhang
Trie Article Assignment
mainsearch.cpp

Purpose: Searches text input from standard input for company names/synonyms from a file called companies.dat using a trie
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <functional>
#include <memory>

#include <unordered_map>
#include <vector>
#include <map>
#include <set>

using namespace std;

//Class which represent a node in a Trie. Child nodes are stored as a map where the key is the next word in the company name, and the value is a shared_ptr to the node
class TrieNode {
public:
	// Initialize your data structure here.
	TrieNode() :isWord(false) {   }
	unordered_map<char, TrieNode*> children;
	bool isWord;
};

class Trie {
public:
	Trie() {
		root = new TrieNode();
	}

	// Inserts a word into the trie.
	void insert(string s) {
		if (s.size() <= 0) return;
		TrieNode * node = root;
		for (auto i = 0; i<s.size(); i++) {
			if (node->children.find(s[i]) == node->children.end()) {
				node->children[s[i]] = new TrieNode();
			}
			node = node->children[s[i]];
		}
		node->isWord = true;
	}

	// Returns if the word is in the trie.
	bool search(string key) {
		return retrieve(key, true);
	}

	// Returns if there is any word in the trie
	// that starts with the given prefix.
	bool startsWith(string prefix) {
		return retrieve(prefix, false);
	}

private:

	inline bool retrieve(const string& key, bool isWord) {
		if (key.size() <= 0) return false;
		cout << key << endl;
		TrieNode * node = root;
		for (int i = 0; i<key.length(); i++) {
			if (node->children.find(key[i]) == node->children.end()) {
				return false;
			}
			node = node->children[key[i]];
		}
		return isWord ? node->isWord : true;
	}

	TrieNode* root;
};

//Class the exposes basic functionality of program - reads in comapnies.dat. reads in article input from the standard input. counts the words in the given article
class maintrie {
private:
	set<string> articles = { "a", "an", "the", "and", "or", "but" };

public:
	maintrie() {}
	Trie companyTrie;
	string inputArticle;

	map<string, vector<string>> companyNames;
	map<string, int> companyNameCount;


	//count of words in article entered. does not include: a, an, the, and, or, but
	int articleWordCount = 0;


	//loads a list of company names and their synonoms from companies.dat.
	//each company and its synonyms are listed on their own line, the main name comes first
	//follow by a tab separated list of synonyms
	//the main name is used as the key to a map of company names and their synonyms
	//synonyms are stored as the value of the map in a vector<string>
	void loadTrie() {
		string line;
		ifstream companyFile("companies.dat");
		while (getline(companyFile, line)) {
			stringstream ss(line);
			string token;
			while (getline(ss, token, '\t')) {
				token.erase(remove_if(token.begin(), token.end(), ptr_fun<int, int>(ispunct)), token.end());
				companyTrie.insert(token);
			}
		}
	}

	//loads dictionary of company names and synonyms to keep track of frequency after analyzing the article text using the trie
	void loadDictionary() {
		string line;
		ifstream companyFile("companies.dat");
		while (getline(companyFile, line)) {
			vector<string> companySynonyms;

			int pos = static_cast<int>(line.find('\t'));
			string companyName = line.substr(0, pos);

			string compNameNoPunct = companyName;
			compNameNoPunct.erase(remove_if(compNameNoPunct.begin(), compNameNoPunct.end(), ptr_fun<int, int>(ispunct)), compNameNoPunct.end());
			string::iterator new_end = std::unique(compNameNoPunct.begin(), compNameNoPunct.end(), BothAreSpaces);
			compNameNoPunct.erase(new_end, compNameNoPunct.end());
			companySynonyms.push_back(compNameNoPunct);

			line = line.substr(pos + 1);
			stringstream ss(line);
			string token;

			while (getline(ss, token, '\t')) {
				token.erase(remove_if(token.begin(), token.end(), ptr_fun<int, int>(ispunct)), token.end());
				companySynonyms.push_back(token);
			}
			companyNames[companyName] = companySynonyms;
			companyNameCount[companyName] = 0;
		}
	}

	//read article to be analyzed from standard input. read text until a line that only contains a . is entered
	void getArticleInput() {
		string line;
		while (getline(cin, line)) {
			if (line == ".") {
				cout << "=============" << endl;
				break;
			}
			inputArticle = inputArticle + " " + line;
		}
	}

	//counts the number of words in the entered article, excluding the words: a, an, the, and, or, but. in the count
	void getArticleWordCount() {
		stringstream ss(inputArticle);
		string word;
		while (getline(ss, word, ' ')) {
			if (articles.find(word) == articles.end() && word != " " && word != "") {
				articleWordCount = articleWordCount + 1;
			}
		}
	}

	//uses string functions to search for all occurrances company names and synonyms
	void findCompanyNames() {
		cout << companyTrie.search(inputArticle) << endl;
	}

	static bool BothAreSpaces(char lhs, char rhs) {
		return (lhs == rhs) && (lhs == ' ');
	}

};

//Main point of entry into the application
int main(int argc, const char * argv[]) {
	maintrie search;
	search.loadTrie();
	search.loadDictionary();
	search.getArticleInput();
	search.getArticleWordCount();
	search.findCompanyNames();
}
