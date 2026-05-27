#include <iostream>
#include <string>
#include <iomanip>      //Định dạng in ra màn hình
#include <ctime>        //Tính toán thời gian thực
#include <fstream>      //Đọc ghi file
using namespace std;

////I.Struct

struct Ngay{
    int ngay, thang, nam;
};

//Thông tin mặt hàng trong kho
struct MatHang {
    string maHang;
    string tenHang;
    string donViTinh;
    double giaNhap;
    double giaBan;
    int soLuongTon;
    Ngay hanSuDung;
    int mucTonToiThieu;
};

// Thông tin nhà cung cấp (dùng trong hoá đơn nhập hàng)
struct NhaCungCap {
    string tenNCC;
    string soDienThoai;
    string diaChi;
    int diemChatLuong;         // Đánh giá từ 1-5 sao, ÉP CỨNG DÙ FILE ĐÃ SAVE TRONG TXT
    int diemUyTin;          // Đánh giá từ 1-5 sao, ÉP CỨNG DÙ FILE ĐÃ SAVE TRONG TXT
};

//Chi tiết hoá đơn, dùng cho cả nhập và xuất hàng, lưu thông tin về mặt hàng nào, số lượng bao nhiêu, giá cả ra sao 
//(giá nhập cho hoá đơn nhập, giá bán cho hoá đơn xuất)
struct ChiTietHoaDon {
    string maHang;
    int soLuong;
    double donGia;
};

// Hoá đơn nhập hàng từ nhà cung cấp
struct HoaDonNhap {
    string maHDN;
    Ngay ngayNhap;
    NhaCungCap ncc;
    
    ChiTietHoaDon* danhSachNhap; 
    int soLuongMatHang; 
    double tongTien;
};

// Hoá đơn xuất hàng đến khách hàng
struct HoaDonXuat {
    string maHDX;
    Ngay ngayXuat;
    string khachHang;
    
    ChiTietHoaDon* danhSachBan; 
    int soLuongMatHang; 
    double tongTien;
};

// Cấu trúc hỗ trợ báo cáo thông minh: theo dõi doanh số bán hàng từng sản phẩm
struct DoanhSoMatHang {
    string maHang;
    int soLuongBan;
};

////II.Hàm core

//Cả 2 quá trình nhập & xuất hàng đều theo cấu trúc 1 hàm chính dùng để xử lí các tác vụ chung (hàm đầu) và các hàm sau dùng để support những yêu cầu 
//cần thiết của quá trình đấy
//Dễ hiểu thì *& là kiểu truyền tham chiếu đến một ptr, dạng kiểu vẫn save address dưới dạng ptr và vẫn có thể direct thay đổi đc những gì bên trong đấy

// A. CÁC HÀM NHẬP HÀNG

void xuLyNhapHang(MatHang*& kho, int& nKho, int& sucChuaKho, HoaDonNhap*& dsHDN, int& nHDN, int& sucChuaHDN);
void themMatHang(MatHang*& kho, int& nKho, int& sucChua, string newMaHang, Ngay newHsd);

// B. CÁC HÀM XUẤT HÀNG

void xuLyXuatHang(MatHang*& kho, int& nKho, int& sucChuaKho, HoaDonXuat*& dsHDX, int& nHDX, int& sucChuaHDX);

// C. CÁC HÀM CẢNH BÁO
//Dùng const để chỉ đọc dữ liệu

void canhBaoHetHan(const MatHang* kho, int nKho, Ngay homNay);
void canhBaoHetHang(const MatHang* kho, int nKho);
void thongKeTonKho(const MatHang* kho, int nKho);

// D. CÁC TÍNH NĂNG ĐẶC BIỆT

// Báo cáo phân tích xu hướng bán hàng
void baoCaoThongMinh(const MatHang* kho, int nKho, const HoaDonXuat* dsHDX, int nHDX);

// Tối ưu sắp xếp kho dựa trên doanh số bán hàng
void toiUuKho(const MatHang* kho, int nKho, const HoaDonXuat* dsHDX, int nHDX);

// E. LƯU & ĐỌC DỮ LIỆU TỪ FILE - các hàm này chủ yếu sẽ được dùng trong quá trình nhập tồn kho, nhập/xuất hàng và chỉ tồn tại ở menu dưới dạng fallback.

void SaveData(const MatHang* kho, int nKho, const HoaDonNhap* dsHDN, int nHDN, const HoaDonXuat* dsHDX, int nHDX, bool isSilent = false);
void ReadData(MatHang*& kho, int& nKho, int& sucChuaKho, HoaDonNhap*& dsHDN, int& nHDN, int& sucChuaHDN, HoaDonXuat*& dsHDX, int& nHDX, int& sucChuaHDX, bool isSilent = false);

// F. CÁC HÀM HỖ TRỢ TIỆN ÍCH
void chuanHoaNgay(Ngay& d);
void capNhatTonKho(MatHang*& kho, int& nKho, string maHang, Ngay hsd, int soLuongThayDoi);           // Cập nhật tồn kho cho cả nhập và xuất
int tinhSoNgayConLai(Ngay hsd, Ngay homNay);
bool checkNgay(Ngay d1, Ngay d2);
int timKiemMatHang(const MatHang* kho, int nKho, string maHangCanTim, Ngay hsd);

//III. Main

