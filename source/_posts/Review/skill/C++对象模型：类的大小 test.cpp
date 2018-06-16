//���²��Ի���ΪGCC 32bit-Releaseģʽ

#include<iostream>
using namespace std;

class A {};
class B {};
class C : public A {
		virtual void c_fun()=0;
};
class D : public B, public C {};

int main() {
	cout<<"sizeof(A):"<<sizeof(A)<<endl;
	cout<<"sizeof(B):"<<sizeof(B)<<endl;
	cout<<"sizeof(C):"<<sizeof(C)<<endl;
	cout<<"sizeof(D):"<<sizeof(D)<<endl;
	return  0;
}

/*
 * ���Ϊ�� 1	1	4	4
 * ���һ��������������йأ�����8
 */

#include<iostream>
using namespace std;

class A {
	private:
		int data;
};

class B {
	private:
		int data;
		static int data1;
};
int B::data1=0;

//ע��void main()����뱨��
main() {
	cout<<"sizeof(A) = "<< sizeof(A) <<endl;
	cout<<"sizeof(B) = "<< sizeof(B) <<endl;
}

/*
 * ���Ϊ�� 4	4
 */

#include<iostream>
using namespace std;

class A {
	public :
		A(int a) {
			a=x;
		}
		void f(int x) {
			cout<<x<<endl;
		}
		~A() {}

	private:
		int x;
		int g;
};
class B {
	public:
	private:
		int data;
		int data2;
		static int xs;
};
int B::xs=0;

int main() {
	A s(10);
	s.f(10);
	cout << "sizeof(a): "<< sizeof(A) << endl;
	cout << "sizeof(b): "<< sizeof(B) << endl;
}

/*
 * ���Ϊ�� 8	8
 */

#include<iostream>
using namespace std;

class X {};
class Y : public virtual X {};
class Z : public virtual X {};
class A : public Y, public Z {};
// ����Ϊ�������ʯ�̳�

int main() {
	cout << "sizeof(X): " << sizeof(X) << endl;
	cout << "sizeof(Y): " << sizeof(Y) << endl;
	cout << "sizeof(Z): " << sizeof(Z) << endl;
	cout << "sizeof(A): " << sizeof(A) << endl;

	return 0;
}

/*
 * ���Ϊ�� 1	4	4	8
 */

#include<iostream>
using namespace std;

class X {
		int a;
};
class Y : public virtual X {};
class Z : public virtual X {};
class A : public Y, public Z {};
// ����Ϊ�������ʯ�̳�

int main() {
	cout << "sizeof(X): " << sizeof(X) << endl;
	cout << "sizeof(Y): " << sizeof(Y) << endl;
	cout << "sizeof(Z): " << sizeof(Z) << endl;
	cout << "sizeof(A): " << sizeof(A) << endl;

	return 0;
}

/*
 * ���Ϊ�� 4	8	8	12
 */

#include<iostream>
using namespace std;

class X {
		int a;
		virtual void fun()=0;
};
class Y : public virtual X {};
class Z : public virtual X {};
class A : public Y, public Z {};
// ����Ϊ�������ʯ�̳�

int main() {
	cout << "sizeof(X): " << sizeof(X) << endl;
	cout << "sizeof(Y): " << sizeof(Y) << endl;
	cout << "sizeof(Z): " << sizeof(Z) << endl;
	cout << "sizeof(A): " << sizeof(A) << endl;

	return 0;
}

/*
 * ���Ϊ�� 8	12	12	16
 */
