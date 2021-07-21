#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
#include<sstream>

#define MAX 100
#define e 2.71828
using namespace std;

// xu li File
fstream f;
void get(string &input) {
	char temp[MAX];
	f.getline(temp, MAX);
	input = temp;
}

// dinh nghia stack va cac ham lien quan
//Point
struct Point {
	char value;
	Point *pNext;
};

void Push(Point* &s, char c) {
	Point *temp = new Point;
	temp->value = c;
	temp->pNext = s;
	s = temp;
}

void Pop(Point* &s) {
	if(s == nullptr) return;
	Point *temp;
	temp = s;
	s = s->pNext;
	delete temp;
	
}
// Point1
struct Point1 {
	double value;
	Point1 *pNext;
};

void Push1(Point1* &s, double c) {
	Point1 *temp = new Point1;
	temp->value = c;
	temp->pNext = s;
	s = temp;
}

void Pop1(Point1* &s) {
	if(s == nullptr) return;
	Point1 *temp;
	temp = s;
	s = s->pNext;
	delete temp;
}
// xu li bai toan

int isNum(char c) {
	return (c >= '0' && c <= '9');
}
int isOperator(char c) {
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}
int isChar(char c) {
	return (c >= 97 && c <= 122);
}

int priority(char c) {
	if (c == '+' || c == '-') return 1;
	if (c == '*' || c == '/') return 2;
	if (c == '^') return 3;
	if (c >= 97 && c <= 122) return 4;
	return 0;
}

// chuan hoa dau vao
void standardized(string &input) {
	int len = input.length();
	// delete space
	for(int i = 0; i <= len; i++) {
		if(input[i] == ' ') {
			for(int j = i; j <= len - 1; j++) {
				input[j] = input[j+1];
			}
			len--;
			i--;
		}
	}
	// to lowercase
	for(int i = 0; i <= len; i++) {
		if(input[i] >= 65 && input[i] <= 90) input[i] += 32;
	}
}

// convert infix to suffix                 
string convertInfixToSuffix(string infix) {
	
	string suffix = "";
	
	Point *s;
	s = nullptr;
	int len = infix.size();
	
	
	for(int i = 0; i < len; i++) {
		if(isNum(infix[i])) {
			if(infix[i-1] == '-' && (i - 1 == 0 || isOperator(infix[i-2]) || infix[i-2] == '(' || isChar(infix[i-2]))) {
				suffix += '-';
			}
			while(isNum(infix[i])) {
				suffix += infix[i];
				i++;
			}
			if(infix[i] != '.') suffix += ' ';
		}
		if(infix[i] == '.') {
			suffix += infix[i];
		}
		if(infix[i] == '(') Push(s, infix[i]);
		if(infix[i] == ')') {
			while(s->value != '(') {
				if(isChar(s->value)) {
					string temp = "";
					while(isChar(s->value)) {
						temp += s->value;
						Pop(s);
					}
					for(int i = temp.length() - 1; i >= 0; i--) {
						suffix += temp[i];
					}
				} else {
					suffix += s->value;
					Pop(s);
				}
				suffix += ' ';
			}
			Pop(s);
		}
		if(isOperator(infix[i])) {
			if(infix[i] == '-' && (i == 0 || infix[i-1] == '(' || isOperator(infix[i-1]) || isChar(infix[i-1]))) {
				
			} else {
				while(s != nullptr && priority(s->value) >= priority(infix[i])) {
					if(isChar(s->value)) {
						string temp = "";
						while(s != nullptr && isChar(s->value)) {
							temp += s->value;
							Pop(s);
						}
						for(int i = temp.length() - 1; i >= 0; i--) {
							suffix += temp[i];
						}
					} else {
						suffix += s->value;
						Pop(s);
					}
					suffix += ' ';
				}
				Push(s, infix[i]);
			} 
		}
		if(isChar(infix[i])) {
			Push(s, infix[i]);
		}
	}
	
	while(s!= nullptr) {
		if(isChar(s->value)) {
			string temp = "";
			while(s != nullptr && isChar(s->value)) {
				temp += s->value;
				Pop(s);
			}
			for(int i = temp.size() - 1; i >= 0; i--) {
				suffix += temp[i];
			}
		} else {
			suffix += s->value;
			Pop(s);
		}
		suffix += ' ';
	}
	return suffix;
}

