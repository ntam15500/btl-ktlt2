#include "Menus.h"

void menuIncidents(AppData& app) {
    while (true) {
        cout << "\n" << string(58, '=') << '\n'
             << "            QUAN LY SU CO INTERNET (UC9)\n"
             << string(58, '=') << '\n'
             << " 1. Danh sach su co\n"
             << " 2. Them su co moi\n"
             << " 3. Cap nhat thong tin su co\n"
             << " 4. Xoa su co\n"
             << " 0. Quay lai\n"
             << string(58, '-') << '\n';

        int chon = readChoice("Chon: ", 0, 4);
        if (chon == 0) return;

        try {
            if (chon == 1) {
                cout << "\n--- DANH SACH SU CO INTERNET ---\n";
                if (app.incidents.all().empty()) {
                    cout << "(Chua co su co nao duoc ghi nhan)\n";
                } else {
                    app.incidents.showAll();
                }
            }
            else if (chon == 2) {
                string maHd = readLine("Nhap ma hop dong gap su co: ");
                Contract* hopDong = app.contracts.findById(maHd);

                if (hopDong == nullptr) {
                    throw invalid_argument("Ma hop dong khong ton tai tren he thong!");
                }
                if (hopDong->getStatus() == "Da huy") {
                    throw invalid_argument("Hop dong da huy, khong the bao su co!");
                }

                Incident sc;
                sc.inputNew(maHd, hopDong->getCustomerId());

                if (app.incidents.findById(sc.getId()) != nullptr) {
                    throw invalid_argument("Ma su co nay da ton tai!");
                }

                app.incidents.add(sc);
                app.incidents.save();
                cout << "=> Tiep nhan su co thanh cong!\n";
            }
            else if (chon == 3) {
                string maTim = readLine("Nhap ma su co can cap nhat: ");
                Incident* sc = app.incidents.findById(maTim);

                if (sc == nullptr) {
                    cout << "Khong tim thay su co nay!\n";
                    continue;
                }

                cout << "Thong tin hien tai: ";
                sc->displayRow();

                sc->updateFields();
                app.incidents.save();
                cout << "=> Cap nhat su co thanh cong!\n";
            }
            else if (chon == 4) {
                string maXoa = readLine("Nhap ma su co can xoa: ");
                Incident* sc = app.incidents.findById(maXoa);

                if (sc == nullptr) {
                    cout << "Khong tim thay su co can xoa!\n";
                    continue;
                }

                sc->displayRow();
                string xacNhan = readLine("Xac nhan xoa (y/n): ");
                if (xacNhan == "y" || xacNhan == "Y") {
                    app.incidents.removeById(sc->getId());
                    app.incidents.save();
                    cout << "=> Da xoa su co thanh cong!\n";
                } else {
                    cout << "=> Da huy thao tac xoa.\n";
                }
            }
        }
        catch (const exception& e) {
            cout << "[Loi] " << e.what() << '\n';
        }
    }
}