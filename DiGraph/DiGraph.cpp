/* James Trollo
CS590A - Final Project
*/

#include <iostream>
#include <iomanip>
#include <istream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <limits>
#include <functional>

using namespace std;

//represents a node of text in the asciiGraph
class AsciiNode {
public:
	AsciiNode(string symType, string val, int x, int y) : value(val), type(symType), xPos(x), yPos(y) {}
	AsciiNode() {}

	string type, value;
	int xPos, yPos;

	bool bottomFill = false, rightFill = false, leftFill = false, topFill = false;
};

//represents a "target" node and the associated cost of traveling to it
class Node {
public:
	Node(int id, int cst) : nodeId(id), cost(cst) {}
	int nodeId;
	int cost;
};

//class encapsulating functionality of a graph using an adjacency list
class DiGraph {

public:
	DiGraph() {}

	map<int, vector<Node>> adjacencyList;

	//below variables store info for build ascii art graph
	vector<vector<string>> asciiGraph;
	map<int, AsciiNode> nodeLocations;

	//Function that loads file from ./graph.dat and initializes an adjacency list to represent a graph
	//NOTE: Most of this implementation is taken from the CS590A Notes on Canvas - provided by Prof. Pfeffer
	void loadGraphFile() {
		ifstream input;
		input.open("graph.dat");
		string currentLine;

		while (getline(input, currentLine)) {
			std::stringstream ss;
			ss.str(currentLine);

			vector<string> graphTokens{ std::istream_iterator<string>{ss}, istream_iterator<string>{} };

			//First 2 nodes in line are a special case handle here
			int startNode = stoi(graphTokens[0]);
			int targetNode = stoi(graphTokens[1]);
			int cost = stoi(graphTokens[2]);

			Node n(targetNode, cost);
			vector<Node> edges;
			edges.push_back(n);
			adjacencyList[startNode] = edges;
			if (adjacencyList.find(targetNode) == adjacencyList.end()) {
				vector<Node> empty;
				adjacencyList[targetNode] = empty;
			}

			for (auto it = graphTokens.begin() + 3; it < graphTokens.end() - 1; it++) {
				int targetNode = stoi(*it);
				int cost = stoi((*(++it)));

				if (adjacencyList.find(targetNode) == adjacencyList.end()) {
					vector<Node> empty;
					adjacencyList[targetNode] = empty;
				}

				Node n(targetNode, cost);
				adjacencyList[startNode].push_back(n);
			}
		}
	}

	//builds and prints an ascii representation of a directed graph
	void printAsciiRepresentation() {
		insertNodes();
		buildGraph();
		printAsciiGraph();
	}

	//prints the constructed ascii art graph to the console, setting the output width to that of the largest string stored in graph
	void printAsciiGraph() {
		int maxWidth = getMaxNodeWidth();
		for (auto row : asciiGraph) {
			//if (!(std::adjacent_find(row.begin(), row.end(), std::not_equal_to<string>()) == row.end()))
			//{
				for (auto node : row) {
					cout << setw(maxWidth);
					cout << node;
				}
				cout << endl;
			//}
		}
	}

	//returns the width of the largest string stored in the ascii art graph
	int getMaxNodeWidth() {
		int maxWidth = 0;
		for (auto row : asciiGraph) {
			for (auto node : row) {
				int stringSize = static_cast<int>(node.size());
				if (stringSize > maxWidth) {
					maxWidth = stringSize;
				}
			}
		}
		return maxWidth;
	}

