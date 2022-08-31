#include<iostream>
using namespace std;
const int m=50;
class ITEM{
	int itemcode[m];
	float itemprice[m];
	int count;
	public:
		void CNT(){
		count=0;
		}
		void get_item();
		void display_sum();
		void reemove();
		void displayitem();
};
void ITEM::get_item(){
			cout<<"Enter Item code: ";
			cin>>itemcode[count];
			cout<<"Enter item Price: ";
			cin>>itemprice[count];
			count++;
			cout<<endl;
		}

void ITEM::display_sum(){
	float sum=0;
	for(int i=0;i<count;i++){
		sum+=itemprice[i];
	}
	cout<<"Total Value = "<<sum<<endl;
}

void ITEM::reemove(){
	int a;
	cout<<"ENter item code to remove: ";
	cin>>a;
	for(int i=0;i<count;i++){
		if(itemcode[i]==a){
			itemprice[i]=0;
		}
	}
}
void ITEM::displayitem(){
	cout<<endl<<"Code Price\n";
	for(int i=0;i<count;i++){
		cout<<endl<<itemcode[i];
		cout<<"  "<<itemprice[i];
	}
	cout<<endl<<endl;
}
//-----------------------------------------------
int main(){
	ITEM order;
	order.CNT();
	int x;
	do{
		cout<<"You can do the following \n\tEnter appropriate number: "<<endl;
		cout<<"1 : Add an item"<<endl;
		cout<<"2 : Display total value"<<endl;
		cout<<"3 : Delete an item"<<endl;
		cout<<"4 : Display items"<<endl;
		cout<<"5 : Quit"<<endl ;
		cin>>x;
		switch(x){
			case 1:
			    order.get_item();
			    break;
			case 2:
				order.display_sum();
				break;
			case 3:
				order.reemove();
				break;
			case 4:
				order.displayitem();
				break;
			case 5:
				break;
			default:
				cout<<"\nError in input; Please try again\n\n";
		}
	}while(x!=5);
	return 0;
}
