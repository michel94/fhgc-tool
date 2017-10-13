#include <bits/stdc++.h>
using namespace std;
inline double absD(double x){
	return x > 0 ? x : -x;
}
double log2(double x){
	return absD(x) < 0.0000001 ? 0 : log(x) / log(2);
}


double entropy(vector<int>& count, double n){
	double s = 0;
	for(int i=0; i < (int)count.size(); i++){
		s += (count[i] / n) * log2(count[i] / n);
	}
	return - s;
}

int main(){
	vector<int> v = {1, 2, 2, 4, 0, 1};
	cout << entropy(v, 10) << endl;
}
