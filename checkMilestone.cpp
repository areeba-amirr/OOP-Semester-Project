#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <conio.h>  // Added for password masking
using namespace std;


// -------------------- CUSTOM EXCEPTIONS --------------------
class FileException {
    string message;
public:
    FileException(string msg) : message(msg) {}
    string what()const { return message; }
};

class LoginException {
    string message;
public:
    LoginException(string msg) : message(msg) {}
    string what()const { return message; }
};

class ValidationException {
    string message;
public:
    ValidationException(string msg) : message(msg) {}
    string what()const { return message; }
};

class QuizException {
    string message;
public:
    QuizException(string msg) : message(msg) {}
    string what()const { return message; }
};
class InvalidChoiceException {
    string msg;
public:
    InvalidChoiceException(string m) : msg(m) {}
    string what() const{ return msg; }
};

class PasswordMasking {
public:
    static string getMaskedPassword() {
        string password = "";
        char ch;

        while (true) {
            ch = getch();

            if (ch == 13 || ch == 10) {
                cout << endl;
                break;
            }
            else if (ch == 8) {
                if (!password.empty()) {
                    password.pop_back();
                    cout << "\b \b";
                }
            }
            else if (ch >= 32 && ch <= 126) {
                password.push_back(ch);
                cout << '*';
            }
        }
        return password;
    }
};

class User {
private:
    string name, email, password, qualification, field;
    int age;

public:
    void setName(string n) { name = n; }
    void setPassword(string p) { password = p; }
    void setEmail(string e) { email = e; }
    void setAge(int a) { if (a > 0) age = a; }
    void setQualification(string q) { qualification = q; }
    void setField(string f) { field = f; }

    string getName() { return name; }
    string getEmail() { return email; }
    string getPassword() { return password; }
    int getAge() { return age; }
    string getQualification() { return qualification; }
    string getField() { return field; }
};

class FileHandling {
public:
    void saveUser(string name, string password, int age, string email,
              string qualification, string field) {
    ofstream file("information.txt", ios::app);
    if (!file)
        throw FileException("Unable to open information.txt for writing");

    file << name << endl;
    file << password << endl;
    file << age << endl;
    file << email << endl;
    file << qualification << endl;
    file << field << endl;
    file.close();
}
    bool loadUser(string uName, string pass,
              string &n, string &p, int &a, string &e,
              string &q, string &f) {
    ifstream file("information.txt");
    if (!file)
        throw FileException("information.txt file not found");

    while (getline(file, n)) {
        getline(file, p);
        file >> a;
        file.ignore();
        getline(file, e);
        getline(file, q);
        getline(file, f);

        if (n == uName && p == pass)
            return true;
    }
    return false;
}

bool loadRoadmap(string userField) {
    if (userField.empty())
        throw ValidationException("Field of interest is empty");

    // 1. Convert input to Lowercase for case-insensitivity
    string inputLower = "";
    for (char c : userField) {
        inputLower += tolower(c);
    }

    string fileName = "";

    // 2. Check for "web" or "app" keywords
    if (inputLower.find("web") != string::npos) {
        fileName = "RoadmapWeb.txt";
        cout << "\n[System]: Web Development identified." << endl;
    } 
    else if (inputLower.find("app") != string::npos) {
        fileName = "AppDev-Roadmap.txt";
        cout << "\n[System]: App Development identified." << endl;
    } 
    else {
        // 3. Exception if neither is found
        throw InvalidChoiceException("No matching roadmap for: '" + userField + "'. Please use 'Web' or 'App'.");
    }

    // 4. File Opening with Exception
    ifstream file(fileName);
    if (!file)
        throw FileException("Error: " + fileName + " missing from directory!");

    string line;
    while (getline(file, line))
        cout << line << endl;

    return true;
}

    // NEW FUNCTION: Load user's quiz results
    vector<string> loadUserResults(const string& userName) {
        vector<string> results;
        ifstream file("quiz_results.txt");
        string line;

        while (getline(file, line)) {
            if (line.find(userName) != string::npos) {
                results.push_back(line);
            }
        }
        file.close();
        return results;
    }
};

