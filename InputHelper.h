#ifndef INPUT_HELPER_H
#define INPUT_HELPER_H

// Tac gia: [Dien ho ten sinh vien phu trach]
#include "Validator.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

using namespace std;

/// Doc mot dong tu ban phim va bao loi neu dau vao ket thuc bat ngo.
inline string readLine(const string& prompt) {
    string value;
    cout << prompt;
    if (!getline(cin, value)) {
        throw runtime_error("Ket thuc dau vao (EOF).");
    }
    return value;
}

/// Doc chuoi, kiem tra chuoi rong va ky tu phan cach theo tuy chon.
inline string readUCText(const string& prompt, bool required = true) {
    while (true) {
        const string value = readLine(prompt);
        try {
            validateText(value, "Du lieu", !required);
            if (required && trimText(value).empty()) {
                throw invalid_argument("Khong duoc de trong.");
            }
            return value;
        } catch (const invalid_argument& error) {
            cout << error.what() << " Vui long nhap lai.\n";
        }
    }
}

/// Yeu cau nguoi dung nhap mot so duong hop le.
inline string readUCPositiveNumber(const string& prompt) {
    while (true) {
        const string value = readLine(prompt);
        try {
            validatePositiveNumber(value, "Gia tri");
            return trimText(value);
        } catch (const invalid_argument& error) {
            cout << error.what() << " Vui long nhap lai.\n";
        }
    }
}

// Cho phep bo trong de giu gia tri cu; neu nhap gia tri moi thi goi setter de kiem tra.
template <typename Setter>
/// Cap nhat truong tuy chon; de trong de giu gia tri cu va nhap lai neu sai.
inline void updateOptionalField(const string& prompt, Setter setter) {
    while (true) {
        const string value = readLine(prompt);
        if (value.empty()) return;
        try {
            setter(value);
            return;
        } catch (const exception& error) {
            cout << error.what() << " Vui long nhap lai hoac de trong de giu nguyen.\n";
        }
    }
}

/// Doc lua chon so nguyen trong khoang cho phep.
inline int readChoice(const string& prompt, int minValue, int maxValue) {
    int choice;
    while (true) {
        cout << prompt;
        if (cin >> choice) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (choice >= minValue && choice <= maxValue) return choice;
        } else {
            if (cin.eof()) throw runtime_error("Ket thuc dau vao (EOF).");
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Lua chon khong hop le, vui long nhap "
             << minValue << "-" << maxValue << ".\n";
    }
}

/// Ham doc lua chon dung chung cho cac use case.
inline int readUCChoice(const string& prompt, int minValue, int maxValue) {
    return readChoice(prompt, minValue, maxValue);
}

/// Yeu cau xac nhan bang y/n va chi chap nhan hai gia tri nay.
inline bool confirmUC() {
    while (true) {
        string answer = trimText(readLine("Xac nhan thuc hien? (y/n): "));
        if (answer == "y" || answer == "Y") return true;
        if (answer == "n" || answer == "N") return false;
        cout << "Vui long nhap y hoac n.\n";
    }
}

// Tach dung so truong. Khong chap nhan dong co du truong hoac thieu truong.
/// Tach mot dong du lieu theo dau | va kiem tra so truong.
inline bool splitPipe(const string& line, string* fields, int count) {
    stringstream stream(line);
    string extra;
    for (int i = 0; i < count; ++i) {
        if (!getline(stream, fields[i], '|')) {
            // Cho phep truong cuoi cung rong neu dong ket thuc bang '|'.
            if (i == count - 1 && !line.empty() && line.back() == '|') {
                fields[i].clear();
                return !getline(stream, extra, '|');
            }
            return false;
        }
    }
    return !getline(stream, extra, '|');
}

#endif // INPUT_HELPER_H
