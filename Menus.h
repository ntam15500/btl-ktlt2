#ifndef MENUS_H
#define MENUS_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdexcept>
#include <limits>
#include <sstream>
#include <cstdlib>

using namespace std;

inline int readChoice(const string& p, int mn, int mx) {
    int c;
    while (cout << p && (!(cin >> c) || c < mn || c > mx)) {
        cout << "Nhap lai (" << mn << "-" << mx << ").\n";
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return c;
}

inline string readLine(const string& p) { string s; cout << p; getline(cin, s); return s; }

inline string addMonths(const string& d, int mAdd) {
    if (d.length() < 10) return "2026-12-31";
    int y = atoi(d.substr(0, 4).c_str()), m = atoi(d.substr(5, 2).c_str()) + mAdd;
    while (m > 12) { m -= 12; y++; }
    ostringstream o; o << y << "-" << (m < 10 ? "0" : "") << m << "-" << d.substr(8, 2);
    return o.str();
}

struct Customer {
    string id, fullName, phone, cccd, address, contact;
    string getId() const { return id; }
    string getFullName() const { return fullName; }
    string getPhone() const { return phone; }
    string getCccd() const { return cccd; }
    void inputNew() { id = readLine("Ma KH: "); updateFields(); }
    void updateFields() {
        fullName = readLine("Ho ten: "); phone = readLine("SDT: ");
        cccd = readLine("CCCD: "); address = readLine("Dia chi: "); contact = readLine("Lien he: ");
    }
    void displayRow() const {
        cout << left << setw(10) << id << setw(22) << fullName << setw(14) << phone
             << setw(15) << cccd << setw(25) << address << setw(20) << contact << '\n';
    }
};

struct Plan {
    string id, name, speed, price, duration;
    string getId() const { return id; }
    string getName() const { return name; }
    void inputNew() { id = readLine("Ma goi: "); updateFields(); }
    void updateFields() {
        name = readLine("Ten goi: "); speed = readLine("Toc do: ");
        price = readLine("Gia cuoc: "); duration = readLine("Thoi han: ");
    }
    void displayRow() const {
        cout << left << setw(12) << id << setw(22) << name << setw(16) << speed
             << setw(18) << price << setw(15) << duration << '\n';
    }
};

struct Contract {
    string id, customerId, planId, startDate, endDate, status = "Dang hoat dong";
    string getId() const { return id; }
    string getCustomerId() const { return customerId; }
    string getEndDate() const { return endDate; }
    string getStatus() const { return status; }
    void setEndDate(const string& e) { endDate = e; }
    void setStatus(const string& s) { status = s; }
    void inputNew() {
        id = readLine("Ma HD: "); customerId = readLine("Ma KH: "); planId = readLine("Ma goi: ");
        startDate = readLine("Ngay BD: "); endDate = readLine("Ngay KT (YYYY-MM-DD): ");
    }
    void updateFields() {
        string st[] = {"", "Dang hoat dong", "Tam ngung", "Da huy", "Het han"};
        status = st[readChoice("Trang thai (1.HD 2.Tam ngung 3.Da huy 4.Het han): ", 1, 4)];
    }
    void displayRow() const {
        cout << id << " | KH:" << customerId << " | Goi:" << planId << " | KT:" << endDate << " | " << status << "\n";
    }
};

struct Renewal {
    string id, contractId, oldEnd, newEnd, status = "Thanh cong"; int months = 1;
    string getId() const { return id; }
    string getContractId() const { return contractId; }
    string getNewEndDate() const { return newEnd; }
    void inputNew(const string& cId, const string& curEnd) {
        id = readLine("Ma GH: "); contractId = cId; oldEnd = curEnd; updateFields();
    }
    void updateFields() {
        int m[] = {0, 1, 3, 6, 12};
        months = m[readChoice("Goi GH (1:1T, 2:3T, 3:6T, 4:12T): ", 1, 4)];
        newEnd = addMonths(oldEnd, months);
    }
    void displayRow() const {
        cout << id << " | HD:" << contractId << " | +" << months << "T | " << oldEnd << " -> " << newEnd << "\n";
    }
};

struct Invoice {
    string id, customerId, amount, status = "Chua thanh toan";
    string getId() const { return id; }
    string getCustomerId() const { return customerId; }
    void setStatus(const string& s) { status = s; }
    void inputNew() { id = readLine("Ma hoa don: "); updateFields(); }
    void updateFields() { customerId = readLine("Ma KH: "); amount = readLine("So tien: "); }
    void displayRow() const { cout << id << " | KH:" << customerId << " | Tien:" << amount << " | " << status << "\n"; }
};

struct Payment {
    string id, invoiceId, amount, status = "Thanh cong";
    string getId() const { return id; }
    string getInvoiceId() const { return invoiceId; }
    string getStatus() const { return status; }
    void inputNew() { id = readLine("Ma TT: "); invoiceId = readLine("Ma hoa don: "); amount = readLine("So tien: "); }
    void updateFields() {
        invoiceId = readLine("Ma hoa don: "); amount = readLine("So tien: ");
        status = (readChoice("1.Thanh cong 2.Huy: ", 1, 2) == 1) ? "Thanh cong" : "Da huy";
    }
    void displayRow() const { cout << id << " | HDuoc:" << invoiceId << " | Tien:" << amount << " | " << status << "\n"; }
};

template <typename T>
class Repository {
    vector<T> v;
public:
    T* findById(const string& id) { for (auto& x : v) if (x.getId() == id) return &x; return nullptr; }
    vector<T>& all() { return v; }
    void add(const T& x) { v.push_back(x); }
    void save() {}
    void removeById(const string& id) {
        for (size_t i = 0; i < v.size(); ++i) if (v[i].getId() == id) { v.erase(v.begin() + i); return; }
    }
    void showAll() const {
        if (v.empty()) cout << "(Danh sach trong)\n";
        for (const auto& x : v) x.displayRow();
    }
};

struct AppData {
    Repository<Customer> customers; Repository<Plan> plans; Repository<Contract> contracts;
    Repository<Renewal> renewals;   Repository<Invoice> invoices; Repository<Payment> payments;
};

int runApplication();
void syncInvoiceStatus(AppData& app, const string& invId);
void menuCustomers(AppData& app);     void menuRegistrations(AppData& app);
void menuPlans(AppData& app);         void menuPlanChanges(AppData& app);
void menuContracts(AppData& app);     void menuRenewals(AppData& app);
void menuPayments(AppData& app);      void menuInvoices(AppData& app);
void menuIncidents(AppData& app);     void menuSupports(AppData& app);

#endif
