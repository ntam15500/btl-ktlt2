#ifndef APP_DATA_H
#define APP_DATA_H

// Tac gia: [Dien ho ten sinh vien phu trach]
#include "Entities.h"
#include "Repository.h"

struct AppData {
    Repository<Customer> customers{"data/customers.txt"};
    Repository<Registration> registrations{"data/registrations.txt"};
    Repository<Plan> plans{"data/plans.txt"};
    Repository<PlanChange> planChanges{"data/plan_changes.txt"};
    Repository<Contract> contracts{"data/contracts.txt"};
    Repository<Renewal> renewals{"data/renewals.txt"};
    Repository<Payment> payments{"data/payments.txt"};
    Repository<Invoice> invoices{"data/invoices.txt"};
    Repository<Incident> incidents{"data/incidents.txt"};
    Repository<Support> supports{"data/supports.txt"};
};

#endif // APP_DATA_H
