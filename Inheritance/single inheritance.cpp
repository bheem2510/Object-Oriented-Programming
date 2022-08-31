#include<iostream>
using namespace std;

class parent{
	private:
		char name[20];
		public:
			void read(){
				cout<<"Enter name: ";
				cin>>name;
			}
			void display(){
				cout<<"Name is: "<<name;
			}
};
class child1:public parent{
	private:
		int age;
		public:
			read_age(){
				read();
				cout<<"Enter age: ";
				cin>>age;
			}
			void disp_age(){
				display();
				cout<<endl<<"Age is: "<<age;
			}
};

class gchild1:public child1{
	private:
		char job[20];
		public:
			void read_work(){
				read_age();
				cout<<"Enter profession: ";
				cin>>job;
			}
			void disp_work(){
				disp_age();
					cout<<endl<<"Profession is : "<<job;
				}
};
int main(){
	gchild1 g1;
	
	g1.read_work();
	g1.disp_work();
	return 0;
}
