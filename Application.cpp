#include "Menus.h"
// Menu chinh va dieu phoi cac use case
int runApplication() {
    AppData app;
    while (true) {
        cout << "\n" << string(58, '=') << '\n'
             << "          HE THONG QUAN LY DICH VU FTTH\n"
             << string(58, '=') << '\n'
             << " 1. Quan ly khach hang\n"
             << " 2. Dang ky dich vu Internet\n"
             << " 3. Quan ly goi cuoc\n"
             << " 4. Dang ky / chuyen doi goi cuoc\n"
             << " 5. Quan ly hop dong\n"
             << " 6. Gia han dich vu\n"
             << " 7. Quan ly thanh toan\n"
             << " 8. Quan ly hoa don cuoc\n"
             << " 9. Quan ly su co Internet\n"
             << "10. Ho tro ky thuat\n"
             << " 0. Thoat chuong trinh\n"
             << string(58, '-') << '\n';
        int choice = readChoice("Chon: ", 0, 10);
        switch (choice) {
            case 1: menuCustomers(app); break;
            case 2: menuRegistrations(app); break;
            case 3: menuPlans(app); break;
            case 4: menuPlanChanges(app); break;
            case 5: menuContracts(app); break;
            case 6: menuRenewals(app); break;
            case 7: menuPayments(app); break;
            case 8: menuInvoices(app); break;
            case 9: menuIncidents(app); break;
            case 10: menuSupports(app); break;
            case 0: cout << "Ket thuc chuong trinh.\n"; return 0;
        }
    }
}