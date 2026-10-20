#ifndef MENUS_H
#define MENUS_H
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <limits>
#include <stdexcept>
using namespace std;

inline string readLine(const string& p) {
    string s; cout << p;
    if (!getline(cin, s)) throw runtime_error("Ket thuc dau vao (EOF).");
    return s;
}

inline int readChoice(const string& p, int mn, int mx) {
    int c;
    while (true) {
        cout << p;
        if (cin >> c) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (c >= mn && c <= mx) return c;
        } else {
            if (cin.eof()) throw runtime_error("Ket thuc dau vao (EOF).");
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Lua chon khong hop le, vui long nhap " << mn << "-" << mx << ".\n";
    }
}

inline int readUCChoice(const string& p, int mn, int mx) { return readChoice(p, mn, mx); }

inline string readUCText(const string& p, bool req = true) {
    while (true) {
        string s = readLine(p);
        if (req && s.find_first_not_of(" \t\r") == string::npos) { cout << "Khong duoc de trong!\n"; continue; }
        if (s.find('|') != string::npos) { cout << "Khong duoc nhap ky tu |.\n"; continue; }
        return s;
    }
}

inline bool confirmUC() { string a = readLine("Xac nhan thuc hien? (y/n): "); return a == "y" || a == "Y"; }

inline bool splitPipe(const string& line, string* t, int n) {
    stringstream ss(line); string extra;
    for (int i = 0; i < n; ++i) if (!getline(ss, t[i], '|')) return false;
    return !getline(ss, extra, '|');
}

class Entity {
public:
    string id;
    virtual ~Entity() = default;
    string getId() const { return id; }
    void setId(const string& i) { id = i; }
    virtual void displayRow() const = 0;
    virtual string toFileString() const = 0;
    virtual bool fromFileString(const string& line) = 0;
};

class Customer : public Entity {
public:
    string fullName, phone, cccd, address, contact;
    string getFullName() const { return fullName; }
    string getPhone() const { return phone; }
    string getCccd() const { return cccd; }
    void inputNew() { id = readLine("Ma KH: "); fullName = readLine("Ho ten: "); phone = readLine("SDT: "); cccd = readLine("CCCD: "); address = readLine("Dia chi: "); contact = readLine("Lien he: "); }
    void updateFields() {
        string s = readLine("Ho ten [" + fullName + "]: "); if (!s.empty()) fullName = s;
        s = readLine("SDT [" + phone + "]: "); if (!s.empty()) phone = s;
    }
    void displayRow() const override { cout << left << setw(10) << id << setw(22) << fullName << setw(14) << phone << setw(15) << cccd << setw(25) << address << setw(20) << contact << "\n"; }
    string toFileString() const override { return id + "|" + fullName + "|" + phone + "|" + cccd + "|" + address + "|" + contact; }
    bool fromFileString(const string& l) override { string t[6]; if (!splitPipe(l, t, 6)) return false; id = t[0]; fullName = t[1]; phone = t[2]; cccd = t[3]; address = t[4]; contact = t[5]; return true; }
};

class Registration : public Entity {
public:
    string customerId, planId, regDate, status = "Cho lap dat";
    string getCustomerId() const { return customerId; }
    string getPlanId() const { return planId; }
    void inputNew() { id = readLine("Ma DK: "); customerId = readLine("Ma KH: "); planId = readLine("Ma goi: "); regDate = readLine("Ngay DK: "); }
    void updateFields() { status = readLine("Trang thai [" + status + "]: "); }
    void displayRow() const override { cout << left << setw(10) << id << setw(10) << customerId << setw(10) << planId << setw(12) << regDate << setw(15) << status << "\n"; }
    string toFileString() const override { return id + "|" + customerId + "|" + planId + "|" + regDate + "|" + status; }
    bool fromFileString(const string& l) override { string t[5]; if (!splitPipe(l, t, 5)) return false; id = t[0]; customerId = t[1]; planId = t[2]; regDate = t[3]; status = t[4]; return true; }
};

class Plan : public Entity {
public:
    string name, speed, price, duration;
    string getName() const { return name; }
    void inputNew() { id = readLine("Ma goi: "); name = readLine("Ten goi: "); speed = readLine("Toc do: "); price = readLine("Gia: "); duration = readLine("Thoi han: "); }
    void updateFields() {
        string s = readLine("Ten [" + name + "]: "); if (!s.empty()) name = s;
        s = readLine("Gia [" + price + "]: "); if (!s.empty()) price = s;
    }
    void displayRow() const override { cout << left << setw(12) << id << setw(22) << name << setw(16) << speed << setw(18) << price << setw(15) << duration << "\n"; }
    string toFileString() const override { return id + "|" + name + "|" + speed + "|" + price + "|" + duration; }
    bool fromFileString(const string& l) override { string t[5]; if (!splitPipe(l, t, 5)) return false; id = t[0]; name = t[1]; speed = t[2]; price = t[3]; duration = t[4]; return true; }
};

