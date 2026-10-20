#include "Menus.h"

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
                change.id = readUCText("Ma dang ky/chuyen doi: ");
                if (app.planChanges.findById(change.id))
                    throw invalid_argument("Ma dang ky/chuyen doi da ton tai.");

                Contract draft;
                Contract* current = nullptr;
                if (choice == 2) {
                    draft.id = readUCText("Ma hop dong moi: ");
                    if (app.contracts.findById(draft.id))
                        throw invalid_argument("Ma hop dong da ton tai.");
                    draft.customerId = readUCText("Ma KH: ");
                    if (!app.customers.findById(draft.customerId))
                        throw invalid_argument("Khach hang khong ton tai.");
                    change.contractId = draft.id;
                } else {
                    change.contractId = readUCText("Ma hop dong: ");
                    current = app.contracts.findById(change.contractId);
                    if (!current) throw invalid_argument("Hop dong khong ton tai.");
                    if (current->status != "Dang hoat dong")
                        throw invalid_argument("Chi chuyen goi cho hop dong dang hoat dong.");
                    if (!app.customers.findById(current->customerId))
                        throw invalid_argument("Khach hang cua hop dong khong con ton tai.");
                    if (!app.plans.findById(current->planId))
                        throw invalid_argument("Goi hien tai khong ton tai; can kiem tra hop dong.");
                    change.oldPlanId = current->planId; // Khong cho nhap tuy y goi cu.
                }
                app.plans.showAll();
                change.newPlanId = readUCText("Ma goi moi: ");
                if (!app.plans.findById(change.newPlanId))
                    throw invalid_argument("Goi moi khong ton tai.");
                if (change.newPlanId == change.oldPlanId)
                    throw invalid_argument("Goi moi trung voi goi dang su dung.");
                change.date = readUCText("Ngay hieu luc (YYYY-MM-DD): ");
                if (!validDate(change.date))
                    throw invalid_argument("Ngay hieu luc khong hop le.");
                if (choice == 2) {
                    draft.planId = change.newPlanId;
                    draft.startDate = change.date;
                    draft.endDate = readUCText("Ngay ket thuc (YYYY-MM-DD): ");
                    if (!validDate(draft.endDate) || draft.endDate < draft.startDate)
                        throw invalid_argument("Ngay ket thuc khong hop le hoac truoc ngay bat dau.");
                } else {
                    if (!validDate(current->startDate) || !validDate(current->endDate) ||
                        change.date < current->startDate || change.date > current->endDate)
                        throw invalid_argument("Ngay chuyen doi phai nam trong thoi han hop dong.");
                    for (const auto& row : app.planChanges.all()) {
                        if (row.contractId == change.contractId && row.status == "Da thuc hien" &&
                            row.date > change.date)
                            throw invalid_argument("Ngay chuyen doi truoc giao dich da thuc hien.");
                    }
                }
                change.displayRow();
                if (!confirmUC()) continue;
                if (choice == 2) app.contracts.add(draft);
                else current->planId = change.newPlanId;
                app.planChanges.add(change);
                app.contracts.save();
                app.planChanges.save();
                cout << "=> Thanh cong. Goi cuoc tren hop dong da duoc cap nhat!\n";
            } else if (choice == 4) {
                PlanChange* change = app.planChanges.findById(readUCText("Ma chuyen doi can huy: "));
                if (!change) throw invalid_argument("Khong tim thay giao dich.");
                if (change->status != "Da thuc hien")
                    throw invalid_argument("Giao dich da huy.");
                if (change->oldPlanId.empty())
                    throw invalid_argument("Day la dang ky moi. Quan ly huy hop dong tai UC5.");
                const PlanChange* latest = nullptr;
                for (const auto& row : app.planChanges.all()) {
                    if (row.contractId == change->contractId && row.status == "Da thuc hien")
                        latest = &row;
                }
                if (latest != change)
                    throw invalid_argument("Chi duoc huy lan chuyen doi gan nhat chua huy.");
                Contract* contract = app.contracts.findById(change->contractId);
                if (!contract || contract->status != "Dang hoat dong" || contract->planId != change->newPlanId)
                    throw invalid_argument("Hop dong da thay doi hoac khong hoat dong; khong the hoan tac.");
                if (!app.plans.findById(change->oldPlanId))
                    throw invalid_argument("Goi cu khong con ton tai.");
                change->displayRow();
                if (!confirmUC()) continue;
                contract->planId = change->oldPlanId;
                change->status = "Da huy"; // Giu lich su, khong xoa dau vet giao dich.
                app.contracts.save();
                app.planChanges.save();
                cout << "=> Da huy va khoi phuc goi cu tren hop dong!\n";
            } else if (choice == 5) {
                string keyword = readUCText("Tim theo ma giao dich, hop dong hoac goi: ");
                bool found = false;
                for (const auto& row : app.planChanges.all()) {
                    if (row.id.find(keyword) != string::npos || row.contractId.find(keyword) != string::npos ||
                        row.oldPlanId.find(keyword) != string::npos || row.newPlanId.find(keyword) != string::npos) {
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
