#include "decode.hpp"
#include "encode.hpp"
#include "headers.hpp"
#include "decode.cpp"
#include "encode.cpp"

void intro() {
	cout<<"================>\t\tText Compression\t\t<================\n"<<endl;
	cout<<"- Lossless Data Compression technique"<<endl;
	cout<<"- Encoding follows the prefix rule"<<endl;
	cout<<"- Reduces cost of transmission"<<endl;
	cout<<"- Variable sized encoding used"<<endl;
	cout<<"- Time complexity O(n.log(n))"<<endl;
}

void menu() {
	int ch;
	while (true) {
		cout<<"\n\n===========>\tMain Menu\t<===========\n"<<endl;
		cout<<"0. Exit"<<endl;
		cout<<"1. Encode a file"<<endl;
	        cout<<"2. Decode a file"<<endl;
	        cout<<"\nEnter choice: ";
	        cin>>ch;
		
		if (!ch) {
			cout<<"\nProgram terminating successfully."<<endl;
                	break;
		}
	        switch (ch) {
	        	case 1: 
	        	        encode();
	        	        break;
	        	case 2:
			        decode();
			        break;
			default: 
				cout<<"\nEnter right choice(0-2)"<<endl;
		}
	}
}

int main() {
	intro();
	menu();
	return 0;
}
