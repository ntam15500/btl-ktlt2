#include "Menus.h"

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
