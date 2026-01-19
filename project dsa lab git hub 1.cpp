#include <iostream>
#include <string>
#include <stack>
using namespace std;
struct Passenger {
    string name;
    int seatNumber;
    Passenger* next;
};
struct Bus {
    int busId;
    string route;
    int totalSeats;
    int availableSeats;
    Passenger* passengers;          
    Bus* left;
    Bus* right;
    stack<int> cancelledSeats;      
};
class BusReservationSystem {
private:
    Bus* root;
    Bus* insertBus(Bus* node, int id, string route, int seats) {
        if (node == NULL) {
            Bus* newBus = new Bus();
            newBus->busId = id;
            newBus->route = route;
            newBus->totalSeats = seats;
            newBus->availableSeats = seats;
            newBus->passengers = NULL;
            newBus->left = NULL;
            newBus->right = NULL;
            return newBus;
        }
        if (id < node->busId) {
            node->left = insertBus(node->left, id, route, seats);
        } else if (id > node->busId) {
            node->right = insertBus(node->right, id, route, seats);
        }
        return node;
    }

    Bus* searchBus(Bus* node, int id) {
        if (node == NULL || node->busId == id) {
            return node;
        }
        if (id < node->busId) {
            return searchBus(node->left, id);
        }
        return searchBus(node->right, id);
    }

    void displayAvailableBuses(Bus* node) {
        if (node != NULL) {
            displayAvailableBuses(node->left);
            if (node->availableSeats > 0 || !node->cancelledSeats.empty()) {
                cout << "Bus ID: " << node->busId << ", Route: " << node->route 
                     << ", Available Seats: " << (node->availableSeats + node->cancelledSeats.size()) << endl;
            }
            displayAvailableBuses(node->right);
        }
    }
    void displayPassengers(Bus* bus) {
        if (bus == NULL) return;
        Passenger* current = bus->passengers;
        cout << "Passengers for Bus " << bus->busId << ":\n";
        while (current != NULL) {
            cout << "Name: " << current->name << ", Seat: " << current->seatNumber << endl;
            current = current->next;
        }
    }
public:
    BusReservationSystem() : root(NULL) {}

    void addBus(int id, string route, int seats) {
        root = insertBus(root, id, route, seats);
        cout << "Bus added successfully.\n";
    }
    void displayAvailable() {
        if (root == NULL) {
            cout << "No buses available.\n";
            return;
        }
        displayAvailableBuses(root);
    }
    void bookTicket(int busId, string passengerName) {
        Bus* bus = searchBus(root, busId);
        if (bus == NULL) {
            cout << "Bus not found.\n";
            return;
        }
        int seat = -1;
        if (!bus->cancelledSeats.empty()) {
            seat = bus->cancelledSeats.top();
            bus->cancelledSeats.pop();
            cout << "Reallocated cancelled seat: " << seat << endl;
        } else if (bus->availableSeats > 0) {
            seat = bus->totalSeats - bus->availableSeats + 1;
            bus->availableSeats--;
        } else {
            cout << "No seats available.\n";
            return;
        }
        Passenger* newPassenger = new Passenger();
        newPassenger->name = passengerName;
        newPassenger->seatNumber = seat;
        newPassenger->next = bus->passengers;
        bus->passengers = newPassenger;

        cout << "Ticket booked for " << passengerName << " on seat " << seat << ".\n";
    }
void cancelTicket(int busId, string passengerName) {
        Bus* bus = searchBus(root, busId);
        if (bus == NULL) {
            cout << "Bus not found.\n";
            return;
        }
        Passenger* current = bus->passengers;
        Passenger* prev = NULL;
        bool found = false;

        while (current != NULL) {
            if (current->name == passengerName) {
                found = true;
                if (prev == NULL) {
                    bus->passengers = current->next;
                } else {
                    prev->next = current->next;
                }
                bus->cancelledSeats.push(current->seatNumber);
                delete current;
                cout << "Ticket cancelled successfully.\n";
                break;
            }
            prev = current;
            current = current->next;
        }
        if (!found) {
            cout << "Passenger not found on this bus.\n";
        }
    }
    void displayPassengerDetails(int busId) {
        Bus* bus = searchBus(root, busId);
        if (bus == NULL) {
            cout << "Bus not found.\n";
            return;
        }
        displayPassengers(bus);
    }
};
int main() {
    BusReservationSystem system;
    int choice;
    do {
        cout << "\n=== Bus Ticket Reservation System ===\n";
        cout << "1. Add new bus\n";
        cout << "2. Display available buses\n";
        cout << "3. Book ticket\n";
        cout << "4. Cancel ticket\n";
        cout << "5. Show passenger details of a bus\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(); 
