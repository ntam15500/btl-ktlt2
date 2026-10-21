#include "Menus.h"

const string& Renewal::getContractId() const {
    return contractId_;
}
const string& Renewal::getNewEndDate() const {
    return newEnd_;
}
const string& Renewal::getStatus() const {
    return status_;
}
void Renewal::setContractId(const string& value) {
    contractId_ = checkedText(value, "Ma hop dong");
}
void Renewal::setOldEndDate(const string& value) {
    oldEnd_ = checkedDateText(value, "Ngay ket thuc cu");
}
void Renewal::setNewEndDate(const string& value) {
    newEnd_ = checkedDateText(value, "Ngay ket thuc moi");
}
void Renewal::setStatus(const string& value) {
    status_ = checkedText(value, "Trang thai gia han");
}
void Renewal::setMonths(int value) {
    if (value < 1 || value > 120) {
        throw invalid_argument("So thang gia han phai tu 1 den 120.");
    }
    months_ = value;
}

void Renewal::inputNew(const string& contractId, const string& currentEndDate) {
    setId(readUCText("Ma GH: "));
    setContractId(contractId);
    setOldEndDate(currentEndDate);
    setNewEndDate(readUCText("Ngay moi: "));
    string value = readLine("So thang gia han [1]: ");
    if (!value.empty()) {
        try {
            size_t used = 0;
            int n = stoi(value, &used);
            if (used != value.size()) {
                throw invalid_argument("invalid");
            }
            setMonths(n);
        } catch (const exception&) {
            cout << "So thang khong hop le; tam thoi dung gia tri 1.\n";
            setMonths(1);
        }
    }
}

void Renewal::updateFields() {
    updateOptionalField("Ngay KT moi [" + newEnd_ + "]: ", [this](const string& v) {
        setNewEndDate(v);
    });
    updateOptionalField("So thang [" + to_string(months_) + "]: ", [this](const string& v) {
        size_t used = 0;
        int n = stoi(v, &used);
        if (used != v.size()) {
            throw invalid_argument("So thang phai la so nguyen.");
        }
        setMonths(n);
    });
    updateOptionalField("Trang thai [" + status_ + "]: ", [this](const string& v) {
        setStatus(v);
    });
}

void Renewal::displayRow() const {
    displayCells({
        {getId(), 10},
        {contractId_, 10},
        {newEnd_, 12},
        {to_string(months_), 8},
        {status_, 15}
    });
}

string Renewal::toFileString() const {
    return getId() + "|" + contractId_ + "|" + oldEnd_
        + "|" + newEnd_ + "|" + to_string(months_) + "|" + status_;
}

bool Renewal::fromFileString(const string& line) {
    string fields[6];
    return parseEntityRecord(line, fields, 6, [this](const string* values) {
        size_t used = 0;
        int parsedMonths = stoi(values[4], &used);
        if (used != values[4].size()) {
            throw invalid_argument("So thang khong hop le.");
        }
        setId(values[0]);
        setContractId(values[1]);
        setOldEndDate(values[2]);
        setNewEndDate(values[3]);
        setMonths(parsedMonths);
        setStatus(values[5]);
    });
}

void menuRenewals(AppData& app) {
    while (true) {
        cout << "\n" << string(58, '=') << '\n'
             << "               GIA HAN DICH VU (UC6)\n"
             << string(58, '=') << '\n'
             << " 1. Danh sach gia han dich vu\n"
             << " 2. Them gia han moi\n"
             << " 3. Cap nhat thong tin gia han\n"
             << " 4. Xoa ban ghi gia han\n"
             << " 0. Quay lai\n"
             << string(58, '-') << '\n';

        int ch = readChoice("Chon: ", 0, 4);
        if (ch == 0) return;
        try {
            if (ch == 1) {
                cout << "\n--- DANH SACH GIA HAN DICH VU ---\n";
                app.renewals.showAll();
            } else if (ch == 2) {
                Contract* c = app.contracts.findById(readLine("Nhap ma HD can gia han: "));
                if (!c) throw invalid_argument("Ma hop dong khong ton tai.");
                if (c->getStatus() == "Da huy") throw invalid_argument("Hop dong da huy.");
                Renewal r; r.inputNew(c->getId(), c->getEndDate());
                if (app.renewals.findById(r.getId())) throw invalid_argument("Ma dinh danh da ton tai.");
                c->setEndDate(r.getNewEndDate()); c->setStatus("Dang hoat dong");
                app.renewals.add(r);
                cout << "=> Them gia han thanh cong! Han moi: " << r.getNewEndDate() << "\n";
            } else if (ch == 3) {
                Renewal* cur = app.renewals.findById(readLine("Nhap ma gia han can cap nhat: "));
                if (!cur) { cout << "Khong tim thay ban ghi!\n"; continue; }
                cur->updateFields();
                if (Contract* c = app.contracts.findById(cur->getContractId())) c->setEndDate(cur->getNewEndDate());
                cout << "=> Cap nhat gia han thanh cong!\n";
            } else if (ch == 4) {
                Renewal* cur = app.renewals.findById(readLine("Nhap ma gia han can xoa: "));
                if (!cur) { cout << "Khong tim thay ban ghi!\n"; continue; }
                cur->displayRow();
                string ans = readLine("Xac nhan xoa (y/n): ");
                if (ans == "y" || ans == "Y") {
                    app.renewals.removeById(cur->getId());
                    cout << "=> Da xoa ban ghi gia han thanh cong!\n";
                } else {
                    cout << "=> Da huy thao tac xoa.\n";
                }
            }
        } catch (const exception& e) { cout << "[Loi] " << e.what() << '\n'; }
    }
}
