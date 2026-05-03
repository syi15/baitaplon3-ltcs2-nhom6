#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>    //idk
using namespace std;

////I.Struct

//Hàm ngày core vlin vì đc dùng để lồn(g) date vào những thứ như mặt hàng, hoá đơn nhập xuất, etc.
struct Ngay{
    int ngay;
    int thang;
    int nam;
};

//Sờ chắc cần thiết, check pdf
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

//Detail một mặt hàng trong hoá đơn, vd kiểu một hoá đơn nó có nhiều thứ thì trong đấy nó có list ra thêm 1 lone thứ khác ấy, b hiểu ý tôi k?
struct ChiTietHoaDon {
    string maHang;
    int soLuong;
    double donGia;
};

//Sờ lai
struct HoaDonNhap {
    string maHDN;
    Ngay ngayNhap;
    string nhaCungCap;
    
    ChiTietHoaDon* danhSachNhap; 
    int soLuongMatHang; 
    double tongTien;
};

//Bắn
struct HoaDonXuat {
    string maHDX;
    Ngay ngayXuat;
    string khachHang;
    
    ChiTietHoaDon* danhSachBan; 
    int soLuongMatHang; 
    double tongTien;
};

//hiện tại logic khi nhập hàng là : lượng tồn + lượng trong hoá đơn, xuất hàng thì ngược lại (để đây case nghĩ ra đc cái logic nếu tính cả sl hàng tồn trong kho)

////II.Hàm core

//Cả 2 quá trình nhập & xuất hàng đều theo cấu trúc 1 hàm chính dùng để xử lí các tác vụ chung (hàm đầu) và các hàm sau dùng để support những yêu cầu 
//cần thiết của quá trình đấy
//Nói dễ hiểu thì *& là kiểu truyền tham chiếu đến một ptr, dạng kiểu vẫn save address dưới dạng ptr và vẫn có thể direct thay đổi đc những gì bên trong đấy

//a.Nhập hàng

void xuLyNhapHang(MatHang*& kho, int& nKho, HoaDonNhap*& dsHDN, int& nHDN);
void themMatHang(MatHang*& kho, int& n, int sucChua);

//b.Xuất hàng

void xuLyXuatHang(MatHang*& kho, int& nKho, HoaDonXuat*& dsHDX, int& nHDX);


void capNhatTonKho(MatHang*& kho, int& n, string maHang, int soLuongThayDoi);           //Hàm này cả quá trình a và b đều sẽ động vào

//c. Cảnh báo(Y.c nghiệp vụ)
//const k để hàm này thay đổi dữ liệu

int tinhSoNgayConLai(Ngay hsd, Ngay homNay) {
    tm time_hsd = {0};          //tm là kiểu dữ liệu có sẵn trong ctime chứa ngày giờ (mday mon year...)
    time_hsd.tm_mday = hsd.ngay;
    time_hsd.tm_mon = hsd.thang - 1;        //Tháng bắt đầu từ 0 -> 11
    time_hsd.tm_year = hsd.nam - 1900;      //Tương tự

    tm time_homNay = {0};
    time_homNay.tm_mday = homNay.ngay;
    time_homNay.tm_mon = homNay.thang - 1;
    time_homNay.tm_year = homNay.nam - 1900;

    time_t t1 = mktime(&time_hsd);      //kiểu time_t lưu trữ tổng số giây tính từ 00:00:00 01/01/1970 (quy đổi thời gian thành giây hết để tính + -)
    time_t t2 = mktime(&time_homNay);   //hàm mktime đổi cấu trúc mm/dd/yyyy thành time_t (nó còn tự sửa input 32/01 -> 01/02)
    
    return difftime(t1, t2) / (60 * 60 * 24);       //difftime tính khoảng cách giữa t1 t2 chia 60s 60m 24h để ra số ngày
}

// Cảnh náo hết hạn
void canhBaoHetHan(const MatHang* kho, int n, Ngay homNay) {
    cout << "\n\n";
    bool coHangSapHetHan = false;

    for (int i = 0; i < n; i++) {
        int soNgayConLai = tinhSoNgayConLai(kho[i].hanSuDung, homNay);

        if (soNgayConLai <=7) {
            cout << "Ma: " << kho[i].maHang << " | Ten: " << kho[i].tenHang << " | HSD: " << kho[i].hanSuDung.ngay << "/" << kho[i].hanSuDung.thang << "/" << kho[i].hanSuDung.nam;

            if (soNgayConLai < 0) cout << " | DA QUA HAN\n";
            else cout << " | SAP HET HAN. Con " << soNgayConLai << " ngay\n";

            coHangSapHetHan = true;
        }
    }

    if (coHangSapHetHan = false) cout << "Khong co mat hang nao sap het han\n";
}

// Cảnh báo hết hàng
void canhBaoHetHang(const MatHang* kho, int n) {
    cout << "\n\n";
    bool coHangThieu = false;

    for (int i = 0; i < n; i++) {
        if (kho[i].soLuongTon < kho[i].mucTonToiThieu) {
            cout << "Ma: " << kho[i].maHang  << " | Ten: " << kho[i].tenHang  << " | Ton kho: " << kho[i].soLuongTon << " " << kho[i].donViTinh << " | Duoi muc toi thieu " << kho[i].mucTonToiThieu << "\n";
            
            coHangThieu = true;
        }
    }

    if (coHangThieu= false) cout << "Cac ma hang deu dam bao so luong ton kho\n";
}