int main(){
// 1.Khởi tạo kho
    MatHang* kho = nullptr;
    int nKho = 0;
    int sucChuaKho = 0;

// 2.Khởi tạo danh sách hoá đơn nhập
    HoaDonNhap* dsHDN = nullptr;
    int nHDN = 0;
    int sucChuaHDN = 0;

// 3.Khởi tạo danh sách hoá đơn xuất
    HoaDonXuat* dsHDX = nullptr;
    int nHDX = 0;
    int sucChuaHDX = 0;

//Tự đọc lại data khi menu được khởi chạy
    ReadData(kho, nKho, sucChuaKho, dsHDN, nHDN, sucChuaHDN, dsHDX, nHDX, sucChuaHDX, true);

// Menu vòng lặp - người dùng chọn chức năng
    int choice;
        do{
            cout << "\n======================================\n";
            cout << "   UNG DUNG QUAN LY KHO SIEU THI\n";
            cout << "======================================\n";
            cout << "1. Nhap hang vao kho\n";
            cout << "2. Xuat hang ra kho\n";
            cout << "3. Kiem tra mat hang sap het han\n";
            cout << "4. Kiem tra mat hang sap het hang\n";
            cout << "5. Thong ke ton kho\n";
            cout << "6. Bao cao xu huong thi truong\n";
            cout << "7. Toi uu kho\n";
            cout << "8. Luu du lieu\n";
            cout << "9. Doc du lieu\n";
            cout << "0. Thoat\n";
            cout << "======================================\n";
            cout << "Nhap lua chon: "; cin >> choice;

        switch (choice){
            case 1: {
                cout << "\n===================================\n";
                cout << "      NHAP HANG VAO KHO\n";
                cout << "===================================\n";
                xuLyNhapHang(kho, nKho, sucChuaKho, dsHDN, nHDN, sucChuaHDN);
                SaveData(kho, nKho, dsHDN, nHDN, dsHDX, nHDX, true);
                break;
            }
            case 2: {
                cout << "\n===================================\n";
                cout << "      XUAT HANG RA KHO\n";
                cout << "===================================\n";
                xuLyXuatHang(kho, nKho, sucChuaKho, dsHDX, nHDX, sucChuaHDX);
                SaveData(kho, nKho, dsHDN, nHDN, dsHDX, nHDX, true);
                break;
            }
            case 3: {
                cout << "\n===================================\n";
                cout << "  KIEM TRA MAT HANG SAP HET HAN\n";
                cout << "===================================\n";
                Ngay homNay;
                cout << "Nhap ngay hien tai (ngay -> thang -> nam): "; cin >> homNay.ngay >> homNay.thang >> homNay.nam; // lấy ngày cứng trước khi kiểm tra
                chuanHoaNgay(homNay);
                canhBaoHetHan(kho, nKho, homNay);
                break;
            }
            case 4: {
                canhBaoHetHang(kho, nKho);
                break;
            }
            case 5: {
                if (nKho == 0) 
                    cout << "Kho dang trong, khong co mat hang nao de thong ke.\n";
                else thongKeTonKho(kho, nKho);
                break;
            }
            case 6: {
                baoCaoThongMinh(kho, nKho, dsHDX, nHDX);
                break;
            }
            case 7: {
                toiUuKho(kho, nKho, dsHDX, nHDX);
                break;
            }
            case 8: {
                cout << "\n===Luu du lieu===\n";
                SaveData(kho, nKho, dsHDN, nHDN, dsHDX, nHDX, false);
                break;
            }
            case 9: {
                char confirm;
                bool valid = false;
                cout << "\n=== DOC DU LIEU TU FILE ===\n";
                cout << "CANH BAO: Thao tac nay se xoa sach cac thay doi chua duoc luu tru tren he thong.\n";

                // Vòng lặp cưỡng ép: Không cho thoát ra nếu không gõ đúng y hoặc n
                while (!valid) {
                    cout << "Ban co chac chan muon tiep tuc? (y/n): ";
                    cin >> confirm;
                    confirm = tolower(confirm); // Chấp nhận cả Y và y, N và n

                    if (confirm == 'y' || confirm == 'n') {
                        valid = true; // Thoát vòng lặp
                    } else {
                        // Nếu gõ ký tự khác, thông báo và yêu cầu gõ lại
                        cout << "Ky tu khong hop le! Chi chap nhan 'y' hoac 'n'.\n";
                    }
                }

                if (confirm == 'y') {
                    ReadData(kho, nKho, sucChuaKho, dsHDN, nHDN, sucChuaHDN, dsHDX, nHDX, sucChuaHDX, false);
                } 
                else {
                    cout << "Da huy thao tac. Du lieu hien tai van duoc giu nguyen.\n";
                }
                break;
            }

            case 0: {
                cout << "Dang thoat ung dung...\n";
                break;
            }
            default: {
                cout << "Lua chon khong hop le, vui long chon lai!\n";
                break;
            }
        }
    } while (choice != 0);
 // 4. Giải phóng bộ nhớ đã cấp phát động
    delete[] kho;
    for (int i = 0; i < nHDN; i++) delete[] dsHDN[i].danhSachNhap;
    delete[] dsHDN;
    for (int i = 0; i < nHDX; i++) delete[] dsHDX[i].danhSachBan;
    delete[] dsHDX;
    return 0;
}


//IV.Viết lại funct

// A. HÀM HỖ TRỢ

// Hàm chuẩn hoá ngày (ép về giá trị hợp lệ gần nhất)
void chuanHoaNgay(Ngay& d) {
    // Ép năm (giới hạn thực tế quản lý từ năm 1900 đến 2100)
    if (d.nam < 1900) d.nam = 1900;
    if (d.nam > 2100) d.nam = 2100;

    // Ép tháng
    if (d.thang < 1) d.thang = 1;
    if (d.thang > 12) d.thang = 12;

    // Xác định số ngày tối đa trong tháng (tính cả năm nhuận)
    int maxNgay = 31;
    if (d.thang == 4 || d.thang == 6 || d.thang == 9 || d.thang == 11) {
        maxNgay = 30;
    } else if (d.thang == 2) {
        if (d.nam % 400 == 0 || (d.nam % 4 == 0 && d.nam % 100 != 0)) maxNgay = 29;
        else maxNgay = 28;
    }

    // Ép ngày
    if (d.ngay < 1) d.ngay = 1;
    if (d.ngay > maxNgay) d.ngay = maxNgay;
}

// So sánh hai cấu trúc ngày có bằng nhau hay không
bool checkNgay(Ngay d1, Ngay d2) {
    return (d1.ngay == d2.ngay && d1.thang == d2.thang && d1.nam == d2.nam);
}

// Tìm kiếm mặt hàng trong kho theo mã hàng và hạn sử dụng
// Trả về: chỉ số mảng nếu tìm thấy, -1 nếu không tìm thấy
int timKiemMatHang(const MatHang* kho, int nKho, string maHangCanTim, Ngay hsd){
    for(int i = 0; i < nKho; i++){
        if(kho[i].maHang == maHangCanTim && checkNgay(kho[i].hanSuDung, hsd)) return i;
    }
    return -1;
}

// Cập nhật số lượng tồn kho (dương từ nhập, âm từ xuất)
void capNhatTonKho(MatHang*& kho, int& nKho, string maHang, Ngay hsd, int soLuongThayDoi){
    int viTri = timKiemMatHang(kho, nKho, maHang, hsd);
    if (viTri != -1){
        kho[viTri].soLuongTon += soLuongThayDoi;
    }   
}

