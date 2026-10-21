#include "Menus.h"

const string& PlanChange::getContractId() const {
    return contractId_;
}
const string& PlanChange::getOldPlanId() const {
    return oldPlanId_;
}
const string& PlanChange::getNewPlanId() const {
    return newPlanId_;
}
const string& PlanChange::getDate() const {
    return date_;
}
const string& PlanChange::getStatus() const {
    return status_;
}
void PlanChange::setContractId(const string& value) {
    contractId_ = checkedText(value, "Ma hop dong");
}
void PlanChange::setOldPlanId(const string& value) {
    oldPlanId_ = checkedText(value, "Ma goi cu", true);
}
void PlanChange::setNewPlanId(const string& value) {
    newPlanId_ = checkedText(value, "Ma goi moi");
}
void PlanChange::setDate(const string& value) {
    date_ = checkedDateText(value, "Ngay thay doi");
}
void PlanChange::setStatus(const string& value) {
    status_ = checkedText(value, "Trang thai");
}

void PlanChange::inputNew() {
    setId(readUCText("Ma doi goi: "));
    setContractId(readUCText("Ma HD: "));
    setOldPlanId(readUCText("Ma goi cu: "));
    setNewPlanId(readUCText("Ma goi moi: "));
    setDate(readUCText("Ngay thay doi: "));
}

void PlanChange::updateFields() {
    updateOptionalField("Ma goi moi [" + newPlanId_ + "]: ", [this](const string& v) {
        setNewPlanId(v);
    });
    updateOptionalField("Trang thai [" + status_ + "]: ", [this](const string& v) {
        setStatus(v);
    });
}

void PlanChange::displayRow() const {
    displayCells({
        {getId(), 10},
        {contractId_, 10},
        {newPlanId_, 15},
        {date_, 12},
        {status_, 15}
    });
}

string PlanChange::toFileString() const {
    return getId() + "|" + contractId_ + "|" + oldPlanId_
        + "|" + newPlanId_ + "|" + date_ + "|" + status_;
}

bool PlanChange::fromFileString(const string& line) {
    string fields[6];
    return parseEntityRecord(line, fields, 6, [this](const string* values) {
        setId(values[0]);
        setContractId(values[1]);
        setOldPlanId(values[2]);
        setNewPlanId(values[3]);
        setDate(values[4]);
        setStatus(values[5]);
    });
}

namespace {
bool validDate(const string& date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') return false;
    for (size_t i = 0; i < date.size(); ++i) {
        if (i != 4 && i != 7 && (date[i] < '0' || date[i] > '9')) return false;
    }
    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));
    if (year < 1900 || month < 1 || month > 12) return false;
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) days[1] = 29;
    return day >= 1 && day <= days[month - 1];
}
}

