/*
Group 50 James Trollo, Zeyu Ni, Shuo Zhang
*/

#include <iostream>
#include <stack>
#include <queue>
#include <string>
using namespace std;

class RPNCalc {
	private:

		queue<string> toPostfix(string infix) {
			queue<char> infixQ;
			queue<string> postQ;
			stack<char> opStack;
			char t;
			infixQ = makeInfixQ(infix);
			while (!infixQ.empty()) {
				t = infixQ.front();
				infixQ.pop();
				if (iswspace(t)) {
					continue;
				} else if (isdigit(t)) {
					string digit = "";
					digit.append(string(1, t));
					while (!infixQ.empty() && isdigit(infixQ.front())) {
						t = infixQ.front();
						digit.append(string(1, t));
						if (isdigit(t)) {
							infixQ.pop();
						}
					}
					postQ.push(digit);
				}
				else if (t == '(') {
					opStack.push(t);
				}
				else if (t == ')') {
					while (opStack.top() != '(') {
						postQ.push(string(1, opStack.top()));
						opStack.pop();
					}
					opStack.pop();	
				}
				else {
					while (!opStack.empty() && opStack.top() != '(' && (precedenceLessThan(t, opStack.top()))) {
						postQ.push(string(1, opStack.top()));
						opStack.pop();
					}
					opStack.push(t);
				}
			}
			while (!opStack.empty()) {
				postQ.push(string(1, opStack.top()));
				opStack.pop();
			}
			return postQ;
		}

		bool precedenceLessThan(char t, char next) {
			if (t == '+' || t == '-') {
				return true;
			} 
			return false;
		}
		
		queue<char> makeInfixQ(string infix) {
			queue<char> infixQ;
			for (char c : infix) {
				infixQ.push(c);
			}
			return infixQ;
		}
		
		//from stack overflow https://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int
		bool isInteger(const std::string & s)
		{
			if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

			char * p;
			strtol(s.c_str(), &p, 10);

			return (*p == 0);
		}

		double calc(queue<string> postQ) {
			stack<int> eval;
			int topNum, nextNum, answer;
			string t;
			while (!postQ.empty()) {
				t = postQ.front();
				postQ.pop();
				if (isInteger(t)) {
					eval.push(stoi(t));
				}
				else {
					topNum = eval.top();
					eval.pop();
					nextNum = eval.top();
					eval.pop();
					switch (t[0]) {
						case '+': answer = nextNum + topNum; break;
						case '-': answer = nextNum - topNum; break;
						case '*': answer = nextNum * topNum; break;
						case '/': answer = nextNum / topNum; break;
					}
					eval.push(answer);
				}
			}
			return eval.top();
		}

		void displayPostFix(queue<string> postFix) {
			cout << "Post fix form: ";
			while (!postFix.empty()) {
				cout << postFix.front() << " ";
				postFix.pop();
			}
			cout << endl;
		}

	public:
		RPNCalc() {}
		int calculate(string input) {
			queue<string> postFix = toPostfix(input);
			displayPostFix(postFix);
			return calc(postFix);
		}


};

int main() {
	string expression;
	RPNCalc calc;
	while (true) {
		cout << "Please enter an infix expression: ";
		getline(cin, expression);
		if (expression == "quit") {
			return 1;
		}
		cout << endl;
		cout << "The solution is: " << calc.calculate(expression) << endl;
	}
}