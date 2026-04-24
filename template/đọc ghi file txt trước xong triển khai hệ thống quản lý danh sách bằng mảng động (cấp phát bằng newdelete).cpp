#include <iostream>
#include <string>
#include <fstream> // Thư viện đọc ghi file
#include <stdexcept>

using namespace std;

// 1. Struct Đồ Uống
struct DoUong {
    string maMon;
    string tenMon;
    string loai; 
    char size;   
    double gia;
    bool conHang;
};

// =========================================================
// PHẦN 1: QUẢN LÝ MẢNG ĐỘNG BẰNG CON TRỎ (NEW/DELETE)
// =========================================================

// Hàm thêm 1 đồ uống vào mảng động (Mở rộng mảng)
void themDoUongVaoMang(DoUong*& ds, int& soLuong, DoUong monMoi) {
    // 1. Cấp phát mảng tạm có kích thước lớn hơn 1
    DoUong* mangTam = new DoUong[soLuong + 1];
    
    // 2. Copy dữ liệu từ mảng cũ sang mảng tạm
    for (int i = 0; i < soLuong; i++) {
        mangTam[i] = ds[i];
    }
    
    // 3. Thêm món mới vào cuối mảng tạm
    mangTam[soLuong] = monMoi;
    
    // 4. Giải phóng (delete) mảng cũ để chống Memory Leak
    delete[] ds;
    
    // 5. Trỏ con trỏ danh sách về mảng mới và tăng số lượng
    ds = mangTam;
    soLuong++;
}

// =========================================================
// PHẦN 2: XỬ LÝ LƯU TRỮ (ĐỌC/GHI FILE .TXT)
// =========================================================

const string FILE_MENU = "menu_cafe.txt";

// Hàm Ghi file (Lưu dữ liệu trước khi thoát)
void ghiFileMenu(DoUong* ds, int soLuong) {
    ofstream file(FILE_MENU);
    if (!file.is_open()) {
        cout << "Loi: Khong the tao file de luu!\n";
        return;
    }
    
    // Lưu số lượng trước để sau này đọc cho dễ
    file << soLuong << "\n";
    for (int i = 0; i < soLuong; i++) {
        file << ds[i].maMon << "\n";
        file << ds[i].tenMon << "\n";
        file << ds[i].loai << "\n";
        file << ds[i].size << "\n";
        file << ds[i].gia << "\n";
        file << ds[i].conHang << "\n";
    }
    file.close();
    cout << "Da luu " << soLuong << " mon vao file " << FILE_MENU << " thanh cong!\n";
}

// Hàm Đọc file (Load dữ liệu khi mở app)
void docFileMenu(DoUong*& ds, int& soLuong) {
    ifstream file(FILE_MENU);
    
    // Yêu cầu 6: Bắt lỗi mở file không tồn tại bằng try-catch
    try {
        if (!file.is_open()) {
            throw runtime_error("File " + FILE_MENU + " chua ton tai. He thong se tao danh sach moi.");
        }
        
        int soLuongTrongFile;
        file >> soLuongTrongFile;
        file.ignore(); // Xóa dấu enter bị thừa
        
        for (int i = 0; i < soLuongTrongFile; i++) {
            DoUong monMoi;
            getline(file, monMoi.maMon);
            getline(file, monMoi.tenMon);
            getline(file, monMoi.loai);
            file >> monMoi.size;
            file >> monMoi.gia;
            file >> monMoi.conHang;
            file.ignore(); // Xóa dấu enter thừa sau khi đọc số
            
            // Dùng hàm mảng động để đẩy vào danh sách
            themDoUongVaoMang(ds, soLuong, monMoi);
        }
        file.close();
        cout << "Da tai xong " << soLuong << " mon tu file!\n";
    }
    catch (const exception& e) {
        cout << "Thong bao: " << e.what() << "\n";
    }
}

// Hàm in danh sách cơ bản (Không dùng iomanip)
void inMenu(DoUong* ds, int soLuong) {
    cout << "\n--- MENU QUAN CAFE ---\n";
    if (soLuong == 0) {
        cout << "Menu dang trong!\n";
        return;
    }
    for (int i = 0; i < soLuong; i++) {
        cout << ds[i].maMon << " | " << ds[i].tenMon << " | " 
             << ds[i].loai << " | Size: " << ds[i].size 
             << " | Gia: " << ds[i].gia << " VND"
             << " | Trang thai: " << (ds[i].conHang ? "Con hang" : "Het hang") << "\n";
    }
}

// =========================================================
// HÀM CHÍNH
// =========================================================
int main() {
    // Khởi tạo con trỏ và số lượng (rất quan trọng)
    DoUong* danhSachMenu = nullptr; 
    int soLuongMon = 0;

    cout << "=== KHOI DONG HE THONG ===\n";
    docFileMenu(danhSachMenu, soLuongMon);

    int luaChon;
    bool dangChay = true;

    while (dangChay) {
        cout << "\n==============================\n";
        cout << "1. Xem Menu hien tai\n";
        cout << "2. Them 1 mon moi vao Menu (Test mang dong)\n";
        cout << "0. Thoat va Luu Du Lieu\n";
        cout << "==============================\n";
        cout << "Chon: ";
        cin >> luaChon;

        if (luaChon == 1) {
            inMenu(danhSachMenu, soLuongMon);
        } 
        else if (luaChon == 2) {
            DoUong monMoi;
            cout << "Nhap Ma mon (vd CF01): "; cin >> monMoi.maMon;
            cin.ignore();
            cout << "Nhap Ten mon (vd Ca phe den): "; getline(cin, monMoi.tenMon);
            cout << "Nhap Loai (Nong/Lanh/Da xay): "; getline(cin, monMoi.loai);
            cout << "Nhap Size (S/M/L): "; cin >> monMoi.size;
            cout << "Nhap Gia (VND): "; cin >> monMoi.gia;
            monMoi.conHang = true;
            
            themDoUongVaoMang(danhSachMenu, soLuongMon, monMoi);
            cout << "Them thanh cong!\n";
        }
        else if (luaChon == 0) {
            ghiFileMenu(danhSachMenu, soLuongMon);
            dangChay = false;
        }
    }

    // CUỐI CÙNG: Dọn dẹp bộ nhớ trước khi tắt app hoàn toàn
    delete[] danhSachMenu;
    danhSachMenu = nullptr;

    return 0;
}