// Tính số ngày còn lại đến hạn sử dụng (số âm nghĩa là đã quá hạn)
int tinhSoNgayConLai(Ngay hsd, Ngay homNay) {
    // Chuyển đổi cấu trúc Ngay thành tm (tháng: 0-11, năm: từ 1900)
    tm timeHsd = {0};
    timeHsd.tm_mday = hsd.ngay;
    timeHsd.tm_mon = hsd.thang - 1;
    timeHsd.tm_year = hsd.nam - 1900;

    tm timeHomNay = {0};
    timeHomNay.tm_mday = homNay.ngay;
    timeHomNay.tm_mon = homNay.thang - 1;
    timeHomNay.tm_year = homNay.nam - 1900;

    // Chuyển thành time_t (giây tính từ thước ký 1970) rồi tính không gian thời gian
    time_t t1 = mktime(&timeHsd);
    time_t t2 = mktime(&timeHomNay);
    
    return difftime(t1, t2) / (60 * 60 * 24);       //difftime tính khoảng cách giữa t1 t2 chia 60s 60m 24h để ra số ngày
}

// B. CÁC HÀM CHÍNH

// Thêm sản phẩm mới vào kho, tự động mở rộng nếu cần
void themMatHang(MatHang*& kho, int& nKho, int& sucChua, string newMaHang, Ngay newHsd){
    // Mở rộng dung lượng nếu đầy
    if (nKho >= sucChua){
        int newSucChua = sucChua * 2 + 1;           
        MatHang* temp = new MatHang[newSucChua];
        for (int i = 0; i < nKho; i++) temp[i] = kho[i];
        delete[] kho;
        kho = temp;
        sucChua = newSucChua;
    }

    kho[nKho].maHang = newMaHang;
    kho[nKho].hanSuDung = newHsd;

    //Khởi tạo số lượng tồn bằng 0, nếu sau này có nhập hàng thì sẽ cập nhật sau
    kho[nKho].soLuongTon = 0;

    // Kiểm tra nếu mã hàng tồn tại trong kho
    int viTriCu = -1;
    for (int i = 0; i < nKho; i++) {
        if (kho[i].maHang == newMaHang) {
            viTriCu = i;
            break; 
        }
    }

    if (viTriCu != -1) {
        // Nếu đã có trong kho sẽ lấy thông tin cũ
        cout << ">> He thong nhan dien ma hang [" << newMaHang << "] da ton tai.\n";
        cout << ">> Tu dong dong bo thong tin san pham: " << kho[viTriCu].tenHang << "\n";
        kho[nKho].tenHang = kho[viTriCu].tenHang;
        kho[nKho].donViTinh = kho[viTriCu].donViTinh;
        kho[nKho].giaNhap = kho[viTriCu].giaNhap;
        kho[nKho].giaBan = kho[viTriCu].giaBan;
        kho[nKho].mucTonToiThieu = kho[viTriCu].mucTonToiThieu;
    } 
    else {
        //Nhập thông tin sản phẩm
        cout << "Hang moi! Tien hanh them vao danh muc kho...\n";
        cout << "-Nhap ten mat hang: ";
        cin.ignore(); getline(cin, kho[nKho].tenHang);
        cout << "-Nhap don vi tinh mat hang: ";
        getline(cin, kho[nKho].donViTinh);
        
        do {
            cout << "-Nhap gia nhap mat hang: "; cin >> kho[nKho].giaNhap;
            if (kho[nKho].giaNhap < 0) cout << "Gia nhap khong duoc am! Vui long nhap lai.\n";
        } while (kho[nKho].giaNhap < 0);

        do {
            cout << "-Nhap gia ban mat hang: "; cin >> kho[nKho].giaBan;
            if (kho[nKho].giaBan < 0) cout << "Gia ban khong duoc am! Vui long nhap lai.\n";
        } while (kho[nKho].giaBan < 0);

        do {
            cout << "-Nhap muc ton toi thieu: "; cin >> kho[nKho].mucTonToiThieu;
            if (kho[nKho].mucTonToiThieu < 0) cout << "Muc ton khong duoc am! Vui long nhap lai.\n";
        } while (kho[nKho].mucTonToiThieu < 0);
    }

    nKho++;
}

