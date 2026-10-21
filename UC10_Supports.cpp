#include "Menus.h"

const string& Support::getCustomerId() const {
    return customerId_;
}
const string& Support::getIncidentId() const {
    return incidentId_;
}
const string& Support::getTechnician() const {
    return technician_;
}
const string& Support::getNote() const {
    return note_;
}
const string& Support::getHistory() const {
    return history_;
}
const string& Support::getStatus() const {
    return status_;
}
void Support::setCustomerId(const string& value) {
    customerId_ = checkedText(value, "Ma khach hang");
}
void Support::setIncidentId(const string& value) {
    incidentId_ = checkedText(value, "Ma su co", true);
}
void Support::setTechnician(const string& value) {
    technician_ = checkedText(value, "Ky thuat vien", true);
}
void Support::setNote(const string& value) {
    note_ = checkedText(value, "Ghi chu", true);
}
void Support::setHistory(const string& value) {
    history_ = checkedText(value, "Lich su xu ly", true);
}
void Support::setStatus(const string& value) {
    status_ = checkedText(value, "Trang thai ho tro");
}

void Support::inputNew() {
    setId(readUCText("Ma HT: "));
    setCustomerId(readUCText("Ma KH: "));
    setIncidentId(readUCText("Ma SC (co the de trong): ", false));
    setNote(readUCText("Ghi chu: ", false));
    setTechnician("");
    setHistory("");
}

void Support::updateFields() {
    updateOptionalField("KTV [" + technician_ + "]: ", [this](const string& v) {
        setTechnician(v);
    });
    updateOptionalField("Ghi chu [" + note_ + "]: ", [this](const string& v) {
        setNote(v);
    });
    updateOptionalField("Lich su [" + history_ + "]: ", [this](const string& v) {
        setHistory(v);
    });
    updateOptionalField("Trang thai [" + status_ + "]: ", [this](const string& v) {
        setStatus(v);
    });
}

void Support::displayRow() const {
    displayCells({
        {getId(), 10},
        {customerId_, 10},
        {status_, 15},
        {history_, 0}
    });
}

string Support::toFileString() const {
    return getId() + "|" + customerId_ + "|" + incidentId_
        + "|" + technician_ + "|" + note_ + "|" + history_ + "|" + status_;
}

bool Support::fromFileString(const string& line) {
    string fields[7];
    return parseEntityRecord(line, fields, 7, [this](const string* values) {
        setId(values[0]);
        setCustomerId(values[1]);
        setIncidentId(values[2]);
        setTechnician(values[3]);
        setNote(values[4]);
        setHistory(values[5]);
        setStatus(values[6]);
    });
}

