#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include <ctime>
#include <conio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>

#define RESET "\033[0m"
#define WHITE "\033[37m"
#define RED   "\033[31m"
#define GREEN "\033[32m"
#define BLACK "\033[30m"

using namespace std;

const string USER_FILE = "users.txt";
const int MAX_USERNAME_LENGTH = 20;
const int MIN_PASSWORD_LENGTH = 5;

class UserManager {
public:
    struct User {
        string username;
        string password;
    };

    vector<User> users;

    UserManager() {
        loadUsers();
    }

    void loadUsers() {
        ifstream file(USER_FILE);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                size_t userPos = line.find("Username : ");
                size_t passPos = line.find(" , Password : ");
                if (userPos != string::npos && passPos != string::npos) {
                    User user;
                    user.username = line.substr(userPos + 11, passPos - (userPos + 11));
                    user.password = line.substr(passPos + 14);
                    users.push_back(user);
                }
            }
            file.close();
        }
    }

    void saveUsers() {
        ofstream file(USER_FILE);
        if (file.is_open()) {
            for (const auto& user : users) {
                file << "Username : " << user.username << " , Password : " << user.password << "\n";
            }
            file.close();
        }
    }

    bool usernameExists(const string& username) {
        for (const auto& user : users) {
            if (user.username == username) {
                return true;
            }
        }
        return false;
    }

    bool registerUser() {
        while (true) {
            system("cls");
            cout << "\n\n\n\t\t\t\t       Sign Up\n";
            cout << "\t\t---------------------------------------------------\n";
    
            string username, password;
            char ch;
    
            // Username Input
            cout << "\t\tEnter your Username (max " << MAX_USERNAME_LENGTH << " chars) : ";
            while ((ch = _getch()) != '\r') {
                if (ch == '\b') {
                    if (!username.empty()) {
                        username.pop_back();
                        cout << "\b \b";
                    }
                }
                else if (username.size() < MAX_USERNAME_LENGTH && isprint(ch)) {
                    username.push_back(ch);
                    cout << ch;
                }
            }
    
            cout << "\n\t\tEnter your Password (min " << MIN_PASSWORD_LENGTH << " chars) : ";
            while ((ch = _getch()) != '\r') {
                if (ch == '\b') {
                    if (!password.empty()) {
                        password.pop_back();
                        cout << "\b \b";
                    }
                }
                else if (password.size() < MAX_USERNAME_LENGTH && isprint(ch)) {
                    password.push_back(ch);
                    cout << "*";
                }
            }
    
            if (password.length() < MIN_PASSWORD_LENGTH) {
                cout << "\n\t\tPassword must be at least " << MIN_PASSWORD_LENGTH << " characters!\n";
                Sleep(2000);
                continue;
            }
    
            if (usernameExists(username)) {
                cout << "\n\n\t\tUsername already exists! Please try another one.\n";
                cout << "\n\t\tPress any key to try again or ESC to cancel...";
                
                ch = _getch();
                if (ch == 27) {
                    return false;
                }
                continue;
            }
            
            users.push_back({username, password});
            saveUsers();
            cout << "\n\t\tRegistration successful! You can now login.\n";
            Sleep(2000);
            return true;
        }
    }

    bool loginUser(string& loggedInUsername) {
        system("cls");
        cout << "\n\n\n\t\t\t\t       Login \n";
        cout << "\t\t---------------------------------------------------\n";

        string username, password;
        char ch;

        cout << "\t\tEnter your Username: ";
        while ((ch = _getch()) != '\r') {
            if (ch == '\b') {
                if (!username.empty()) {
                    username.pop_back();
                    cout << "\b \b";
                }
            } 
            else if (username.size() < MAX_USERNAME_LENGTH && isprint(ch)) {
                username.push_back(ch);
                cout << ch;
            }
        }

        cout << "\n\t\tEnter your Password: ";
        while ((ch = _getch()) != '\r') {
            if (ch == '\b') {
                if (!password.empty()) {
                    password.pop_back();
                    cout << "\b \b";
                }
            }
            else if (password.size() < MAX_USERNAME_LENGTH && isprint(ch)) {
                password.push_back(ch);
                cout << "*";
            }
        }

        for (const auto& user : users) {
            if (user.username == username && user.password == password) {
                loggedInUsername = username;
                return true;
            }
        }

        cout << "\n\t\tInvalid username or password!\n";
        Sleep(2000);
        return false;
    }
};

