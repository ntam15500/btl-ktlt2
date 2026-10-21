CAU TRUC MA NGUON FTTH

- main.cpp: diem bat dau, chi co mot ham main().
- Application.cpp: menu chinh va dieu phoi 10 use case.
- Menus.h: khai bao cac ham menu dung trong toan chuong trinh.
- Entities.h: lop co so truu tuong Entity va 10 lop thuc the.
- InputHelper.h: ham nhap lieu, doc lua chon, tach ban ghi.
- Validator.h: kiem tra chuoi, so va ngay thang.
- Repository.h: template quan ly danh sach va doc/ghi file.
- AppData.h: tap trung 10 Repository cua cac doi tuong.
- UC1_Customers.cpp den UC10_Supports.cpp: code cua tung use case, tach rieng.
- data/: cac file du lieu cho 10 doi tuong.

QUAN TRONG

1. Dung bo file trong thu muc nay cung nhau; khong copy Entities.h moi vao
   mot thu muc cu co Entity.h va cac UC*.cpp phien ban khac.
2. Trong bo nay, lop Entity da nam trong Entities.h. Khong can Entity.h rieng,
   va khong can cac header lop cu nhu Customer.h, Plan.h, Incident.h, v.v.
3. Thay [Dien ho ten sinh vien phu trach] trong cac file bang ten tac gia that.
4. Moi file trong data/ can co it nhat 10 ban ghi hop le truoc khi nop.

BIEN DICH TREN GCC/MINGW (C++17)

g++ -std=c++17 -Wall -Wextra main.cpp Application.cpp UC1_Customers.cpp UC2_Registrations.cpp UC3_Plans.cpp UC4_PlanChanges.cpp UC5_Contracts.cpp UC6_Renewals.cpp UC7_Payments.cpp UC8_Invoices.cpp UC9_Incidents.cpp UC10_Supports.cpp -o main.exe

Trong VS Code, mo dung thu muc VScode nay, sau do nhan Ctrl+Shift+B de chay
build task "Build FTTH project (C++17)". Khong chon "build active file" vi project
can bien dich tat ca file .cpp vao mot chuong trinh duy nhat.

TOI UU
- Bo cac lenh save() lap lai ngay sau Repository::add() va removeById(), vi hai ham nay da tu luu file.
- Chi luu Repository khi cap nhat truc tiep ban ghi dang ton tai.
- Bo tao thu muc data lap lai trong Application.cpp; Repository tu tao thu muc khi nap/ghi file.
- Giu moi UC trong mot file .cpp rieng; khong gop nhieu cau lenh vao mot dong.
