#include <iostream>
#include <string>
// Không dùng <iomanip> hay setw, giữ code cơ bản nhất

using namespace std;

// 1. Struct Đồ Uống
struct DoUong {
    string maMon;
    string tenMon;
    string loai; // Nong/Lanh/Da xay
    char size;   // S/M/L
    double gia;
    bool conHang;
};

// 2. Struct Bàn
struct Ban {
    int soBan;
    string viTri; // Trong/Ngoai/Tang 2
    int soGhe;
    int trangThai; // 0: Trong, 1: Co khach, 2: Dang don
};

// 3. Struct Hóa Đơn
struct HoaDon {
    string maHoaDon;
    int soBan;
    // Tạm thời dùng mảng tĩnh cho dễ hình dung, sau này chuyển sang con trỏ/mảng động
    DoUong danhSachMon[50]; 
    int soLuongMon;
    double tongTienMon;
    double thueVAT; // 8%
    double giamGiaVIP;
    double thanhTien;
    string thoiGian;
};

// Hàm vẽ menu console cơ bản (Yêu cầu 4)
void hienThiMenu() {
    cout << "\n=========================================\n";
    cout << "      UNG DUNG QUAN LY QUAN CAFE       \n";
    cout << "=========================================\n";
    cout << "1. Quan ly Ban (Chon ban, doi trang thai)\n";
    cout << "2. Goi mon (Them do uong, ghi chu)\n";
    cout << "3. Thanh toan (Tinh tien, in hoa don)\n";
    cout << "4. Thong ke Doanh thu & Best Seller\n";
    cout << "0. Thoat va Luu du lieu\n";
    cout << "=========================================\n";
    cout << "Nhap lua chon cua ban: ";
}

int main() {
    int luaChon;
    bool dangChay = true;

    while (dangChay) {
        hienThiMenu();
        
        // Yêu cầu 6: Bắt lỗi nhập sai kiểu dữ liệu (vd: nhập chữ vào ô số)
        try {
            if (!(cin >> luaChon)) {
                throw runtime_error("Loi: Vui long nhap mot so nguyen!");
            }

            switch (luaChon) {
                case 1:
                    cout << "\n[Chuc nang Quan ly Ban dang phat trien...]\n";
                    break;
                case 2:
                    cout << "\n[Chuc nang Goi mon dang phat trien...]\n";
                    break;
                case 0:
                    cout << "\nDang luu du lieu... Tam biet!\n";
                    dangChay = false;
                    break;
                default:
                    cout << "\nLua chon khong hop le! Vui long nhap tu 0 den 4.\n";
            }
        } 
        catch (const exception& e) {
            cout << "\n" << e.what() << "\n";
            cin.clear(); // Xóa cờ lỗi của cin
            cin.ignore(10000, '\n'); // Bỏ qua các ký tự thừa trong buffer
        }
    }

    return 0;
}