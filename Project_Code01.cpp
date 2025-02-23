#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
using namespace std;

unordered_map<string, string> registeredUsers; // Email to Password mapping
unordered_set<string> registeredContacts;

const string fixedUsername = "admin";
const string fixedPassword = "admin123";

bool isPasswordValid(const string& password, const string& confirmPassword) {
    return password == confirmPassword;
}

bool isUniqueRegistration(const string& email, const string& contact) {
    return registeredUsers.find(email) == registeredUsers.end() &&
           registeredContacts.find(contact) == registeredContacts.end();
}

void loadUserData() {
    ifstream inputFile("users.txt");
    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string email, password, contact;
            if (getline(ss, email, ',') && getline(ss, password, ',') && getline(ss, contact)) {
                registeredUsers[email] = password;
                registeredContacts.insert(contact);
            }
        }
        inputFile.close();
    }
}

void saveUserData() {
    ofstream outputFile("users.txt");
    if (outputFile.is_open()) {
        for (const auto& pair : registeredUsers) {
            outputFile << pair.first << "," << pair.second << "," << *registeredContacts.find(pair.first) << endl;
        }
        outputFile.close();
    }
}

void login() {
    string email, password;
    int attempts = 0;

    while (attempts < 3) {
        cout << "\nEnter Email: ";
        cin >> email;
        cin.ignore();
        cout << "Enter Password: ";
        getline(cin, password);

        if (email == fixedUsername && password == fixedPassword) {
            cout << "\nLogin Successful! (Admin)\n";
            return;
        }

        if (registeredUsers.find(email) != registeredUsers.end() && registeredUsers[email] == password) {
            cout << "\nLogin Successful!\n";
            return;
        } else {
            cout << "\nIncorrect Email or Password. Try again.\n";
            attempts++;
        }
    }
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
        registeredUsers[email] = password;
        registeredContacts.insert(contact);

        cout << "\nRegistration Successful\n";
        cout << "Name: " << name << endl;
        cout << "Email: " << email << endl;
        cout << "Contact: " << contact << endl;
        cout << "Address: " << address << endl;
        cout << "NID: " << nid << endl;

        saveUserData(); // Save user data to file
    } else {
        cout << "\nPassword mismatch! Please try again.\n";
    }
}

int main() {
    loadUserData(); // Load user data from file

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
