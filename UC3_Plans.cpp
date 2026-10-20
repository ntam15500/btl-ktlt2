#include "Menus.h"

void menuPlans(AppData& app) {
    while (true) {
        cout << "\n" << string(58, '=') << '\n'
             << "               QUAN LY GOI CUOC (UC3)\n"
             << string(58, '=') << '\n'
             << " 1. Danh sach goi cuoc\n"
             << " 2. Them goi cuoc moi\n"
             << " 3. Cap nhat goi cuoc\n"
             << " 4. Xoa goi cuoc\n"
             << " 5. Tim kiem goi cuoc\n"
             << " 0. Quay lai\n"
             << string(58, '-') << '\n';

        int choice = readChoice("Chon: ", 0, 5);
        if (choice == 0) return;

        switch (choice) {
            case 1: {
                cout << "\n--- DANH SACH GOI CUOC ---\n" << left
                     << setw(12) << "Ma Goi"
                     << setw(22) << "Ten Goi"
                     << setw(16) << "Toc Do"
                     << setw(18) << "Gia Cuoc"
                     << setw(15) << "Thoi Han" << '\n'
                     << string(83, '-') << '\n';
                app.plans.showAll();
                break;
            }
            case 2: {
                Plan p;
                p.inputNew();
                if (app.plans.findById(p.getId()) != nullptr) {
                    cout << "[Loi] Ma goi cuoc da ton tai!\n";
                } else {
                    app.plans.add(p);
                    app.plans.save();
                    cout << "=> Them goi cuoc thanh cong!\n";
                }
                break;
            }
            case 3: {
                string id = readLine("Nhap ma goi cuoc can cap nhat: ");
                Plan* p = app.plans.findById(id);
                if (!p) {
                    cout << "Khong tim thay goi cuoc!\n";
                } else {
                    p->updateFields();
                    app.plans.save();
                    cout << "=> Cap nhat goi cuoc thanh cong!\n";
                }
                break;
            }
            case 4: {
                string id = readLine("Nhap ma goi cuoc can xoa: ");
                if (!app.plans.findById(id)) {
                    cout << "Khong tim thay goi cuoc!\n";
                } else {
                    app.plans.removeById(id);
                    app.plans.save();
                    cout << "=> Da xoa goi cuoc thanh cong!\n";
                }
                break;
            }
            case 5: {
                string kw = readLine("Nhap Ma goi hoac Ten goi can tim: ");
                bool found = false;
                for (const auto& p : app.plans.all()) {
                    if (p.getId().find(kw) != string::npos ||
                        p.getName().find(kw) != string::npos) {
                        p.displayRow();
                        found = true;
                    }
                }
                if (!found) cout << "Khong tim thay goi cuoc phu hop!\n";
                break;
            }
        }
    }
}
