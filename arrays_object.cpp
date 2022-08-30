#include<iostream>
using namespace std;

class employee{
	char name[20];
	int age;
	public:
		void get_data();
		void put_data();
		
};
void employee::get_data(){
	cout<<"Enter name : ";
	cin>>name;
	cout<<"Enter age : ";
	cin>>age;
}
void employee::put_data(){
	
	cout<<"Name : "<<name<<endl;
	cout<<"Age : "<<age<<endl;
}
const int size=3;
int main(){
	
	employee manager[size];
	
	for(int i=0;i<size;i++){
		cout<<endl<<"Details of manager "<<i+1<<" : "<<endl;
		manager[i].get_data();
	}
	for(int i=0;i<size;i++){
		cout<<endl<<"Manager : "<<endl;
		manager[i].put_data();
	}
	return 0;
}