// -------------------- SIMPLE QUIZ CLASS --------------------
class SimpleQuiz {
private:
    struct Question {
        string question;
        string options[4];
        char correctAnswer;
    };

public:
    // Function to load quiz questions from file
   vector<Question> loadQuiz(const string& fileName) {
   	 vector<Question> quizQuestions;
    ifstream file(fileName);
    if (!file){
        throw QuizException("Quiz file not found: " + fileName);
        }
        
        string line;
        Question currentQuestion;
        int lineCount = 0;
        
        while (getline(file, line)) {
            lineCount++;
            
            if (line.empty()) {
                // End of a question block
                if (!currentQuestion.question.empty()) {
                    quizQuestions.push_back(currentQuestion);
                    currentQuestion = Question(); // Reset
                    lineCount = 0;
                }
                continue;
            }
            
            if (lineCount == 1) {
                // First line is the question
                currentQuestion.question = line;
            }
            else if (lineCount >= 2 && lineCount <= 5) {
                // Lines 2-5 are options
                currentQuestion.options[lineCount-2] = line;
            }
            else if (lineCount == 6) {
                // Line 6 is the answer
                if (!line.empty()) {
                    currentQuestion.correctAnswer = tolower(line[0]);
                }
            }
        }
        
        // Don't forget the last question if file doesn't end with empty line
        if (!currentQuestion.question.empty()) {
            quizQuestions.push_back(currentQuestion);
        }
        
        file.close();
        
        if (quizQuestions.empty()) {
            cout << "Warning: No questions loaded from " << fileName << endl;
        }
        
        return quizQuestions;
    }

    // Function to start quiz
    void startQuiz(const string& userName, const string& quizType) {
        string fileName;
        string quizName;
        
        if (quizType == "web") {
            fileName = "web-dev.txt";
            quizName = "WEB DEVELOPMENT";
        } else if (quizType == "app") {
            fileName = "app-dev.txt";
            quizName = "APP DEVELOPMENT";
        } else {
            cout << "Invalid quiz type!\n";
            return;
        }

        cout << "\n================================\n";
        cout << "      " << quizName << " QUIZ\n";
        cout << "================================\n\n";
        
        vector<Question> allQuestions = loadQuiz(fileName);
        
        if (allQuestions.empty())
    throw QuizException("No questions available for quiz");

        // FIXED: Better shuffle with time-based seed
        unsigned seed = time(0) + userName.length(); // Add userName length for more variation
        mt19937 g(seed);
        shuffle(allQuestions.begin(), allQuestions.end(), g);

        // Take 10 questions
        int totalQuestions = min(10, (int)allQuestions.size());
        int score = 0;
        
        cout << "Total Questions: " << totalQuestions << endl;
        cout << "Instructions: Type a, b, c, or d for your answer\n";
        cout << "--------------------------------\n\n";

        for (int i = 0; i < totalQuestions; i++) {
            Question q = allQuestions[i];
            
            cout << "QUESTION " << (i+1) << ":\n";
            cout << q.question << "\n\n";
            cout << "a) " << q.options[0] << endl;
            cout << "b) " << q.options[1] << endl;
            cout << "c) " << q.options[2] << endl;
            cout << "d) " << q.options[3] << endl;
            
            cout << "\nYour choice: ";
            string answer;
            getline(cin, answer);
            
            if (answer.empty()) {
                cout << "You skipped this question.\n";
                continue;
            }
            
            char userAnswer = tolower(answer[0]);
            
            if (userAnswer == q.correctAnswer) {
                cout << "? Correct!\n";
                score++;
            } else {
                cout << "? Wrong! Correct answer is: " << q.correctAnswer << "\n";
            }
            cout << "--------------------------------\n";
        }

        // Show result
        cout << "\n================================\n";
        cout << "           RESULTS\n";
        cout << "================================\n";
        cout << "User: " << userName << endl;
        cout << "Score: " << score << "/" << totalQuestions << endl;
        
        float percentage = (score * 100.0) / totalQuestions;
        cout << "Percentage: " << percentage << "%" << endl;
        
        if (percentage >= 80) {
            cout << "Grade: A - Excellent! ??\n";
        } else if (percentage >= 60) {
            cout << "Grade: B - Good! ??\n";
        } else if (percentage >= 40) {
            cout << "Grade: C - Average\n";
        } else {
            cout << "Grade: F - Needs Improvement\n";
        }
        cout << "================================\n";
        
        // Save to results file
        saveResult(userName, quizType, score, totalQuestions);
    }

private:
    // Function to save quiz result
    void saveResult(const string& userName, const string& quizType, 
                    int score, int total) {
        ofstream resultFile("quiz_results.txt", ios::app);
        
        if (resultFile.is_open()) {
            time_t now = time(0);
            char* dt = ctime(&now);
            // Remove newline from ctime
            string timeStr = dt;
            timeStr = timeStr.substr(0, timeStr.length()-1);
            
            resultFile << userName << " | " << quizType << " | " 
                      << score << "/" << total << " | " << timeStr << endl;
            resultFile.close();
            cout << "Result saved to quiz_results.txt\n";
        }
    }
};

