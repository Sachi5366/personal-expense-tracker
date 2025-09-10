#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <ctime>

using namespace std;

struct Transaction {
    string type;
    double amount;
    string category;
    string timestamp;
};

vector<Transaction> history;
double balance = 0.0;

string getCurrentTime() {
    time_t now = time(0);
    char* dt = ctime(&now);
    string timeStr(dt);
    timeStr.pop_back(); // remove newline
    return timeStr;
}

void saveTransaction(const Transaction& t) {
    ofstream file("expenses.txt", ios::app);
    if (file.is_open()) {
        file << t.type << "," << t.amount << "," << t.category << "," << t.timestamp << "\n";
        file.close();
    }
}

void loadTransactions() {
    ifstream file("expenses.txt");
    if (!file.is_open()) return;

    string type, category, timestamp;
    double amount;
    char comma;

    while (file >> ws && getline(file, type, ',') &&
           file >> amount >> comma &&
           getline(file, category, ',') &&
           getline(file, timestamp)) {
        Transaction t{type, amount, category, timestamp};
        history.push_back(t);
        if (type == "Income") balance += amount;
        else if (type == "Expense") balance -= amount;
    }

    file.close();
}

void addTransaction(const string& type) {
    Transaction t;
    t.type = type;
    cout << "Enter amount: ";
    cin >> t.amount;
    cin.ignore();
    cout << "Enter category (e.g., Food, Rent, Salary): ";
    getline(cin, t.category);
    t.timestamp = getCurrentTime();

    if (type == "Income") balance += t.amount;
    else balance -= t.amount;

    history.push_back(t);
    saveTransaction(t);

    cout << type << " recorded successfully!\n\n";
}

void showBalance() {
    cout << fixed << setprecision(2);
    cout << "Current Balance: ₹ " << balance << "\n\n";
}

void showHistory() {
    cout << "\n--- Transaction History ---\n";
    cout << left << setw(10) << "Type" << setw(10) << "Amount" << setw(15) << "Category" << "Date & Time\n";
    cout << "-----------------------------------------------------\n";
    for (const auto& t : history) {
        cout << left << setw(10) << t.type
             << "₹" << setw(8) << t.amount
             << setw(15) << t.category
             << t.timestamp << "\n";
    }
    cout << "-----------------------------------------------------\n\n";
}

int main() {
    loadTransactions();
    int choice;

    do {
        cout << "----- Personal Expense Tracker -----\n";
        cout << "1. Add Income\n2. Add Expense\n3. View Balance\n4. View Transaction History\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: addTransaction("Income"); break;
            case 2: addTransaction("Expense"); break;
            case 3: showBalance(); break;
            case 4: showHistory(); break;
            case 5: cout << "Exiting... Stay financially fit!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);

    return 0;
}
