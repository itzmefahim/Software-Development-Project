#include <iostream>
#include <string>
#include <unordered_set>
#include <thread>
#include <chrono>
using namespace std;

unordered_set<string> registeredEmails;
unordered_set<string> registeredContacts;

const string fixedUsername = "admin";
const string fixedPassword = "admin123";

bool isPasswordValid(const string& password, const string& confirmPassword) {
    return password == confirmPassword;
}

bool isUniqueRegistration(const string& email, const string& contact) {
    return registeredEmails.find(email) == registeredEmails.end() && 
           registeredContacts.find(contact) == registeredContacts.end();
}

void login() {
    string username, password;
    int attempts = 0;

    while (attempts < 3) {
        cout << "\nEnter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        if (username == fixedUsername && password == fixedPassword) {
            cout << "\nLogin Successful!\n";
            return;
        } else {
            cout << "\nIncorrect Username or Password. Try again.\n";
            attempts++;
        }
    }
    
    cout << "\nToo many failed attempts. Locking for 30 seconds...\n";
    this_thread::sleep_for(chrono::seconds(30));
    cout << "\nYou can try logging in again.\n";
}

void registerUser() {
    string name, email, contact, address, nid, password, confirmPassword;

    cout << "Enter your Name: ";
    getline(cin, name);

    cout << "Enter your Email: ";
    getline(cin, email);

    cout << "Enter your Contact Number: ";
    getline(cin, contact);

    // Check if email or contact is already registered
    if (!isUniqueRegistration(email, contact)) {
        cout << "\nThis email or contact number is already registered. Please use a different one.\n";
        return;
    }

    cout << "Enter your Address: ";
    getline(cin, address);

    cout << "Enter your National ID (NID): ";
    getline(cin, nid);

    cout << "Enter your Password: ";
    getline(cin, password);

    cout << "Confirm your Password: ";
    getline(cin, confirmPassword);

    // Validate password
    if (isPasswordValid(password, confirmPassword)) {
        // Store email and contact as registered
        registeredEmails.insert(email);
        registeredContacts.insert(contact);

        cout << "\nRegistration Successful\n";
        cout << "Name: " << name << endl;
        cout << "Email: " << email << endl;
        cout << "Contact: " << contact << endl;
        cout << "Address: " << address << endl;
        cout << "NID: " << nid << endl;
    } else {
        cout << "\nPassword mismatch! Please try again.\n";
    }
}

int main() {
    cout << "\nWelcome to Swift Book. Fastest Online Ticket Reservation System\n";
    
    while (true) {
        cout << "\nChoose an option:\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore(); // To handle newline character

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                login();
                break;
            case 3:
                cout << "\nThank you for using Swift Book. Goodbye!\n";
                return 0;
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }
    }
}
