#include<iostream>
#include<fstream>
using namespace std;
int main(){
		char choice;
	cout<<" Welcome To MILESTONE - Your Career Advisery Platform "<<endl;
	do{
		cout<<"Welcome  To  Main Menu please choose an option below"<<endl;
		cout<<"a. New User? Create an Account (Sign up)"<<endl<<" b. My Portal (Log in)"<<endl<<"c. Exit the Application"<<endl;
		cin>>choice;
		switch(choice){
			case 'a' : {
				string name,password,qualification,field,email;
				int age;
					cin.ignore();
				cout<<"Enter Your User Name :";
				getline(cin,name);
				cout<<"Enter Your Password :";
				cin>>password;
				cin.ignore();
				cout<<"Enter Your Highest Qualifiaction :";
				getline(cin,qualification);
				cout<<"Enter Your Email :";
				cin>>email;
				cin.ignore();
				cout<<"Enter Your Field of Interest :";
				getline(cin,field);
				cout<<"Enter Your age :";
				cin>>age;
				ofstream file("info.txt");
				file<<name<<endl;
				file<<password<<endl;
				file<<qualification<<endl;
				file<<email<<endl;
				file<<field<<endl;
				file<<age<<endl;
				file.close();
				cout<<"Registration Successfully!!!"<<endl;
				break;
			}
		case 'b' : {
			bool found;
			int loginAttempts = 0;
			string uName,password;
			while(loginAttempts < 3){
					cin.ignore();
				cout<<"Enter Your UserName & Password: ";
				getline(cin,uName);
				cout<<"Enter Your  Password: ";
				cin>>password;
				ifstream file("info.txt");
				 string fileUser, filePass;
//				int i= 0;
				while (file >> fileUser >> filePass) {
            if (fileUser == uName && filePass == password) {
                found = true;
                break; // stop reading file
            }
        }

        if (found) {
            cout << "Login Successful!" << endl;
            int choice;
            do{
            	cout<<"User's Menu"<<endl;
            	cout<<"1. My Details"<<endl<<"2. Quiz"<<"3. Roadmap"<<endl<<"4. Future Scope"<<endl<<"5. Re Quiz"<<endl<<"6. Log out"<<endl;
            	cin>>choice;
            switch(choice){
            	case 1 :{
            		cout<<" User's Personal Info "<<endl;
            			ifstream file("info.txt");
            			 string fileUser, filePass, fileQ ,fileM,fileF, fileA;
            			 	while (file >> fileUser >> filePass>>fileQ >>fileM>>fileF>>fileA) {
            			 		cout<<"Name : "<<fileUser<<endl;
            			 		cout<<"Age : "<<fileA<<endl;
            			 		cout<<"Email : "<<fileM<<endl;
            			 		cout<<"Qualification : "<<fileQ<<endl;
            			 		cout<<"Field Of Interest : "<<fileF<<endl;
							 }
					break;
				}
				case 2 :{
            		
					break;
				}
				case 3 :{
            		
					break;
				}
				case 4 :{
            		
					break;
				}
				case 5 :{
            		
					break;
				}
				case 6 :{
            		cout<<"Log Out!!"<<endl;
					break;
				}
			}	
			} while(choice != 6);
            break; // exit loginAttempts loop
        } else {
            cout << "Invalid Credentials, Try Again!!" << endl;
            loginAttempts++;
        }
    }
//    	cout<<"Try Again Later!!"<<endl;
////			choice == 'c';
			break;
			}

			case 'c' :{
				cout<<"Exiting the application...."<<endl;
				break;	}
			}
		}
	 while(choice != 'c');
	
	return 0;
}