	//applies dijkstras algorithm to the graph stored in this object
	void doDijkstra() {
		const char separator = ' ';
		for (auto startNode : adjacencyList) {
			if (!startNode.second.empty()) {
				dijkstra(startNode.first);
			}
			else {
				cout << "------------------------------------------------------------------------------------------------" << endl;
				cout << "Source To Target \t\t\t Cost \t\t\t Path" << endl;
				cout << "------------------------------------------------------------------------------------------------" << endl;
				for (auto d : adjacencyList) {
					if (d.first == startNode.first) {
						cout << setw(21) << left << setfill(separator) << to_string(startNode.first) + "-->" + to_string(d.first) << "\t\t\t"
							<< setw(10) << left << setfill(separator) << "0" << "\t\t"
							<< setw(10) << left << setfill(separator) << startNode.first << endl;
					}
					else {
						cout << setw(21) << left << setfill(separator) << to_string(startNode.first) + "-->" + to_string(d.first) << "\t\t\t"
							<< setw(10) << left << setfill(separator) << "INFINITY" << "\t\t"
							<< setw(10) << left << setfill(separator) << "UNREACHABLE" << endl;
					}
				}
			}
		}
	}

private:
	//initalizes the ascii art graph by inserting nodes and empty vectors
	void insertNodes() {
		vector<string> nodeRow;
		for (auto node : adjacencyList) {
			for (int i = 0; i < 7; i++) {
				nodeRow.push_back("");
			}
			nodeRow.push_back("[" + to_string(node.first) + "]");
			AsciiNode n("NODE", to_string(node.first), (static_cast<int>(nodeRow.size()) - 1), 3);
			nodeLocations[node.first] = n;
		}
		for (int i = 0; i < 7; i++) {
			nodeRow.push_back("");
		}
		for (int i = 0; i < static_cast<int>(adjacencyList.size()) * 10; i++) {
			if (i == 3) {
				asciiGraph.push_back(nodeRow);
			}
			vector<string> row(nodeRow.size() + 10, "");
			asciiGraph.push_back(row);
		}
	}

	//returns the longest string length of either node "name" or a cost
	int maxString() {
		int maxWidth = 0;
		for (auto node : adjacencyList) {
			int nodeLength = static_cast<int>(to_string(node.first).size()) + 2;
			for (auto neighbor : node.second) {
				int costLength = static_cast<int>(to_string(neighbor.cost).size());
				if (costLength > maxWidth) {
					maxWidth = costLength;
				}
			}
			if (nodeLength > maxWidth) {
				maxWidth = nodeLength;
			}
		}
		return maxWidth;
	}

	//generates the appropriate lengthed horizontal string based on the size of the largest node "name" or cost
	string generateHorizontalDashString() {
		int maxHorizontalDash = maxString();
		string horizontalLine = "";
		for (int i = 0; i < maxHorizontalDash + 1; i++) {
			horizontalLine = horizontalLine + "-";
		}
		return horizontalLine;
	}

	//builds ascii art graph
	//the algorithm used is based on a comment from the CS590 forum provided by Prof. Pfeffer
	void buildGraph() {
		int downCount = 5;
		string horizontalLine = generateHorizontalDashString();

		for (auto start : adjacencyList) {
			AsciiNode& startNode = nodeLocations[start.first];
			if (!start.second.empty()) {
				for (auto target : start.second) {
					AsciiNode& targetNode = nodeLocations[target.nodeId];
					if (!startNode.bottomFill) {
						startNode.bottomFill = true;
						goDown(downCount, startNode, targetNode, target.cost, horizontalLine);
					}
					else if (!startNode.rightFill) {
						startNode.rightFill = true;
						goRight(downCount, startNode, targetNode, target.cost, horizontalLine);
					}
					else if (!startNode.leftFill) {
						startNode.leftFill = true;
						goLeft(downCount, startNode, targetNode, target.cost, horizontalLine);
					}
					else if (!startNode.topFill) {
						startNode.topFill = true;
						goUp(downCount, startNode, targetNode, target.cost, horizontalLine);
					}
					downCount = downCount + 2;
				}
			}
		}
	}

	string generateHorizontalDashPrefix(string horizontalLine, int numberToSubtract) {
		string prefix = "";
		for (int i = 0; i < static_cast<int>(horizontalLine.length()) - numberToSubtract; i++) {
			prefix = prefix + "-";
		}
		return prefix;
	}

