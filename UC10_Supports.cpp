#include "Menus.h"

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
                if (app.supports.findById(request.id))
                    throw invalid_argument("Ma ho tro da ton tai.");
                if (!app.customers.findById(request.customerId))
                    throw invalid_argument("Khach hang khong ton tai.");
                // UC9 chua lam van tao duoc ho tro truc tiep cho khach hang.
                if (!request.incidentId.empty()) {
                    Incident* incident = app.incidents.findById(request.incidentId);
                    if (!incident) throw invalid_argument("Ma su co khong ton tai.");
                    if (incident->customerId != request.customerId)
                        throw invalid_argument("Su co khong thuoc khach hang nay.");
                    if (incident->status == "Da xu ly" || incident->status == "Da dong")
                        throw invalid_argument("Su co da xu ly hoac da dong.");
                    for (const auto& row : app.supports.all()) {
                        if (row.incidentId == request.incidentId &&
                            row.status != "Hoan tat" && row.status != "Da huy")
                            throw invalid_argument("Su co da co yeu cau ho tro dang mo.");
                    }
                }
                request.history = "Tiep nhan yeu cau";
                request.displayRow();
                if (!confirmUC()) continue;
                app.supports.add(request);
                app.supports.save();
                cout << "=> Da tao yeu cau thanh cong, cho phan cong nhan vien!\n";
            } else if (choice == 6) {
                string keyword = readUCText("Tim theo ma ho tro, KH, su co, nhan vien hoac trang thai: ");
                bool found = false;
                for (const auto& row : app.supports.all()) {
                    if (row.id.find(keyword) != string::npos || row.customerId.find(keyword) != string::npos ||
                        row.incidentId.find(keyword) != string::npos || row.technician.find(keyword) != string::npos ||
                        row.status.find(keyword) != string::npos) {
                        row.displayRow();
                        found = true;
                    }
                }
                if (!found) cout << "Khong tim thay ket qua phu hop!\n";
            } else {
                Support* current = app.supports.findById(readUCText("Ma ho tro: "));
                if (!current) throw invalid_argument("Khong tim thay yeu cau.");
                if (current->status == "Hoan tat" || current->status == "Da huy")
                    throw invalid_argument("Yeu cau da ket thuc, khong the sua/huy.");
                Support updated = *current; // Chi ghi de sau khi kiem tra va xac nhan.
                Incident* incident = nullptr;
                if (!current->incidentId.empty())
                    incident = app.incidents.findById(current->incidentId);
                if (choice != 5) {
                    if (!app.customers.findById(current->customerId))
                        throw invalid_argument("Khach hang cua yeu cau khong con ton tai.");
                    if (!current->incidentId.empty() && !incident)
                        throw invalid_argument("Su co lien ket khong con ton tai.");
                    if (incident && (incident->status == "Da xu ly" || incident->status == "Da dong"))
                        throw invalid_argument("Su co da ket thuc; hay kiem tra hoac huy yeu cau.");
                }
                if (choice == 3) {
                    updated.updateFields();
                    if (updated.status == "Cho phan cong") updated.status = "Da phan cong";
                    updated.history += " -> Phan cong: " + updated.technician;
                    if (!updated.note.empty()) updated.history += " (" + updated.note + ")";
                } else if (choice == 4) {
                    if (current->technician.empty() || current->status == "Cho phan cong")
                        throw invalid_argument("Phai phan cong nhan vien truoc khi xu ly.");
                    int step = readUCChoice("Tien do (1.Dang xu ly 2.Hoan tat): ", 1, 2);
                    if (step == 2 && current->status != "Dang xu ly")
                        throw invalid_argument("Phai chuyen sang Dang xu ly truoc khi Hoan tat.");
                    updated.note = readUCText("Ghi chu / ket qua (bat buoc khi hoan tat): ", step == 2);
                    updated.status = step == 1 ? "Dang xu ly" : "Hoan tat";
                    updated.history += " -> " + updated.status;
                    if (!updated.note.empty()) updated.history += ": " + updated.note;
                } else if (choice == 5) {
                    updated.note = readUCText("Ly do huy: ");
                    updated.status = "Da huy";
                    updated.history += " -> Da huy: " + updated.note;
                }
                updated.displayRow();
                if (!confirmUC()) continue;
                *current = updated;
                // Huy ho tro khong co nghia su co da duoc khac phuc.
                if (incident && choice == 4)
                    incident->status = updated.status == "Hoan tat" ? "Da xu ly" : "Dang xu ly";
                if (incident && choice == 5 && incident->status == "Dang xu ly")
                    incident->status = "Moi tiep nhan";
                app.supports.save();
                app.incidents.save();
                cout << "=> Cap nhat yeu cau ho tro thanh cong!\n";
            }
        } catch (const exception& error) {
            cout << "[Loi] " << error.what() << '\n';
        }
    }
}