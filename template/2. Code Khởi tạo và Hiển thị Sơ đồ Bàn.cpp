// =========================================================
// PHẦN 4: QUẢN LÝ BÀN (SƠ ĐỒ QUÁN)
// =========================================================

// Khởi tạo mảng động danh sách bàn mặc định (vd: 10 bàn)
void khoiTaoBan(Ban*& dsBan, int& soLuongBan) {
    soLuongBan = 10;
    dsBan = new Ban[soLuongBan];
    for (int i = 0; i < soLuongBan; i++) {
        dsBan[i].soBan = i + 1;
        dsBan[i].soGhe = 4; // Mặc định 4 ghế
        dsBan[i].trangThai = 0; // 0: Trống
        if (i < 5) dsBan[i].viTri = "Tang 1";
        else dsBan[i].viTri = "Tang 2";
    }
}

// In sơ đồ bàn (Vẽ các khối vuông đơn giản)
void inSoDoBan(Ban* dsBan, int soLuongBan) {
    cout << "\n--- SO DO BAN ---\n";
    cout << "( 0: Trong | 1: Co khach | 2: Dang don )\n\n";
    
    for (int i = 0; i < soLuongBan; i++) {
        cout << "[ Ban " << dsBan[i].soBan << " - ";
        if (dsBan[i].trangThai == 0) cout << "Trong ";
        else if (dsBan[i].trangThai == 1) cout << "KHACH ";
        else cout << "DON   ";
        cout << "]\t";
        
        // Cứ 5 bàn thì xuống dòng cho đẹp
        if ((i + 1) % 5 == 0) cout << "\n";
    }
}

// Đổi trạng thái bàn
void capNhatTrangThaiBan(Ban* dsBan, int soLuongBan, int soBanCanSua, int trangThaiMoi) {
    if (soBanCanSua < 1 || soBanCanSua > soLuongBan) {
        cout << "So ban khong hop le!\n";
        return;
    }
    dsBan[soBanCanSua - 1].trangThai = trangThaiMoi;
    cout << "Da cap nhat Ban so " << soBanCanSua << "!\n";
}