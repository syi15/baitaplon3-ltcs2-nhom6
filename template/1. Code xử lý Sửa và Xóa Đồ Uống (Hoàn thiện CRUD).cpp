// =========================================================
// PHẦN 3: SỬA VÀ XÓA ĐỒ UỐNG (CRUD)
// =========================================================

// Hàm Sửa thông tin đồ uống
void suaDoUong(DoUong* ds, int soLuong, string maCanSua) {
    bool timThay = false;
    for (int i = 0; i < soLuong; i++) {
        if (ds[i].maMon == maCanSua) {
            timThay = true;
            cout << "Tim thay mon: " << ds[i].tenMon << "\n";
            cout << "Nhap Ten mon moi: "; 
            cin.ignore(); getline(cin, ds[i].tenMon);
            cout << "Nhap Gia moi (VND): "; cin >> ds[i].gia;
            cout << "Trang thai con hang khong? (1: Co, 0: Khong): "; 
            int tt; cin >> tt;
            ds[i].conHang = (tt == 1);
            cout << "Cap nhat thanh cong!\n";
            break;
        }
    }
    if (!timThay) cout << "Khong tim thay mon co ma: " << maCanSua << "\n";
}

// Hàm Xóa đồ uống (Kỹ thuật thu hẹp mảng động)
void xoaDoUong(DoUong*& ds, int& soLuong, string maCanXoa) {
    int viTriXoa = -1;
    for (int i = 0; i < soLuong; i++) {
        if (ds[i].maMon == maCanXoa) {
            viTriXoa = i;
            break;
        }
    }

    if (viTriXoa == -1) {
        cout << "Khong tim thay mon co ma: " << maCanXoa << " de xoa!\n";
        return;
    }

    // 1. Tạo mảng mới nhỏ hơn 1 phần tử
    DoUong* mangTam = new DoUong[soLuong - 1];
    int j = 0; // Index cho mảng mới
    
    // 2. Copy các phần tử (bỏ qua phần tử cần xóa)
    for (int i = 0; i < soLuong; i++) {
        if (i != viTriXoa) {
            mangTam[j] = ds[i];
            j++;
        }
    }

    // 3. Giải phóng mảng cũ và cập nhật con trỏ
    delete[] ds;
    ds = mangTam;
    soLuong--;
    cout << "Da xoa mon thanh cong!\n";
}