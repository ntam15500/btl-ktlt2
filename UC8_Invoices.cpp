#include "Menus.h"

const string& Invoice::getCustomerId() const {
    return customerId_;
}
const string& Invoice::getStatus() const {
    return status_;
}
void Invoice::setCustomerId(const string& value) {
    customerId_ = checkedText(value, "Ma khach hang");
}
void Invoice::setAmount(const string& value) {
    amount_ = checkedPositiveNumber(value, "So tien hoa don");
}
void Invoice::setStatus(const string& value) {
    status_ = checkedText(value, "Trang thai hoa don");
}

void Invoice::inputNew() {
    setId(readUCText("Ma hoa don: "));
    setCustomerId(readUCText("Ma KH: "));
    setAmount(readUCPositiveNumber("So tien (so > 0): "));
}

void Invoice::updateFields() {
    updateOptionalField("So tien [" + amount_ + "]: ", [this](const string& v) {
        setAmount(v);
    });
    updateOptionalField("Trang thai [" + status_ + "]: ", [this](const string& v) {
        setStatus(v);
    });
}

void Invoice::displayRow() const {
    displayCells({
        {getId(), 10},
        {customerId_, 10},
        {amount_, 12},
        {status_, 15}
    });
}

string Invoice::toFileString() const {
    return getId() + "|" + customerId_ + "|" + amount_ + "|" + status_;
}

bool Invoice::fromFileString(const string& line) {
    string fields[4];
    return parseEntityRecord(line, fields, 4, [this](const string* values) {
        setId(values[0]);
        setCustomerId(values[1]);
        setAmount(values[2]);
        setStatus(values[3]);
    });
}

// UC8 - Quan ly hoa don cuoc
void menuInvoices(AppData& app) {
    while (true) {
        cout << "\n" << string(58, '=') << '\n'
             << "             QUAN LY HOA DON CUOC (UC8)\n"
             << string(58, '=') << '\n'
             << " 1. Danh sach hoa don cuoc\n"
             << " 2. Them hoa don moi\n"
             << " 3. Cap nhat hoa don\n"
             << " 4. Xoa hoa don\n"
             << " 0. Quay lai\n"
             << string(58, '-') << '\n';

        int ch = readChoice("Chon: ", 0, 4);
        if (ch == 0) return;
        try {
            if (ch == 1) {
                cout << "\n--- DANH SACH HOA DON CUOC ---\n";
                app.invoices.showAll();
            } else if (ch == 2) {
                Invoice inv; inv.inputNew();
                if (app.invoices.findById(inv.getId())) throw invalid_argument("Ma dinh danh da ton tai.");
                if (!app.customers.all().empty() && !app.customers.findById(inv.getCustomerId()))
                    throw invalid_argument("Ma khach hang khong ton tai.");
                app.invoices.add(inv);
                cout << "=> Them hoa don thanh cong!\n";
            } else if (ch == 3) {
                Invoice* cur = app.invoices.findById(readLine("Nhap ma hoa don can cap nhat: "));
                if (!cur) { cout << "Khong tim thay ban ghi!\n"; continue; }
                Invoice upd = *cur; upd.updateFields();
                if (!app.customers.all().empty() && !app.customers.findById(upd.getCustomerId()))
                    throw invalid_argument("Ma khach hang khong ton tai.");
                *cur = upd; app.invoices.save();
                syncInvoiceStatus(app, cur->getId());
                cout << "=> Cap nhat hoa don thanh cong!\n";
            } else if (ch == 4) {
                Invoice* cur = app.invoices.findById(readLine("Nhap ma hoa don can xoa: "));
                if (!cur) { cout << "Khong tim thay ban ghi!\n"; continue; }
                for (size_t i = 0; i < app.payments.all().size(); ++i)
                    if (app.payments.all()[i].getInvoiceId() == cur->getId())
                        throw invalid_argument("Hoa don dang duoc giao dich thanh toan tham chieu.");
                cur->displayRow();
                string ans = readLine("Xac nhan xoa (y/n): ");
                if (ans == "y" || ans == "Y") {
                    app.invoices.removeById(cur->getId());
                    cout << "=> Da xoa hoa don thanh cong!\n";
                } else {
                    cout << "=> Da huy thao tac xoa.\n";
                }
            }
        } catch (const exception& e) { cout << "[Loi] " << e.what() << '\n'; }
    }
}
