#include "Menus.h"

void menuRenewals(AppData& app) {
    while (true) {
        cout << "\n===== GIA HAN DICH VU (UC 6) =====\n"
             << "1. Them\n2. Xem danh sach\n3. Sua\n4. Xoa\n0. Quay lai\n";
        int ch = readChoice("Chon: ", 0, 4);
        if (ch == 0) return;
        try {
            if (ch == 1) {
                Contract* c = app.contracts.findById(readLine("Nhap ma HD can gia han: "));
                if (!c) throw invalid_argument("Ma hop dong khong ton tai.");
                if (c->getStatus() == "Da huy") throw invalid_argument("Hop dong da huy.");
                Renewal r; r.inputNew(c->getId(), c->getEndDate());
                if (app.renewals.findById(r.getId())) throw invalid_argument("Ma dinh danh da ton tai.");
                c->setEndDate(r.getNewEndDate()); c->setStatus("Dang hoat dong");
                app.renewals.add(r);
                cout << "Them thanh cong. Han moi: " << r.getNewEndDate() << "\n";
            } else if (ch == 2) {
                app.renewals.showAll();
            } else if (ch == 3) {
                Renewal* cur = app.renewals.findById(readLine("Ma can sua: "));
                if (!cur) { cout << "Khong tim thay ban ghi.\n"; continue; }
                cur->updateFields();
                if (Contract* c = app.contracts.findById(cur->getContractId())) c->setEndDate(cur->getNewEndDate());
                cout << "Cap nhat thanh cong.\n";
            } else if (ch == 4) {
                Renewal* cur = app.renewals.findById(readLine("Ma can xoa: "));
                if (!cur) { cout << "Khong tim thay ban ghi.\n"; continue; }
                cur->displayRow();
                string ans = readLine("Xac nhan xoa (y/n): ");
                if (ans == "y" || ans == "Y") {
                    app.renewals.removeById(cur->getId());
                    cout << "Xoa thanh cong.\n";
                }
            }
        } catch (const exception& e) { cout << "[Loi] " << e.what() << '\n'; }
    }
}