#ifndef VALIDATOR_H
#define VALIDATOR_H

// Tac gia: [Dien ho ten sinh vien phu trach]
#include <string>
#include <stdexcept>
#include <cmath>
#include <cctype>

using namespace std;

/// Loai khoang trang o dau va cuoi chuoi.
inline string trimText(const string& value) {
    const size_t first = value.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    const size_t last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

/// Kiem tra chuoi hop le, khong rong neu bat buoc va khong chua dau phan cach file.
inline void validateText(const string& value, const string& fieldName,
                         bool allowEmpty = false) {
    if (!allowEmpty && trimText(value).empty()) {
        throw invalid_argument(fieldName + " khong duoc de trong.");
    }
    if (value.find('|') != string::npos || value.find('\n') != string::npos ||
        value.find('\r') != string::npos) {
        throw invalid_argument(fieldName + " khong duoc chua ky tu | hoac xuong dong.");
    }
}

/// Kiem tra chuoi so phai la mot gia tri huu han lon hon 0.
inline void validatePositiveNumber(const string& value, const string& fieldName) {
    validateText(value, fieldName);
    try {
        size_t consumed = 0;
        const double number = stod(trimText(value), &consumed);
        const string cleaned = trimText(value);
        if (consumed != cleaned.size() || !isfinite(number) || number <= 0) {
            throw invalid_argument(fieldName + " phai la so lon hon 0.");
        }
    } catch (const invalid_argument&) {
        throw invalid_argument(fieldName + " phai la so lon hon 0.");
    } catch (const out_of_range&) {
        throw invalid_argument(fieldName + " nam ngoai pham vi cho phep.");
    }
}

/// Kiem tra ngay theo dinh dang YYYY-MM-DD hoac DD/MM/YYYY va ngay lich hop le.
inline bool isValidDateText(const string& input) {
    const string value = trimText(input);
    int year = 0, month = 0, day = 0;
    if (value.size() == 10 && value[4] == '-' && value[7] == '-') {
        for (size_t i = 0; i < value.size(); ++i) {
            if (i != 4 && i != 7 && !isdigit(static_cast<unsigned char>(value[i]))) return false;
        }
        year = stoi(value.substr(0, 4));
        month = stoi(value.substr(5, 2));
        day = stoi(value.substr(8, 2));
    } else if (value.size() == 10 && value[2] == '/' && value[5] == '/') {
        for (size_t i = 0; i < value.size(); ++i) {
            if (i != 2 && i != 5 && !isdigit(static_cast<unsigned char>(value[i]))) return false;
        }
        day = stoi(value.substr(0, 2));
        month = stoi(value.substr(3, 2));
        year = stoi(value.substr(6, 4));
    } else {
        return false;
    }
    if (year < 1900 || month < 1 || month > 12) return false;
    const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int maxDay = daysInMonth[month - 1];
    if (month == 2 && (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))) maxDay = 29;
    return day >= 1 && day <= maxDay;
}

/// Nem ngoai le neu ngay khong hop le.
inline void validateDateText(const string& value, const string& fieldName) {
    validateText(value, fieldName);
    if (!isValidDateText(value)) {
        throw invalid_argument(fieldName + " khong hop le (dung YYYY-MM-DD hoac DD/MM/YYYY).");
    }
}

#endif // VALIDATOR_H
