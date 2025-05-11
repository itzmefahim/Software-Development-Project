#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

// Structures
struct User {
    string username, email, contact, nidNo, address, password;
    bool isAdmin;
};

struct Route {
    int id;
    string type; // Bus, Train, Airline
    string origin, destination;
    string time;
    int totalSeats, availableSeats;
    double price;
};

struct Booking {
    int bookingId;
    string username;
    int routeId;
    int seatNumber;
};

// Function Prototypes
void registerUser();
bool loginUser(string &username, bool &isAdmin);
bool userExists(const string &username); // New function to check for duplicate usernames
void addRoute();
void initializeRoutes();
void viewRoutes();
void bookTicket(const string &username);
void cancelTicket(const string &username);
void viewTicket(const string &username);
void saveUser(const User &user);
void saveRoute(const Route &route);
void saveBooking(const Booking &booking);
vector<Route> loadRoutes();
vector<Booking> loadBookings();
int generateBookingId();
bool routeExists(const string &type, const string &origin, const string &destination);

// Global Variables
const string USER_FILE = "users.txt";
const string ROUTE_FILE = "routes.txt";
const string BOOKING_FILE = "bookings.txt";

int main() {
    string username;
    bool isAdmin;
    int choice;

    // Initialize predefined routes at program start
    initializeRoutes();

    while (true) {
        cout << "\n=== Online Ticket Reservation System (Swift Book)===\n";
        cout << "1. Register\n2. Login\n3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            registerUser();
        } else if (choice == 2) {
            if (loginUser(username, isAdmin)) {
                while (true) {
                    cout << "\n=== " << (isAdmin ? "Admin" : "User") << " Menu ===\n";
                    if (isAdmin) {
                        cout << "1. Add Route\n2. View Routes\n3. Exit\n";
                    } else {
                        cout << "1. View Routes\n2. Book Ticket\n3. Cancel Ticket\n4. View Tickets\n5. Exit\n";
                    }
                    cout << "Enter choice: ";
                    cin >> choice;
                    cin.ignore();

                    if (isAdmin) {
                        if (choice == 1) addRoute();
                        else if (choice == 2) viewRoutes();
                        else if (choice == 3) break;
                    } else {
                        if (choice == 1) viewRoutes();
                        else if (choice == 2) bookTicket(username);
                        else if (choice == 3) cancelTicket(username);
                        else if (choice == 4) viewTicket(username);
                        else if (choice == 5) break;
                    }
                }
            }
        } else if (choice == 3) {
            cout << "Goodbye!\n";
            break;
        }
    }
    return 0;
}

void registerUser() {
    User user;
    string confirmPassword;

    cout << "Enter username: ";
    getline(cin, user.username);
    if (userExists(user.username)) {
        cout << "Username already exists! Please choose another.\n";
        return;
    }

    cout << "Enter email: ";
    getline(cin, user.email);
    cout << "Enter contact number: ";
    getline(cin, user.contact);
    cout << "Enter NID number: ";
    getline(cin, user.nidNo);
    cout << "Enter address: ";
    getline(cin, user.address);
    cout << "Enter password: ";
    getline(cin, user.password);
    cout << "Confirm password: ";
    getline(cin, confirmPassword);

    if (user.password != confirmPassword) {
        cout << "Passwords do not match!\n";
        return;
    }

    user.isAdmin = false; // Default: not admin
    saveUser(user);
    cout << "Registration successful!\n";
}

bool loginUser(string &username, bool &isAdmin) {
    string password;
    cout << "Enter username: ";
    getline(cin, username);
    cout << "Enter password: ";
    getline(cin, password);

    ifstream file(USER_FILE);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        User u;
        string temp;
        getline(ss, u.username, ',');
        getline(ss, u.email, ',');
        getline(ss, u.contact, ',');
        getline(ss, u.nidNo, ',');
        getline(ss, u.address, ',');
        getline(ss, u.password, ',');
        ss >> u.isAdmin;
        if (u.username == username && u.password == password) {
            isAdmin = u.isAdmin;
            file.close();
            cout << "Login successful!\n";
            return true;
        }
    }
    file.close();
    cout << "Invalid credentials!\n";
    return false;
}