// UC4: dang ky tao hop dong moi; chuyen doi cap nhat goi tren hop dong cu.
void menuPlanChanges(AppData& app) {
    while (true) {
        cout << "\n" << string(58, '=') << '\n'
             << "        DANG KY / CHUYEN DOI GOI CUOC (UC4)\n"
             << string(58, '=') << '\n'
             << " 1. Danh sach lich su dang ky / chuyen doi\n"
             << " 2. Dang ky goi moi (tao hop dong)\n"
             << " 3. Chuyen doi goi cua hop dong\n"
             << " 4. Huy lan chuyen doi gan nhat\n"
             << " 5. Tim kiem giao dich\n"
             << " 0. Quay lai\n"
             << string(58, '-') << '\n';

        int choice = readUCChoice("Chon: ", 0, 5);
        if (choice == 0) return;
        try {
            if (choice == 1) {
                cout << "\n--- LICH SU DANG KY / CHUYEN DOI GOI CUOC ---\n";
                app.planChanges.showAll();
            } else if (choice == 2 || choice == 3) {
                if (app.customers.all().empty() || app.plans.all().empty())
                    throw invalid_argument("Can them khach hang (UC1) va goi cuoc (UC3) truoc.");

                PlanChange change;
                change.setId(readUCText("Ma dang ky/chuyen doi: "));
                if (app.planChanges.findById(change.getId()))
                    throw invalid_argument("Ma dang ky/chuyen doi da ton tai.");

                Contract draft;
                Contract* current = nullptr;
                if (choice == 2) {
                    draft.setId(readUCText("Ma hop dong moi: "));
                    if (app.contracts.findById(draft.getId()))
                        throw invalid_argument("Ma hop dong da ton tai.");
                    draft.setCustomerId(readUCText("Ma KH: "));
                    if (!app.customers.findById(draft.getCustomerId()))
                        throw invalid_argument("Khach hang khong ton tai.");
                    change.setContractId(draft.getId());
                    change.setOldPlanId(""); // Dang ky moi chua co goi cu.
                } else {
                    change.setContractId(readUCText("Ma hop dong: "));
                    current = app.contracts.findById(change.getContractId());
                    if (!current) throw invalid_argument("Hop dong khong ton tai.");
                    if (current->getStatus() != "Dang hoat dong")
                        throw invalid_argument("Chi chuyen goi cho hop dong dang hoat dong.");
                    if (!app.customers.findById(current->getCustomerId()))
                        throw invalid_argument("Khach hang cua hop dong khong con ton tai.");
                    if (!app.plans.findById(current->getPlanId()))
                        throw invalid_argument("Goi hien tai khong ton tai; can kiem tra hop dong.");
                    change.setOldPlanId(current->getPlanId());
                }

                app.plans.showAll();
                change.setNewPlanId(readUCText("Ma goi moi: "));
                if (!app.plans.findById(change.getNewPlanId()))
                    throw invalid_argument("Goi moi khong ton tai.");
                if (change.getNewPlanId() == change.getOldPlanId())
                    throw invalid_argument("Goi moi trung voi goi dang su dung.");

                change.setDate(readUCText("Ngay hieu luc (YYYY-MM-DD): "));
                if (!validDate(change.getDate()))
                    throw invalid_argument("Ngay hieu luc khong hop le.");

                if (choice == 2) {
                    draft.setPlanId(change.getNewPlanId());
                    draft.setStartDate(change.getDate());
                    const string endDate = readUCText("Ngay ket thuc (YYYY-MM-DD): ");
                    if (!validDate(endDate) || endDate < draft.getStartDate())
                        throw invalid_argument("Ngay ket thuc khong hop le hoac truoc ngay bat dau.");
                    draft.setEndDate(endDate);
                } else {
                    if (!validDate(current->getStartDate()) || !validDate(current->getEndDate()) ||
                        change.getDate() < current->getStartDate() || change.getDate() > current->getEndDate())
                        throw invalid_argument("Ngay chuyen doi phai nam trong thoi han hop dong.");
                    for (const auto& row : app.planChanges.all()) {
                        if (row.getContractId() == change.getContractId() &&
                            row.getStatus() == "Da thuc hien" && row.getDate() > change.getDate())
                            throw invalid_argument("Ngay chuyen doi truoc giao dich da thuc hien.");
                    }
                }

                change.displayRow();
                if (!confirmUC()) continue;
                if (choice == 2) {
                    app.contracts.add(draft);
                } else {
                    current->setPlanId(change.getNewPlanId());
                    app.contracts.save();
                }
                app.planChanges.add(change);
                cout << "=> Thanh cong. Goi cuoc tren hop dong da duoc cap nhat!\n";
            } else if (choice == 4) {
                PlanChange* change = app.planChanges.findById(readUCText("Ma chuyen doi can huy: "));
                if (!change) throw invalid_argument("Khong tim thay giao dich.");
                if (change->getStatus() != "Da thuc hien")
                    throw invalid_argument("Giao dich da huy.");
                if (change->getOldPlanId().empty())
                    throw invalid_argument("Day la dang ky moi. Quan ly huy hop dong tai UC5.");

                const PlanChange* latest = nullptr;
                for (const auto& row : app.planChanges.all()) {
                    if (row.getContractId() == change->getContractId() && row.getStatus() == "Da thuc hien")
                        latest = &row;
                }
                if (latest != change)
                    throw invalid_argument("Chi duoc huy lan chuyen doi gan nhat chua huy.");

                Contract* contract = app.contracts.findById(change->getContractId());
                if (!contract || contract->getStatus() != "Dang hoat dong" ||
                    contract->getPlanId() != change->getNewPlanId())
                    throw invalid_argument("Hop dong da thay doi hoac khong hoat dong; khong the hoan tac.");
                if (!app.plans.findById(change->getOldPlanId()))
                    throw invalid_argument("Goi cu khong con ton tai.");

                change->displayRow();
                if (!confirmUC()) continue;
                contract->setPlanId(change->getOldPlanId());
                change->setStatus("Da huy"); // Giu lich su, khong xoa dau vet giao dich.
                app.contracts.save();
                app.planChanges.save();
                cout << "=> Da huy va khoi phuc goi cu tren hop dong!\n";
            } else if (choice == 5) {
                string keyword = readUCText("Tim theo ma giao dich, hop dong hoac goi: ");
                bool found = false;
                for (const auto& row : app.planChanges.all()) {
                    if (row.getId().find(keyword) != string::npos ||
                        row.getContractId().find(keyword) != string::npos ||
                        row.getOldPlanId().find(keyword) != string::npos ||
                        row.getNewPlanId().find(keyword) != string::npos) {
                        row.displayRow();
                        found = true;
                    }
                }
                if (!found) cout << "Khong tim thay ket qua phu hop!\n";
            }
        } catch (const exception& error) {
            cout << "[Loi] " << error.what() << '\n';
        }
    }
}