// Xử lý nhập hàng: thu thập thông tin nhà cung cấp và chi tiết sản phẩm
void xuLyNhapHang(MatHang*& kho, int& nKho, int& sucChuaKho, HoaDonNhap*& dsHDN, int& nHDN, int& sucChuaHDN) {
    HoaDonNhap hdn;
    // Bước 1: Thu thập thông tin cơ bản hoá đơn nhập
    // Các mã hoá đơn nhập sẽ có định dạng "SMINxxxxxx" với xxxxxx là số thứ tự 6 chữ số, tự động tăng dần theo số lượng hoá đơn nhập đã có
    string numStr = to_string(nHDN + 1);
    while (numStr.length() < 6) {
        numStr = "0" + numStr;
    }
    hdn.maHDN = "SMIN" + numStr;

    cout << "**Ma hoa don nhap : " << hdn.maHDN << "\n";
    cout << "Nhap ten nha cung cap: ";
    cin.ignore(); getline(cin, hdn.ncc.tenNCC);
    cout << "Nhap sdt nha cung cap: "; getline(cin, hdn.ncc.soDienThoai);
    cout << "Nhap dia chi nha cung cap: "; getline(cin, hdn.ncc.diaChi);
    cout << "Nhap diem chat luong cua nha cung cap (1 - 5): "; cin >> hdn.ncc.diemChatLuong;
    if (hdn.ncc.diemChatLuong < 1) {
        cout << "Diem chat luong khong hop le! Tu dong dat ve 1.\n";
        hdn.ncc.diemChatLuong = 1;
    }
    if (hdn.ncc.diemChatLuong > 5) {
        cout << "Diem chat luong khong hop le! Tu dong dat ve 5.\n";
        hdn.ncc.diemChatLuong = 5;
    }
    cout << "Nhap diem uy tin cua nha cung cap (1 - 5): "; cin >> hdn.ncc.diemUyTin;
    if (hdn.ncc.diemUyTin < 1) {
        cout << "Diem uy tin khong hop le! Tu dong dat ve 1.\n";
        hdn.ncc.diemUyTin = 1;
    }
    if (hdn.ncc.diemUyTin > 5) {
        cout << "Diem uy tin khong hop le! Tu dong dat ve 5.\n";
        hdn.ncc.diemUyTin = 5;
    }

    cout << "Nhap ngay hang duoc nhap ve (ngay -> thang -> nam): "; cin >> hdn.ngayNhap.ngay >> hdn.ngayNhap.thang >> hdn.ngayNhap.nam;
    chuanHoaNgay(hdn.ngayNhap);

    // Bước 2: Nhập chi tiết các hẳng hàng trong hoá đơn
    do{
        cout << "Nhap so luong mat hang can nhap: "; cin >> hdn.soLuongMatHang;
        if (hdn.soLuongMatHang <= 0) cout << "So luong mat hang phai lon hon 0! Vui long nhap lai.\n";
    } while (hdn.soLuongMatHang <= 0);

    // Tạo mảng động cho cấc hàng trong hoá đơn:
    hdn.danhSachNhap = new ChiTietHoaDon[hdn.soLuongMatHang];
    hdn.tongTien = 0;

    for (int i = 0; i < hdn.soLuongMatHang; i++) {
        string maTemp;
        Ngay hsdTemp;
        cout << "\nNhap ma hang thu " << i + 1 << ": "; cin >> maTemp;
        do{
            cout << "Nhap han su dung (ngay -> thang -> nam): "; cin >> hsdTemp.ngay >> hsdTemp.thang >> hsdTemp.nam;
            chuanHoaNgay(hsdTemp);
            if (tinhSoNgayConLai(hsdTemp, hdn.ngayNhap) < 0) {
                cout << "Loi: Han su dung da qua moc ngay nhap kho! Khong the nhap hang het han. Vui long nhap lai.\n";
            }
        }while(tinhSoNgayConLai(hsdTemp, hdn.ngayNhap) < 0);

        // Check xem id có trong kho chưa
        int viTri = timKiemMatHang(kho, nKho, maTemp, hsdTemp); 

        if (viTri != -1) {                              // Case hàng còn tồn trong kho
            int sl;
            do {
                cout << "Hang da co trong kho. Nhap so luong nap them vao kho: "; cin >> sl;
                if (sl <= 0) cout << "So luong phai lon hon 0! Vui long nhap lai.\n";
            } while (sl <= 0);
            
            // Cập nhật tồn kho
            capNhatTonKho(kho, nKho, maTemp, hsdTemp, sl); 
            
            // Ghi nhẬn chi tiết trong hoá đơn
            hdn.danhSachNhap[i] = {maTemp, sl, kho[viTri].giaNhap};
        } 
        else {                                          // Case hàng mới
            // Đăng ký sản phẩm mới (nKho sẽ tăng bên trong themMatHang)
            themMatHang(kho, nKho, sucChuaKho, maTemp, hsdTemp);
 
            int slNew;
            do {
                cout << "Nhap so luong nap them vao kho: "; cin >> slNew;
                if (slNew <= 0) cout << "So luong phai lon hon 0! Vui long nhap lai.\n";
            } while (slNew <= 0);
            
            // Đặt số lượng cho sản phẩm vừa thêm
            kho[nKho - 1].soLuongTon = slNew;

            // Ghi nhẬn chi tiết trong hoá đơn
            hdn.danhSachNhap[i] = {kho[nKho - 1].maHang, slNew, kho[nKho - 1].giaNhap};
        }
        hdn.tongTien += (hdn.danhSachNhap[i].soLuong * hdn.danhSachNhap[i].donGia);
    }

    // Bước 3: In hoá đơn xác nhận
    if (hdn.soLuongMatHang > 0) {
        cout << "\n========================================================\n";
        cout << "       XAC NHAN DANH SACH MAT HANG DA NHAP\n";
        cout << "========================================================\n";
        cout << left << setw(15) << "Ma Hang" << setw(25) << "Ten Hang" << "So Luong\n";
        cout << "--------------------------------------------------------\n";
        for (int i = 0; i < hdn.soLuongMatHang; i++) {
            // Giá trị mặc định, đề phòng trường hợp có mặt hàng nhưng bị xoá tên trong file
            string ten = "None";
            for (int j = 0; j < nKho; j++) {
                if (kho[j].maHang == hdn.danhSachNhap[i].maHang) {
                    ten = kho[j].tenHang;
                    break; 
                }
            }
            cout << left << setw(15) << hdn.danhSachNhap[i].maHang 
                 << setw(25) << ten 
                 << hdn.danhSachNhap[i].soLuong << "\n";
        }
        cout << "========================================================\n";
    }

    // Bước 4: Lưu hoá đơn vào danh sách quản lý (mở rộng nếu cần)
    if(nHDN >= sucChuaHDN){
        int newSucChua = sucChuaHDN * 2 + 1;
        HoaDonNhap* temp = new HoaDonNhap[newSucChua];
        for (int i = 0; i < nHDN; i++) temp[i] = dsHDN[i];
        delete[] dsHDN;
        dsHDN = temp;
        sucChuaHDN = newSucChua;
    }

    dsHDN[nHDN++] = hdn;
    cout << "\n-Xu ly nhap hang thanh cong. Tong tien: " << fixed << setprecision(0) << hdn.tongTien << "VND.\n";
    cout << defaultfloat;
}

