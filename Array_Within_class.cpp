#include<iostream>
#include<bits/stdc++.h>
using namespace std;
int const m=50;


class ITEM{
	
	int itemCode[m];
	float itemPrice[m];
	int count;
	public:
		void CNT(void){
			count=0;
		}
		void getitem(void);
		void displaySum(void);
		void remove(void);
		void displayItem(void);
	
};

//============================================
void ITEM::getitem(void)
{
	cout<<"Enter item code: ";
	cin>>itemCode[count];
	cout<<"Enter item Price: ";
	cin>>itemPrice[count];
	count++;
}
//===============================================

void ITEM::displaySum(void){
	
	float sum=0;
	
	for(int i=0;i<count;i++)
	{
		sum+=itemPrice[i];
	}
	cout<<endl<<"Total Value : "<<sum<<endl;
}
//=================================================

void ITEM::remove()
{
	int a;
	cout<<"Enter item code : ";
	cin>>a;
	
	for(int i=0; i<count; i++){
		if(itemCode[i]==a)
		itemPrice[i]=0;
	}
}

//========================================================

void ITEM :: displayItem(void)
{
	cout<<endl<<"Code Price\n";
	
	for(int i=0;i<count;i++)
	{
		cout<<endl<<itemCode[i];
		cout<<"   "<<itemPrice[i];
	}
	cout<<endl;
}
//========================================================

int main(){
	ITEM order;
	
	order.CNT();
	int x;
	
	do
	{
		cout<<"You can do the following: \n"<<"\n\tEnter appropriate number \n";
		cout<<endl<<"\t1 : Add an item ";
		cout<<endl<<"\t2 : Display total value ";
		cout<<endl<<"\t3 : Delete an item ";
		cout<<endl<<"\t4 : Display all item ";
		cout<<endl<<"\t5 : Quit";
		cout<<"\n\n\tWhat is your option? ";
		cin>>x;
		
		switch(x)
		{
			case 1:
				order.getitem();
				break;
			case 2:
				order.displaySum();
				break;
			case 3:
				order.remove();
				break;
			case 4:
				order.displayItem();
				break;
			case 5:
				break;
			default:
				cout<<"Error in input; try again"<<endl;
		}
	}
	while(x!=5);
	return 0;
}
