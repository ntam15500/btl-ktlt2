#ifndef MENUS_H
#define MENUS_H

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <limits>
#include <sstream>
#include <cstdlib>

using namespace std;

inline int readChoice(const string& prompt, int minVal, int maxVal) {
    int ch;
    while (cout << prompt && (!(cin >> ch) || ch < minVal || ch > maxVal)) {
        cout << "Loi! Nhap lai tu " << minVal << " den " << maxVal << ".\n";
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return ch;
}

inline string readLine(const string& prompt) {
    string s; cout << prompt; getline(cin, s); return s;
}

inline string addMonths(const string& d, int mAdd) {
    if (d.length() < 10) return "2026-12-31";
    int y = atoi(d.substr(0, 4).c_str()), m = atoi(d.substr(5, 2).c_str()) + mAdd, day = atoi(d.substr(8, 2).c_str());
    while (m > 12) { m -= 12; y++; }
    ostringstream o; o << y << "-" << (m < 10 ? "0" : "") << m << "-" << (day < 10 ? "0" : "") << day;
    return o.str();
}

struct Customer { string id; string getId() const { return id; } void displayRow() const {} };

class Contract {
    string id, customerId, planId, startDate, endDate, status = "Dang hoat dong";
public:
    string getId() const { return id; }
    string getCustomerId() const { return customerId; }
    string getEndDate() const { return endDate; }
    string getStatus() const { return status; }
    void setEndDate(const string& e) { endDate = e; }
    void setStatus(const string& s) { status = s; }

    void inputNew() {
        id = readLine("Ma HD: "); if (id.empty()) throw invalid_argument("Ma khong duoc trong.");
        customerId = readLine("Ma KH: "); planId = readLine("Ma goi: ");
        startDate = readLine("Ngay BD (YYYY-MM-DD): "); endDate = readLine("Ngay KT (YYYY-MM-DD): ");
    }
    void updateFields() {
        int c = readChoice("Trang thai (1.Hoat dong 2.Tam ngung 3.Da huy 4.Het han): ", 1, 4);
        status = (c == 1) ? "Dang hoat dong" : (c == 2) ? "Tam ngung" : (c == 3) ? "Da huy" : "Het han";
    }
    void displayRow() const {
        cout << "Ma HD: " << id << " | KH: " << customerId << " | Goi: " << planId
             << " | " << startDate << " -> " << endDate << " | " << status << "\n";
    }
};

class Renewal {
    string id, contractId, oldEnd, newEnd, rDate, status = "Thanh cong";
    int months = 1;
public:
    string getId() const { return id; }
    string getContractId() const { return contractId; }
    string getNewEndDate() const { return newEnd; }

    void inputNew(const string& cId, const string& curEnd) {
        id = readLine("Ma GH: "); if (id.empty()) throw invalid_argument("Ma khong duoc trong.");
        contractId = cId; oldEnd = curEnd;
        int opt = readChoice("Goi GH (1: 1T, 2: 3T, 3: 6T, 4: 12T): ", 1, 4);
        months = (opt == 2) ? 3 : (opt == 3) ? 6 : (opt == 4) ? 12 : 1;
        rDate = readLine("Ngay GH (YYYY-MM-DD): ");
        newEnd = addMonths(oldEnd, months);
    }
    void updateFields() {
        int opt = readChoice("Sua goi GH (1: 1T, 2: 3T, 3: 6T, 4: 12T): ", 1, 4);
        months = (opt == 2) ? 3 : (opt == 3) ? 6 : (opt == 4) ? 12 : 1;
        newEnd = addMonths(oldEnd, months);
    }
    void displayRow() const {
        cout << "Ma GH: " << id << " | HD: " << contractId << " | +" << months
             << "T | " << oldEnd << " -> " << newEnd << " | " << status << "\n";
    }
};

class Invoice {
    string id, customerId, amount, status = "Chua thanh toan";
public:
    string getId() const { return id; }
    string getCustomerId() const { return customerId; }
    void setStatus(const string& s) { status = s; }

    void inputNew() {
        id = readLine("Ma hoa don: "); if (id.empty()) throw invalid_argument("Ma khong duoc trong.");
        customerId = readLine("Ma KH: "); amount = readLine("So tien: ");
    }
    void updateFields() {
        customerId = readLine("Ma KH moi: "); amount = readLine("So tien moi: ");
    }
    void displayRow() const {
        cout << "Ma HDuoc: " << id << " | KH: " << customerId << " | Tien: " << amount << " | " << status << "\n";
    }
};

class Payment {
    string id, invoiceId, amount, status = "Thanh cong";
public:
    string getId() const { return id; }
    string getInvoiceId() const { return invoiceId; }
    string getStatus() const { return status; }

    void inputNew() {
        id = readLine("Ma TT: "); if (id.empty()) throw invalid_argument("Ma khong duoc trong.");
        invoiceId = readLine("Ma hoa don: "); amount = readLine("So tien TT: ");
    }
    void updateFields() {
        invoiceId = readLine("Ma hoa don moi: "); amount = readLine("So tien moi: ");
        status = (readChoice("Trang thai (1.Thanh cong 2.Huy): ", 1, 2) == 1) ? "Thanh cong" : "Da huy";
    }
    void displayRow() const {
        cout << "Ma TT: " << id << " | HDuoc: " << invoiceId << " | Tien: " << amount << " | " << status << "\n";
    }
};

template <typename T>
class Repository {
    vector<T> v;
public:
    T* findById(const string& id) {
        for (size_t i = 0; i < v.size(); ++i) if (v[i].getId() == id) return &v[i];
        return nullptr;
    }
    vector<T>& all() { return v; }
    void add(const T& item) { v.push_back(item); }
    void save() {}
    void removeById(const string& id) {
        for (size_t i = 0; i < v.size(); ++i) if (v[i].getId() == id) { v.erase(v.begin() + i); return; }
    }
    void showAll() const {
        if (v.empty()) cout << "(Danh sach trong)\n";
        for (size_t i = 0; i < v.size(); ++i) v[i].displayRow();
    }
};

struct AppData {
    Repository<Customer> customers;
    Repository<Contract> contracts;
    Repository<Renewal> renewals;
    Repository<Invoice> invoices;
    Repository<Payment> payments;
};

int runApplication();
void syncInvoiceStatus(AppData& app, const string& invId);
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