// Xử lý xuất hàng: thu thập thông tin khách hàng và chi tiết sản phẩm bán
void xuLyXuatHang(MatHang*& kho, int& nKho, int& sucChuaKho, HoaDonXuat*& dsHDX, int& nHDX, int& sucChuaHDX){
    // Kiểm tra xem kho có hàng không
    if(nKho == 0){
        cout << "Xin loi, kho dang trong, hien khong co mat hang nao de ban.\n";
        return;
    }

    HoaDonXuat hdx;

    // Bước 1: Thu thập thông tin hoá đơn xuất
    // Các mã hoá đơn nhập sẽ có định dạng "SMOUTxxxxxx" với xxxxxx là số thứ tự 6 chữ số, tự động tăng dần theo số lượng hoá đơn nhập đã có
    string numStr = to_string(nHDX + 1);
    while (numStr.length() < 6) {
        numStr = "0" + numStr;
    }
    hdx.maHDX = "SMOUT" + numStr;

    cout << "**Ma hoa don xuat : " << hdx.maHDX << "\n";
    cout << "Nhap ten khach hang: ";
    cin.ignore(); getline(cin, hdx.khachHang);
    cout << "Nhap ngay xuat hang (ngay -> thang -> nam): "; cin >> hdx.ngayXuat.ngay >> hdx.ngayXuat.thang >> hdx.ngayXuat.nam;
    chuanHoaNgay(hdx.ngayXuat);

    // Bước 2: Chọn sản phẩm bán
    cout << "Chon so luong loai hang muon mua: "; cin >> hdx.soLuongMatHang;

    // Cấp phát mảng động cho chi tiết hàng trong hoá đơn:
    hdx.danhSachBan = new ChiTietHoaDon[hdx.soLuongMatHang];
    hdx.tongTien = 0;

    // Struct + mảng động hỗ trợ gộp các lô hàng cùng loại để hiển thị tổng tồn kho trước khi chọn bán
    struct HangHoaGop{ 
        string maHang; 
        string tenHang; 
        int tongTon; 
    };
    HangHoaGop* dsGop = new HangHoaGop[nKho]; 

    for (int i = 0; i < hdx.soLuongMatHang; i++){
        int soLoaiHang = 0;
        for (int k = 0; k < nKho; k++) {
            if (kho[k].soLuongTon <= 0) continue;

            //Bỏ qua không gộp các lô hàng đã hết hạn
            if (tinhSoNgayConLai(kho[k].hanSuDung, hdx.ngayXuat) < 0) continue;
            
            bool found = false;
            for (int j = 0; j < soLoaiHang; j++) {
                if (dsGop[j].maHang == kho[k].maHang) {
                    dsGop[j].tongTon += kho[k].soLuongTon;
                    found = true; break;
                }
            }
            if (!found) {
                dsGop[soLoaiHang].maHang = kho[k].maHang;
                dsGop[soLoaiHang].tenHang = kho[k].tenHang;
                dsGop[soLoaiHang].tongTon = kho[k].soLuongTon;
                soLoaiHang++;
            }
        }

        if (soLoaiHang == 0) {
            cout << "Kho hien tai da het sach hang de tiep tuc ban!\n";
            hdx.soLuongMatHang = i; 
            break; 
        }

        cout << "\n--- DANH MUC MAT HANG CO SAN (Lan chon " << i+1 << "/" << hdx.soLuongMatHang << ") ---\n";
        cout << left << setw(5) << "STT" << setw(15) << "Ma Hang" << setw(25) << "Ten Hang" << "Tong Ton Kho\n";
        for(int j = 0; j < soLoaiHang; j++) {
            cout << left << setw(5) << j+1 << setw(15) << dsGop[j].maHang << setw(25) << dsGop[j].tenHang << dsGop[j].tongTon << "\n";
        }
        
        int chonMon;
        do {
            cout << "Chon STT mat hang muon xuat (1 - " << soLoaiHang << "): ";
            cin >> chonMon;
            if (chonMon < 1 || chonMon > soLoaiHang) cout << "Lua chon khong hop le!\n";
        } while (chonMon < 1 || chonMon > soLoaiHang);
        
        string maCanTim = dsGop[chonMon - 1].maHang;
        
        int soLoThucTe = 0;
        for (int j = 0; j < nKho; j++) {
            if (kho[j].maHang == maCanTim && kho[j].soLuongTon > 0 && tinhSoNgayConLai(kho[j].hanSuDung, hdx.ngayXuat) >= 0) soLoThucTe++;
        }

        int* dsViTriDong = new int[soLoThucTe]; 
        int chiSo = 0;
        for (int j = 0; j < nKho; j++) {
            if (kho[j].maHang == maCanTim && kho[j].soLuongTon > 0 && tinhSoNgayConLai(kho[j].hanSuDung, hdx.ngayXuat) >= 0) dsViTriDong[chiSo++] = j;
        }

        cout << "\nTim thay " << soLoThucTe << " lo hang cho san pham [" << dsGop[chonMon-1].tenHang << "]. Chon STT lo:\n";
        for (int j = 0; j < soLoThucTe; j++) {
            int vt = dsViTriDong[j];
            cout << j + 1 << ". HSD: " << kho[vt].hanSuDung.ngay << "/" << kho[vt].hanSuDung.thang << "/" << kho[vt].hanSuDung.nam 
                 << " - Ton: " << kho[vt].soLuongTon << " (" << kho[vt].donViTinh << ")\n";
        }

        int choice;
        do {
            cout << "Chon STT lo hang muon xuat (1-" << soLoThucTe << "): ";
            cin >> choice;
            if (choice < 1 || choice > soLoThucTe) {
                cout << "STT khong hop le, moi ban nhap lai!\n";
            }
        } while (choice < 1 || choice > soLoThucTe);

        int vtThucTe = dsViTriDong[choice - 1]; 
        int sl; 
        do {
            cout << "Nhap so luong mua: "; cin >> sl;
            if (sl <= 0) cout << "So luong phai lon hon 0! Vui long nhap lai.\n";
        } while (sl <= 0);

        if (kho[vtThucTe].soLuongTon >= sl) {
            capNhatTonKho(kho, nKho, maCanTim, kho[vtThucTe].hanSuDung, -sl); 

            hdx.danhSachBan[i] = {maCanTim, sl, kho[vtThucTe].giaBan};
            hdx.tongTien += (sl * hdx.danhSachBan[i].donGia);
            cout << ">> Xac nhan ban " << sl << " " << kho[vtThucTe].donViTinh << " tu lo HSD " << kho[vtThucTe].hanSuDung.ngay << "/" << kho[vtThucTe].hanSuDung.thang << "/" << kho[vtThucTe].hanSuDung.nam << ".\n";
        } 
        else {
            cout << "Loi: Khong du hang trong lo nay. Vui long chon lai.\n"; i--;
        }
        delete[] dsViTriDong;
    }
    
    delete[] dsGop;

    // Bước 3: In hoá đơn xác nhận (Bill)
    if (hdx.soLuongMatHang > 0) {
        cout << "\n========================================================\n";
        cout << "       XAC NHAN DANH SACH MAT HANG DA XUAT\n";
        cout << "========================================================\n";
        cout << left << setw(15) << "Ma Hang" << setw(25) << "Ten Hang" << "So Luong\n";
        cout << "--------------------------------------------------------\n";
        for (int i = 0; i < hdx.soLuongMatHang; i++) {
            string ten = "None";
            for (int j = 0; j < nKho; j++) {
                if (kho[j].maHang == hdx.danhSachBan[i].maHang) {
                    ten = kho[j].tenHang;
                    break;
                }
            }
            cout << left << setw(15) << hdx.danhSachBan[i].maHang 
                 << setw(25) << ten 
                 << hdx.danhSachBan[i].soLuong << "\n";
        }
        cout << "========================================================\n";
    }

    // Bước 4: Lưu hoá đơn vào danh sách quản lý (mở rộng nếu cần)
    if(nHDX >= sucChuaHDX){
        int newSucChua = sucChuaHDX * 2 + 1; 
        HoaDonXuat* temp = new HoaDonXuat[newSucChua];
        for (int i = 0; i < nHDX; i++) temp[i] = dsHDX[i];
        delete[] dsHDX;
        dsHDX = temp;
        sucChuaHDX = newSucChua;
    }

    dsHDX[nHDX++] = hdx;
    cout << "-Xu ly xuat hang thanh cong. Tong tien: " << fixed << setprecision(0) << hdx.tongTien << "VND.\n";
    cout << defaultfloat;
}

