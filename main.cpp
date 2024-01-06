#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <windows.h>
using namespace std;

struct Visit {
    string doctorname;
    string time;
    string problem;
};

struct UserData {
    string login;
    string password;
    string fullname;
    int phoneNumber = 0;
    long long pesel = 0;
    vector<Visit> visits;
};

struct Doctor{
    string fullname;
    vector<string> time;
};

fstream file; // declare a file to read or/and write
void logInSignUp(UserData& enterUser);
void saveData(const UserData& userData);
bool checkData(string& login, string& password);
bool checkLogin(string& login);
void scheduleVisit(UserData& userData, const Doctor& fahrenheit, const Doctor& somebodies, const Doctor& nowak);
void showInfo(const string& login, const string& password);
void showHistory(string& login);
void saveHistory(const UserData& userData);
void RemoveTime(Doctor& doctor1, const string& userTypedHour);

int main() {
    int userCase;
    UserData currentUser;
    logInSignUp(currentUser);

    Doctor fahrenheit{"S. Fahreheit", {"7:45", "8:15", "9:30"}};
    Doctor somebodies{"M. Somebodies", {"12:15", "14:00", "15:45"}};
    Doctor nowak{"K. Nowak", {"18:00", "19:10", "20:05"}};

    while (!cin.fail()) {
        cout << "\nWhat do you want to do?\n"
             << "1 - see your profile\n"
             << "2 - check your history of doctor visits\n"
             << "3 - schedule a new doctor visit\n"
             << "q - to quit a program\n";
        cin >> userCase;
        switch (userCase) {
            case 1:
                system("CLS"); // clear the console
                showInfo(currentUser.login, currentUser.password);
                break;
            case 2:
                system("CLS");
                showHistory(currentUser.login);
                break;
            case 3:
                system("CLS");
                scheduleVisit(currentUser, fahrenheit, somebodies, nowak);
                break;
            default:
                return 0;
        }
    }

    return 0;
}

bool checkData(string& login, string& password) { // create a function with the references to strings
    file.open(R"(C:\Users\migga\Documents\PJATK\! Clinic database\database.txt)"); // open a file
    string line;
    bool found = false;
    while (getline(file, line)) { // reading (cin) a provided line
        if (line.find("Login: " + login) != string::npos && line.find("Password: " + password) != string::npos) {
            // find a line in a file that includes login and password until the end of the file
            found = true;
            break;
        }
    }
    file.close();
    return found;
}

bool checkLogin(string& login) {
    file.open(R"(C:\Users\migga\Documents\PJATK\! Clinic database\database.txt)");
    string line;
    bool found = false;
    while (getline(file, line)) {
        if (line.find("Login: " + login) != string::npos) {
            // find a line with login until the end of the file
            found = true;
            break;
        }
    }
    file.close();
    return found;
}

void saveData(const UserData& userData) {
    // open a file in appending mode (it won't be overwritten, but add to the existed text)
    file.open(R"(C:\Users\migga\Documents\PJATK\! Clinic database\database.txt)", ios::app);
    file << "Login: " << userData.login << " | ";
    file << "Password: " << userData.password << endl;
    file << "Fullname: " << userData.fullname << endl;
    file << "Phone number: " << userData.phoneNumber << endl;
    file << "PESEL: " << userData.pesel << endl;
    file << "-------------------------------------------------------" << endl;
    file.close();
}

void registration(UserData& newUser) {
    system("CLS");
    cout << "Write your new login: ";
    cin >> newUser.login;
    while (checkLogin(newUser.login)) {
        cout << "This login already exists!" << endl;
        cin >> newUser.login;
    }
    cout << "Write your new password: ";
    cin >> newUser.password;
    cout << "Write your fullname: ";
    cin.ignore(); // clear a cache of cins
    getline(cin, newUser.fullname);
    cout << "Write your phone number (without country code): ";
    cin >> newUser.phoneNumber;
    cout << "Write your PESEL: ";
    cin >> newUser.pesel;
}

void saveHistory(const UserData& userData) {
    file.open(R"(C:\Users\migga\Documents\PJATK\! Clinic database\historyOfVisits.txt)", ios::app);
    file << "Visit history of " << userData.login << ":\n";
    // create a loop with iteration over each element in user's visits (doctor, time, problem)
    for (auto& visit : userData.visits) { // variable with auto type to automatically choose variable type
        file << "Doctor: " << visit.doctorname << " | Time: " << visit.time << " | Problem: " << visit.problem << endl;
    }
    file << "-----------------------------------------------\n";
    file.close();
}

