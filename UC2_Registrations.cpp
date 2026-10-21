#include "Menus.h"

const string& Registration::getCustomerId() const {
    return customerId_;
}
const string& Registration::getPlanId() const {
    return planId_;
}
const string& Registration::getStatus() const {
    return status_;
}
void Registration::setCustomerId(const string& value) {
    customerId_ = checkedText(value, "Ma khach hang");
}
void Registration::setPlanId(const string& value) {
    planId_ = checkedText(value, "Ma goi cuoc");
}
void Registration::setRegDate(const string& value) {
    regDate_ = checkedDateText(value, "Ngay dang ky");
}
void Registration::setStatus(const string& value) {
    status_ = checkedText(value, "Trang thai");
}

void Registration::inputNew() {
    setId(readUCText("Ma DK: "));
    setCustomerId(readUCText("Ma KH: "));
    setPlanId(readUCText("Ma goi: "));
    setRegDate(readUCText("Ngay DK: "));
}

void Registration::updateFields() {
    updateOptionalField("Trang thai [" + status_ + "]: ", [this](const string& v) {
        setStatus(v);
    });
}

void Registration::displayRow() const {
    displayCells({
        {getId(), 10},
        {customerId_, 10},
        {planId_, 10},
        {regDate_, 12},
        {status_, 15}
    });
}

string Registration::toFileString() const {
    return getId() + "|" + customerId_ + "|" + planId_ + "|" + regDate_ + "|" + status_;
}

bool Registration::fromFileString(const string& line) {
    string fields[5];
    return parseEntityRecord(line, fields, 5, [this](const string* values) {
        setId(values[0]);
        setCustomerId(values[1]);
        setPlanId(values[2]);
        setRegDate(values[3]);
        setStatus(values[4]);
    });
}

void menuRegistrations(AppData& app) {
    while (true) {
        cout << "\n" << string(58, '=') << '\n'
             << "          DANG KY DICH VU INTERNET (UC2)\n"
             << string(58, '=') << '\n'
             << " 1. Danh sach don dang ky\n"
             << " 2. Them don dang ky moi\n"
             << " 3. Cap nhat don dang ky\n"
             << " 4. Xoa don dang ky\n"
             << " 0. Quay lai\n"
             << string(58, '-') << '\n';

        int chon = readChoice("Chon: ", 0, 4);
        if (chon == 0) return;

        try {
            if (chon == 1) {
                cout << "\n--- DANH SACH DON DANG KY INTERNET ---\n";
                app.registrations.showAll();
            }
            else if (chon == 2) {
                Registration don;
                don.inputNew();

                if (app.registrations.findById(don.getId()) != nullptr) {
                    throw invalid_argument("Ma don dang ky nay da ton tai!");
                }

                if (app.customers.findById(don.getCustomerId()) == nullptr) {
                    throw invalid_argument("Ma khach hang khong ton tai! Hay tao KH o UC1 truoc.");
                }

                if (app.plans.findById(don.getPlanId()) == nullptr) {
                    throw invalid_argument("Ma goi cuoc khong ton tai trong he thong!");
                }

                app.registrations.add(don);
                cout << "=> Them don dang ky thanh cong!\n";
            }
            else if (chon == 3) {
                string maSua = readLine("Nhap ma don can cap nhat: ");
                Registration* don = app.registrations.findById(maSua);

                if (don == nullptr) {
                    cout << "Khong tim thay don nay!\n";
                    continue;
                }

                Registration tam = *don;
                tam.updateFields();

                if (app.customers.findById(tam.getCustomerId()) == nullptr) {
                    throw invalid_argument("Ma khach hang moi khong ton tai!");
                }
                if (app.plans.findById(tam.getPlanId()) == nullptr) {
                    throw invalid_argument("Ma goi cuoc moi khong ton tai!");
                }

                *don = tam;
                app.registrations.save();
                cout << "=> Cap nhat don dang ky thanh cong!\n";
            }
            else if (chon == 4) {
                string maXoa = readLine("Nhap ma don can xoa: ");
                Registration* don = app.registrations.findById(maXoa);

                if (don == nullptr) {
                    cout << "Khong tim thay don can xoa!\n";
                    continue;
                }

                don->displayRow();
                string xacNhan = readLine("Xac nhan xoa (y/n): ");
                if (xacNhan == "y" || xacNhan == "Y") {
                    app.registrations.removeById(don->getId());
                    cout << "=> Da xoa don dang ky thanh cong!\n";
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
