#include "Menus.h"

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
        cout << "\n===== QUAN LY THANH TOAN (UC 7) =====\n"
             << "1. Them\n2. Xem danh sach\n3. Sua\n4. Xoa\n0. Quay lai\n";
        int ch = readChoice("Chon: ", 0, 4);
        if (ch == 0) return;
        try {
            if (ch == 1) {
                Payment p; p.inputNew();
                if (app.payments.findById(p.getId())) throw invalid_argument("Ma dinh danh da ton tai.");
               // if (!app.invoices.findById(p.getInvoiceId())) throw invalid_argument("Ma hoa don khong ton tai.");
                app.payments.add(p);
                syncInvoiceStatus(app, p.getInvoiceId());
                cout << "Them thanh cong.\n";
            } else if (ch == 2) {
                app.payments.showAll();
            } else if (ch == 3) {
                Payment* cur = app.payments.findById(readLine("Ma can sua: "));
                if (!cur) { cout << "Khong tim thay ban ghi.\n"; continue; }
                string oldInv = cur->getInvoiceId();
                Payment upd = *cur; upd.updateFields();
                if (!app.invoices.findById(upd.getInvoiceId())) throw invalid_argument("Ma hoa don khong ton tai.");
                *cur = upd; app.payments.save();
                syncInvoiceStatus(app, oldInv); syncInvoiceStatus(app, upd.getInvoiceId());
                cout << "Cap nhat thanh cong.\n";
            } else if (ch == 4) {
                Payment* cur = app.payments.findById(readLine("Ma can xoa: "));
                if (!cur) { cout << "Khong tim thay ban ghi.\n"; continue; }
                cur->displayRow();
                string invId = cur->getInvoiceId();
                string ans = readLine("Xac nhan xoa (y/n): ");
                if (ans == "y" || ans == "Y") {
                    app.payments.removeById(cur->getId());
                    syncInvoiceStatus(app, invId);
                    cout << "Xoa thanh cong.\n";
                }
            }
        } catch (const exception& e) { cout << "[Loi] " << e.what() << '\n'; }
    }
}