// Cảnh báo các sản phẩm sắp hết hạn sử dụng
void canhBaoHetHan(const MatHang* kho, int nKho, Ngay homNay) {
    cout << "\n======================================================================\n";
    cout << "                  CANH BAO MAT HANG SAP HET HAN\n";
    cout << "======================================================================\n";
    cout << left << setw(10) << "Ma Hang" << setw(25) << "Ten Hang" << setw(15) << "Han Su Dung" << "Trang Thai\n";
    cout << "----------------------------------------------------------------------\n";

    bool coHangSapHetHan = false;

    for (int i = 0; i < nKho; i++) {
        int soNgayConLai = tinhSoNgayConLai(kho[i].hanSuDung, homNay);

        if (soNgayConLai <= 7) {
            // Tạo chuỗi hạn sử dụng bằng cách ghép số lại
            string hsdStr = to_string(kho[i].hanSuDung.ngay) + "/" + to_string(kho[i].hanSuDung.thang) + "/" + to_string(kho[i].hanSuDung.nam);
            
            // Dùng if-else truyền thống để xét trạng thái
            string trangThai;
            if (soNgayConLai < 0) {
                trangThai = "DA QUA HAN!";
            } else {
                trangThai = "Con " + to_string(soNgayConLai) + " ngay";
            }

            cout << left << setw(10) << kho[i].maHang 
                 << setw(25) << kho[i].tenHang 
                 << setw(15) << hsdStr 
                 << trangThai << "\n";
            
            coHangSapHetHan = true;
        }
    }

    if (!coHangSapHetHan) cout << "Khong co mat hang nao sap het han\n";
}

// Cảnh báo các sản phẩm thiếu so với mức tồn tối thiểu
void canhBaoHetHang(const MatHang* kho, int nKho) {
    cout << "\n======================================================================\n";
    cout << "              CANH BAO MAT HANG SAP HET HANG\n";
    cout << "======================================================================\n";
    cout << left << setw(10) << "Ma Hang" << setw(25) << "Ten Hang" << setw(15) << "Ton Kho" << "Muc Toi Thieu\n";
    cout << "----------------------------------------------------------------------\n";
    bool coHangThieu = false;

    for (int i = 0; i < nKho; i++) {
        if (kho[i].soLuongTon < kho[i].mucTonToiThieu) {
            cout << left << setw(10) << kho[i].maHang << setw(25) << kho[i].tenHang << setw(15) << kho[i].soLuongTon << kho[i].mucTonToiThieu << "\n";
            
            coHangThieu = true;
        }
    }

    if (!coHangThieu) cout << "Cac ma hang deu dam bao so luong ton kho\n";
    cout << "======================================================================\n";
}

// Thống kê giá trị tồn kho theo từng sản phẩm
void thongKeTonKho(const MatHang* kho, int nKho) {
    cout << "\n======================================================================\n";
    cout << "                THONG KE GIA TRI TON KHO\n";
    cout << "======================================================================\n";
    cout << left << setw(10) << "Ma Hang" << setw(25) << "Ten Hang" << setw(15) << "So Luong" << "Gia Tri Nhap\n";
    cout << "----------------------------------------------------------------------\n";
    double tongGiaTri = 0;

    for (int i = 0; i < nKho; i++) {
        double giaTri = kho[i].soLuongTon * kho[i].giaNhap;
        tongGiaTri += giaTri;

        cout << left << setw(10) << kho[i].maHang << setw(25) << kho[i].tenHang << setw(15) << (to_string(kho[i].soLuongTon) + " " + kho[i].donViTinh)<< fixed << setprecision(0) << giaTri << " VND\n";
        cout << defaultfloat;
    }

    cout << "======================================================================\n";
    cout << "TONG GIA TRI KHO: " << fixed << setprecision(0) << tongGiaTri << " VND\n";
    cout << "======================================================================\n";
    cout << defaultfloat;
}

// Phân tích xu hướng bán hàng: xác định sản phẩm bán chạy nhất
void baoCaoThongMinh(const MatHang* kho, int nKho, const HoaDonXuat* dsHDX, int nHDX){
    cout << "\n========================================================\n";
    cout << "             BAO CAO PHAN TICH XU HUONG\n";
    cout << "========================================================\n";
    if (nHDX == 0) {
        cout << "Chua co hoa don xuat nao de thong ke.\n";
        cout << "========================================================\n";
        return;
    }
    
    // Đếm tổng số chi tiết hàng bán
    int tongChiTiet = 0;
    for (int i = 0; i < nHDX; i++) {
        tongChiTiet += dsHDX[i].soLuongMatHang;
    }
    
    // Cấp phát mảng động
    string* dsMaHang = new string[tongChiTiet];
    int* dsTongSoLuong = new int[tongChiTiet];
    int soLoaiHang = 0;
    
    // Duyệt qua tất cả hoá đơn xuất, tính tổng số lượng bán mỗi loại hàng
    for (int i = 0; i < nHDX; i++) {
        for (int j = 0; j < dsHDX[i].soLuongMatHang; j++) {
            string maHang = dsHDX[i].danhSachBan[j].maHang;
            int soLuong = dsHDX[i].danhSachBan[j].soLuong;
            
            // Kiểm tra xem mã hàng này đã tồn tại chưa
            bool found = false;
            for (int k = 0; k < soLoaiHang; k++) {
                if (dsMaHang[k] == maHang) {
                    dsTongSoLuong[k] += soLuong;
                    found = true;
                    break;
                }
            }
            
            // Nếu chưa có thì thêm vào
            if (!found) {
                dsMaHang[soLoaiHang] = maHang;
                dsTongSoLuong[soLoaiHang] = soLuong;
                soLoaiHang++;
            }
        }
    }
    
    // Tìm mặt hàng bán chạy nhất
    if (soLoaiHang > 0){
        string maHangBanChay = dsMaHang[0];
        int maxSoLuongBan = dsTongSoLuong[0];
        
        for (int i = 1; i < soLoaiHang; i++) {
            if (dsTongSoLuong[i] > maxSoLuongBan) {
                maxSoLuongBan = dsTongSoLuong[i];
                maHangBanChay = dsMaHang[i];
            }
        }

        string tenHangBanChay = "None";
        for (int i = 0; i < nKho; i++) {
            if (kho[i].maHang == maHangBanChay) {
                tenHangBanChay = kho[i].tenHang;
                break;
            }
        }
    
        cout << left << setw(25) << "Mat hang ban chay nhat:" << "[" << maHangBanChay << "] - " << tenHangBanChay << "\n";
        cout << left << setw(25) << "Tong so luong da ban:" << maxSoLuongBan << "\n";
        cout << "--------------------------------------------------------\n";
        cout << "DU DOAN XU HUONG:\n";
        cout << ">> Ma hang " << maHangBanChay << " van se tiep tuc ban chay, nen du tru them mat hang nay.\n";
    }

    else{
        cout << "Khong co mat hang nao duoc ban.\n";
    }
    cout << "========================================================\n";
    
    delete[] dsMaHang;
    delete[] dsTongSoLuong;
}