// Thống kê tồn kho
void thongKeTonKho(const MatHang* kho, int n) {
    cout << "\n\n";
    double tongGiaTri = 0;

    for (int i = 0; i < n; i++) {
        double giaTri = kho[i].soLuongTon * kho[i].giaNhap;
        tongGiaTri += giaTri;

        cout << "Ma: " << kho[i].maHang << " | Ten: " << kho[i].tenHang << " | SL: " << kho[i].soLuongTon << " " << kho[i].donViTinh << " | Tong gia tri nhap: " << fixed << setprecision(0) << giaTri << "VND\n";
    }

    cout << "TONG GIA TRI KHO: " << fixed << setprecision(0) << tongGiaTri << "\n";
}

//d.Misc shit, cái này tôi nhớ ông có nói là sẽ dùng như kiểu để đọc file trong kho lưu và lấy ra cái gì à? giờ tôi chưa bt nên làm gì nên chắc cứ để đây

void docFile();
void luuFile();

//e.even more misc shit, nơi làm từ từ sẽ tìm thêm fuckton of workloads để doubledown bs mình cần làm và nhớ để khi hỏi k bị giãy đành đạch
int timKiemMatHang(const MatHang* kho, int nKho, string maCanTim);



//III. Main

int main(){

    //ném gì đó vào đây đi
    return 0;
}


//IV.Viết lại funct

//0.Cập nhật tồn kho viết lên đây

//1.Le' mở bát
//A.sub fuct

int timKiemMatHang(const MatHang* kho, int nKho, string maCanTim){
    for(int i = 0; i < nKho; i++){
        if(kho[i].maHang == maCanTim) return i;         //found at i
    }
    return -1;                                          //not found
}

//B.main fuct

void themMatHang(MatHang*& kho, int& n, int sucChua){}

void xuLyNhapHang(MatHang*& kho, int& nKho, int& sucChuaKho, HoaDonNhap*& dsHDN, int& nHDN, int& sucChuaHDN) {
    HoaDonNhap hdn;
    //a. Nhập thông tin cơ bản cho hoá đơn nhập
    cout << "Nhap ma hoa don nhap hang: "; cin >> hdn.maHDN;
    cout << "Nhap ten nha cung cap: ";
    cin.ignore(); getline(cin, hdn.nhaCungCap);
    cout << "Nhap ngay hang duoc nhap ve (day -> month -> year): "; cin >> hdn.ngayNhap.ngay >> hdn.ngayNhap.thang >> hdn.ngayNhap.nam;

    //b. Nhập chi tiết các đơn hàng:
    cout << "Nhap so luong mat hang can nhap: ";
    cin >> hdn.soLuongMatHang;

    //Tạo mảng động cho cấc hàng trong hoá đơn:
    hdn.danhSachNhap = new ChiTietHoaDon[hdn.soLuongMatHang];
    hdn.tongTien = 0;

    for (int i = 0; i < hdn.soLuongMatHang; i++) {
        string maTemp;
        cout << "Nhap ma hang thu " << i + 1 << ": ";
        cin >> maTemp;

        // Check xem id có trong kho chưa
        int viTri = timKiemMatHang(kho, nKho, maTemp); 

        if (viTri != -1) {                              // Case hàng còn tồn trong kho
            int sl;
            cout << "Hang da co trong kho. Nhap so luong nap them vao kho: "; cin >> sl;
            
            // Cập nhật tồn kho
            capNhatTonKho(kho, nKho, maTemp, sl); 
            
            // Lưu vào chi tiết hóa đơn
            hdn.danhSachNhap[i] = {maTemp, sl, kho[viTri].giaNhap};
        } 
        else {                                          // Case hàng mới 100%
            cout << "Hang moi! Tien hanh them vao danh muc kho...\n";
            
            // Thêm hàng
            themMatHang(kho, nKho, sucChuaKho);         //Kho++ sẽ dc thêm ở đây
 
            int slNew;
            cout << "Nhap so luong nap them vao kho: "; cin >> slNew;
            
            // Cập nhật số lượng cho mặt hàng vừa được thêm vào cuối mảng
            kho[nKho - 1].soLuongTon = slNew;

            //Lưu vào chi tiết hoá đơn
            hdn.danhSachNhap[i] = {kho[nKho - 1].maHang, slNew, kho[nKho - 1].giaNhap};
        }
        hdn.tongTien += (hdn.danhSachNhap[i].soLuong * hdn.danhSachNhap[i].donGia);
    }

    //c.Lưu hoá đơn vào danh sách quản lý
    if(nHDN >= sucChuaHDN){
        int newSucChua = sucChuaHDN * 2 + 1;                // +1 xử lý trường hợp sức chứa = 0 
        HoaDonNhap* temp = new HoaDonNhap[newSucChua];
        for (int i = 0; i < nHDN; i++) temp[i] = dsHDN[i];
        delete[] dsHDN;
        dsHDN = temp;
        sucChuaHDN = newSucChua;
    }

    dsHDN[nHDN++] = hdn;
    cout << "Xu ly nhap hang thanh cong. Tong tien: " << hdn.tongTien << "\n";
}



//Sơ qua là như thế lày, somebody púsh ônêgàii

//ai đó xử lý main, r 2 ng còn lại phân ra làm nhập/ xuất phần còn lại bổ sung gì thì lúc đấy lại phân ra làm/ bổ xung tiếp
//Sau khi xong sẽ revise lại comment để chỉnh nó lên 1 chỗ thoáng hơn/ nhìn đỡ ngứa cái code
