#include<iostream>
#include<fstream>
using namespace std;
class Milestone{
	public :
		Milestone(){
			cout<<" Welcome To MILESTONE - Your Career Advisery Platform "<<endl;
		}
string name,password,qualification,field,email;
				int age;
	void saveUser(const Milestone &m1){
		ofstream file("info.txt",ios::app);
		file<<m1.name<<endl;
		file<<m1.password<<endl;
		file<<m1.age<<endl;
		file<<m1.email<<endl;
		file<<m1.qualification<<endl;
		file<<m1.field<<endl;
		file.close();
	}
	bool Login(){
	}
	bool checkgmail(string email){
    string gmail="@gmail.com";
    
    int eLength = email.length();
    int gLength = gmail.length();
    
    if(eLength < gLength){
        cout<<"Email too short and something is missing"<<endl;
        return false;
    }
    
    for(int i=0; i<gLength; i++){
        if(email[eLength-gLength+i] != gmail[i]){
            cout<<"Error occurred. Email should end with @gmail.com\n";
            return false;
        }
    }
    return true;
}	
};
class Roadmap{
	public :
		void loadApp(){
			string line;
		ifstream file("Roadmap.txt");
		while(getline(file,line)){
			
			cout<<line<<endl;
//			cout<<"File load successfully";
		}
		file.close();
		}
		void loadWeb(){
			string line;
		ifstream file("RoadmapWeb.txt");
		while(getline(file,line)){
			
			cout<<line<<endl;
//			cout<<"File load successfully";
		}
		file.close();
		}
		
};
int main(){
	Milestone m1;
		char choice;
//	cout<<" Welcome To MILESTONE - Your Career Advisery Platform "<<endl;
	do{
		cout<<"Welcome  To  Main Menu please choose an option below"<<endl;
		cout<<"a. New User? Create an Account (Sign up)"<<endl<<" b. My Portal (Log in)"<<endl<<"c. Exit the Application"<<endl;
		cin>>choice;
		switch(choice){
			case 'a' : {
					cout<<"Enter your Name : ";
				cin.ignore();
	getline(cin,m1.name);
	cout<<"Enter your Password : ";
	getline(cin,m1.password);

        bool validEmail = false;
        while(!validEmail){
            cout<<"Enter Your Email :";
            cin>>m1.email;

            validEmail = m1.checkgmail(m1.email);   //Email Verification
        }
	cout<<"Enter your age ";
	cin>>m1.age;
	
	cin.ignore();
	cout<<"Enter your Qualification : ";
	getline(cin,m1.qualification);
	cout<<"Enter your field ";
	getline(cin,m1.field);
	m1.saveUser(m1);
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