void toiUuKho(const MatHang* kho, int nKho, const HoaDonXuat* dsHDX, int nHDX){
    if (nHDX == 0) {
        cout << "\n========================================================\n";
        cout << "             BAO CAO TOI UU KHO\n";
        cout << "========================================================\n";
        cout << "Chua co du lieu ban hang de toi uu kho.\n";
        cout << "========================================================\n";
        return;
    }

    // Lấy danh sách sản phẩm duy nhất từ kho
    DoanhSoMatHang* dsDoanhSo = new DoanhSoMatHang[nKho];
    int soLoaiHang = 0;

    for (int i = 0; i < nKho; i++) {
        // Kiểm tra xem mã này đã có trong dsDoanhSo chưa
        bool tonTai = false;
        for (int j = 0; j < soLoaiHang; j++) {
            if (dsDoanhSo[j].maHang == kho[i].maHang) {
                tonTai = true; break;
            }
        }
        
        if (!tonTai) {
            dsDoanhSo[soLoaiHang].maHang = kho[i].maHang;
            dsDoanhSo[soLoaiHang].soLuongBan = 0;
            
            // Tính tổng số lượng đã bán cho sản phẩm này từ tất cả hoá đơn xuất
            for (int h = 0; h < nHDX; h++) {
                for (int k = 0; k < dsHDX[h].soLuongMatHang; k++) {
                    if (dsHDX[h].danhSachBan[k].maHang == kho[i].maHang) {
                        dsDoanhSo[soLoaiHang].soLuongBan += dsHDX[h].danhSachBan[k].soLuong;
                    }
                }
            }
            soLoaiHang++;
        }
    }

    //Sắp xếp ds theo số lượng bán giảm dần
    for (int i = 0; i < soLoaiHang - 1; i++) {
        for (int j = 0; j < soLoaiHang - i - 1; j++) {
            if (dsDoanhSo[j].soLuongBan < dsDoanhSo[j + 1].soLuongBan) {
                DoanhSoMatHang temp = dsDoanhSo[j];
                dsDoanhSo[j] = dsDoanhSo[j + 1];
                dsDoanhSo[j + 1] = temp;
            }
        }
    }

    // Hiển thị danh sách mặt hàng theo thứ tự doanh số
    //setw(n) căn chỉnh khoảng trắng sau n ký tự, left căn lề trái.
    cout << "\n=== Danh sach mat hang theo doanh so ===\n";
    cout << left << setw(15) << "Ma Hang" << setw(15) << "Tong Ban" << "Vi Tri De Xuat\n";
    for (int i = 0; i < soLoaiHang; i++) {
        string viTri;
        if (i < 3) viTri = "KHU VUC A (Gan cua)";
        else if (i < 7) viTri = "KHU VUC B (Giua kho)";
        else viTri = "KHU VUC C (Cuoi kho)";
        
        cout << left << setw(15) << dsDoanhSo[i].maHang << setw(15) << dsDoanhSo[i].soLuongBan << viTri << "\n";
    }

    delete[] dsDoanhSo;
}

void SaveData(const MatHang* kho, int nKho, const HoaDonNhap* dsHDN, int nHDN, const HoaDonXuat* dsHDX, int nHDX, bool isSilent){
    //Lưu thông tin kho hàng
    ofstream fKho("Kho.txt");

    if (fKho.is_open()) {
        fKho << nKho << "\n";
        for (int i = 0; i < nKho; i++){
            fKho << kho[i].maHang << "\n";
            fKho << kho[i].tenHang << "\n";
            fKho << kho[i].donViTinh << "\n";
            fKho << kho[i].giaNhap << " " << kho[i].giaBan << " " << kho[i].soLuongTon << "\n";
            fKho << kho[i].hanSuDung.ngay << " " << kho[i].hanSuDung.thang << " " << kho[i].hanSuDung.nam << "\n";
            fKho << kho[i].mucTonToiThieu << "\n";
        }

        fKho.close();
    }
    else if (!isSilent) {
        cout << "Loi! Khong the tao/mo file kho.\n";
    }

    //Lưu thông tin hoá đơn nhập
    ofstream fHDN("HoaDonNhap.txt");
    
    if(fHDN.is_open()){
        fHDN << nHDN << "\n";
        for (int i = 0; i < nHDN; i++) {
            fHDN << dsHDN[i].maHDN << "\n";
            fHDN << dsHDN[i].ngayNhap.ngay << " " << dsHDN[i].ngayNhap.thang << " " << dsHDN[i].ngayNhap.nam << "\n";
            fHDN << dsHDN[i].ncc.tenNCC << "\n"; 
            fHDN << dsHDN[i].ncc.soDienThoai << "\n";
            fHDN << dsHDN[i].ncc.diaChi << "\n";
            fHDN << dsHDN[i].ncc.diemChatLuong << "\n";
            fHDN << dsHDN[i].ncc.diemUyTin << "\n";
            
            fHDN << dsHDN[i].soLuongMatHang << "\n";
            for (int j = 0; j < dsHDN[i].soLuongMatHang; j++) {
                fHDN << dsHDN[i].danhSachNhap[j].maHang << " " << dsHDN[i].danhSachNhap[j].soLuong << " " << dsHDN[i].danhSachNhap[j].donGia << "\n";
            }
            fHDN << dsHDN[i].tongTien << "\n";
        }
    
        fHDN.close();
    }
    else if (!isSilent) {
        cout << "Loi! Khong the tao/mo file hoa don nhap.\n";
    }


    //Lưu thông tin hoá đơn xuất
    ofstream fHDX("HoaDonXuat.txt");

    if(fHDX.is_open()){
        fHDX << nHDX << "\n";
        for (int i = 0; i < nHDX; i++){
            fHDX << dsHDX[i].maHDX << "\n";
            fHDX << dsHDX[i].ngayXuat.ngay << " " << dsHDX[i].ngayXuat.thang << " " << dsHDX[i].ngayXuat.nam << "\n";
            fHDX << dsHDX[i].khachHang <<"\n";

            fHDX << dsHDX[i].soLuongMatHang << "\n";
            for (int j = 0; j < dsHDX[i].soLuongMatHang; j++){
                fHDX << dsHDX[i].danhSachBan[j].maHang << " " << dsHDX[i].danhSachBan[j].soLuong << " " << dsHDX[i].danhSachBan[j].donGia << "\n";
            }
            fHDX << dsHDX[i].tongTien << "\n";
        }

        fHDX.close();
    }
    else if (!isSilent) {
        cout << "Loi! Khong the tao/mo file hoa don xuat.\n";
    }

    if (!isSilent) {
        cout << "Da ghi file thanh cong.\n";
    }
}


