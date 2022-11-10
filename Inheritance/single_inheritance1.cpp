#include<iostream>
using namespace std;

class B{
	int a;
	public:
		int b;
		void get_ab();
		int get_a(void);
		void show_a(void);
};
//------------------------
class D:public B{
	int c;
	public:
		void mult(void);
		void display(void);
};
//--------------------------
void B:: get_ab(){
	a=10,b=27;
}
int B::get_a(){
	return a;
}
void D::mult(){
c=b*get_a();
}
void D::display(){
	cout<<"a = "<<get_a()<<endl;
	cout<<"b = "<<b<<endl;
	cout<<"Multiplication = "<<c<<endl;
}
//--------------------------------------------
int main(){
	
	D d;
	d.get_ab();
	d.mult();
	d.display();
	return 0;
}
