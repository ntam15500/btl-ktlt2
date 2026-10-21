#ifndef MENUS_H
#define MENUS_H

// Tac gia: [Dien ho ten sinh vien phu trach]
#include "InputHelper.h"
#include "AppData.h"

// Khai bao cac menu use case va ham dieu phoi ung dung.
int runApplication();
void syncInvoiceStatus(AppData& app, const string& invoiceId);
void menuCustomers(AppData& app);
void menuRegistrations(AppData& app);
void menuPlans(AppData& app);
void menuPlanChanges(AppData& app);
void menuContracts(AppData& app);
void menuRenewals(AppData& app);
void menuPayments(AppData& app);
void menuInvoices(AppData& app);
void menuIncidents(AppData& app);
void menuSupports(AppData& app);

#endif // MENUS_H
