#include<iostream>
using namespace std;

//======= Creating Class ====================

class employee{
	
	char name[30];
	float age;
	public:
		void getdata(void);
		void putdata(void);
};

//=============== Getting information =================

void employee::getdata(){
	cout<<"Enter name : ";
	cin>>name;
	cout<<"Enter age : ";
	cin>>age;
	
}

void employee::putdata(){
	cout<<"Name : "<<name<<endl;
	cout<<"Age : "<<age<<endl;
}
const int size=3;

//==================== Code Driver ============================

int main(){
	employee manager[size];
	
	for(int i=0;i<size;i++)
	{
	cout<<"Details of manager "<<i+1<<" : "<<endl;
	manager[i].getdata();
}

cout<<endl;
for(int i=0;i<size;i++){
	cout<<"Manager "<<i+1<<" : "<<endl;
	manager[i].putdata();
}
}
