//============================================================================
// Name        : LinkedList.cpp
// Author      : Eleen Limon
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Lab 3-2 Lists and Searching
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>
#include "CSVparser.hpp" // Include library for CSV parsing

using namespace std;

//============================================================================
// Utility Function Prototypes
//============================================================================

// Convert a string to a double, removing a specific character
double strToDouble(string str, char ch);

//============================================================================
// Structure Definition: Bid
//============================================================================

// Structure to hold bid information
struct Bid {
    string bidId;  // unique identifier for a bid
    string title;  // title of the bid
    string fund;   // funding source for the bid
    double amount; // amount of the bid

    // Default constructor initializes amount to 0.0
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Linked-List Class Definition
//============================================================================

class LinkedList {
private:
    // Internal node structure representing each bid in the list
    struct Node {
        Bid bid;
        Node* next;

        // Default constructor initializes `next` to nullptr
        Node() {
            next = nullptr;
        }

        // Parameterized constructor to initialize a node with a bid
        Node(Bid aBid) {
            bid = aBid;
            next = nullptr;
        }
    };

    Node* head;  // Pointer to the first element in the list
    Node* tail;  // Pointer to the last element in the list
    int size;    // Number of elements in the list

public:
    // Constructor
    LinkedList();
    // Destructor
    virtual ~LinkedList();

    // Add a bid to the end of the list
    void Append(Bid bid);
    // Add a bid to the start of the list
    void Prepend(Bid bid);
    // Print all bids in the list
    void PrintList();
    // Remove a bid by its ID
    void Remove(string bidId);
    // Search for a bid by its ID
    Bid Search(string bidId);
    // Get the current size of the list
    int Size();
};

//============================================================================
// LinkedList Method Definitions
//============================================================================

// Constructor initializes an empty list
LinkedList::LinkedList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// Destructor to free memory
LinkedList::~LinkedList() {
    Node* current = head;
    Node* temp = nullptr;

    // Traverse the list and delete each node
    while (current != nullptr) {
        temp = current;
        current = current->next;
        delete temp;
    }
}

// Append a new bid to the end of the list
void LinkedList::Append(Bid bid) {
    Node* newNode = new Node(bid); // Create a new node with the given bid

    if (head == nullptr) { // If list is empty
        head = newNode;
        tail = newNode;
    } else { // Otherwise, append to the end
        tail->next = newNode;
        tail = newNode;
    }
    size++; // Increment list size
}

// Prepend a new bid to the start of the list
void LinkedList::Prepend(Bid bid) {
    Node* newNode = new Node(bid); // Create a new node with the given bid

    if (head != nullptr) { // If the list is not empty, link new node to current head
        newNode->next = head;
    }
    head = newNode; // New node becomes the head

    if (tail == nullptr) { // If the list was empty, set tail to the new node
        tail = newNode;
    }
    size++; // Increment list size
}

// Print all bids in the list
void LinkedList::PrintList() {
    Node* current = head;

    while (current != nullptr) {
        cout << current->bid.bidId << ": " << current->bid.title 
             << " | " << current->bid.amount << " | " << current->bid.fund << endl;
        current = current->next; // Move to the next node
    }
}

// Remove a bid by its ID
void LinkedList::Remove(string bidId) {
    if (head == nullptr) { // List is empty
        cout << "List is empty. No bids to remove." << endl;
        return;
    }

    // Special case: Removing the head node
    if (head->bid.bidId == bidId) {
        Node* temp = head;
        head = head->next;
        delete temp;
        size--;
        cout << "Bid " << bidId << " removed successfully." << endl;
        return;
    }

    // General case: Traverse and find the bid to remove
    Node* current = head;
    while (current->next != nullptr) {
        if (current->next->bid.bidId == bidId) {
            Node* temp = current->next;
            current->next = current->next->next;
            delete temp;
            size--;
            cout << "Bid " << bidId << " removed successfully." << endl;
            return;
        }
        current = current->next; // Move to the next node
    }

    // If we reach here, bid was not found
    cout << "Bid " << bidId << " not found." << endl;
}

// Search for a bid by its ID
Bid LinkedList::Search(string bidId) {
    Node* current = head;

    while (current != nullptr) {
        if (current->bid.bidId == bidId) {
            return current->bid; // Return the found bid
        }
        current = current->next; // Move to the next node
    }
    return Bid(); // Return an empty bid if not found
}

// Return the current size of the list
int LinkedList::Size() {
    return size;
}

//============================================================================
// Utility Functions
//============================================================================

// Function to display a bid
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | " << bid.fund << endl;
}

// Function to capture user input and return a Bid
Bid getBid() {
    Bid bid;
    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);
    cout << "Enter title: ";
    getline(cin, bid.title);
    cout << "Enter fund: ";
    cin >> bid.fund;
    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');
    return bid;
}

// Function to load bids from a CSV file and append them to the list
void loadBids(string csvPath, LinkedList* list) {
    cout << "Loading CSV file " << csvPath << endl;

    csv::Parser file = csv::Parser(csvPath); // Use CSV parser library

    try {
        for (int i = 0; i < file.rowCount(); i++) {
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');
            list->Append(bid);
        }
    } catch (csv::Error& e) {
        cerr << e.what() << endl; // Catch CSV parsing errors
    }
}

// Convert string to double and remove specified character
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end()); // Remove unwanted character
    return atof(str.c_str()); // Convert string to double
}

//============================================================================
// Main Function
//============================================================================
int main(int argc, char* argv[]) {
    string csvPath, bidKey;

    // Parse command-line arguments
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109"; // Default bid key
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2]; // User-specified bid key
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv"; // Default CSV file path
        bidKey = "98109"; // Default bid key
    }

    clock_t ticks;  // Clock ticks for performance measurement
    LinkedList bidList;  // Create a LinkedList to hold bids
    Bid bid;
    int choice = 0;  // Menu choice

    // Main program loop
    while (choice != 9) {
        // Display menu
        cout << "Menu:" << endl;
        cout << "  1. Enter a Bid" << endl;
        cout << "  2. Load Bids" << endl;
        cout << "  3. Display All Bids" << endl;
        cout << "  4. Find Bid" << endl;
        cout << "  5. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        // Process user input
        switch (choice) {
        case 1:  // Enter a new bid
            bid = getBid();
            bidList.Append(bid);
            displayBid(bid);
            break;

        case 2:  // Load bids from CSV
            ticks = clock();
            loadBids(csvPath, &bidList);
            cout << bidList.Size() << " bids read" << endl;
            ticks = clock() - ticks;
            cout << "time: " << ticks << " milliseconds" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 3:  // Display all bids
            bidList.PrintList();
            break;

        case 4:  // Find a bid by its ID
            ticks = clock();
            bid = bidList.Search(bidKey);
            ticks = clock() - ticks;
            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }
            cout << "time: " << ticks << " milliseconds" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 5:  // Remove a bid by its ID
            bidList.Remove(bidKey);
            break;
        }
    }

    cout << "Goodbye." << endl;
    return 0;
}

