#include "Menus.h"

int main() {
    try {
        return runApplication();
    } catch (const exception& error) {
        cerr << "Loi chuong trinh: " << error.what() << '\n';
        return 1;
    }
}