	//moves down to the specified level so that a connection can be made to the target node
	int goDown(int downCount, AsciiNode& startNode, AsciiNode& targetNode, int cost, string horizontalLine) {
		for (int i = startNode.yPos + 1; i < startNode.yPos + downCount; i++) {
			if (asciiGraph[i][startNode.xPos] == horizontalLine) {
				string equalPrefix = "";
				if (startNode.type == "NODE") {
					equalPrefix = generateHorizontalDashPrefix(horizontalLine, 2) + "=-";
				}
				else {
					equalPrefix = generateHorizontalDashPrefix(horizontalLine, 1) + "=";
				}
				asciiGraph[i][startNode.xPos] = equalPrefix;
			}
			else {
				if (startNode.type == "NODE") {
					asciiGraph[i][startNode.xPos] = "| ";
				}
				else {
					asciiGraph[i][startNode.xPos] = "|";
				}
			}
		}
		if (targetNode.xPos < startNode.xPos) {
			if (startNode.type == "NODE") {
				asciiGraph[startNode.yPos + downCount][startNode.xPos] = generateHorizontalDashPrefix(horizontalLine, 2) + "+ ";
			}
			else {
				asciiGraph[startNode.yPos + downCount][startNode.xPos] = generateHorizontalDashPrefix(horizontalLine, 1) + "+";
			}
		}
		else {
			if (startNode.type == "NODE") {
				asciiGraph[startNode.yPos + downCount][startNode.xPos] = "+-";
			}
			else {
				asciiGraph[startNode.yPos + downCount][startNode.xPos] = "+";
			}
		}

		connectToTargetNode(downCount, startNode, targetNode, cost, horizontalLine);

		return downCount;
	}

	//calls appropriate function for drawing a line from the start node to the target node based on what open sides are on the target node
	void connectToTargetNode(int downCount, AsciiNode& startNode, AsciiNode& targetNode, int cost, string horizontalLine) {
		int xPos = startNode.xPos;
		int yPos = startNode.yPos + downCount;

		if (xPos < targetNode.xPos) {
			if (!targetNode.bottomFill) {
				bottomRightConnect(xPos, yPos, targetNode, cost, horizontalLine);
				targetNode.bottomFill = true;
			}
			else if (!targetNode.rightFill) {
				rightRightConnect(xPos, yPos, targetNode, cost, horizontalLine);
				targetNode.rightFill = true;
			}
			else if (!targetNode.leftFill) {
				leftRightConnect(xPos, yPos, targetNode, cost, horizontalLine);
				targetNode.leftFill = true;
			}
			else if (!targetNode.topFill) {
				topRightConnect(xPos, yPos, targetNode, cost, horizontalLine);
				targetNode.topFill = true;
			}
		}
		else {
			if (!targetNode.bottomFill) {
				bottomLeftConnect(xPos, yPos, targetNode, cost, horizontalLine);
				targetNode.bottomFill = true;
			}
			else if (!targetNode.rightFill) {
				rightLeftConnect(xPos, yPos, targetNode, cost, horizontalLine);
				targetNode.rightFill = true;
			}
			else if (!targetNode.leftFill) {
				leftLeftConnect(xPos, yPos, targetNode, cost, horizontalLine);
				targetNode.leftFill = true;
			}
			else if (!targetNode.topFill) {
				topLeftConnect(xPos, yPos, targetNode, cost, horizontalLine);
				targetNode.topFill = true;
			}
		}
	}

