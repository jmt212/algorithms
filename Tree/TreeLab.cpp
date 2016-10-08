//Group 50
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class node
{
public:
	node(string name, int num) :name(name), num(num){}
	void getInput();
	void traverse_size(node root);
	int size(node root);
	void employeeNoSubordinates(node);
	void traverseTree(node n);
	string name;
	vector<node> children;
private:
	int num;
};

void node::getInput()
{
	for (int i = 0; i < num; i++)
	{
		std::cout << "Please enter name and number of subordinates " << name << "'s " << i + 1 << " subordinates: ";
		string name2;
		cin >> name2;
		int num2;
		cin >> num2;
		node n(name2, num2);
		children.push_back(n);
	}
	for (int i = 0; i < num; i++)
		children.at(i).getInput();
}

void node::traverse_size(node root) {
	if (size(root) - 1 > 3) {
		cout << "Employee " << root.name << " has more than 3 employees" << endl;
	}
	if (root.children.size() == 0) {
		return;
	}
	for (node child : root.children) {
		traverse_size(child);
	}
}

int node::size(node root) {
	int count = 0;

	++count;
	if (root.children.size() != 0)
		for (node ch : root.children)
			count = count + size(ch);
	return (count);
}

void node::employeeNoSubordinates(node root) {
	if (root.children.size() == 0) {
		cout << "lone employee hierarch for employee: " << root.name << endl;
		return;
	}
	else {
		for (node child : root.children) {
			employeeNoSubordinates(child);
			cout << root.name << endl;
		}
	}
}

void node::traverseTree(node rootNode)
{
	cout << rootNode.name << endl;
	if (rootNode.children.size() != 0)
		for (node ch : rootNode.children) {
			traverseTree(ch);
		}
}


int main()
{
	cout << "Please enter employees name: ";
	string s;
	cin >> s;
	cout << endl;

	cout << "Please enter the number of subordinates: ";
	int num; 
	cin >> num;
	cout << endl;

	cout << endl;
	node a(s, num);
	a.getInput();
	a.traverse_size(a);
	a.employeeNoSubordinates(a);
	return 1;
}