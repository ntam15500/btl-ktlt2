#include "Menus.h"


void menuCustomers(AppData& app) {
    while (true) {
        cout << "\n" << string(58, '=') << '\n'
             << "                 QUAN LY KHACH HANG\n"
             << string(58, '=') << '\n'
             << " 1. Danh sach khach hang\n"
             << " 2. Them khach hang moi\n"
             << " 3. Cap nhat thong tin khach hang\n"
             << " 4. Xoa khach hang\n"
             << " 5. Tim kiem khach hang\n"
             << " 0. Quay lai\n"
             << string(58, '-') << '\n';

        int choice = readChoice("Chon: ", 0, 5);
        if (choice == 0) return;

        switch (choice) {
            case 1: {
                cout << "\n" << left
                     << setw(10) << "Ma KH"
                     << setw(22) << "Ho ten"
                     << setw(14) << "SDT"
                     << setw(15) << "CCCD"
                     << setw(25) << "Dia chi"
                     << setw(20) << "Lien he" << '\n'
                     << string(106, '-') << '\n';
                app.customers.showAll();
                break;
            }
            case 2: {
                Customer c;
                c.inputNew();
                if (app.customers.findById(c.getId()) != nullptr) {
                    cout << "Loi: Ma khach hang da ton tai!\n";
                } else {
                    app.customers.add(c);
                    app.customers.save();
                    cout << "Them khach hang thanh cong!\n";
                }
                break;
            }
            case 3: {
                string id = readLine("Nhap ma khach hang can cap nhat: ");
                Customer* c = app.customers.findById(id);
                if (!c) {
                    cout << "Khong tim thay khach hang!\n";
                } else {
                    c->updateFields();
                    app.customers.save();
                    cout << "Cap nhat thanh cong!\n";
                }
                break;
            }
            case 4: {
                string id = readLine("Nhap ma khach hang can xoa: ");
                if (!app.customers.findById(id)) {
                    cout << "Khong tim thay khach hang!\n";
                } else {
                    app.customers.removeById(id);
                    app.customers.save();
                    cout << "Da xoa khach hang!\n";
                }
                break;
            }
            case 5: {
                string kw = readLine("Nhap Ma KH, Ten, SDT hoac CCCD can tim: ");
                bool found = false;
                for (const auto& c : app.customers.all()) {
                    if (c.getId().find(kw) != string::npos ||
                        c.getFullName().find(kw) != string::npos ||
                        c.getPhone().find(kw) != string::npos ||
                        c.getCccd().find(kw) != string::npos) {
                        c.displayRow();
                        found = true;
                    }
                }
                if (!found) cout << "Khong tim thay ket qua phu hop!\n";
                break;
            }
        }
    }
}