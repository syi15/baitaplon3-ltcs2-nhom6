#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>    //idk
#include <fstream>
using namespace std;

////I.Struct

//Hàm ngày core vlin vì đc dùng để lồn(g) date vào những thứ như mặt hàng, hoá đơn nhập xuất, etc.
struct Ngay{
    int ngay, thang, nam;
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

//Shibai
struct NhaCungCap {
    string tenNCC;
    string soDienThoai;
    string diaChi;
    int diemChatLuong;         // Đánh giá từ 1-5 sao
    int diemUyTin;          // Đánh giá từ 1-5 sao
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
    NhaCungCap ncc;
    
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

//Struct helper cho baoCaoThongMinh, chủ yếu dùng để tính tần suất
struct DoanhSoMatHang {
    string maHang;
    int soLuongBan;
};

//hiện tại logic khi nhập hàng là : lượng tồn + lượng trong hoá đơn, xuất hàng thì ngược lại (để đây case nghĩ ra đc cái logic nếu tính cả sl hàng tồn trong kho)

////II.Hàm core

//Cả 2 quá trình nhập & xuất hàng đều theo cấu trúc 1 hàm chính dùng để xử lí các tác vụ chung (hàm đầu) và các hàm sau dùng để support những yêu cầu 
//cần thiết của quá trình đấy
//Nói dễ hiểu thì *& là kiểu truyền tham chiếu đến một ptr, dạng kiểu vẫn save address dưới dạng ptr và vẫn có thể direct thay đổi đc những gì bên trong đấy

//a.Nhập hàng

void xuLyNhapHang(MatHang*& kho, int& nKho, int& sucChuaKho, HoaDonNhap*& dsHDN, int& nHDN, int& sucChuaHDN);
void themMatHang(MatHang*& kho, int& nKho, int& sucChua, string newMaHang, Ngay newHsd);

//b.Xuất hàng

void xuLyXuatHang(MatHang*& kho, int& nKho, int& sucChuaKho, HoaDonXuat*& dsHDX, int& nHDX, int& sucChuaHDX);


void capNhatTonKho(MatHang*& kho, int& nKho, string maHang, Ngay hsd, int soLuongThayDoi);           //Hàm này cả quá trình a và b đều sẽ động vào

//c. Cảnh báo(Y.c nghiệp vụ)
//const k để hàm này thay đổi dữ liệu

int tinhSoNgayConLai(Ngay hsd, Ngay homNay);
void canhBaoHetHan(const MatHang* kho, int nKho, Ngay homNay);
void canhBaoHetHang(const MatHang* kho, int nKho);
void thongKeTonKho(const MatHang* kho, int nKho);


//d.Lưu & đọc các dữ liệu trong (kho, dshdn, dshdx)

void SaveData(const MatHang* kho, int nKho, const HoaDonNhap* dsHDN, int nHDN, const HoaDonXuat* dsHDX, int nHDX);
void ReadData(MatHang*& kho, int& nKho, int& sucChuaKho, HoaDonNhap*& dsHDN, int& nHDN, int& sucChuaHDN, HoaDonXuat*& dsHDX, int& nHDX, int& sucChuaHDX);

//e. Tính năng đặc biệt

// Báo cáo thông minh
void baoCaoThongMinh(const MatHang* kho, int nKho, const HoaDonXuat* dsHDX, int nHDX);

// Tối ưu kho
void toiUuKho(const MatHang* kho, int nKho, const HoaDonXuat* dsHDX, int nHDX);

//f.even more misc shit, nơi làm từ từ sẽ tìm thêm fuckton of workloads để doubledown bs mình cần làm và nhớ để khi hỏi k bị giãy đành đạch
bool checkNgay(Ngay d1, Ngay d2);
int timKiemMatHang(const MatHang* kho, int nKho, string maHangCanTim, Ngay hsd);

//todo:
//+Quản lý nhà cung cấp
//+Main



//III. Main (chưa check)

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

