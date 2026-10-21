#ifndef REPOSITORY_H
#define REPOSITORY_H

// Tac gia: [Dien ho ten sinh vien phu trach]
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T>
class Repository {
private:
    vector<T> records_;
    string path_;

public:
    /// Khoi tao kho du lieu gan voi mot duong dan va nap du lieu tu file.
    explicit Repository(const string& path = "") : path_(path) { load(); }

    /// Doc file du lieu, bo qua va canh bao cac dong sai dinh dang.
    void load() {
        records_.clear();
        if (path_.empty()) return;

        filesystem::create_directories("data");
        if (!filesystem::exists(path_)) {
            ofstream createFile(path_);
            if (!createFile) throw runtime_error("Khong tao duoc file du lieu: " + path_);
            return;
        }

        ifstream input(path_);
        if (!input) throw runtime_error("Khong mo duoc file du lieu: " + path_);

        string line;
        size_t lineNumber = 0;
        while (getline(input, line)) {
            ++lineNumber;
            if (line.empty() || line[0] == '#') continue;
            T object;
            if (object.fromFileString(line) && !object.getId().empty()) {
                // Bo qua ban ghi co ma trung de tranh du lieu khong nhat quan.
                const bool duplicate = any_of(records_.begin(), records_.end(), [&](const T& oldRecord) {
                    return oldRecord.getId() == object.getId();
                });
                if (!duplicate) records_.push_back(object);
                else cout << "Canh bao: ma trung tai " << path_ << ", dong " << lineNumber << ". Bo qua.\n";
            } else {
                cout << "Canh bao: dong sai dinh dang tai " << path_ << ", dong " << lineNumber << ". Bo qua.\n";
            }
        }
        if (input.bad()) throw runtime_error("Loi khi doc file du lieu: " + path_);
    }

    /// Ghi toan bo ban ghi hien co vao file du lieu.
    void save() const {
        if (path_.empty()) return;
        filesystem::create_directories("data");
        ofstream output(path_, ios::trunc);
        if (!output) throw runtime_error("Khong ghi duoc file du lieu: " + path_);
        for (const auto& record : records_) output << record.toFileString() << '\n';
        if (!output) throw runtime_error("Loi khi ghi file du lieu: " + path_);
    }

    /// Tim ban ghi theo ma va tra ve con tro, hoac nullptr neu khong co.
    T* findById(const string& id) {
        for (auto& record : records_) {
            if (record.getId() == id) return &record;
        }
        return nullptr;
    }

    /// Tim ban ghi theo ma trong kho chi doc.
    const T* findById(const string& id) const {
        for (const auto& record : records_) {
            if (record.getId() == id) return &record;
        }
        return nullptr;
    }

    /// Tra ve danh sach ban ghi co the chinh sua de phuc vu nghiep vu.
    vector<T>& all() { return records_; }
    /// Tra ve danh sach ban ghi chi doc.
    const vector<T>& all() const { return records_; }
    /// Kiem tra kho du lieu co rong hay khong.
    bool empty() const { return records_.empty(); }
    /// Tra ve so luong ban ghi trong kho.
    size_t size() const { return records_.size(); }

    /// Them ban ghi neu ma chua ton tai va luu ngay vao file.
    bool add(const T& object) {
        if (object.getId().empty()) throw invalid_argument("Ban ghi phai co ma dinh danh.");
        if (findById(object.getId()) != nullptr) return false;
        records_.push_back(object);
        save();
        return true;
    }

    /// Xoa ban ghi theo ma va luu file neu co thay doi.
    bool removeById(const string& id) {
        const auto oldSize = records_.size();
        records_.erase(remove_if(records_.begin(), records_.end(), [&](const T& record) {
            return record.getId() == id;
        }), records_.end());
        if (records_.size() == oldSize) return false;
        save();
        return true;
    }

    /// Hien thi tat ca ban ghi trong kho.
    void showAll() const {
        if (records_.empty()) {
            cout << "(Trong)\n";
            return;
        }
        for (const auto& record : records_) record.displayRow();
    }
};

#endif // REPOSITORY_H