bool userExists(const string &username) {
    ifstream file(USER_FILE);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string u;
        getline(ss, u, ',');
        if (u == username) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

void addRoute() {
    Route route;
    route.id = loadRoutes().size() + 1;
    cout << "Enter transport type (Bus/Train/Airline): ";
    getline(cin, route.type);
    cout << "Enter origin: ";
    getline(cin, route.origin);
    cout << "Enter destination: ";
    getline(cin, route.destination);

    if (routeExists(route.type, route.origin, route.destination)) {
        cout << "Route already exists!\n";
        return;
    }

    cout << "Enter time (e.g., 14:30): ";
    getline(cin, route.time);
    cout << "Enter total seats: ";
    cin >> route.totalSeats;
    route.availableSeats = route.totalSeats;
    cout << "Enter price: ";
    cin >> route.price;
    cin.ignore();
    saveRoute(route);
    cout << "Route added!\n";
}

void initializeRoutes() {
    vector<Route> routes = loadRoutes();
    vector<pair<string, string>> routeList = {
        {"Dhaka", "Chittagong"},
        {"Dhaka", "Cox's Bazar"},
        {"Dhaka", "Sylhet"},
        {"Dhaka", "Rajshahi"},
        {"Dhaka", "Jashore"},
        {"Dhaka", "Khulna"},
        {"Dhaka", "Rangpur"},
        {"Jashore", "Rajshahi"},
        {"Jashore", "Dhaka"},
        {"Sylhet", "Dhaka"},
        {"Rajshahi", "Dhaka"},
        {"Cox's Bazar", "Dhaka"},
        {"Chittagong", "Dhaka"}
    };

    vector<string> types = {"Bus", "Train", "Airline"};
    int id = routes.size() + 1;

    for (const auto &type : types) {
        for (const auto &route : routeList) {
            string origin = route.first;
            string destination = route.second;

            if (routeExists(type, origin, destination)) {
                continue;
            }

            Route r;
            r.id = id++;
            r.type = type;
            r.origin = origin;
            r.destination = destination;
            r.time = (type == "Airline") ? "10:00" : "08:00";
            r.totalSeats = (type == "Airline") ? 100 : 50;
            r.availableSeats = r.totalSeats;
            r.price = (type == "Airline") ? 5000.0 : (type == "Train") ? 500.0 : 300.0;
            saveRoute(r);
        }
    }
}

bool routeExists(const string &type, const string &origin, const string &destination) {
    vector<Route> routes = loadRoutes();
    for (const auto &r : routes) {
        if (r.type == type && r.origin == origin && r.destination == destination) {
            return true;
        }
    }
    return false;
}

void viewRoutes() {
    vector<Route> routes = loadRoutes();
    if (routes.empty()) {
        cout << "No routes available.\n";
        return;
    }
    cout << "\n=== Available Routes ===\n";
    for (const auto &r : routes) {
        cout << "ID: " << r.id << ", Type: " << r.type
             << ", Route: " << r.origin << " to " << r.destination
             << ", Time: " << r.time << ", Seats: " << r.availableSeats
             << "/" << r.totalSeats << ", Price: $" << r.price << "\n";
    }
}

void bookTicket(const string &username) {
    viewRoutes();
    int routeId, seat;
    cout << "Enter route ID to book: ";
    cin >> routeId;
    cout << "Enter seat number (1-" << loadRoutes()[routeId-1].totalSeats << "): ";
    cin >> seat;
    cin.ignore();

    vector<Route> routes = loadRoutes();
    vector<Booking> bookings = loadBookings();

    if (routeId < 1 || routeId > routes.size()) {
        cout << "Invalid route ID!\n";
        return;
    }
    Route &route = routes[routeId-1];
    if (seat < 1 || seat > route.totalSeats) {
        cout << "Invalid seat number!\n";
        return;
    }
    for (const auto &b : bookings) {
        if (b.routeId == routeId && b.seatNumber == seat) {
            cout << "Seat already booked!\n";
            return;
        }
    }
    if (route.availableSeats == 0) {
        cout << "No seats available!\n";
        return;
    }

    Booking booking;
    booking.bookingId = generateBookingId();
    booking.username = username;
    booking.routeId = routeId;
    booking.seatNumber = seat;
    saveBooking(booking);

    route.availableSeats--;
    ofstream file(ROUTE_FILE, ios::trunc);
    for (const auto &r : routes) {
        file << r.id << "," << r.type << "," << r.origin << ","
             << r.destination << "," << r.time << "," << r.totalSeats << ","
             << r.availableSeats << "," << r.price << "\n";
    }
    file.close();
    cout << "Ticket booked! Booking ID: " << booking.bookingId << "\n";
}

void cancelTicket(const string &username) {
    vector<Booking> bookings = loadBookings();
    int bookingId;
    cout << "Enter booking ID to cancel: ";
    cin >> bookingId;
    cin.ignore();

    for (auto it = bookings.begin(); it != bookings.end(); ++it) {
        if (it->bookingId == bookingId && it->username == username) {
            vector<Route> routes = loadRoutes();
            Route &route = routes[it->routeId-1];
            route.availableSeats++;
            ofstream file(ROUTE_FILE, ios::trunc);
            for (const auto &r : routes) {
                file << r.id << "," << r.type << "," << r.origin << ","
                     << r.destination << "," << r.time << "," << r.totalSeats << ","
                     << r.availableSeats << "," << r.price << "\n";
            }
            file.close();
            bookings.erase(it);
            ofstream bookingFile(BOOKING_FILE, ios::trunc);
            for (const auto &b : bookings) {
                bookingFile << b.bookingId << "," << b.username << ","
                            << b.routeId << "," << b.seatNumber << "\n";
            }
            bookingFile.close();
            cout << "Booking canceled!\n";
            return;
        }
    }
    cout << "Booking not found!\n";
}

void viewTicket(const string &username) {
    vector<Booking> bookings = loadBookings();
    vector<Route> routes = loadRoutes();
    bool found = false;
    cout << "\n=== Your Tickets ===\n";
    for (const auto &b : bookings) {
        if (b.username == username) {
            found = true;
            Route r = routes[b.routeId-1];
            cout << "Booking ID: " << b.bookingId << ", Type: " << r.type
                 << ", Route: " << r.origin << " to " << r.destination
                 << ", Time: " << r.time << ", Seat: " << b.seatNumber
                 << ", Price: $" << r.price << "\n";
        }
    }
    if (!found) cout << "No tickets found.\n";
}

void saveUser(const User &user) {
    ofstream file(USER_FILE, ios::app);
    file << user.username << "," << user.email << "," << user.contact << ","
         << user.nidNo << "," << user.address << "," << user.password << ","
         << user.isAdmin << "\n";
    file.close();
}

void saveRoute(const Route &route) {
    ofstream file(ROUTE_FILE, ios::app);
    file << route.id << "," << route.type << "," << route.origin << ","
         << route.destination << "," << route.time << "," << route.totalSeats << ","
         << route.availableSeats << "," << route.price << "\n";
    file.close();
}

void saveBooking(const Booking &booking) {
    ofstream file(BOOKING_FILE, ios::app);
    file << booking.bookingId << "," << booking.username << ","
         << booking.routeId << "," << booking.seatNumber << "\n";
    file.close();
}

vector<Route> loadRoutes() {
    vector<Route> routes;
    ifstream file(ROUTE_FILE);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Route r;
        string temp;
        getline(ss, temp, ','); r.id = stoi(temp);
        getline(ss, r.type, ',');
        getline(ss, r.origin, ',');
        getline(ss, r.destination, ',');
        getline(ss, r.time, ',');
        getline(ss, temp, ','); r.totalSeats = stoi(temp);
        getline(ss, temp, ','); r.availableSeats = stoi(temp);
        getline(ss, temp); r.price = stod(temp);
        routes.push_back(r);
    }
    file.close();
    return routes;
}

vector<Booking> loadBookings() {
    
    vector<Booking> bookings;
    ifstream file(BOOKING_FILE);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Booking b;
        string temp;
        getline(ss, temp, ','); b.bookingId = stoi(temp);
        getline(ss, b.username, ',');
        getline(ss, temp, ','); b.routeId = stoi(temp);
        getline(ss, temp); b.seatNumber = stoi(temp);
        bookings.push_back(b);
    }
    file.close();
    return bookings;
}

int generateBookingId() {
    vector<Booking> bookings = loadBookings();
    return bookings.empty() ? 1 : bookings.back().bookingId + 1;
}
