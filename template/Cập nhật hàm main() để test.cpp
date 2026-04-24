int main() {
    // ... (Giữ nguyên đoạn đầu đọc file menu) ...

    // Thêm phần khởi tạo Bàn
    Ban* danhSachBan = nullptr;
    int soLuongBan = 0;
    khoiTaoBan(danhSachBan, soLuongBan);

    int luaChon;
    bool dangChay = true;

    while (dangChay) {
        cout << "\n==============================\n";
        cout << "1. Xem Menu\n";
        cout << "2. Them mon moi\n";
        cout << "3. Sua thong tin mon\n";
        cout << "4. Xoa mon\n";
        cout << "5. Xem So do Ban\n";
        cout << "6. Khach vao Ban (Doi trang thai)\n";
        cout << "0. Thoat va Luu Du Lieu\n";
        cout << "==============================\n";
        cout << "Chon: ";
        cin >> luaChon;

        // ... (Giữ nguyên case 1 và 2) ...

        if (luaChon == 3) {
            string maSua;
            cout << "Nhap Ma mon can sua: "; cin >> maSua;
            suaDoUong(danhSachMenu, soLuongMon, maSua);
        }
        else if (luaChon == 4) {
            string maXoa;
            cout << "Nhap Ma mon can xoa: "; cin >> maXoa;
            xoaDoUong(danhSachMenu, soLuongMon, maXoa);
        }
        else if (luaChon == 5) {
            inSoDoBan(danhSachBan, soLuongBan);
        }
        else if (luaChon == 6) {
            int sb, tt;
            cout << "Nhap So ban: "; cin >> sb;
            cout << "Chon trang thai (0: Trong, 1: Co Khach, 2: Dang Don): "; cin >> tt;
            capNhatTrangThaiBan(danhSachBan, soLuongBan, sb, tt);
        }
        else if (luaChon == 0) {
            ghiFileMenu(danhSachMenu, soLuongMon);
            dangChay = false;
        }
    }

    // Đừng quên giải phóng bộ nhớ cho Bàn trước khi thoát
    delete[] danhSachBan;
    danhSachBan = nullptr;
    delete[] danhSachMenu;
    danhSachMenu = nullptr;

    return 0;
}