#ifndef INPUT_HELPER_H
#define INPUT_HELPER_H

// Tac gia: [Dien ho ten sinh vien phu trach]
#include "Validator.h"
#include <iostream>
#include <string>
#include <stdexcept>

/// Doc mot dong tu ban phim va bao loi neu dau vao ket thuc bat ngo.
inline std::string readLine(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    if (!std::getline(std::cin, value)) {
        throw std::runtime_error("Ket thuc dau vao (EOF).");
    }
    // Loai bo ky tu '\r' neu dau vao bi anh huong boi dinh dang CRLF
    if (!value.empty() && value.back() == '\r') {
        value.pop_back();
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

/// Cap nhat truong tuy chon; bam Enter ngay de giu gia tri cu, nhap sai se yeu cau nhap lai.
template <typename Setter>
inline void updateOptionalField(const std::string& prompt, Setter setter) {
    while (true) {
        const std::string rawValue = readLine(prompt);
        if (rawValue.empty()) return; // Chi giu nguyen khi nguoi dung bam Enter ngay
        try {
            setter(trimText(rawValue));
            return;
        } catch (const std::exception& error) {
            std::cout << error.what() << " Vui long nhap lai hoac bam Enter de giu nguyen.\n";
        }
    }
}

/// Doc lua chon so nguyen trong khoang cho phep.
inline int readChoice(const std::string& prompt, int minValue, int maxValue) {
    while (true) {
        const std::string line = trimText(readLine(prompt));
        if (!line.empty()) {
            try {
                std::size_t pos = 0;
                int choice = std::stoi(line, &pos);
                if (pos == line.length() && choice >= minValue && choice <= maxValue) {
                    return choice;
                }
            } catch (const std::exception&) {
                // Bat loi std::invalid_argument va std::out_of_range cua stoi
            }
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
        const std::string answer = trimText(readLine("Xac nhan thuc hien? (y/n): "));
        if (answer == "y" || answer == "Y") return true;
        if (answer == "n" || answer == "N") return false;
        std::cout << "Vui long nhap y hoac n.\n";
    }
}

/// Tach mot dong du lieu theo dau | va kiem tra chinh xac so truong.
inline bool splitPipe(const std::string& rawLine, std::string* fields, int count) {
    if (count <= 0 || fields == nullptr || rawLine.empty()) {
        return false;
    }

    // Xu ly ky tu '\r' cuoi dong neu doc tu file Windows tren Linux/macOS
    std::string line = rawLine;
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }

    std::size_t start = 0;
    for (int i = 0; i < count - 1; ++i) {
        std::size_t pos = line.find('|', start);
        if (pos == std::string::npos) return false; // Thieu truong
        fields[i] = line.substr(start, pos - start);
        start = pos + 1;
    }

    // Neu van con dau '|' phia sau -> Du truong
    if (line.find('|', start) != std::string::npos) {
        return false;
    }

    fields[count - 1] = line.substr(start);
    return true;
}

#endif // INPUT_HELPER_H
