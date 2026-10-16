#ifndef MENUS_H
#define MENUS_H

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <limits>

using namespace std;

// --- MOCK DATA CLASSES (Để code của bạn biên dịch được) ---
class Invoice {
public:
    string getId() const { return "INV001"; }
    string getCustomerId() const { return "CUST001"; }
    void setStatus(const string& s) {}
    void inputNew() {}
    void updateFields() {}
    void displayRow() {}
};

class Payment {
public:
    string getId() const { return "PAY001"; }
    string getInvoiceId() const { return "INV001"; }
    string getStatus() const { return "Thanh cong"; }
    void inputNew() {}
    void updateFields() {}
    void displayRow() {}
};

template <typename T>
class Repository {
public:
    T* findById(const string& id) { return nullptr; }
    vector<T>& all() { static vector<T> v; return v; }
    void add(const T& item) {}
    void save() {}
    void removeById(const string& id) {}
    void showAll() { cout << "(Danh sach trong)\n"; }
};

struct AppData {
    Repository<Invoice> invoices;
    Repository<Payment> payments;
    struct {
        vector<int> all() { return {}; }
        bool findById(const string& id) { return false; }
    } customers;
};

// --- KHAI BÁO CÁC HÀM TIỆN ÍCH ---
inline int readChoice(const string& prompt, int minVal, int maxVal) {
    int choice;
    while (true) {
        cout << prompt;
        if (cin >> choice && choice >= minVal && choice <= maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }
        cout << "Loi! Nhap lai tu " << minVal << " den " << maxVal << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

inline string readLine(const string& prompt) {
    string line; cout << prompt; getline(cin, line); return line;
}

// --- KHAI BÁO CÁC MENU VÀ HÀM ---
int runApplication();
void syncInvoiceStatus(AppData& app, const string& invId); // Sửa lỗi UC8 không tìm thấy hàm này
void menuCustomers(AppData& app);
void menuRegistrations(AppData& app);
void menuPlans(AppData& app);
void menuPlanChanges(AppData& app);
void menuContracts(AppData& app);
void menuRenewals(AppData& app);
void menuIncidents(AppData& app);
void menuSupports(AppData& app);
void menuInvoices(AppData& app);
void menuPayments(AppData& app);

#endif