#include <bits/stdc++.h>
using namespace std;

int main() {
	int n;
	set<int> s;
	while(!cin.eof()){
		cin >> n;
		s.insert(n);
	}

	printf("%d\n", s.size());

	return 0;
}