// calculate Suffix
double calcuSuffix(string suffix) {
	Point1 *s;
	s = nullptr;
	
	double x = 0,a,b,c;
	
	int len = suffix.size();
	
	for(int i = 0; i < len; i++) {
		// number
		if(isNum(suffix[i]) || suffix[i] == '.') {
			int j = i,k = i;
			if(suffix[i-1] == '-') k = i - 1;
			while(suffix[j] == '.' || isNum(suffix[j]) == 1) j++;
			string str = "";
			for(k; k < j; k++) {
				str += suffix[k];
			}
			stringstream ss(str);
			ss >> x;
			Push1(s, x);
			x = 0;
			i = j; 
		}
		// operator
		if(isOperator(suffix[i])) {
			if(!isNum(suffix[i+1])) {
				if(s) {
					a = s->value;
					Pop1(s);
				} else a = 0;
				if(s) {
					b = s->value;
					Pop1(s);
				} else b = 0;
				
				if(suffix[i] == '-') c = b - a;
				if(suffix[i] == '+') c = b + a;
				if(suffix[i] == '*') c = b * a;
				if(suffix[i] == '/') c = b / a;
				if(suffix[i] == '^') c = pow(b,a);
				Push1(s, c);
			}
		}
		// transcendental function
		if(isChar(suffix[i])) { 
			if(suffix[i] == 's' && suffix[i+1] == 'i' && suffix[i+2] == 'n') {
				a = s->value;
				Pop1(s);
				c = sin(a);
				i += 2;
			}
			if(suffix[i] == 'c' && suffix[i+1] == 'o' && suffix[i+2] == 's') {
				a = s->value;
				Pop1(s);
				c = cos(a);
				i += 2;
			}
			if(suffix[i] == 't' && suffix[i+1] == 'a' && suffix[i+2] == 'n') {
				a = s->value;
				Pop1(s);
				c = tan(a);
				i += 2;
			}
			if(suffix[i] == 's' && suffix[i+1] == 'q' && suffix[i+2] == 'r' && suffix[i+3] == 't') {
				a = s->value;
				Pop1(s);
				c = sqrt(a);
				i += 3;
			}
			if(suffix[i] == 'l' && suffix[i+1] == 'n') {
				a = s->value;
				Pop1(s);
				c = log(a);
				i += 1;
			}
			if(suffix[i] == 'l' && suffix[i+1] == 'o' && suffix[i+2] == 'g') {
				a = s->value;
				Pop1(s);
				b = s->value;
				Pop1(s);
				c = log(a)/log(b);
				i += 2;
			}
			if(suffix[i] == 'e' && suffix[i+1] == 'x' && suffix[i+2] == 'p') {
				a = s->value;
				Pop1(s);
				c = pow(e,a);
				i += 2;
			}
			Push1(s, c);
		}
	}
	double k = s->value;
	delete s;
	return k;
}
            
int main() {
	string chucnang, fileInput, input;
	// clear file output
	f.open("Output.txt", ios::out);
	f.close();
	//
	cout << "De 803: Chuyen doi bieu thuc trung to sang hau to va tinh gia tri bieu thuc" << endl << endl;
	cout << "\tGiao vien huong dan: Nguyen Van Nguyen" << endl;
	cout << "\tSinh vien thuc hien:" << endl;
	cout << "\t  + Dang Huynh Khanh Duong     20TCLC_KHDL" << endl;
	cout << "\t  + Nguyen Kieu Quoc           20TCLC_KHDL" << endl << endl;
	cout << "\t ======= CAC CHUC NANG CUA CHUONG TRINH =======" << endl;
	cout << "\t|                                              |" << endl;
	cout << "\t| 1. Bien doi bieu thuc trung to thanh hau to  |" << endl;
	cout << "\t| 2. Tinh gia tri cua bieu thuc trung to       |" << endl;
	cout << "\t| #. Thoat khoi chuong trinh                   |" << endl;
	cout << "\t|                                              |" << endl;
	cout << "\t ==============================================" << endl;
	cout << "Lua chon chuc nang: ";
	cin >> chucnang;
	if(chucnang != "1" && chucnang != "2") return 0;
	while(chucnang == "1" || chucnang == "2") {
		cout << "Lua chon file input: ";
		cin >> fileInput;
		if(chucnang == "1") {
			f.open(fileInput);
			get(input);
			f.close();
			standardized(input);
			string output = convertInfixToSuffix(input);
			cout << "Input: " << input << endl;
			cout << "Output: " << output << endl << endl;
			f.open("Output.txt", ios::app);
			f << output;
			f << "\n";
			f.close();
		} 
		if(chucnang == "2") {
			f.open(fileInput);
			get(input);
			f.close();
			standardized(input);
			string suffix = convertInfixToSuffix(input);	
			double output = calcuSuffix(suffix);
			cout << "Input: " << input << endl;
			cout << "Output: " << output << endl << endl;	
			f.open("Output.txt", ios::app);
			f << output;
			f << "\n";
			f.close();
		}
		cout << "Nhap tiep chuc nang muon thuc hien: ";
		cin >> chucnang;
	}
	return 0;
}