class AgeCalculator {
public:
    int current_date, current_month, current_year;
    int birth_day, birth_month, birth_year;
    int age_date, age_month, age_year;

    void current_time() {
        time_t t = time(NULL);
        struct tm time = *localtime(&t);
        current_year = time.tm_year + 1900;
        current_month = time.tm_mon + 1;
        current_date = time.tm_mday;
        cout << "\n\t\t";
        cout << "         The current time : " << (current_date < 10 ? "0" : "") << current_date << "/" 
             << (current_month < 10 ? "0" : "") << current_month << "/" << current_year << "  ";
        set_reminder();
    }

    void set_reminder() {
        time_t now = time(0);
        tm* endtime = localtime(&now);
        cout << asctime(endtime) << endl;
    }

    void birth_age_info() {
        cout << "\t\t\t\t\t Welcome To Age Calculator : \n";
        cout << "\t\t\t     --------------------------------------------------\n";
        cout << "\n\t\t\t\t\t  Enter your birth day : ";
        while (!(cin >> birth_day) || birth_day < 1 || birth_day > 31) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\t\t\t\t\t  Invalid day! Enter again (1-31): ";
        }
        cout << "\t\t\t\t\t  Enter your birth month : ";
        while (!(cin >> birth_month) || birth_month < 1 || birth_month > 12) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\t\t\t\t\t  Invalid month! Enter again (1-12): ";
        }
        cout << "\t\t\t\t\t  Enter your birth year : ";
        while (!(cin >> birth_year) || birth_year < 1900 || birth_year > current_year) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\t\t\t\t\t  Invalid year! Enter again (1900-" << current_year << "): ";
        }
    }

    void Calculate_Age() {
        age_date = current_date - birth_day;
        age_month = current_month - birth_month;
        age_year = current_year - birth_year;

        if (age_date < 0) {
            age_month--;
            int daysInPrevMonth;
            if (current_month == 1) {
                daysInPrevMonth = 31; 
            }
            else {
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                tm.tm_mon = current_month - 2; 
                tm.tm_mday = 1; 
                mktime(&tm);
                daysInPrevMonth = 31 - tm.tm_mday;
            }
            age_date += daysInPrevMonth;
        }

        if (age_month < 0) {
            age_year--;
            age_month += 12;
        }

        cout << "\n\t\t\t\t==) Your Age is : " << age_year << " Years Old, " 
             << age_month << " Months, " << age_date << " Days." << endl;
        cout << "\n\t\t\t   ----------------------------------------------------------\n";
    }

    void birthday() {
        Sleep(1000);
        system("cls");
        cout << "\n\t\t";
        current_time();
        cout << "\n\t\t\t\t\t        From : *****...";
        cout << "\n\t\t\t\t\t        To : " << endl;
        char h = 2;
        cout << "\n\t\t       ";
        for (int i = 0; i < 59; i++) {
            cout << "*";
        }
        cout << "\n";
        Sleep(1000);
        system("color A");
        cout << "\t\t\t\t             i   i   i   i   i\n";
        Sleep(1000);
        system("color 7");
        cout << "\t\t\t\t             i   i   i   i   i\n";
        Sleep(1000);
        system("color A");
        cout << "\t\t\t\t          ___i___i___i___i___i___\n";
        Sleep(1000);
        system("color 7");
        cout << "\t\t\t\t         | ** FREE  PALESTINE ** |\n";
        Sleep(1000);
        system("color A");
        cout << "\t\t\t\t      ___| " << h << " **  " << h << "  22 / 08  " << h << "  ** " << h << " |___\n";
        Sleep(1000);
        system("color 7");
        cout << "\t\t\t\t     | " << h << " **** " << h << " *** " << h << "  ***  " << h << " *** " << h << " **** " << h << " |\n";
        Sleep(1000);
        system("color A");
        cout << "\t\t\t\t  ___| " << h << " *** " << h << " ** " << h << "   HAPPY   " << h << " ** " << h << " *** " << h << " |___\n";
        Sleep(1000);
        system("color 7");
        cout << "\t\t\t\t | " << h << " *** " << h << " ** " << h << "      BIRTHDAY     " << h << " ** " << h << " *** " << h << " |\n";
        Sleep(1000);
        system("color A");
        cout << "\t\t\t      ___| " << h << " **** " << h << " *** " << h << "     SALAH     " << h << " *** " << h << " **** " << h << " |___\n";
        Sleep(1000);
        system("color 7");
        cout << "\t\t\t     | " << h << "  **** " << h << " *** " << h << " ** " << h << "  B** *5 CS  " << h << " ** " << h << " *** " << h << " ****  " << h << " |\n";
        Sleep(1000);
        system("color A");
        cout << "\t\t\t  ___| ***   *   *** _________________ ***   *   *** |___\n";
        Sleep(1000);
        system("color 7");
        cout << "\t\t\t | " << h << "   *****  " << h << "  ****  " << h << " *** " << h << "  ***  " << h << " *** " << h << "  ****  " << h << "  *****   " << h << " |\n";
        Sleep(1000);
        system("color A");
        cout << "\t\t\t |_______________________________________________________|\n";
    }

    void free_palestine() {
        cout << "\n\n";
        Sleep(1000);
        cout << RED << "\t\t\t\t\t |" << BLACK << "|||||||||||||||||||||||||||\n";
        Sleep(1000);
        cout << RED << "\t\t\t\t\t ||" << BLACK << "||||||||||||||||||||||||||\n";
        Sleep(1000);
        cout << RED << "\t\t\t\t\t |||" << BLACK << "|||||||||||||||||||||||||\n";
        Sleep(1000);
        cout << RED << "\t\t\t\t\t ||||" << WHITE << "||||||||||||||||||||||||\n";
        Sleep(1000);
        cout << RED << "\t\t\t\t\t |||||" << WHITE << "|||||||||||||||||||||||\n";
        Sleep(1000);
        cout << RED << "\t\t\t\t\t ||||" << WHITE << "||||||||||||||||||||||||\n";
        Sleep(1000);
        cout << RED << "\t\t\t\t\t |||" << GREEN << "|||||||||||||||||||||||||\n";
        Sleep(1000);
        cout << RED << "\t\t\t\t\t ||" << GREEN << "||||||||||||||||||||||||||\n";
        Sleep(1000);
        cout << RED << "\t\t\t\t\t |" << GREEN << "|||||||||||||||||||||||||||\n";
        cout << "\n";
        Sleep(1000);
        char sml = 3;
        cout << GREEN << "\t\t\t\t\t    FREE" << BLACK << " PAL" << WHITE << "EST" << RED << "INE " << GREEN << sml << " " << BLACK << sml << " " << WHITE << sml << " " << RED << sml;
    }
};

int main() {
    UserManager userManager;
    string loggedInUsername;
    AgeCalculator ac;
    
    while (true) {
        system("cls");
        system("color A");
        cout << "\n\n\n\t\t\t\tWELCOME TO AGE CALCULATOR\n";
        cout << "\t\t---------------------------------------------------\n";
        cout << "\t\t\t\t1. Sign Up\n";
        cout << "\t\t\t\t2. Login \n";
        cout << "\t\t\t\t3. Exit\n";
        cout << "\t\t---------------------------------------------------\n";
        cout << "\t\tEnter your choice : ";

        char choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case '1':
            userManager.registerUser();
            break;
        case '2':
            if (userManager.loginUser(loggedInUsername)) {
                goto AgeCalculator;
            }
            break;
        case '3':
            return 0;
        default:
            cout << "\t\tInvalid choice! Please try again.\n";
            Sleep(1000);
            break;
        }
    }

AgeCalculator:
    ac.current_time();
    ac.birth_age_info();
    ac.Calculate_Age();
    Sleep(1000);
    ac.birthday();
    cout << RED << "\n\t\t\t\t\t     WAIT WAIT......!!!!";
    Sleep(1000);
    ac.free_palestine();
    return 0;
}