	//connects a start node that is to the left of its target to the targets "top"
	void topRightConnect(int xPos, int yPos, AsciiNode& targetNode, int cost, string horizontalLine) {
		while (xPos < targetNode.xPos + 1) {
			xPos = xPos + 1;
			if (asciiGraph[yPos][xPos] != "+") {
				asciiGraph[yPos][xPos] = horizontalLine;
			}
		}
		xPos = xPos + 1;
		asciiGraph[yPos][xPos] = generateHorizontalDashPrefix(horizontalLine, 1) + "+";

		while (yPos > targetNode.yPos - 3) {
			yPos = yPos - 1;
			if (asciiGraph[yPos][xPos] == horizontalLine) {
				asciiGraph[yPos][xPos] = generateHorizontalDashPrefix(horizontalLine, 1) + "=";
			}
			else {
				asciiGraph[yPos][xPos] = "|";
			}
		}
		asciiGraph[yPos + 2][xPos] = to_string(cost);
		asciiGraph[yPos][xPos] = generateHorizontalDashPrefix(horizontalLine, 1) + "+";

		while (xPos > targetNode.xPos) {
			xPos = xPos - 1;
			asciiGraph[yPos][xPos] = horizontalLine;
		}
		asciiGraph[yPos][xPos] = "+-";
		yPos = yPos + 1;
		asciiGraph[yPos][xPos] = "| ";
		yPos = yPos + 1;
		asciiGraph[yPos][xPos] = "V ";
	}

	//connects a start node that is to the left of its target to the targets "right"
	void leftRightConnect(int xPos, int yPos, AsciiNode& targetNode, int cost, string horizontalLine) {
		while (xPos < targetNode.xPos + 1) {
			xPos = xPos + 1;
			if (asciiGraph[yPos][xPos] != "+") {
				asciiGraph[yPos][xPos] = horizontalLine;
			}
		}
		xPos = xPos + 1;
		asciiGraph[yPos][xPos] = horizontalLine;
		xPos = xPos + 1;

		asciiGraph[yPos][xPos] = generateHorizontalDashPrefix(horizontalLine, 1) + "+";

		bool setCost = true;
		while (yPos > targetNode.yPos) {
			yPos = yPos - 1;
			asciiGraph[yPos][xPos] = "|";
			if (setCost) {
				yPos = yPos - 1;
				asciiGraph[yPos][xPos] = to_string(cost);
				setCost = false;
			}
		}
		asciiGraph[yPos][xPos] = generateHorizontalDashPrefix(horizontalLine, 1) + "+";;

		xPos = xPos - 1;
		asciiGraph[yPos][xPos] = horizontalLine;
		xPos = xPos - 1;

		string arrowPrefix = "";
		asciiGraph[yPos][xPos] = "<" + generateHorizontalDashPrefix(horizontalLine, 1);

	}

	//connects a start node that is to the right of its target to the targets "bottom"
	void bottomLeftConnect(int xPos, int yPos, AsciiNode& targetNode, int cost, string horizontalLine) {
		while (xPos > targetNode.xPos) {
			xPos = xPos - 1;
			if (asciiGraph[yPos][xPos] != "+") {
				asciiGraph[yPos][xPos] = horizontalLine;
			}
		}
		asciiGraph[yPos][xPos] = "+-";
		while (yPos > targetNode.yPos + 1) {
			yPos = yPos - 1;
			if (asciiGraph[yPos][xPos] == horizontalLine) {
				asciiGraph[yPos][xPos] = generateHorizontalDashPrefix(horizontalLine, 2) + "=-";
			}
			else {
				asciiGraph[yPos][xPos] = "| ";
			}
		}
		asciiGraph[yPos + 2][xPos] = to_string(cost) + " ";
		asciiGraph[yPos][xPos] = "^ ";
	}

