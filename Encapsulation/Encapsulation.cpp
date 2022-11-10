#include<iostream>
using namespace std;

class Employee{
	private:
	
		string Name;
		string Company;
		int Age;
	public:
		void setName(string name){
			Name=name;
		}
		string getName(){
			return Name;
		}
		void setCompany(string company){
			Company=company;
		}
	
		string getCompany(){
			return Company;
		}
		
		void setAge(int age){
			Age=age;
		}
		int getAge(){
			return Age;
		}
		void Introduction(string name, string company, int age){
			cout<<"Name : "<<Name<<endl;
			cout<<"Company : "<<Company<<endl;
			cout<<"Age : "<<Age<<endl;
		}
};

int main(){
	
	Employee employee1 = Employee("Deepak Singh","Infosys",25);
	employee1.Introduction();	
	
	cout<<"-------------------"<<endl;
	Employee employee2 = Employee("Hariom Singh","TCS",30);
	employee2.Introduction();
	
}