// -------------------- STUDENT CLASS --------------------
class Student : public User {
private:
    FileHandling fm;
    SimpleQuiz quiz;
    
public:
    void showMenu() {
        cout << "\n\t" << string(30, '=') << "\n";
    cout << "\t      STUDENT DASHBOARD\n";
    cout << "\t" << string(30, '=') << "\n";
    cout << "\t 1. [ View My Profile ]\n";
    cout << "\t 2. [ Update Information ]\n";
    cout << "\t 3. [ Learning Roadmap ]\n";
    cout << "\t 4. [ Attempt Skill Quiz ]\n";
    cout << "\t 5. [ Secure Logout ]\n";
    cout << "\t" << string(30, '-') << "\n";
    cout << "\t Choice: ";
    }

    void showDetails() {
        cout << "\n\t+---------------------------------------+\n";
    cout << "\t|            USER PROFILE               |\n";
    cout << "\t+---------------------------------------+\n";
    cout << "\t  NAME:          " << getName() << "\n";
    cout << "\t  AGE:           " << getAge() << " Years\n";
    cout << "\t  EMAIL:         " << getEmail() << "\n";
    cout << "\t  QUALIFICATION: " << getQualification() << "\n";
    cout << "\t  INTEREST:      " << getField() << "\n";
    cout << "\t+---------------------------------------+\n";

        // NEW: Show quiz history
        cout << "\n----- QUIZ ATTEMPT HISTORY -----\n";
        vector<string> attempts = fm.loadUserResults(getName());
        if (attempts.empty()) {
            cout << "No quiz attempts yet.\n";
        } else {
            for (size_t i = 0; i < attempts.size(); ++i) {
                // Parse the result line
                string result = attempts[i];
                size_t pos1 = result.find(" | ");
                size_t pos2 = result.find(" | ", pos1 + 3);
                size_t pos3 = result.find(" | ", pos2 + 3);
                
                if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
                    string quizType = result.substr(pos1 + 3, pos2 - pos1 - 3);
                    string score = result.substr(pos2 + 3, pos3 - pos2 - 3);
                    string timeStamp = result.substr(pos3 + 3);
                    
                    cout << (i + 1) << ". ";
                    cout << (quizType == "web" ? "Web Development" : "App Development");
                    cout << " Quiz - Score: " << score;
                    cout << " - Date: " << timeStamp << endl;
                } else {
                    cout << (i + 1) << ". " << result << endl;
                }
            }
        }
        cout << "-------------------------------\n";
    }
    int getValidStudentChoice() {
    int opt;

    if (!(cin >> opt)) {
        cin.clear();
        cin.ignore(1000, '\n');
        throw InvalidChoiceException("Numbers only allowed!");
    }

    if (opt < 1 || opt > 6) {
        throw InvalidChoiceException("Enter between 1 and 6 only!");
    }

    cin.ignore();
    return opt;
}
    // Update Profile Function with Email option - FIXED CANCEL ISSUE
  void updateProfile() {
    string oldName = getName();
    int choice;

    while (true) { // Approach 1: Input Loop for validation
        system("cls"); // Optional: Taake har baar screen saaf dikhe
        cout << "\n======= UPDATE PROFILE =======\n";
        showDetails();
        
        cout << "\nWhat would you like to update?\n";
        cout << "1. Name\n";
        cout << "2. Age\n";
        cout << "3. Email\n";
        cout << "4. Qualification\n";
        cout << "5. Field of Interest\n";
        cout << "6. Cancel\n";
        cout << "Enter choice: ";

        try {
            choice = getValidStudentChoice(); // Yeh 1-6 ke ilawa error throw karega
            break; // Agar choice sahi hai toh loop se bahar
        }
        catch (InvalidChoiceException &e) {
            cout << "\nERROR: " << e.what() << " Please try again." << endl;
            cout << "Press any key to continue...";
            _getch(); // Thori dair rukne ke liye
        }
    }

    // Agar user ne 6 (Cancel) choose kiya
    if (choice == 6) {
        cout << "\nUpdate cancelled. Returning to main menu.\n";
        return;
    }

    // Sahi choice milne ke baad switch case
    switch (choice) {
        case 1: {
            string newName;
            cout << "Enter new name: ";
            getline(cin, newName);
            setName(newName);
            cout << "Name updated successfully!\n";
            break;
        }
        case 2: {
            while (true) {
                try {
                    int newAge;
                    cout << "Enter Age: ";
                    cin >> newAge;
                    if (cin.fail()) {
                        cin.clear(); cin.ignore(1000, '\n');
                        throw ValidationException("Age must be a NUMBER!");
                    }
                    if (newAge <= 0 || newAge > 120) throw ValidationException("Age must be (1-120)!");
                    cin.ignore();
                    setAge(newAge);
                    cout << "Age updated successfully!\n";
                    break;
                } catch (ValidationException &e) { cout << "INPUT ERROR: " << e.what() << endl; }
            }
            break;
        }
        case 3: {
            string newEmail;
            cout << "Enter new email: ";
            getline(cin, newEmail);
            setEmail(newEmail);
            cout << "Email updated successfully!\n";
            break;
        }
        case 4: {
            string newQualification;
            cout << "Enter new qualification: ";
            getline(cin, newQualification);
            setQualification(newQualification);
            cout << "Qualification updated successfully!\n";
            break;
        }
        case 5: {
            string newField;
            cout << "Enter new field (Web/App Development): ";
            getline(cin, newField);
            setField(newField);
            cout << "Field updated successfully!\n";
            break;
        }
    }

    // Save logic
    cout << "\nDo you want to save changes to file? (y/n): ";
    char saveChoice;
    cin >> saveChoice;
    cin.ignore();

    if (tolower(saveChoice) == 'y') {
        saveToFile(oldName);
    }
}
    // Save updated info to file using old name to find the record
    void saveToFile(string oldName) {
        // Read all users from file
        ifstream inFile("information.txt");
        vector<string> allLines;
        string line;
        
        while (getline(inFile, line)) {
            allLines.push_back(line);
        }
        inFile.close();
        
        // Find and update current user's data using OLD name
        ofstream outFile("information.txt");
        bool found = false;
        
        for (size_t i = 0; i < allLines.size(); i += 6) {
            if (i + 5 < allLines.size()) {
                string storedName = allLines[i];
                
                if (storedName == oldName) {  // Find using OLD name
                    // Update this user's information with NEW data
                    outFile << getName() << endl;          // NEW name
                    outFile << getPassword() << endl;      // Same password
                    outFile << getAge() << endl;           // Updated age
                    outFile << getEmail() << endl;         // Updated email
                    outFile << getQualification() << endl; // Updated qualification
                    outFile << getField() << endl;         // Updated field
                    found = true;
                } else {
                    // Write other users as they are
                    outFile << allLines[i] << endl;
                    if (i+1 < allLines.size())
					 outFile << allLines[i+1] << endl;
                    if (i+2 < allLines.size()) 
					outFile << allLines[i+2] << endl;
                    if (i+3 < allLines.size()) 
					outFile << allLines[i+3] << endl;
                    if (i+4 < allLines.size())
					 outFile << allLines[i+4] << endl;
                    if (i+5 < allLines.size())
					 outFile << allLines[i+5] << endl;
                }
            }
        }
        
        outFile.close();
        
        if (found) {
            cout << "Profile updated in file successfully!\n";
        } else {
            cout << "Error: Could not find user in file!\n";
        }
    }
    
    void showRoadmap() {
    try {
        fm.loadRoadmap(getField());
    }
    catch (ValidationException &e) {
        cout << "\n[INPUT ERROR]: " << e.what() << endl;
    }
    catch (FileException &e) {
        cout << "\n[FILE ERROR]: " << e.what() << endl;
    }
}
    
    void startQuiz() {
        string field = getField();
        string fieldLower = "";
        
        // Convert field to lowercase
        for (char c : field) {
            if (c >= 'A' && c <= 'Z') 
                fieldLower += tolower(c);
            else 
                fieldLower += c;
        }
        
        // Automatically select quiz based on field of interest
        cout << "\n====================\n";
        cout << "    QUIZ PORTAL\n";
        cout << "====================\n";
        cout << "Your field: " << getField() << endl;
        
        // Determine quiz type based on field
        string quizType = "";
        if (fieldLower.find("web") != string::npos || 
            fieldLower.find("webdev") != string::npos ||
            fieldLower.find("web development") != string::npos) {
            quizType = "web";
            cout << "\nAutomatically selected: Web Development Quiz\n";
        }
        else if (fieldLower.find("app") != string::npos || 
                 fieldLower.find("appdev") != string::npos ||
                 fieldLower.find("app development") != string::npos ||
                 fieldLower.find("mobile") != string::npos) {
            quizType = "app";
            cout << "\nAutomatically selected: App Development Quiz\n";
        }
        else {
            // If field doesn't match known patterns, ask user
            cout << "\nCould not determine quiz type from your field.\n";
            cout << "Available Quizzes:\n";
            cout << "1. Web Development Quiz\n";
            cout << "2. App Development Quiz\n";
            cout << "\nEnter your choice (1-2): ";
            
            int choice;
            cin >> choice;
            cin.ignore();
            
            if (choice == 1) {
                quizType = "web";
            } else if (choice == 2) {
                quizType = "app";
            } else {
                cout << "Invalid choice! Returning to menu.\n";
                return;
            }
        }
        try {

        quiz.startQuiz(getName(), quizType);
    } 
    catch (QuizException &e) {

        cout << "\n[QUIZ ERROR]: " << e.what() << endl;
    }
    catch (FileException &e) {

        cout << "\n[FILE ERROR]: " << e.what() << endl;
    }
   
    }
};

