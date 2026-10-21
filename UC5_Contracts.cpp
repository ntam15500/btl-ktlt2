#include "Menus.h"

const string& Contract::getCustomerId() const {
    return customerId_;
}
const string& Contract::getPlanId() const {
    return planId_;
}
const string& Contract::getStartDate() const {
    return startDate_;
}
const string& Contract::getEndDate() const {
    return endDate_;
}
const string& Contract::getStatus() const {
    return status_;
}
void Contract::setCustomerId(const string& value) {
    customerId_ = checkedText(value, "Ma khach hang");
}
void Contract::setPlanId(const string& value) {
    planId_ = checkedText(value, "Ma goi cuoc");
}
void Contract::setStartDate(const string& value) {
    startDate_ = checkedDateText(value, "Ngay bat dau");
}
void Contract::setEndDate(const string& value) {
    endDate_ = checkedDateText(value, "Ngay ket thuc");
}
void Contract::setStatus(const string& value) {
    status_ = checkedText(value, "Trang thai hop dong");
}

void Contract::inputNew() {
    setId(readUCText("Ma HD: "));
    setCustomerId(readUCText("Ma KH: "));
    setPlanId(readUCText("Ma goi: "));
    setStartDate(readUCText("Ngay BD: "));
    setEndDate(readUCText("Ngay KT: "));
}

void Contract::updateFields() {
    updateOptionalField("Ngay KT [" + endDate_ + "]: ", [this](const string& v) {
        setEndDate(v);
    });
    updateOptionalField("Trang thai [" + status_ + "]: ", [this](const string& v) {
        setStatus(v);
    });
}

void Contract::displayRow() const {
    displayCells({
        {getId(), 10},
        {customerId_, 10},
        {planId_, 10},
        {endDate_, 12},
        {status_, 15}
    });
}

string Contract::toFileString() const {
    return getId() + "|" + customerId_ + "|" + planId_
        + "|" + startDate_ + "|" + endDate_ + "|" + status_;
}

bool Contract::fromFileString(const string& line) {
    string fields[6];
    return parseEntityRecord(line, fields, 6, [this](const string* values) {
        setId(values[0]);
        setCustomerId(values[1]);
        setPlanId(values[2]);
        setStartDate(values[3]);
        setEndDate(values[4]);
        setStatus(values[5]);
    });
}

void menuContracts(AppData& app) {
    while (true) {
        cout << "\n" << string(58, '=') << '\n'
             << "               QUAN LY HOP DONG (UC5)\n"
             << string(58, '=') << '\n'
             << " 1. Danh sach hop dong\n"
             << " 2. Them hop dong moi\n"
             << " 3. Cap nhat hop dong\n"
             << " 4. Xoa hop dong\n"
             << " 0. Quay lai\n"
             << string(58, '-') << '\n';

        int ch = readChoice("Chon: ", 0, 4);
        if (ch == 0) return;
        try {
            if (ch == 1) {
                cout << "\n--- DANH SACH HOP DONG ---\n";
                app.contracts.showAll();
            } else if (ch == 2) {
                Contract c; c.inputNew();
                if (app.contracts.findById(c.getId())) throw invalid_argument("Ma hop dong da ton tai.");
                app.contracts.add(c);
                cout << "=> Them hop dong thanh cong!\n";
            } else if (ch == 3) {
                Contract* cur = app.contracts.findById(readLine("Nhap ma hop dong can cap nhat: "));
                if (!cur) { cout << "Khong tim thay hop dong!\n"; continue; }
                Contract upd = *cur; upd.updateFields();
                *cur = upd;
                app.contracts.save();
                cout << "=> Cap nhat hop dong thanh cong!\n";
            } else if (ch == 4) {
                Contract* cur = app.contracts.findById(readLine("Nhap ma hop dong can xoa: "));
                if (!cur) { cout << "Khong tim thay hop dong!\n"; continue; }
                cur->displayRow();
                string ans = readLine("Xac nhan xoa (y/n): ");
                if (ans == "y" || ans == "Y") {
                    app.contracts.removeById(cur->getId());
                    cout << "=> Da xoa hop dong thanh cong!\n";
                } else {
                    cout << "=> Da huy thao tac xoa.\n";
                }
            }
        } catch (const exception& e) { cout << "[Loi] " << e.what() << '\n'; }
    }
}