    // "Thực đơn"(china system) chọn chức năng cho ứng dụng
    int choice;
    do{
        cout << "UNG DUNG QUAN LY KHO SIEU THI" << endl;
        cout << "1. Nhap hang vao kho\n"; // xuất hoá đơn nhập
        cout << "2. Xuat hang ra kho\n"; // xuất hoá đơn xuất
        cout << "3. Kiem tra mat hang sap het han"; // So sánh ngày hiện tại với HSD, cảnh báo trước 7 ngày, nếu hết hạn sẽ báo hết hạn
        cout << "4. Kiem tra mat hang sap het hang\n"; // So sánh số lượng tồn với mức tồn tối thiểu, nếu dưới sẽ báo thiếu hàng
        cout << "5. Thong ke ton kho\n"; 
        cout << "6. Luu du lieu\n"; // sẽ lưu dữ liệu vào file để lần sau mở ra vẫn còn hàng đã nhập, tránh tình trạng tắt chương trình là mất dữ liệu
        cout << "7. Doc du lieu\n"; // đọc dữ liệu từ file đã lưu vào để tiếp tục quản lý
        cout << "0. Thoat\n";
        cout << "======================================\n";
        cout << "Nhap lua chon: "; cin >> choice;

    switch (choice){
        case 1: {
            cout << "\n===Nhap hang vao kho===\n";
            xuLyNhapHang(kho, nKho, sucChuaKho, dsHDN, nHDN, sucChuaHDN);
            break;
        }
        case 2: {
            cout << "\n===Xuat hang ra kho===\n";
            xuLyXuatHang(kho, nKho, sucChuaKho, dsHDX, nHDX, sucChuaHDX);
            break;
        }
        case 3: {
            cout << "\n===Kiem tra mat hang sap het han===\n";
            Ngay homNay;
            cout << "Nhap ngay hien tai (ngay -> thang -> nam): "; cin >> homNay.ngay >> homNay.thang >> homNay.nam; // lấy ngày cứng để kiểm tra, muốn realtime có thể dùng ctime
            canhBaoHetHan(kho, nKho, homNay);
            break;
        }
        case 4: {
            cout << "\n===Kiem tra mat hang sap het hang===\n";
            canhBaoHetHang(kho, nKho);
            break;
        }
        case 5: {
            cout << "\n===Thong ke ton kho===\n";
            if (nKho == 0) 
                cout << "Kho dang trong, khong co mat hang nao de thong ke.\n"; // nếu kho trống thì báo không có mặt hàng
            else
                thongKeTonKho(kho, nKho);
            break;
        }
        case 6: {
            cout << "\n===Luu du lieu===\n";
            SaveData(kho, nKho, dsHDN, nHDN, dsHDX, nHDX);
            break;
        }
        case 7: {
            cout << "\n===Doc du lieu===\n";
            ReadData(kho, nKho, sucChuaKho, dsHDN, nHDN, sucChuaHDN, dsHDX, nHDX, sucChuaHDX);
            break;
        }
        case 0: {
            cout << "Tat ung dung, tam biet.!\n";
            break;
        }
        default: {
            cout << "Lua chon khong hop le, vui long chon lai!\n";
            break;
        }
    }
 } while (choice != 0);
 // 4': Giải phóng bộ nhớ đã cấp phát động
    delete[] kho;
    for (int i = 0; i < nHDN; i++) delete[] dsHDN[i].danhSachNhap;
    delete[] dsHDN;
    for (int i = 0; i < nHDX; i++) delete[] dsHDX[i].danhSachBan;
    delete[] dsHDX;
    return 0;
}


//IV.Viết lại funct

//0.Cập nhật tồn kho viết lên đây

//1.Le' mở bát
//A.sub fuct

bool checkNgay(Ngay d1, Ngay d2) {
    return (d1.ngay == d2.ngay && d1.thang == d2.thang && d1.nam == d2.nam);
}

int timKiemMatHang(const MatHang* kho, int nKho, string maHangCanTim, Ngay hsd){
    for(int i = 0; i < nKho; i++){
        if(kho[i].maHang == maHangCanTim && checkNgay(kho[i].hanSuDung, hsd)) return i;         //found
    }
    return -1;                                          //not found
}

//B.main fuct

void capNhatTonKho(MatHang*& kho, int& nKho, string maHang, Ngay hsd, int soLuongThayDoi){
    int viTri = timKiemMatHang(kho, nKho, maHang, hsd);
    if (viTri != -1){
        kho[viTri].soLuongTon += soLuongThayDoi;                //Vì dùng cho cả nhập/xuất nên + - được
    }
}

void themMatHang(MatHang*& kho, int& nKho, int& sucChua, string newMaHang, Ngay newHsd){
    //a. Kiểm tra xem kho có đầy chưa, chưa thì mở thêm
    if (nKho >= sucChua){
        int newSucChua = sucChua * 2 + 1;           
        //Tóm gọn cái thuật toán như sau: * 2 để giảm lượng làm việc của máy, do mỗi lần tạo và xoá array sau mỗi lần đầy kho rất tốn cpu, +1 để đảm bảo trường hợp succhua = 0 (0 * 2 + 1 = 0)
        MatHang* temp = new MatHang[newSucChua];
        for (int i = 0; i < nKho; i++) temp[i] = kho[i];
        delete[] kho;
        kho = temp;
        sucChua = newSucChua;
    }

    //b. Nhập thông tin cho mặt hàng mới
    kho[nKho].maHang = newMaHang;
    cout << "-Nhap ten mat hang: ";
    cin.ignore(); getline(cin, kho[nKho].tenHang);
    cout << "-Nhap don vi tinh mat hang: ";
    getline(cin, kho[nKho].donViTinh);
    cout << "-Nhap gia nhap mat hang: "; cin >> kho[nKho].giaNhap;
    cout << "-Nhap gia ban mat hang: "; cin >> kho[nKho].giaBan;
    kho[nKho].hanSuDung = newHsd;
    cout << "-Nhap muc ton toi thieu: "; cin >> kho[nKho].mucTonToiThieu;

    kho[nKho].soLuongTon = 0;

    nKho++;
}

void xuLyNhapHang(MatHang*& kho, int& nKho, int& sucChuaKho, HoaDonNhap*& dsHDN, int& nHDN, int& sucChuaHDN) {
    HoaDonNhap hdn;
    //a. Nhập thông tin cơ bản cho hoá đơn nhập
    cout << "Nhap ma hoa don nhap hang: "; cin >> hdn.maHDN;
    cout << "Nhap ten nha cung cap: ";
    cin.ignore(); getline(cin, hdn.ncc.tenNCC);
    cout << "Nhap sdt nha cung cap: "; getline(cin, hdn.ncc.soDienThoai);
    cout << "Nhap dia chi nha cung cap: "; getline(cin, hdn.ncc.diaChi);
    cout << "Nhap diem chat luong cua nha cung cap (1 - 5): "; cin >> hdn.ncc.diemChatLuong;
    cout << "Nhap diem uy tin cua nha cung cap (1 - 5): "; cin >> hdn.ncc.diemUyTin;

    cout << "Nhap ngay hang duoc nhap ve (ngay -> thang -> nam): "; cin >> hdn.ngayNhap.ngay >> hdn.ngayNhap.thang >> hdn.ngayNhap.nam;

    //b. Nhập chi tiết các đơn hàng:
    cout << "Nhap so luong mat hang can nhap: "; cin >> hdn.soLuongMatHang;

    // Tạo mảng động cho cấc hàng trong hoá đơn:
    hdn.danhSachNhap = new ChiTietHoaDon[hdn.soLuongMatHang];
    hdn.tongTien = 0;

    for (int i = 0; i < hdn.soLuongMatHang; i++) {
        string maTemp;
        Ngay hsdTemp;
        cout << "Nhap ma hang thu " << i + 1 << ": "; cin >> maTemp;
        cout << "Nhap han su dung (ngay -> thang -> nam): "; cin >> hsdTemp.ngay >> hsdTemp.thang >> hsdTemp.nam;

        // Check xem id có trong kho chưa
        int viTri = timKiemMatHang(kho, nKho, maTemp, hsdTemp); 

        if (viTri != -1) {                              // Case hàng còn tồn trong kho
            int sl;
            cout << "Hang da co trong kho. Nhap so luong nap them vao kho: "; cin >> sl;
            
            // Cập nhật tồn kho
            capNhatTonKho(kho, nKho, maTemp, hsdTemp, sl); 
            
            // Lưu vào chi tiết hóa đơn
            // Dòng dưới này là viết tắt của hdn.danhSachNhap[i].maHang = maTemp;....
            hdn.danhSachNhap[i] = {maTemp, sl, kho[viTri].giaNhap};



        } 
        else {                                          // Case hàng mới 100%
            cout << "Hang moi! Tien hanh them vao danh muc kho...\n";
            
            // Thêm hàng
            themMatHang(kho, nKho, sucChuaKho, maTemp, hsdTemp);         //Kho++ sẽ dc thêm ở đây
 
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
        int newSucChua = sucChuaHDN * 2 + 1;                // +1 xử lý trường hợp sức chứa = 0, giải thích ở trên
        HoaDonNhap* temp = new HoaDonNhap[newSucChua];
        for (int i = 0; i < nHDN; i++) temp[i] = dsHDN[i];
        delete[] dsHDN;
        dsHDN = temp;
        sucChuaHDN = newSucChua;
    }

    dsHDN[nHDN++] = hdn;
    cout << "\n-Xu ly nhap hang thanh cong. Tong tien: " << hdn.tongTien << ".\n";
}

void xuLyXuatHang(MatHang*& kho, int& nKho, int& sucChuaKho, HoaDonXuat*& dsHDX, int& nHDX, int& sucChuaHDX){
    if(nKho == 0){                          //check xem kho có hàng không
        cout << "Xin loi, kho dang trong, hien khong co mat hang nao de ban.\n";
        return;
    }

    HoaDonXuat hdx;

    //a. Nhập thông tin hàng
    cout << "Nhap ma hoa don xuat: "; cin >> hdx.maHDX;
    cout << "Nhap ten khach hang: ";
    cin.ignore(); getline(cin, hdx.khachHang);
    cout << "Nhap ngay xuat hang (ngay -> thang -> nam): "; cin >> hdx.ngayXuat.ngay >> hdx.ngayXuat.thang >> hdx.ngayXuat.nam;

    //b. Chọn mặt hàng, kiểm tra tồn kho
    cout << "Chon so luong mat hang muon mua: "; cin >> hdx.soLuongMatHang;

    // Tạo mảng động cho cấc hàng trong hoá đơn:
    hdx.danhSachBan = new ChiTietHoaDon[hdx.soLuongMatHang];
    hdx.tongTien = 0;

    for (int i = 0; i < hdx.soLuongMatHang; i++){
        string maCanTim;
        cout << "Nhap ma hang thu " << i+1 << ":"; cin >> maCanTim;
        
//mark đang ngẫm chỗ này.
        int soLoThucTe = 0;
        for (int j = 0; j < nKho; j++) {
            if (kho[j].maHang == maCanTim && kho[j].soLuongTon > 0) soLoThucTe++;
        }

        if (soLoThucTe == 0) {
            cout << "Loi: Het hang.\n"; i--; continue;
        }

        // Cấp mảng động
        int* dsViTriDong = new int[soLoThucTe]; 
        int chiSo = 0;
        for (int j = 0; j < nKho; j++) {
            if (kho[j].maHang == maCanTim && kho[j].soLuongTon > 0) dsViTriDong[chiSo++] = j;
        }

        // Hiển thị menu chọn lô
        cout << "Tim thay " << soLoThucTe << " lo hang. Chon STT:\n";
        for (int j = 0; j < soLoThucTe; j++) {
            int vt = dsViTriDong[j];
            cout << j + 1 << ". HSD: " << kho[vt].hanSuDung.ngay << "/" << kho[vt].hanSuDung.thang 
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

        int vtThucTe = dsViTriDong[choice - 1]; // Vị trí thật trong kho (do array lấy -1)
        int sl; cout << "Nhap so luong mua: "; cin >> sl;

        if (kho[vtThucTe].soLuongTon >= sl) {

            // Truyền dấu trừ để hàm hiểu là xuất hàng
            capNhatTonKho(kho, nKho, maCanTim, kho[vtThucTe].hanSuDung, -sl); 

            hdx.danhSachBan[i] = {maCanTim, sl, kho[vtThucTe].giaBan};
            hdx.tongTien += (sl * hdx.danhSachBan[i].donGia);
            cout << "Xac nhan ban " << sl << " " << kho[vtThucTe].donViTinh << ".\n";
        } 
        else {
            cout << "Loi: Khong du hang.\n"; i--;
        }

        delete[] dsViTriDong;
    }

    //c. Lưu hoá đơn vào danh sách quản lý
    if(nHDX >= sucChuaHDX){
        int newSucChua = sucChuaHDX * 2 + 1;                // +1 xử lý trường hợp sức chứa = 0 
        HoaDonXuat* temp = new HoaDonXuat[newSucChua];
        for (int i = 0; i < nHDX; i++) temp[i] = dsHDX[i];
        delete[] dsHDX;
        dsHDX = temp;
        sucChuaHDX = newSucChua;
    }

    dsHDX[nHDX++] = hdx;
    cout << "-Xu ly xuat hang thanh cong. Tong tien: " << hdx.tongTien << ".\n";
}

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

// Cảnh báo hết hạn
void canhBaoHetHan(const MatHang* kho, int nKho, Ngay homNay) {
    cout << "\n\n";
    bool coHangSapHetHan = false;

    for (int i = 0; i < nKho; i++) {
        int soNgayConLai = tinhSoNgayConLai(kho[i].hanSuDung, homNay);

        if (soNgayConLai <=7) {
            cout << "Ma: " << kho[i].maHang << " | Ten: " << kho[i].tenHang << " | HSD: " << kho[i].hanSuDung.ngay << "/" << kho[i].hanSuDung.thang << "/" << kho[i].hanSuDung.nam;

            if (soNgayConLai < 0) cout << " | DA QUA HAN\n";
            else cout << " | SAP HET HAN. Con " << soNgayConLai << " ngay\n";

            coHangSapHetHan = true;
        }
    }

    if (!coHangSapHetHan) cout << "Khong co mat hang nao sap het han\n";
}

// Cảnh báo hết hàng
void canhBaoHetHang(const MatHang* kho, int nKho) {
    cout << "\n\n";
    bool coHangThieu = false;

    for (int i = 0; i < nKho; i++) {
        if (kho[i].soLuongTon < kho[i].mucTonToiThieu) {
            cout << "Ma: " << kho[i].maHang  << " | Ten: " << kho[i].tenHang  << " | Ton kho: " << kho[i].soLuongTon << " " << kho[i].donViTinh << " | Duoi muc toi thieu " << kho[i].mucTonToiThieu << "\n";
            
            coHangThieu = true;
        }
    }

    if (coHangThieu == false) cout << "Cac ma hang deu dam bao so luong ton kho\n";
}

// Thống kê tồn kho
void thongKeTonKho(const MatHang* kho, int nKho) {
    cout << "\n\n";
    double tongGiaTri = 0;

    for (int i = 0; i < nKho; i++) {
        double giaTri = kho[i].soLuongTon * kho[i].giaNhap;
        tongGiaTri += giaTri;

        cout << "Ma: " << kho[i].maHang << " | Ten: " << kho[i].tenHang << " | SL: " << kho[i].soLuongTon << " " << kho[i].donViTinh << " | Tong gia tri nhap: " << fixed << setprecision(0) << giaTri << "VND\n";
    }

    cout << "TONG GIA TRI KHO: " << fixed << setprecision(0) << tongGiaTri << "\n";
}

void SaveData(const MatHang* kho, int nKho, const HoaDonNhap* dsHDN, int nHDN, const HoaDonXuat* dsHDX, int nHDX){
    ofstream f("Kho_va_HoaDon.txt");
    if (!f.is_open()){
        cout << "Loi! Khong the tao/mo file.\n";
        return;
    }

    //Lưu thông tin kho hàng
    f << nKho << "\n";
    for (int i = 0; i < nKho; i++){
        f << kho[i].maHang << "\n";
        f << kho[i].tenHang << "\n";
        f << kho[i].donViTinh << "\n";
        f << kho[i].giaNhap << " " << kho[i].giaBan << " " << kho[i].soLuongTon << "\n";
        f << kho[i].hanSuDung.ngay << " " << kho[i].hanSuDung.thang << " " << kho[i].hanSuDung.nam << "\n";
        f << kho[i].mucTonToiThieu << "\n";
    }

    //Lưu thông tin hoá đơn nhập
    f << nHDN << "\n";
    for (int i = 0; i < nHDN; i++) {
        f << dsHDN[i].maHDN << "\n";
        f << dsHDN[i].ngayNhap.ngay << " " << dsHDN[i].ngayNhap.thang << " " << dsHDN[i].ngayNhap.nam << "\n";
        f << dsHDN[i].ncc.tenNCC << "\n"; // Theo Level 2 bạn mới nâng cấp
        f << dsHDN[i].ncc.soDienThoai << "\n";
        f << dsHDN[i].ncc.diaChi << "\n";
        f << dsHDN[i].ncc.diemChatLuong << "\n";
        f << dsHDN[i].ncc.diemUyTin << "\n";
        
        f << dsHDN[i].soLuongMatHang << "\n";
        for (int j = 0; j < dsHDN[i].soLuongMatHang; j++) {
            f << dsHDN[i].danhSachNhap[j].maHang << " " << dsHDN[i].danhSachNhap[j].soLuong << " " << dsHDN[i].danhSachNhap[j].donGia << "\n";
        }
        f << dsHDN[i].tongTien << "\n";
    }

    //Lưu thông tin hoá đơn xuất
    f << nHDX << "\n";
    for (int i = 0; i < nHDX; i++){
        f << dsHDX[i].maHDX << "\n";
        f << dsHDX[i].ngayXuat.ngay << " " << dsHDX[i].ngayXuat.thang << " " << dsHDX[i].ngayXuat.nam << "\n";
        f << dsHDX[i].khachHang <<"\n";

        f << dsHDX[i].soLuongMatHang << "\n";
        for (int j = 0; j < dsHDX[i].soLuongMatHang; j++){
            f << dsHDX[i].danhSachBan[j].maHang << " " << dsHDX[i].danhSachBan[j].soLuong << " " << dsHDX[i].danhSachBan[j].donGia << "\n";
        }
        f << dsHDX[i].tongTien << "\n";
    }

    f.close();                  //Close the text file
    cout << "Da ghi file thanh cong.\n";
}


void ReadData(MatHang*& kho, int& nKho, int& sucChuaKho, HoaDonNhap*& dsHDN, int& nHDN, int& sucChuaHDN, HoaDonXuat*& dsHDX, int& nHDX, int& sucChuaHDX)    {
    fstream f ("Kho_va_HoaDon.txt");
    if(!f.is_open()){
        cout << "Loi! Khong the mo file.\n";
        return;
    }
    
    f >> nKho;
    sucChuaKho = nKho + 10;
    kho = new MatHang[sucChuaKho];  

    for(int i = 0; i < nKho; i++){
        f >> kho[i].maHang;
        f.ignore();
        getline(f,kho[i].tenHang);
        getline(f,kho[i].donViTinh);
        f >> kho[i].giaNhap >> kho[i].giaBan >> kho[i].soLuongTon;
        f >> kho[i].hanSuDung.ngay >> kho[i].hanSuDung.thang >> kho[i].hanSuDung.nam;
        f >> kho[i].mucTonToiThieu;
    }

    f >> nHDN;
    sucChuaHDN = nHDN + 10;
    dsHDN = new HoaDonNhap[sucChuaHDN];

    for(int i = 0; i < nHDN; i++){
        f >> dsHDN[i].maHDN;
        f >> dsHDN[i].ngayNhap.ngay >> dsHDN[i].ngayNhap.thang >> dsHDN[i].ngayNhap.nam;
        f.ignore();
        getline(f, dsHDN[i].ncc.tenNCC);
        getline(f, dsHDN[i].ncc.soDienThoai);
        getline(f, dsHDN[i].ncc.diaChi);
        f >> dsHDN[i].ncc.diemChatLuong;
        f >> dsHDN[i].ncc.diemUyTin;

        f >> dsHDN[i].soLuongMatHang;
        dsHDN[i].danhSachNhap = new ChiTietHoaDon[dsHDN[i].soLuongMatHang];
        for (int j = 0; j < dsHDN[i].soLuongMatHang; j++) {
            f >> dsHDN[i].danhSachNhap[j].maHang >> dsHDN[i].danhSachNhap[j].soLuong >> dsHDN[i].danhSachNhap[j].donGia;
        }
        f >> dsHDN[i].tongTien;
    }

    f >> nHDX;
    sucChuaHDX = nHDX + 10;
    dsHDX = new HoaDonXuat[sucChuaHDX];

    for (int i = 0; i < nHDX; i++){
        f >> dsHDX[i].maHDX;
        f >> dsHDX[i].ngayXuat.ngay >> dsHDX[i].ngayXuat.thang >> dsHDX[i].ngayXuat.nam;
        f.ignore();
        getline(f, dsHDX[i].khachHang);

        f >> dsHDX[i].soLuongMatHang;
        dsHDX[i].danhSachBan = new ChiTietHoaDon[dsHDX[i].soLuongMatHang];
        for (int j = 0; j < dsHDX[i].soLuongMatHang; j++){
            f >> dsHDX[i].danhSachBan[j].maHang >> dsHDX[i].danhSachBan[j].soLuong >> dsHDX[i].danhSachBan[j].donGia;
        }
        f >> dsHDX[i].tongTien;
    }

    f.close();
    cout << "Da doc file thanh cong!\n";
}

// Chưa check logic
void baoCaoThongMinh(const MatHang* kho, int nKho, const HoaDonXuat* dsHDX, int nHDX){
    cout << "\n === Phan tich xu huong ban hang ===\n";
    if (nHDX == 0) {
        cout << "Chua co hoa don xuat nao de thong ke.\n";
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
    
        cout << "Mat hang ban chay nhat: " << maHangBanChay << " | Tong so luong da ban: " << maxSoLuongBan << "\n";
        cout << "Du doan xu huong: " << maHangBanChay << " van se tiep tuc ban chay, nen du tru them mat hang nay.\n";
    }
    
    else{
        cout << "Khong co mat hang nao duoc ban.\n";
    }
    
    delete[] dsMaHang;
    delete[] dsTongSoLuong;
}

void toiUuKho(const MatHang* kho, int nKho, const HoaDonXuat* dsHDX, int nHDX){
    if (nHDX == 0) {
        cout << "Chua co du lieu ban hang de toi uu kho.\n";
        return;
    }

    //Lấy danh sách các mã hàng duy nhất trong kho
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
            
            // Tính tổng số lượng đã bán của mã này từ dsHDX[cite: 3]
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

//Sơ qua là như thế lày, somebody púsh ônêgàii

//ai đó xử lý main, r 2 ng còn lại phân ra làm nhập/ xuất phần còn lại bổ sung gì thì lúc đấy lại phân ra làm/ bổ xung tiếp
//Sau khi xong sẽ revise lại comment để chỉnh nó lên 1 chỗ thoáng hơn/ nhìn đỡ ngứa cái code