// -------------------- MILESTONE APP CLASS --------------------
class MilestoneApp {
    FileHandling fm;

public:
   bool checkEmail(string email) {
    return (email.find('@') != string::npos);
}


    bool passwordStrength(string password) {
        int passScore = 0;
        
        if(password.length() >= 8) {
            cout << "Password length is good!" << endl;
            passScore++;
        } else {
            cout << "Password must be at least 8 characters!" << endl;
        }

        bool hasLower = false, hasUpper = false, hasDigit = false;
        for(int i = 0; i < password.length(); i++) {
            if(password[i] >= 'A' && password[i] <= 'Z') {
                hasUpper = true;
            }
            else if(password[i] >= 'a' && password[i] <= 'z') {
                hasLower = true;
            }
            else if(password[i] >= '0' && password[i] <= '9') {
                hasDigit = true;
            }
        }
        
        if(hasLower) {
            passScore++;
            cout << "Contains lowercase letters" << endl;
        } else {
            cout << "Add lowercase letters" << endl;
        }
        
        if(hasUpper) {
            passScore++;
            cout << "Contains uppercase letters" << endl;
        } else {
            cout << "Add uppercase letters" << endl;
        }
        
        if(hasDigit) {
            passScore++;
            cout << "Contains numbers" << endl;
        } else {
            cout << "Add numbers" << endl;
        }
        
        cout << "Password Strength: ";
        if(passScore >= 4) {
            cout << "STRONG" << endl;
        }
        else if(passScore >= 3) {
            cout << "MODERATE" << endl;
        }
        else {
            cout << "WEAK (Easy to hack!)" << endl;
        }
        
        if (passScore >= 3 && password.length() >= 8) {
            return true;
        } else {
            return false;
        }
    }
    char getValidMainChoice() {
    char ch;
    cin >> ch;
    cin.ignore();

    ch = tolower(ch);

    if (ch != 'a' && ch != 'b' && ch != 'c') {
        throw InvalidChoiceException("Only a, b or c is allowed!");
    }
    return ch;
}