	//connects a start node that is to the right of its target to the targets "left"
	void leftLeftConnect(int xPos, int yPos, AsciiNode& targetNode, int cost, string horizontalLine) {
		while (xPos > targetNode.xPos - 3) {
			xPos = xPos - 1;
			if (asciiGraph[yPos][xPos] != "+") {
				asciiGraph[yPos][xPos] = horizontalLine;
			}
		}
		asciiGraph[yPos][xPos] = "+";

		while (yPos > targetNode.yPos) {
			yPos = yPos - 1;
			if (asciiGraph[yPos][xPos] == horizontalLine) {
				asciiGraph[yPos][xPos] = generateHorizontalDashPrefix(horizontalLine, 1) + "=";
			}
			else {
				asciiGraph[yPos][xPos] = "|";
			}
		}
		asciiGraph[yPos][xPos] = "+";
		asciiGraph[yPos + 2][xPos] = to_string(cost);
		xPos = xPos + 1;
		asciiGraph[yPos][xPos] = horizontalLine;
		xPos = xPos + 1;

		asciiGraph[yPos][xPos] = generateHorizontalDashPrefix(horizontalLine, 0);

		string arrowPrefix = "";
		for (int i = 0; i < static_cast<int>((horizontalLine.length() - asciiGraph[yPos][xPos + 1].length()) - 1); i++) {
			arrowPrefix = arrowPrefix + "-";
		}
		arrowPrefix = arrowPrefix + ">";
		asciiGraph[yPos][xPos + 1] = arrowPrefix + asciiGraph[yPos][xPos + 1];
	}

	//connects a start node that is to the right of its target to the targets "top"
	void topLeftConnect(int xPos, int yPos, AsciiNode& targetNode, int cost, string horizontalLine) {
		while (xPos > targetNode.xPos + 5) {
			xPos = xPos - 1;
			if (asciiGraph[yPos][xPos] != "+") {
				asciiGraph[yPos][xPos] = horizontalLine;
			}
		}
		xPos = xPos - 1;
		asciiGraph[yPos][xPos] = "+";

		while (yPos > targetNode.yPos - 3) {
			yPos = yPos - 1;
			if (asciiGraph[yPos][xPos] == horizontalLine) {
				asciiGraph[yPos][xPos] = generateHorizontalDashPrefix(horizontalLine, 1) + "=";
			}
			else {
				asciiGraph[yPos][xPos] = "|";
			}
		}
		asciiGraph[yPos + 2][xPos] = to_string(cost);
		asciiGraph[yPos][xPos] = generateHorizontalDashPrefix(horizontalLine, 1) + "+";

		while (xPos > targetNode.xPos) {
			xPos = xPos - 1;
			asciiGraph[yPos][xPos] = horizontalLine;
		}
		asciiGraph[yPos][xPos] = "+-";
		yPos = yPos + 1;
		asciiGraph[yPos][xPos] = "| ";
		yPos = yPos + 1;
		asciiGraph[yPos][xPos] = "V ";
	}

	//connects a start node that is to the right of its target to the targets "right"
	void rightLeftConnect(int xPos, int yPos, AsciiNode& targetNode, int cost, string horizontalLine) {
		while (xPos > targetNode.xPos + 4) {
			xPos = xPos - 1;
			if (asciiGraph[yPos][xPos] != "+") {
				asciiGraph[yPos][xPos] = horizontalLine;
			}
		}
		xPos = xPos - 1;
		asciiGraph[yPos][xPos] = "+";

		while (yPos > targetNode.yPos) {
			yPos = yPos - 1;
			if (asciiGraph[yPos][xPos] == horizontalLine) {
				asciiGraph[yPos][xPos] = generateHorizontalDashPrefix(horizontalLine, 1) + "=";
			}
			else {
				asciiGraph[yPos][xPos] = "|";
			}
		}
		asciiGraph[yPos + 2][xPos] = to_string(cost);

		asciiGraph[yPos][xPos] = generateHorizontalDashPrefix(horizontalLine, 1) + "+";;

		xPos = xPos - 1;
		asciiGraph[yPos][xPos] = horizontalLine;
		xPos = xPos - 1;

		asciiGraph[yPos][xPos] = "<" + generateHorizontalDashPrefix(horizontalLine, 1);
	}

