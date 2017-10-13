#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;

int main(){
	ifstream commFile("community.dat");
	int a, b;
	while(commFile >> a >> b){
		cout << b - 1 << endl;
	}
}