class PlanChange : public Entity {
public:
    string contractId, oldPlanId, newPlanId, date, status = "Da thuc hien";
    void displayRow() const override { cout << left << setw(10) << id << setw(10) << contractId << setw(15) << newPlanId << setw(12) << date << setw(15) << status << "\n"; }
    string toFileString() const override { return id + "|" + contractId + "|" + oldPlanId + "|" + newPlanId + "|" + date + "|" + status; }
    bool fromFileString(const string& l) override { string t[6]; if (!splitPipe(l, t, 6)) return false; id = t[0]; contractId = t[1]; oldPlanId = t[2]; newPlanId = t[3]; date = t[4]; status = t[5]; return true; }
};

class Contract : public Entity {
public:
    string customerId, planId, startDate, endDate, status = "Dang hoat dong";
    string getCustomerId() const { return customerId; }
    string getPlanId() const { return planId; }
    string getStartDate() const { return startDate; }
    string getEndDate() const { return endDate; }
    string getStatus() const { return status; }
    void setEndDate(const string& e) { endDate = e; }
    void setStatus(const string& s) { status = s; }
    void inputNew() { id = readLine("Ma HD: "); customerId = readLine("Ma KH: "); planId = readLine("Ma goi: "); startDate = readLine("Ngay BD: "); endDate = readLine("Ngay KT: "); }
    void updateFields() { status = readLine("Trang thai [" + status + "]: "); }
    void displayRow() const override { cout << left << setw(10) << id << setw(10) << customerId << setw(10) << planId << setw(12) << endDate << setw(15) << status << "\n"; }
    string toFileString() const override { return id + "|" + customerId + "|" + planId + "|" + startDate + "|" + endDate + "|" + status; }
    bool fromFileString(const string& l) override { string t[6]; if (!splitPipe(l, t, 6)) return false; id = t[0]; customerId = t[1]; planId = t[2]; startDate = t[3]; endDate = t[4]; status = t[5]; return true; }
};

class Renewal : public Entity {
public:
    string contractId, oldEnd, newEnd, status = "Thanh cong"; int months = 1;
    string getContractId() const { return contractId; }
    string getNewEndDate() const { return newEnd; }
    void inputNew(const string& cId, const string& curEnd) { id = readLine("Ma GH: "); contractId = cId; oldEnd = curEnd; newEnd = readLine("Ngay moi: "); }
    void updateFields() {
        string s = readLine("Ngay KT moi [" + newEnd + "]: "); if (!s.empty()) newEnd = s;
        s = readLine("So thang [" + to_string(months) + "]: ");
        if (!s.empty()) { try { int n = stoi(s); if (n > 0 && n <= 120) months = n; } catch (...) {} }
    }
    void displayRow() const override { cout << left << setw(10) << id << setw(10) << contractId << setw(12) << newEnd << "\n"; }
    string toFileString() const override { return id + "|" + contractId + "|" + oldEnd + "|" + newEnd + "|" + to_string(months) + "|" + status; }
    bool fromFileString(const string& l) override {
        string t[6]; if (!splitPipe(l, t, 6)) return false;
        size_t p = 0; int m = stoi(t[4], &p); if (p != t[4].size() || m <= 0 || m > 120) return false;
        id = t[0]; contractId = t[1]; oldEnd = t[2]; newEnd = t[3]; months = m; status = t[5]; return true;
    }
};

class Payment : public Entity {
public:
    string invoiceId, amount, status = "Thanh cong";
    string getInvoiceId() const { return invoiceId; }
    string getStatus() const { return status; }
    void inputNew() { id = readLine("Ma TT: "); invoiceId = readLine("Ma HDuoc: "); amount = readLine("So tien: "); }
    void updateFields() { status = readLine("Trang thai [" + status + "]: "); }
    void displayRow() const override { cout << left << setw(10) << id << setw(10) << invoiceId << setw(12) << amount << setw(15) << status << "\n"; }
    string toFileString() const override { return id + "|" + invoiceId + "|" + amount + "|" + status; }
    bool fromFileString(const string& l) override { string t[4]; if (!splitPipe(l, t, 4)) return false; id = t[0]; invoiceId = t[1]; amount = t[2]; status = t[3]; return true; }
};

