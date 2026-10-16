#include "Menus.h"

// UC8 - Quan ly hoa don cuoc
void menuInvoices(AppData& app) {
    while (true) {
        cout << "\n===== QUAN LY HOA DON CUOC (UC 8) =====\n"
             << "1. Them\n2. Xem danh sach\n3. Sua\n4. Xoa\n0. Quay lai\n";
        int ch = readChoice("Chon: ", 0, 4);
        if (ch == 0) return;
        try {
            if (ch == 1) {
                Invoice inv; inv.inputNew();
                if (app.invoices.findById(inv.getId())) throw invalid_argument("Ma dinh danh da ton tai.");
                if (!app.customers.all().empty() && !app.customers.findById(inv.getCustomerId()))
                    throw invalid_argument("Ma khach hang khong ton tai.");
                app.invoices.add(inv);
                cout << "Them thanh cong.\n";
            } else if (ch == 2) {
                app.invoices.showAll();
            } else if (ch == 3) {
                Invoice* cur = app.invoices.findById(readLine("Ma can sua: "));
                if (!cur) { cout << "Khong tim thay ban ghi.\n"; continue; }
                Invoice upd = *cur; upd.updateFields();
                if (!app.customers.all().empty() && !app.customers.findById(upd.getCustomerId()))
                    throw invalid_argument("Ma khach hang khong ton tai.");
                *cur = upd; app.invoices.save();
                syncInvoiceStatus(app, cur->getId());
                cout << "Cap nhat thanh cong.\n";
            } else if (ch == 4) {
                Invoice* cur = app.invoices.findById(readLine("Ma can xoa: "));
                if (!cur) { cout << "Khong tim thay ban ghi.\n"; continue; }
                for (size_t i = 0; i < app.payments.all().size(); ++i)
                    if (app.payments.all()[i].getInvoiceId() == cur->getId())
                        throw invalid_argument("Hoa don dang duoc giao dich thanh toan tham chieu.");
                cur->displayRow();
                string ans = readLine("Xac nhan xoa (y/n): ");
                if (ans == "y" || ans == "Y") {
                    app.invoices.removeById(cur->getId());
                    cout << "Xoa thanh cong.\n";
                }
            }
        } catch (const exception& e) { cout << "[Loi] " << e.what() << '\n'; }
    }
}