void ReadData(MatHang*& kho, int& nKho, int& sucChuaKho, HoaDonNhap*& dsHDN, int& nHDN, int& sucChuaHDN, HoaDonXuat*& dsHDX, int& nHDX, int& sucChuaHDX, bool isSilent)    {


    // Trước khi đọc dữ liệu mới, giải phóng bộ nhớ cũ nếu đã có
    // Dữ liệu được xoá theo thứ tự kho -> hoá đơn nhập -> hoá đơn xuất để tránh lỗi truy cập bộ nhớ
    if (kho != nullptr) {
        delete[] kho;
        kho = nullptr;
    }

    if (dsHDN != nullptr) {
        for (int i = 0; i < nHDN; i++) delete[] dsHDN[i].danhSachNhap;
        delete[] dsHDN;
        dsHDN = nullptr;
    }

    if (dsHDX != nullptr) {
        for (int i = 0; i < nHDX; i++) delete[] dsHDX[i].danhSachBan;
        delete[] dsHDX;
        dsHDX = nullptr;
    }

    // Reset lại chỉ số phòng trường hợp file không tồn tại (Lần chạy đầu tiên)
    nKho = 0; nHDN = 0; nHDX = 0;
    sucChuaKho = 0; sucChuaHDN = 0; sucChuaHDX = 0;

    //Đọc dữ liệu kho
    ifstream fKho ("Kho.txt");
    
    if(fKho.is_open()){
        fKho >> nKho;
        sucChuaKho = nKho + 10;
        kho = new MatHang[sucChuaKho];  

        for(int i = 0; i < nKho; i++){
            fKho >> kho[i].maHang;
            fKho.ignore();
            getline(fKho,kho[i].tenHang);
            getline(fKho,kho[i].donViTinh);
            fKho >> kho[i].giaNhap >> kho[i].giaBan >> kho[i].soLuongTon;
            fKho >> kho[i].hanSuDung.ngay >> kho[i].hanSuDung.thang >> kho[i].hanSuDung.nam;
            chuanHoaNgay(kho[i].hanSuDung);
            fKho >> kho[i].mucTonToiThieu;
        }

        fKho.close();
    }
    else if (!isSilent) {
        cout << "Loi! Khong the mo file kho.\n";
    }

    //Đọc dữ liệu hoá đơn nhập
    ifstream fHDN("HoaDonNhap.txt");

    if(fHDN.is_open()){
        fHDN >> nHDN;
        sucChuaHDN = nHDN + 10;
        dsHDN = new HoaDonNhap[sucChuaHDN];

        for(int i = 0; i < nHDN; i++){
            fHDN >> dsHDN[i].maHDN;
            fHDN >> dsHDN[i].ngayNhap.ngay >> dsHDN[i].ngayNhap.thang >> dsHDN[i].ngayNhap.nam;
            chuanHoaNgay(dsHDN[i].ngayNhap);
            
            fHDN.ignore();
            getline(fHDN, dsHDN[i].ncc.tenNCC);
            getline(fHDN, dsHDN[i].ncc.soDienThoai);
            getline(fHDN, dsHDN[i].ncc.diaChi);
            fHDN >> dsHDN[i].ncc.diemChatLuong;
            if (dsHDN[i].ncc.diemChatLuong < 1) dsHDN[i].ncc.diemChatLuong = 1;
            if (dsHDN[i].ncc.diemChatLuong > 5) dsHDN[i].ncc.diemChatLuong = 5;
            fHDN >> dsHDN[i].ncc.diemUyTin;
            if (dsHDN[i].ncc.diemUyTin < 1) dsHDN[i].ncc.diemUyTin = 1;
            if (dsHDN[i].ncc.diemUyTin > 5) dsHDN[i].ncc.diemUyTin = 5;

            fHDN >> dsHDN[i].soLuongMatHang;
            dsHDN[i].danhSachNhap = new ChiTietHoaDon[dsHDN[i].soLuongMatHang];
            for (int j = 0; j < dsHDN[i].soLuongMatHang; j++) {
                fHDN >> dsHDN[i].danhSachNhap[j].maHang >> dsHDN[i].danhSachNhap[j].soLuong >> dsHDN[i].danhSachNhap[j].donGia;
            }
            fHDN >> dsHDN[i].tongTien;
        }

        fHDN.close();
    }
    else if (!isSilent) {
        cout << "Loi! Khong the mo file hoa don nhap.\n";
    }

    //Đọc dữ liệu hoá đơn xuất
    ifstream fHDX("HoaDonXuat.txt");

    if(fHDX.is_open()){
        fHDX >> nHDX;
        sucChuaHDX = nHDX + 10;
        dsHDX = new HoaDonXuat[sucChuaHDX];

        for (int i = 0; i < nHDX; i++){
            fHDX >> dsHDX[i].maHDX;
            fHDX >> dsHDX[i].ngayXuat.ngay >> dsHDX[i].ngayXuat.thang >> dsHDX[i].ngayXuat.nam;
            chuanHoaNgay(dsHDX[i].ngayXuat);

            fHDX.ignore();
            getline(fHDX, dsHDX[i].khachHang);

            fHDX >> dsHDX[i].soLuongMatHang;
            dsHDX[i].danhSachBan = new ChiTietHoaDon[dsHDX[i].soLuongMatHang];
            for (int j = 0; j < dsHDX[i].soLuongMatHang; j++){
                fHDX >> dsHDX[i].danhSachBan[j].maHang >> dsHDX[i].danhSachBan[j].soLuong >> dsHDX[i].danhSachBan[j].donGia;
            }
            fHDX >> dsHDX[i].tongTien;
        }

        fHDX.close();
    }
    else if (!isSilent) {
        cout << "Loi! Khong the mo file hoa don xuat.\n";
    }

    if (!isSilent) {
    cout << "Da doc file thanh cong!\n";
    }
}