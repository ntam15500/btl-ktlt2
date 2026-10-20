#include "Menus.h"

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