// UC10: tiep nhan -> phan cong -> dang xu ly -> hoan tat / huy.
void menuSupports(AppData& app) {
    while (true) {
        cout << "\n" << string(58, '=') << '\n'
             << "       XU LY YEU CAU HO TRO KY THUAT (UC10)\n"
             << string(58, '=') << '\n'
             << " 1. Danh sach va lich su ho tro\n"
             << " 2. Tao yeu cau ho tro moi\n"
             << " 3. Phan cong / doi nhan vien ky thuat\n"
             << " 4. Cap nhat tien do / ket qua\n"
             << " 5. Huy yeu cau ho tro\n"
             << " 6. Tim kiem yeu cau ho tro\n"
             << " 0. Quay lai\n"
             << string(58, '-') << '\n';

        int choice = readUCChoice("Chon: ", 0, 6);
        if (choice == 0) return;
        try {
            if (choice == 1) {
                cout << "\n--- DANH SACH VA LICH SU YEU CAU HO TRO ---\n";
                app.supports.showAll();
            } else if (choice == 2) {
                if (app.customers.all().empty())
                    throw invalid_argument("Can them khach hang o UC1 truoc.");
                Support request;
                request.inputNew();
                if (app.supports.findById(request.getId()))
                    throw invalid_argument("Ma ho tro da ton tai.");
                if (!app.customers.findById(request.getCustomerId()))
                    throw invalid_argument("Khach hang khong ton tai.");

                // UC9 chua lam van tao duoc ho tro truc tiep cho khach hang.
                if (!request.getIncidentId().empty()) {
                    Incident* incident = app.incidents.findById(request.getIncidentId());
                    if (!incident) throw invalid_argument("Ma su co khong ton tai.");
                    if (incident->getCustomerId() != request.getCustomerId())
                        throw invalid_argument("Su co khong thuoc khach hang nay.");
                    if (incident->getStatus() == "Da xu ly" || incident->getStatus() == "Da dong")
                        throw invalid_argument("Su co da xu ly hoac da dong.");
                    for (const auto& row : app.supports.all()) {
                        if (row.getIncidentId() == request.getIncidentId() &&
                            row.getStatus() != "Hoan tat" && row.getStatus() != "Da huy")
                            throw invalid_argument("Su co da co yeu cau ho tro dang mo.");
                    }
                }
                request.setHistory("Tiep nhan yeu cau");
                request.displayRow();
                if (!confirmUC()) continue;
                app.supports.add(request);
                cout << "=> Da tao yeu cau thanh cong, cho phan cong nhan vien!\n";
            } else if (choice == 6) {
                string keyword = readUCText("Tim theo ma ho tro, KH, su co, nhan vien hoac trang thai: ");
                bool found = false;
                for (const auto& row : app.supports.all()) {
                    if (row.getId().find(keyword) != string::npos ||
                        row.getCustomerId().find(keyword) != string::npos ||
                        row.getIncidentId().find(keyword) != string::npos ||
                        row.getTechnician().find(keyword) != string::npos ||
                        row.getStatus().find(keyword) != string::npos) {
                        row.displayRow();
                        found = true;
                    }
                }
                if (!found) cout << "Khong tim thay ket qua phu hop!\n";
            } else {
                Support* current = app.supports.findById(readUCText("Ma ho tro: "));
                if (!current) throw invalid_argument("Khong tim thay yeu cau.");
                if (current->getStatus() == "Hoan tat" || current->getStatus() == "Da huy")
                    throw invalid_argument("Yeu cau da ket thuc, khong the sua/huy.");

                Support updated = *current; // Chi ghi de sau khi kiem tra va xac nhan.
                Incident* incident = nullptr;
                if (!current->getIncidentId().empty())
                    incident = app.incidents.findById(current->getIncidentId());

                if (choice != 5) {
                    if (!app.customers.findById(current->getCustomerId()))
                        throw invalid_argument("Khach hang cua yeu cau khong con ton tai.");
                    if (!current->getIncidentId().empty() && !incident)
                        throw invalid_argument("Su co lien ket khong con ton tai.");
                    if (incident && (incident->getStatus() == "Da xu ly" || incident->getStatus() == "Da dong"))
                        throw invalid_argument("Su co da ket thuc; hay kiem tra hoac huy yeu cau.");
                }

                if (choice == 3) {
                    updated.updateFields();
                    if (updated.getStatus() == "Cho phan cong") updated.setStatus("Da phan cong");
                    updated.setHistory(updated.getHistory() + " -> Phan cong: " + updated.getTechnician());
                    if (!updated.getNote().empty())
                        updated.setHistory(updated.getHistory() + " (" + updated.getNote() + ")");
                } else if (choice == 4) {
                    if (current->getTechnician().empty() || current->getStatus() == "Cho phan cong")
                        throw invalid_argument("Phai phan cong nhan vien truoc khi xu ly.");
                    int step = readUCChoice("Tien do (1.Dang xu ly 2.Hoan tat): ", 1, 2);
                    if (step == 2 && current->getStatus() != "Dang xu ly")
                        throw invalid_argument("Phai chuyen sang Dang xu ly truoc khi Hoan tat.");
                    updated.setNote(readUCText("Ghi chu / ket qua (bat buoc khi hoan tat): ", step == 2));
                    updated.setStatus(step == 1 ? "Dang xu ly" : "Hoan tat");
                    updated.setHistory(updated.getHistory() + " -> " + updated.getStatus());
                    if (!updated.getNote().empty())
                        updated.setHistory(updated.getHistory() + ": " + updated.getNote());
                } else if (choice == 5) {
                    updated.setNote(readUCText("Ly do huy: "));
                    updated.setStatus("Da huy");
                    updated.setHistory(updated.getHistory() + " -> Da huy: " + updated.getNote());
                }

                updated.displayRow();
                if (!confirmUC()) continue;
                *current = updated;
                // Huy ho tro khong co nghia su co da duoc khac phuc.
                if (incident && choice == 4)
                    incident->setStatus(updated.getStatus() == "Hoan tat" ? "Da xu ly" : "Dang xu ly");
                if (incident && choice == 5 && incident->getStatus() == "Dang xu ly")
                    incident->setStatus("Moi tiep nhan");
                app.supports.save();
                app.incidents.save();
                cout << "=> Cap nhat yeu cau ho tro thanh cong!\n";
            }
        } catch (const exception& error) {
            cout << "[Loi] " << error.what() << '\n';
        }
    }
}
