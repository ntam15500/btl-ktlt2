#ifndef ENTITIES_H
#define ENTITIES_H
// Tac gia: Nguyen Duc Tam

#include "InputHelper.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <initializer_list>
#include <utility>
using namespace std;

inline void displayCells(initializer_list<pair<string, int>> cells) {
    for (const auto& cell : cells) {
        cout << left
             << setw(cell.second)
             << cell.first;
    }
    cout << '\n';
}

template <typename AssignFields>
bool parseEntityRecord(const string& line, string* fields, int count,
                       AssignFields assignFields) {
    if (!splitPipe(line, fields, count)) {
        return false;
    }
    try {
        assignFields(fields);
        return true;
    } catch (const exception&) {
        return false;
    }
}

inline string checkedText(const string& value, const string& fieldName,
                          bool allowEmpty = false) {
    validateText(value, fieldName, allowEmpty);
    return trimText(value);
}

inline string checkedDateText(const string& value, const string& fieldName) {
    validateDateText(value, fieldName);
    return trimText(value);
}

inline string checkedPositiveNumber(const string& value, const string& fieldName) {
    validatePositiveNumber(value, fieldName);
    return trimText(value);
}

class Entity {
private:
    string id_;
public:
    virtual ~Entity() = default;
    const string& getId() const {
        return id_;
    }
    void setId(const string& value) {
        id_ = checkedText(value, "Ma dinh danh");
    }
    virtual void displayRow() const = 0;
    virtual string toFileString() const = 0;
    virtual bool fromFileString(const string& line) = 0;
};

class Customer : public Entity {
private:
    string fullName_;
    string phone_;
    string cccd_;
    string address_;
    string contact_;
public:
    const string& getFullName() const;
    const string& getPhone() const;
    const string& getCccd() const;
    const string& getAddress() const { return address_; }
    const string& getContact() const { return contact_; }

    void setFullName(const string& value);
    void setPhone(const string& value);
    void setCccd(const string& value);
    void setAddress(const string& value);
    void setContact(const string& value);

    void inputNew();
    void updateFields();
    void displayRow() const override;
    string toFileString() const override;
    bool fromFileString(const string& line) override;
};

class Registration : public Entity {
private:
    string customerId_;
    string planId_;
    string regDate_;
    string status_ = "Cho lap dat";
public:
    const string& getCustomerId() const;
    const string& getPlanId() const;
    const string& getRegDate() const { return regDate_; }
    const string& getStatus() const;

    void setCustomerId(const string& value);
    void setPlanId(const string& value);
    void setRegDate(const string& value);
    void setStatus(const string& value);

    void inputNew();
    void updateFields();
    void displayRow() const override;
    string toFileString() const override;
    bool fromFileString(const string& line) override;
};

class Plan : public Entity {
private:
    string name_;
    string speed_;
    string price_;
    string duration_;
public:
    const string& getName() const;
    const string& getSpeed() const { return speed_; }
    const string& getPrice() const { return price_; }
    const string& getDuration() const { return duration_; }

    void setName(const string& value);
    void setSpeed(const string& value);
    void setPrice(const string& value);
    void setDuration(const string& value);

    void inputNew();
    void updateFields();
    void displayRow() const override;
    string toFileString() const override;
    bool fromFileString(const string& line) override;
};

class PlanChange : public Entity {
private:
    string contractId_;
    string oldPlanId_;
    string newPlanId_;
    string date_;
    string status_ = "Da thuc hien";
public:
    const string& getContractId() const;
    const string& getOldPlanId() const;
    const string& getNewPlanId() const;
    const string& getDate() const;
    const string& getStatus() const;

    void setContractId(const string& value);
    void setOldPlanId(const string& value);
    void setNewPlanId(const string& value);
    void setDate(const string& value);
    void setStatus(const string& value);

    void inputNew();
    void updateFields();
    void displayRow() const override;
    string toFileString() const override;
    bool fromFileString(const string& line) override;
};

class Contract : public Entity {
private:
    string customerId_;
    string planId_;
    string startDate_;
    string endDate_;
    string status_ = "Dang hoat dong";
public:
    const string& getCustomerId() const;
    const string& getPlanId() const;
    const string& getStartDate() const;
    const string& getEndDate() const;
    const string& getStatus() const;

    void setCustomerId(const string& value);
    void setPlanId(const string& value);
    void setStartDate(const string& value);
    void setEndDate(const string& value);
    void setStatus(const string& value);

    void inputNew();
    void updateFields();
    void displayRow() const override;
    string toFileString() const override;
    bool fromFileString(const string& line) override;
};

class Renewal : public Entity {
private:
    string contractId_;
    string oldEnd_;
    string newEnd_;
    string status_ = "Thanh cong";
    int months_ = 1;
public:
    const string& getContractId() const;
    const string& getOldEndDate() const { return oldEnd_; }
    const string& getNewEndDate() const;
    const string& getStatus() const;
    int getMonths() const { return months_; }

    void setContractId(const string& value);
    void setOldEndDate(const string& value);
    void setNewEndDate(const string& value);
    void setStatus(const string& value);
    void setMonths(int value);

    void inputNew(const string& contractId, const string& currentEndDate);
    void updateFields();
    void displayRow() const override;
    string toFileString() const override;
    bool fromFileString(const string& line) override;
};

class Payment : public Entity {
private:
    string invoiceId_;
    string amount_;
    string status_ = "Thanh cong";
public:
    const string& getInvoiceId() const;
    const string& getAmount() const { return amount_; }
    const string& getStatus() const;

    void setInvoiceId(const string& value);
    void setAmount(const string& value);
    void setStatus(const string& value);

    void inputNew();
    void updateFields();
    void displayRow() const override;
    string toFileString() const override;
    bool fromFileString(const string& line) override;
};

class Invoice : public Entity {
private:
    string customerId_;
    string amount_;
    string status_ = "Chua thanh toan";
public:
    const string& getCustomerId() const;
    const string& getAmount() const { return amount_; }
    const string& getStatus() const;

    void setCustomerId(const string& value);
    void setAmount(const string& value);
    void setStatus(const string& value);

    void inputNew();
    void updateFields();
    void displayRow() const override;
    string toFileString() const override;
    bool fromFileString(const string& line) override;
};

class Incident : public Entity {
private:
    string contractId_;
    string customerId_;
    string description_;
    string status_ = "Moi tiep nhan";
public:
    const string& getContractId() const;
    const string& getCustomerId() const;
    const string& getDescription() const { return description_; }
    const string& getStatus() const;

    void setContractId(const string& value);
    void setCustomerId(const string& value);
    void setDescription(const string& value);
    void setStatus(const string& value);

    void inputNew(const string& contractId = "", const string& customerId = "");
    void updateFields();
    void displayRow() const override;
    string toFileString() const override;
    bool fromFileString(const string& line) override;
};

class Support : public Entity {
private:
    string customerId_;
    string incidentId_;
    string technician_;
    string note_;
    string history_;
    string status_ = "Cho phan cong";
public:
    const string& getCustomerId() const;
    const string& getIncidentId() const;
    const string& getTechnician() const;
    const string& getNote() const;
    const string& getHistory() const;
    const string& getStatus() const;

    void setCustomerId(const string& value);
    void setIncidentId(const string& value);
    void setTechnician(const string& value);
    void setNote(const string& value);
    void setHistory(const string& value);
    void setStatus(const string& value);

    void inputNew();
    void updateFields();
    void displayRow() const override;
    string toFileString() const override;
    bool fromFileString(const string& line) override;
};

#endif // ENTITIES_H