	//connects a start node that is to the left of its target to the targets "bottom"
	void bottomRightConnect(int xPos, int yPos, AsciiNode& targetNode, int cost, string horizontalLine) {
		while (xPos < targetNode.xPos - 2) {
			xPos = xPos + 1;
			if (asciiGraph[yPos][xPos] != "+") {
				asciiGraph[yPos][xPos] = horizontalLine;
			}
		}
		xPos = xPos + 1;
		asciiGraph[yPos][xPos] = horizontalLine;
		xPos = xPos + 1;
		asciiGraph[yPos][xPos] = generateHorizontalDashPrefix(horizontalLine, 2) + "+ ";
		while (yPos > targetNode.yPos + 1) {
			yPos = yPos - 1;
			if (yPos - 1 == targetNode.yPos) {
				asciiGraph[yPos][xPos] = "^ ";
				break;
			}
			if (yPos - 3 == targetNode.yPos) {
				asciiGraph[yPos][xPos] = to_string(cost) + " ";
				continue;
			}
			if (asciiGraph[yPos][xPos] == horizontalLine) {
				asciiGraph[yPos][xPos] = generateHorizontalDashPrefix(horizontalLine, 2) + "=-";
			}
			else {
				asciiGraph[yPos][xPos] = "| ";
			}
		}
	}

	//connects a start node that is to the left of its target to the targets "right"
	void rightRightConnect(int xPos, int yPos, AsciiNode& targetNode, int cost, string horizontalLine) {
		while (xPos < targetNode.xPos + 3) {
			xPos = xPos + 1;
			if (asciiGraph[yPos][xPos] != "+") {
				asciiGraph[yPos][xPos] = horizontalLine;
			}
		}
		asciiGraph[yPos][xPos] = generateHorizontalDashPrefix(horizontalLine, 1) + "+";;
		while (yPos > targetNode.yPos) {
			yPos = yPos - 1;
			if (asciiGraph[yPos][xPos] == horizontalLine) {
				asciiGraph[yPos][xPos] = generateHorizontalDashPrefix(horizontalLine, 1) + "=";
			}
			else {
				asciiGraph[yPos][xPos] = "|";
			}
		}
		asciiGraph[yPos + 2][xPos] = to_string(cost);

		asciiGraph[yPos][xPos] = generateHorizontalDashPrefix(horizontalLine, 1) + "+";

		xPos = xPos - 1;
		asciiGraph[yPos][xPos] = horizontalLine;
		xPos = xPos - 1;

		asciiGraph[yPos][xPos] = "<" + generateHorizontalDashPrefix(horizontalLine, 1);
	}

	//generates a right line out of the start node, that then turn down
	int goRight(int downCount, AsciiNode& startNode, AsciiNode& targetNode, int cost, string horizontalLine) {

		asciiGraph[startNode.yPos][startNode.xPos + 1] = horizontalLine;
		asciiGraph[startNode.yPos][startNode.xPos + 2] = generateHorizontalDashPrefix(horizontalLine, 1) + "+";

		AsciiNode n("PLUS", "+", startNode.xPos + 2, startNode.yPos);
		goDown(downCount, n, targetNode, cost, horizontalLine);

		return downCount;
	}

	//generates a left line out of the start node that then turns down
	int goLeft(int downCount, AsciiNode& startNode, AsciiNode& targetNode, int cost, string horizontalLine) {
		string prefixLine = "";
		asciiGraph[startNode.yPos][startNode.xPos] = generateHorizontalDashPrefix(horizontalLine, static_cast<int>(asciiGraph[startNode.yPos][startNode.xPos].length())) + asciiGraph[startNode.yPos][startNode.xPos];
		asciiGraph[startNode.yPos][startNode.xPos - 1] = horizontalLine;
		asciiGraph[startNode.yPos][startNode.xPos - 2] = "+";

		AsciiNode n("PLUS", "+", startNode.xPos - 2, startNode.yPos);
		goDown(downCount, n, targetNode, cost, horizontalLine);

		return downCount;
	}

