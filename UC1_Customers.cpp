#include "Menus.h"

const string& Customer::getFullName() const {
    return fullName_;
}
const string& Customer::getPhone() const {
    return phone_;
}
const string& Customer::getCccd() const {
    return cccd_;
}
void Customer::setFullName(const string& value) {
    fullName_ = checkedText(value, "Ho ten");
}
void Customer::setPhone(const string& value) {
    phone_ = checkedText(value, "So dien thoai");
}
void Customer::setCccd(const string& value) {
    cccd_ = checkedText(value, "CCCD");
}
void Customer::setAddress(const string& value) {
    address_ = checkedText(value, "Dia chi");
}
void Customer::setContact(const string& value) {
    contact_ = checkedText(value, "Thong tin lien he", true);
}

void Customer::inputNew() {
    setId(readUCText("Ma KH: "));
    setFullName(readUCText("Ho ten: "));
    setPhone(readUCText("SDT: "));
    setCccd(readUCText("CCCD: "));
    setAddress(readUCText("Dia chi: "));
    setContact(readUCText("Lien he (co the de trong): ", false));
}

void Customer::updateFields() {
    updateOptionalField("Ho ten [" + fullName_ + "]: ", [this](const string& v) {
        setFullName(v);
    });
    updateOptionalField("SDT [" + phone_ + "]: ", [this](const string& v) {
        setPhone(v);
    });
    updateOptionalField("Dia chi [" + address_ + "]: ", [this](const string& v) {
        setAddress(v);
    });
    updateOptionalField("Lien he [" + contact_ + "]: ", [this](const string& v) {
        setContact(v);
    });
}

void Customer::displayRow() const {
    displayCells({
        {getId(), 10},
        {fullName_, 22},
        {phone_, 14},
        {cccd_, 15},
        {address_, 25},
        {contact_, 20}
    });
}

string Customer::toFileString() const {
    return getId() + "|" + fullName_ + "|" + phone_
        + "|" + cccd_ + "|" + address_ + "|" + contact_;
}

bool Customer::fromFileString(const string& line) {
    string fields[6];
    return parseEntityRecord(line, fields, 6, [this](const string* values) {
        setId(values[0]);
        setFullName(values[1]);
        setPhone(values[2]);
        setCccd(values[3]);
        setAddress(values[4]);
        setContact(values[5]);
    });
}

void menuCustomers(AppData& app) {
    while (true) {
        cout << "\n" << string(58, '=') << '\n'
             << "             QUAN LY KHACH HANG (UC1)\n"
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
                cout << "\n--- DANH SACH KHACH HANG ---\n" << left
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
                    cout << "[Loi] Ma khach hang da ton tai!\n";
                } else {
                    app.customers.add(c);
                    cout << "=> Them khach hang thanh cong!\n";
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
                    cout << "=> Cap nhat khach hang thanh cong!\n";
                }
                break;
            }
            case 4: {
                string id = readLine("Nhap ma khach hang can xoa: ");
                if (!app.customers.findById(id)) {
                    cout << "Khong tim thay khach hang!\n";
                } else {
                    app.customers.removeById(id);
                    cout << "=> Da xoa khach hang thanh cong!\n";
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