void showHistory(string& login){
    file.open(R"(C:\Users\migga\Documents\PJATK\! Clinic database\historyOfVisits.txt)");
    string line;
    bool found = false;
    while (getline(file, line)) {
        if (line.find("Visit history of " + login) != string::npos) { // find a line with user's login
            found = true;
            while (getline(file, line) && line.find("-----------------------------------------------") == string::npos) {
                cout << line << endl; // print details of visit
            }
        }
    }
    if (!found) {
        cout << "You don't have any visits yet." << endl;
    }
    file.close();
}

void scheduleVisit(UserData& userData, const Doctor& fahrenheit, const Doctor& somebodies, const Doctor& nowak) {
    string userCase;
    Doctor doctorInfo;
    cout << "Available hours for Dr. " << fahrenheit.fullname << ": ";
    for (auto& time : fahrenheit.time) { // printing a timetable of doctor's hours
        cout << time << " ";
    }
    cout << endl;
    cout << "Available hours for Dr. " << somebodies.fullname << ": ";
    for (auto& time : somebodies.time) {
        cout << time << " ";
    }
    cout << endl;
    cout << "Available hours for Dr. " << nowak.fullname << ": ";
    for (auto& time : nowak.time) {
        cout << time << " ";
    }
    cout << endl;
    cout << "Choose a time (hh:mm): ";
    cin.ignore();
    getline(cin, userCase);
    bool timeFound = false;
    if (find(begin(fahrenheit.time), end(fahrenheit.time), userCase) != end(fahrenheit.time)) {
        // find a time until iterator "find" won't come to the end iterator of the vector
        doctorInfo = fahrenheit;
        timeFound = true;
    } else if (find(begin(somebodies.time), end(somebodies.time), userCase) != end(somebodies.time)) {
        doctorInfo = somebodies;
        timeFound = true;
    } else if (find(begin(nowak.time), end(nowak.time), userCase) != end(nowak.time)) {
        doctorInfo = nowak;
        timeFound = true;
    }

    cout << "Write shortly reason of visit: ";
    Visit visit;
    getline(cin, visit.problem);

    if (timeFound) {
        visit.doctorname = doctorInfo.fullname;
        visit.time = userCase;
        userData.visits.push_back(visit); // push the info about user's visit to the vector
        cout << "Appointment scheduled with Dr. " << doctorInfo.fullname << " at " << userCase << endl;
        RemoveTime(doctorInfo, userCase);
        saveHistory(userData);
    } else {
        cout << "Time is not found for any doctor. Please make sure you entered a valid time\n";
    }
}

void logInSignUp(UserData& enterUser) {
    string userLogin, userPassword;
    int userCase;
    cout << "1 - log in\n"
         << "2 - sign up\n";
    cin >> userCase;
    switch (userCase) {
        case 1: {
            do {
                system("CLS");
                cout << "Login: ";
                cin >> userLogin;
                cout << "Password: ";
                cin >> userPassword;
                cout << endl;
                if (checkData(userLogin, userPassword)) {
                    // check if login and password provided are written in the file
                    cout << "You were logged in!" << endl;
                    enterUser.login = userLogin; // save info
                    enterUser.password = userPassword;
                } else {
                    cout << "Login or password isn't correct! Try again" << endl;
                    Sleep(2000);
                }
            } while (!checkData(userLogin, userPassword));
            break;
        }
        case 2: {
            registration(enterUser);
            saveData(enterUser);
            break;
        }
        default:
            break;
    }
}

void showInfo(const string& login, const string& password) {
    file.open(R"(C:\Users\migga\Documents\PJATK\! Clinic database\database.txt)");
    string line;

    while (getline(file, line)) {
        size_t loginPos = line.find("Login: " + login); // define a position of login
        size_t passwordPos = line.find("Password: " + password);
        if (loginPos != string::npos && passwordPos != string::npos) {
            size_t pos = line.find("Login: ") + 7;  // 7 is the length of "Login: "
            size_t endPos = line.find(" | Password: ");
            string extractedLogin = line.substr(pos, endPos - pos); // extract password and print only login
            cout << "Login: " << extractedLogin << endl;
            getline(file, line);
            cout << line << endl;  // print fullname
            getline(file, line);
            cout << line << endl;  // print Phone number
            getline(file, line);
            cout << line << endl;  // print PESEL
            break;
        }
    }
    file.close();
}

void RemoveTime(Doctor& doctor1, const string& userTypedHour) {
    doctor1.time.erase(
            remove_if(
                    doctor1.time.begin(),
                    doctor1.time.end(),
                    [userTypedHour](const string &doctorHour) {
                        return doctorHour == userTypedHour;
                    }
            ),
            doctor1.time.end()
    );
}