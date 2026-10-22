#ifndef INPUT_HELPER_H
#define INPUT_HELPER_H

// Tac gia: [Dien ho ten sinh vien phu trach]
#include "Validator.h"
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <limits>

/// Doc mot dong tu ban phim va bao loi neu dau vao ket thuc bat ngo.
inline std::string readLine(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    if (!std::getline(std::cin, value)) {
        throw std::runtime_error("Ket thuc dau vao (EOF).");
    }
    return value;
}

/// Doc chuoi, kiem tra chuoi rong va ky tu phan cach theo tuy chon.
inline std::string readUCText(const std::string& prompt, bool required = true) {
    while (true) {
        const std::string value = trimText(readLine(prompt));
        try {
            validateText(value, "Du lieu", !required);
            if (required && value.empty()) {
                throw std::invalid_argument("Khong duoc de trong.");
            }
            return value;
        } catch (const std::invalid_argument& error) {
            std::cout << error.what() << " Vui long nhap lai.\n";
        }
    }
}

/// Yeu cau nguoi dung nhap mot so duong hop le.
inline std::string readUCPositiveNumber(const std::string& prompt) {
    while (true) {
        const std::string value = trimText(readLine(prompt));
        try {
            validatePositiveNumber(value, "Gia tri");
            return value;
        } catch (const std::invalid_argument& error) {
            std::cout << error.what() << " Vui long nhap lai.\n";
        }
    }
}

/// Cap nhat truong tuy chon; de trong de giu gia tri cu va nhap lai neu sai.
template <typename Setter>
inline void updateOptionalField(const std::string& prompt, Setter setter) {
    while (true) {
        const std::string value = trimText(readLine(prompt));
        if (value.empty()) return;
        try {
            setter(value);
            return;
        } catch (const std::exception& error) {
            std::cout << error.what() << " Vui long nhap lai hoac de trong de giu nguyen.\n";
        }
    }
}

/// Doc lua chon so nguyen trong khoang cho phep (An toan voi du lieu hon hop).
inline int readChoice(const std::string& prompt, int minValue, int maxValue) {
    while (true) {
        const std::string line = trimText(readLine(prompt));
        try {
            std::size_t pos = 0;
            int choice = std::stoi(line, &pos);
            // Dam bao toan bo chuoi deu la so (khong co ky tu la phia sau nhu "1abc")
            if (pos == line.length() && choice >= minValue && choice <= maxValue) {
                return choice;
            }
        } catch (const std::exception&) {
            // Bo qua loi stoi va in thong bao ben duoi
        }
        std::cout << "Lua chon khong hop le, vui long nhap so nguyen tu "
                  << minValue << " den " << maxValue << ".\n";
    }
}

/// Ham doc lua chon dung chung cho cac use case.
inline int readUCChoice(const std::string& prompt, int minValue, int maxValue) {
    return readChoice(prompt, minValue, maxValue);
}

/// Yeu cau xac nhan bang y/n va chi chap nhan hai gia tri nay.
inline bool confirmUC() {
    while (true) {
        std::string answer = trimText(readLine("Xac nhan thuc hien? (y/n): "));
        if (answer == "y" || answer == "Y") return true;
        if (answer == "n" || answer == "N") return false;
        std::cout << "Vui long nhap y hoac n.\n";
    }
}

/// Tach mot dong du lieu theo dau | va kiem tra chinh xac so truong.
inline bool splitPipe(const std::string& line, std::string* fields, int count) {
    if (count <= 0) return line.empty();
    
    std::size_t start = 0;
    for (int i = 0; i < count - 1; ++i) {
        std::size_t pos = line.find('|', start);
        if (pos == std::string::npos) return false; // Thieu truong
        fields[i] = line.substr(start, pos - start);
        start = pos + 1;
    }
    
    // Kiem tra xem con dau '|' nao bi du o phan cuoi khong
    if (line.find('|', start) != std::string::npos) {
        return false; // Du truong
    }
    
    fields[count - 1] = line.substr(start);
    return true;
}

#endif // INPUT_HELPER_H