  void run() {
    char choice;

    do {
        cout << "\n========= MAIN MENU =========\n";
        cout << "a. Sign Up\n";
        cout << "b. Login\n";
        cout << "c. Exit\n";
        cout << "Enter choice: ";

        try {
            choice = getValidMainChoice();
        }
        catch (InvalidChoiceException &e) {
            cout << "ERROR: " << e.what() << endl;
            continue;
        }


        switch (choice) {

        // ================= SIGN UP =================
        case 'a': {
            try {
                string n, p, e, q, f;
                int a;

                cout << "Enter Name: ";
                getline(cin, n);

                // -------- PASSWORD --------
                bool validPass = false;
                while (!validPass) {
                    cout << "Enter Password: ";
                    p = PasswordMasking::getMaskedPassword();

                    if (p.empty())
                        throw LoginException("Password cannot be empty!");

                    validPass = passwordStrength(p);
                    if (!validPass)
                        cout << "Please enter a stronger password!\n";
                }

                // -------- EMAIL --------
                bool ok = false;
                while (!ok) {
                    cout << "Enter Email: ";
                    cin >> e;
                    cin.ignore();

                    if (!checkEmail(e)) {
                        cout << "Invalid Email! Try again.\n";
                    } else {
                        ok = true;
                    }
                }

                // -------- AGE (FIXED PART) --------
                while (true) {
                    try {
                        cout << "Enter Age: ";
                        cin >> a;

                        if (cin.fail()) {
                            cin.clear();
                            cin.ignore(1000, '\n');
                            throw ValidationException("Age must be a NUMBER!");
                        }

                        if (a <= 0 || a > 120)
                            throw ValidationException("Age must be between 1 and 120!");

                        cin.ignore();
                        break;   // ? correct input ? exit loop
                    }
                    catch (ValidationException &e) {
                        cout << "Input Error: " << e.what() << endl;
                    }
                }

                cout << "Enter Qualification: ";
                getline(cin, q);

                cout << "Enter Field (Web/App Development): ";
                getline(cin, f);

                fm.saveUser(n, p, a, e, q, f);
                cout << "Account Created Successfully!\n";
            }
            catch (LoginException &e) {
                cout << "ERROR: " << e.what() << endl;
            }
            catch (FileException &e) {
                cout << "FILE ERROR: " << e.what() << endl;
            }
            catch (ValidationException &e) {
                cout << "INPUT ERROR: " << e.what() << endl;
            }

            break;
        }
case 'b': {
    string uName, pass;
    int attempts = 0;

    while (attempts < 3) {
        try {
            cout << "Enter Username: ";
            getline(cin, uName);

            cout << "Enter Password: ";
            pass = PasswordMasking::getMaskedPassword();

            string n, p, e, q, f;
            int a;

            if (!fm.loadUser(uName, pass, n, p, a, e, q, f))
                throw LoginException("Invalid username or password!");

            // ---------- SUCCESS ----------
            cout << "\nLogin Successful!\n";

            Student st;
            st.setName(n);
            st.setPassword(p);
            st.setAge(a);
            st.setEmail(e);
            st.setQualification(q);
            st.setField(f);

            int opt;
            do {
                st.showMenu();
                cin >> opt;
                cin.ignore();

                switch (opt) {
                	
                case 1: system("cls"); 
				st.showDetails(); break;
                case 2: system("cls");
				st.updateProfile(); break;
                case 3:system("cls");
				 st.showRoadmap(); break;
                case 4: system("cls");
				st.startQuiz(); break;
                case 5: system("cls");
				cout << "Logout Successful!\n"; break;
                default: cout << "Invalid option!\n";
                }
            } while (opt != 5);

            return; // ? login ho gaya ? case 'b' exit

        }
        catch (LoginException &e) {
            attempts++;
            cout << "LOGIN ERROR: " << e.what() << endl;
            cout << "Attempts left: " << (3 - attempts) << endl;
        }
    }

    cout << "\nToo many failed attempts. Login blocked!\n";
    break;
}


case 'c':
    cout << "Exiting Application...\n";
    break;
}

        } while (choice != 'c');
    }
};

int main() {
    // Console window clear karne ke liye (Windows)
    system("cls"); 
//    system("color 0B"); // Optional: Cyan color text ke liye (Black background)

    cout << "\n\t" << string(40, '=') << "\n";
    cout << "\t|                                      |\n";
    cout << "\t|       WELCOME TO MILESTONE APP       |\n";
    cout << "\t|     Your Career Path Starts Here     |\n";
    cout << "\t|                                      |\n";
    cout << "\t" << string(40, '=') << "\n";
    
    MilestoneApp app;
    app.run();
    
    system("cls");
    cout << "\n\n\t" << string(40, '*') << "\n";
    cout << "\t   THANK YOU FOR USING MILESTONE APP!\n";
    cout << "\t        Have a Great Day Ahead!\n";
    cout << "\t" << string(40, '*') << "\n\n";
    
    return 0;
}
