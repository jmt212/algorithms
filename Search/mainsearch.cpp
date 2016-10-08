/*
CS590A - Group 50 - James Trollo, Zeyu Ni, Shuo Zhang
Trie Article Assignment
mainsearch.cpp

Purpose: Searches text input from standard input for company names/synonyms from a file called companies.dat
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <functional>
#include <map>
#include <vector>
#include <set>
#include <regex>

using namespace std;

class mainsearch {
private:
	set<string> articles = { "a", "an", "the", "and", "or", "but" };

public:
	map<string, vector<string>> companyNames;
	map<string, int> companyNameCount;
	string inputArticle;

	//count of words in article entered. does not include: a, an, the, and, or, but
	int articleWordCount = 0;

	//loads a list of company names and their synonoms from companies.dat.
	//each company and its synonyms are listed on their own line, the main name comes first
	//follow by a tab separated list of synonyms
	//the main name is used as the key to a map of company names and their synonyms
	//synonyms are stored as the value of the map in a vector<string>
	void loadDictionary() {
		string line;
		ifstream companyFile("companies.dat");
		while (getline(companyFile, line)) {
			vector<string> companySynonyms;

			int pos = static_cast<int>(line.find('\t'));
			string companyName = line.substr(0, pos);

			string compSynonym = companyName;
			compSynonym.erase(remove_if(compSynonym.begin(), compSynonym.end(), ptr_fun<int, int>(ispunct)), compSynonym.end());
			string::iterator new_end = std::unique(compSynonym.begin(), compSynonym.end(), BothAreSpaces);
			compSynonym.erase(new_end, compSynonym.end());
			companySynonyms.push_back(compSynonym);

			line = line.substr(pos + 1);
			stringstream ss(line);
			string token;

			while (getline(ss, token, '\t')) {
				token.erase(remove_if(token.begin(), token.end(), ptr_fun<int, int>(ispunct)), token.end());
				string::iterator new_end = std::unique(token.begin(), token.end(), BothAreSpaces);
				token.erase(new_end, token.end());
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

	//uses string functions to search for all occurrances of company names and synonyms
	void findCompanyNames() {
		map<string, int> synonymFreq = {};
		inputArticle.erase(remove_if(inputArticle.begin(), inputArticle.end(), ptr_fun<int, int>(ispunct)), inputArticle.end());
		string::iterator new_end = std::unique(inputArticle.begin(), inputArticle.end(), BothAreSpaces);
		inputArticle.erase(new_end, inputArticle.end());

		for (auto it = companyNames.cbegin(); it != companyNames.cend(); ++it) {
			string compName = it->first;
			companyNameCount[compName] = 0;
			for (auto i = it->second.cbegin(); i != it->second.cend(); ++i) {
				regex reg("\\b" + (*i) + "\\b");
				smatch m;
				string tempArticle = inputArticle;
				while (regex_search(tempArticle, m, reg)) {
					companyNameCount[compName] += 1;
					synonymFreq[(*i)] += 1;
					tempArticle = m.suffix().str();

				}
			}
			//to remove duplicate matches
			if (companyNameCount[compName] != 0) {
				for (auto el : synonymFreq) {
					for (auto el2 : synonymFreq) {
						if (el != el2) {
							regex reg("\\b" + (el.first) + "\\b");
							smatch m;
							string tempName = el2.first;
							int count = 0;
							while (regex_search(tempName, m, reg)) {
								count += 1;
								tempName = m.suffix().str();

							}
							companyNameCount[compName] -= count;
							count = 0;
						}
					}
				}
			}
		}
	}

	//prints results from search for company names in article text
	void printResults() {
		for (auto it = companyNameCount.cbegin(); it != companyNameCount.cend(); ++it) {
			cout << it->first << " " << it->second << "/" << articleWordCount << endl;
		}
	}

	int maxWords(std::vector<std::string> lines) {
		int max = 0;
		for (auto it = lines.cbegin(); it != lines.cend(); it++) {
			int count = 0;
			string name = *it;
			stringstream ss(name);
			string token;
			while (getline(ss, token, ' ')) {
				count++;
				if (count > max) {
					max = count;
				}
			}
		}
		return max;
	}

	static bool BothAreSpaces(char lhs, char rhs) {
		return (lhs == rhs) && (lhs == ' ');
	}
};

//Main point of entry for the application
int main(int argc, const char * argv[]) {
	mainsearch search;
	search.loadDictionary();
	search.getArticleInput();
	search.getArticleWordCount();
	search.findCompanyNames();
	search.printResults();
}
