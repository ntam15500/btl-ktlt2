#include "Menus.h"

void menuContracts(AppData& app) {
    while (true) {
        cout << "\n===== QUAN LY HOP DONG (UC 5) =====\n"
             << "1. Them hop dong\n2. Xem danh sach\n3. Sua hop dong\n4. Xoa hop dong\n0. Quay lai\n";
        int ch = readChoice("Chon: ", 0, 4);
        if (ch == 0) return;
        try {
            if (ch == 1) {
                Contract c; c.inputNew();
                if (app.contracts.findById(c.getId())) throw invalid_argument("Ma hop dong da ton tai.");
                app.contracts.add(c);
                cout << "Them hop dong thanh cong.\n";
            } else if (ch == 2) {
                app.contracts.showAll();
            } else if (ch == 3) {
                Contract* cur = app.contracts.findById(readLine("Ma hop dong can sua: "));
                if (!cur) { cout << "Khong tim thay hop dong.\n"; continue; }
                Contract upd = *cur; upd.updateFields();
                *cur = upd;
                app.contracts.save();
                cout << "Cap nhat hop dong thanh cong.\n";
            } else if (ch == 4) {
                Contract* cur = app.contracts.findById(readLine("Ma hop dong can xoa: "));
                if (!cur) { cout << "Khong tim thay hop dong.\n"; continue; }
                cur->displayRow();
                string ans = readLine("Xac nhan xoa (y/n): ");
                if (ans == "y" || ans == "Y") {
                    app.contracts.removeById(cur->getId());
                    cout << "Xoa hop dong thanh cong.\n";
                }
            }
        } catch (const exception& e) { cout << "[Loi] " << e.what() << '\n'; }
    }
}