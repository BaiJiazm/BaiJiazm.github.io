#include <iostream>
using namespace std;

class Test {
	public:
		static void fun() const {
			cout<<"test fun"<<endl;
		}
};

int main() {
	Test::fun();
	return 0;
}
