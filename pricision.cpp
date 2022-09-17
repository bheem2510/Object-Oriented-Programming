#include<iostream>
#include<cmath>
using namespace std;
int main(){
	cout<<"Pricision set to 3 digits \n\n: ";
	cout.precision(3);
	
	cout.width(10);
	cout<<"Value";
	cout.width(15);
	cout<<"SQRT of value"<< " \n";
	
	for(int i=1;i<=5;i++){
		cout.width(8);
		cout<<i;
		cout.width(13);
		cout<<sqrt(i)<<"\n";
	}
		cout<<"\n Precision set to 5 digits \n\n";
		cout.precision(5);
		cout<<" sqrt(10) = "<<sqrt(10)<<endl<<endl;
		cout.precision(0);
		cout<<" sqrt(10) = "<<sqrt(10)<<"(Default setting)\n"; 
			 
	return 0;
	
	
}
