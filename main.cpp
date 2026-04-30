#include <iostream>
#include <string>
using namespace std;

////1.Struct

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
    int soLuongLoaiHang; 
    double tongTien;
};

//Bắn
struct HoaDonXuat {
    string maHDX;
    Ngay ngayXuat;
    string khachHang;
    
    ChiTietHoaDon* danhSachBan; 
    int soLuongLoaiHang; 
    double tongTien;
};

//hiện tại logic khi nhập hàng là : lượng tồn + lượng trong hoá đơn, xuất hàng thì ngược lại (để đây case nghĩ ra đc cái logic nếu tính cả sl hàng tồn trong kho)

////2.Hàm core
//Cả 2 quá trình nhập & xuất hàng đều theo cấu trúc 1 hàm chính dùng để xử lí các tác vụ chung (hàm đầu) và các hàm sau dùng để support những yêu cầu 
//cần thiết của quá trình đấy
//Nói dễ hiểu thì *& là kiểu truyền tham chiếu đến một ptr, dạng kiểu vẫn save address dưới dạng ptr và vẫn có thể direct thay đổi đc những gì bên trong đấy
//a.Nhập hàng

void xuLyNhapHang(MatHang*& kho, int& nKho, HoaDonNhap*& dsHDN, int& sucChua);
void themMatHang(MatHang*& kho, int& n, int sucChua);

//b.Xuất hàng

void xuLyXuatHang(MatHang*& kho, int& nKho, HoaDonXuat*& dsHDX, int& sucChua);

//Hàm này cả quá trình a và b đều sẽ động vào

void capNhatTonKho(MatHang*& kho, int& n, string maHang, int soLuongThayDoi);

//c. Cảnh báo(Y.c nghiệp vụ)
//const k để hàm này thay đổi dữ liệu
void canhBaoHetHan(const MatHang* kho, int n, Ngay homNay);
void canhBaoHetHang(const MatHang* kho, int n);
void thongKeTonKho(const MatHang* kho, int n);


//Sơ qua là như thế lày, somebody púsh ônêgàii

//ai đó xử lý main, r 2 ng còn lại phân ra làm nhập/ xuất phần còn lại bổ sung gì thì lúc đấy lại phân ra làm/ bổ xung tiếp