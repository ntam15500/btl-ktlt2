#include "Menus.h"

const string& Payment::getInvoiceId() const {
    return invoiceId_;
}
const string& Payment::getStatus() const {
    return status_;
}
void Payment::setInvoiceId(const string& value) {
    invoiceId_ = checkedText(value, "Ma hoa don");
}
void Payment::setAmount(const string& value) {
    amount_ = checkedPositiveNumber(value, "So tien thanh toan");
}
void Payment::setStatus(const string& value) {
    status_ = checkedText(value, "Trang thai thanh toan");
}

void Payment::inputNew() {
    setId(readUCText("Ma TT: "));
    setInvoiceId(readUCText("Ma hoa don: "));
    setAmount(readUCPositiveNumber("So tien (so > 0): "));
}

void Payment::updateFields() {
    updateOptionalField("So tien [" + amount_ + "]: ", [this](const string& v) {
        setAmount(v);
    });
    updateOptionalField("Trang thai [" + status_ + "]: ", [this](const string& v) {
        setStatus(v);
    });
}

void Payment::displayRow() const {
    displayCells({
        {getId(), 10},
        {invoiceId_, 10},
        {amount_, 12},
        {status_, 15}
    });
}

string Payment::toFileString() const {
    return getId() + "|" + invoiceId_ + "|" + amount_ + "|" + status_;
}

bool Payment::fromFileString(const string& line) {
    string fields[4];
    return parseEntityRecord(line, fields, 4, [this](const string* values) {
        setId(values[0]);
        setInvoiceId(values[1]);
        setAmount(values[2]);
        setStatus(values[3]);
    });
}

// UC7 - Quan ly thanh toan
void syncInvoiceStatus(AppData& app, const string& invId) {
    Invoice* inv = app.invoices.findById(invId);
    if (!inv) return;
    bool paid = false;
    for (size_t i = 0; i < app.payments.all().size(); ++i)
        if (app.payments.all()[i].getInvoiceId() == invId &&
            app.payments.all()[i].getStatus() == "Thanh cong") paid = true;
    inv->setStatus(paid ? "Da thanh toan" : "Chua thanh toan");
    app.invoices.save();
}

void menuPayments(AppData& app) {
    while (true) {
        cout << "\n" << string(58, '=') << '\n'
             << "              QUAN LY THANH TOAN (UC7)\n"
             << string(58, '=') << '\n'
             << " 1. Danh sach giao dich thanh toan\n"
             << " 2. Them giao dich thanh toan\n"
             << " 3. Cap nhat giao dich thanh toan\n"
             << " 4. Xoa giao dich thanh toan\n"
             << " 0. Quay lai\n"
             << string(58, '-') << '\n';

        int ch = readChoice("Chon: ", 0, 4);
        if (ch == 0) return;
        try {
            if (ch == 1) {
                cout << "\n--- DANH SACH GIAO DICH THANH TOAN ---\n";
                app.payments.showAll();
            } else if (ch == 2) {
                Payment p; p.inputNew();
                if (app.payments.findById(p.getId())) throw invalid_argument("Ma dinh danh da ton tai.");
               // if (!app.invoices.findById(p.getInvoiceId())) throw invalid_argument("Ma hoa don khong ton tai.");
                app.payments.add(p);
                syncInvoiceStatus(app, p.getInvoiceId());
                cout << "=> Them giao dich thanh toan thanh cong!\n";
            } else if (ch == 3) {
                Payment* cur = app.payments.findById(readLine("Nhap ma thanh toan can cap nhat: "));
                if (!cur) { cout << "Khong tim thay ban ghi!\n"; continue; }
                string oldInv = cur->getInvoiceId();
                Payment upd = *cur; upd.updateFields();
                if (!app.invoices.findById(upd.getInvoiceId())) throw invalid_argument("Ma hoa don khong ton tai.");
                *cur = upd; app.payments.save();
                syncInvoiceStatus(app, oldInv); syncInvoiceStatus(app, upd.getInvoiceId());
                cout << "=> Cap nhat giao dich thanh toan thanh cong!\n";
            } else if (ch == 4) {
                Payment* cur = app.payments.findById(readLine("Nhap ma thanh toan can xoa: "));
                if (!cur) { cout << "Khong tim thay ban ghi!\n"; continue; }
                cur->displayRow();
                string invId = cur->getInvoiceId();
                string ans = readLine("Xac nhan xoa (y/n): ");
                if (ans == "y" || ans == "Y") {
                    app.payments.removeById(cur->getId());
                    syncInvoiceStatus(app, invId);
                    cout << "=> Da xoa giao dich thanh toan thanh cong!\n";
                } else {
                    cout << "=> Da huy thao tac xoa.\n";
                }
            }
        } catch (const exception& e) { cout << "[Loi] " << e.what() << '\n'; }
    }
}