class Invoice : public Entity {
public:
    string customerId, amount, status = "Chua thanh toan";
    string getCustomerId() const { return customerId; }
    string getStatus() const { return status; }
    void setStatus(const string& s) { status = s; }
    void inputNew() { id = readLine("Ma HDuoc: "); customerId = readLine("Ma KH: "); amount = readLine("So tien: "); }
    void updateFields() { status = readLine("Trang thai [" + status + "]: "); }
    void displayRow() const override { cout << left << setw(10) << id << setw(10) << customerId << setw(12) << amount << setw(15) << status << "\n"; }
    string toFileString() const override { return id + "|" + customerId + "|" + amount + "|" + status; }
    bool fromFileString(const string& l) override { string t[4]; if (!splitPipe(l, t, 4)) return false; id = t[0]; customerId = t[1]; amount = t[2]; status = t[3]; return true; }
};

class Incident : public Entity {
public:
    string contractId, customerId, desc, status = "Moi tiep nhan";
    void inputNew(const string& cId = "", const string& custId = "") { id = readLine("Ma SC: "); contractId = cId; customerId = custId; desc = readLine("Mo ta: "); }
    void updateFields() { status = readLine("Trang thai [" + status + "]: "); }
    void displayRow() const override { cout << left << setw(10) << id << setw(10) << contractId << setw(20) << desc << setw(15) << status << "\n"; }
    string toFileString() const override { return id + "|" + contractId + "|" + customerId + "|" + desc + "|" + status; }
    bool fromFileString(const string& l) override { string t[5]; if (!splitPipe(l, t, 5)) return false; id = t[0]; contractId = t[1]; customerId = t[2]; desc = t[3]; status = t[4]; return true; }
};

class Support : public Entity {
public:
    string customerId, incidentId, technician, note, history, status = "Cho phan cong";
    void inputNew() { id = readLine("Ma HT: "); customerId = readLine("Ma KH: "); incidentId = readLine("Ma SC: "); note = readLine("Ghi chu: "); }
    void updateFields() { technician = readLine("KTV: "); }
    void displayRow() const override { cout << left << setw(10) << id << setw(10) << customerId << setw(15) << status << history << "\n"; }
    string toFileString() const override { return id + "|" + customerId + "|" + incidentId + "|" + technician + "|" + note + "|" + history + "|" + status; }
    bool fromFileString(const string& l) override { string t[7]; if (!splitPipe(l, t, 7)) return false; id = t[0]; customerId = t[1]; incidentId = t[2]; technician = t[3]; note = t[4]; history = t[5]; status = t[6]; return true; }
};

template <typename T>
class Repository {
    vector<T> v; string path;
public:
    explicit Repository(const string& p = "") : path(p) { load(); }
    void load() {
        v.clear(); if (path.empty()) return;
        filesystem::create_directories("data");
        if (!filesystem::exists(path)) { ofstream(path); return; }
        ifstream in(path); string line;
        while (getline(in, line)) {
            if (line.empty() || line[0] == '#') continue;
            T obj; try { if (obj.fromFileString(line) && !obj.getId().empty()) v.push_back(obj); } catch (...) {}
        }
    }
    void save() const {
        if (path.empty()) return;
        filesystem::create_directories("data");
        ofstream out(path, ios::trunc);
        for (const auto& x : v) out << x.toFileString() << "\n";
    }
    T* findById(const string& id) {
        for (auto& x : v) if (x.getId() == id) return &x;
        return nullptr;
    }
    vector<T>& all() { return v; }
    void add(const T& x) { v.push_back(x); save(); }
    void removeById(const string& id) {
        v.erase(remove_if(v.begin(), v.end(), [&](const T& x){ return x.getId() == id; }), v.end());
        save();
    }
    void showAll() const {
        if (v.empty()) { cout << "(Trong)\n"; return; }
        for (const auto& x : v) x.displayRow();
    }
};

struct AppData {
    Repository<Customer> customers{"data/customers.txt"};
    Repository<Registration> registrations{"data/registrations.txt"};
    Repository<Plan> plans{"data/plans.txt"};
    Repository<PlanChange> planChanges{"data/plan_changes.txt"};
    Repository<Contract> contracts{"data/contracts.txt"};
    Repository<Renewal> renewals{"data/renewals.txt"};
    Repository<Invoice> invoices{"data/invoices.txt"};
    Repository<Payment> payments{"data/payments.txt"};
    Repository<Incident> incidents{"data/incidents.txt"};
    Repository<Support> supports{"data/supports.txt"};
};

int runApplication();
void syncInvoiceStatus(AppData& app, const string& invId);
void menuCustomers(AppData& app);     void menuRegistrations(AppData& app);
void menuPlans(AppData& app);         void menuPlanChanges(AppData& app);
void menuContracts(AppData& app);     void menuRenewals(AppData& app);
void menuPayments(AppData& app);      void menuInvoices(AppData& app);
void menuIncidents(AppData& app);     void menuSupports(AppData& app);

#endif