	//generates a line out of the top of the start node that then turns down
	int goUp(int downCount, AsciiNode& startNode, AsciiNode& targetNode, int cost, string horizontalLine) {
		for (int i = startNode.yPos - 1; i >= 0; i--) {
			asciiGraph[i][startNode.xPos] = "| ";
		}

		asciiGraph[startNode.yPos - 3][startNode.xPos] = "+-";
		asciiGraph[startNode.yPos - 3][startNode.xPos + 1] = horizontalLine;
		asciiGraph[startNode.yPos - 3][startNode.xPos + 2] = horizontalLine;
		asciiGraph[startNode.yPos - 3][startNode.xPos + 3] = horizontalLine;

		asciiGraph[startNode.yPos - 3][startNode.xPos + 4] = generateHorizontalDashPrefix(horizontalLine, 1) + "+";;

		asciiGraph[startNode.yPos - 2][startNode.xPos + 4] = "|";
		asciiGraph[startNode.yPos - 1][startNode.xPos + 4] = "|";

		AsciiNode n("VERTICAL", "|", startNode.xPos + 4, startNode.yPos - 1);
		goDown(downCount + 1, n, targetNode, cost, horizontalLine);

		return downCount;
	}

	//extracts the minimum cost from a map
	int mapMin(map<int, int> dist, set<int> Q) {
		int min = numeric_limits<int>::max();
		int minKey = -1;
		for (int q : Q) {
			int d = dist[q];
			if (d < min) {
				min = d;
				minKey = q;
			}
		}
		return minKey;
	}

	//Applies Dijkstra's algorithm to the given graph - and prints the cost and shortest path from each start node to each target node
	//algorithm from https://en.wikipedia.org/wiki/Dijkstra's_algorithm#Pseudocode
	void dijkstra(int startNode) {
		set<int> Q;
		map<int, int> dist;
		map<int, int> prev;

		for (auto v : adjacencyList) {
			dist[v.first] = numeric_limits<int>::max();
			prev[v.first] = -1;
			Q.insert(v.first);
		}
		dist[startNode] = 0;

		while (!Q.empty()) {
			int u = mapMin(dist, Q);
			Q.erase(u);
			if (u == -1) {
				break;
			}
			for (auto v : adjacencyList[u]) {
				int alt = dist[u] + v.cost;
				if (alt < dist[v.nodeId]) {
					dist[v.nodeId] = alt;
					prev[v.nodeId] = u;
				}
			}
		}
		const char separator = ' ';
		cout << "------------------------------------------------------------------------------------------------" << endl;
		cout << "Source To Target \t\t\t Cost \t\t\t Path" << endl;
		cout << "------------------------------------------------------------------------------------------------" << endl;
		for (auto d : dist) {
			if (d.second != numeric_limits<int>::max()) {
				cout << setw(21) << left << setfill(separator) << to_string(startNode) + "-->" + to_string(d.first) << "\t\t\t"
					<< setw(10) << left << setfill(separator) << d.second << "\t\t"
					<< setw(10) << left << setfill(separator) << printPath(startNode, d.first, prev) << endl;
			}
			else {
				cout << setw(21) << left << setfill(separator) << to_string(startNode) + "-->" + to_string(d.first) << "\t\t\t"
					<< setw(10) << left << setfill(separator) << "INFINITY" << "\t\t"
					<< setw(10) << left << setfill(separator) << "UNREACHABLE" << endl;
			}
		}
	}

	//prints the shortest path from a given start node, to a given target node
	//algorithm from: https://en.wikipedia.org/wiki/Dijkstra's_algorithm#Pseudocode
	string printPath(int startNode, int target, map<int, int> prev) {
		vector<int> S;
		int u = target;
		string path = "";
		while (prev.find(u) != prev.end()) {
			S.push_back(u);
			u = prev[u];
		}
		reverse(S.begin(), S.end());
		for (auto s : S) {
			path = path + to_string(s);
			if ((find(S.begin(), S.end(), s) + 1) != S.end()) {
				path = path + "-->";
			}
		}
		return path;
	}

};

//main entry point for the application
int main(int argc, const char * argv[]) {
	DiGraph diGraph;

	diGraph.loadGraphFile();
	diGraph.printAsciiRepresentation();
	diGraph.doDijkstra();

	return 0;
}