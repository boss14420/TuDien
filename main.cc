/*
 * =====================================================================================
 *
 *       Filename:  main.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/19/2013 05:53:33 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include "tu_dien.hh"
#include <iostream>
#include <cstdlib>

#define DEFAULT_FILE "tu_dien.txt";

/////////////////////////////////////////////////////
////// Khai báo hàm
/////////////////////////////////////////////////////

/*! \file */

//! Hàm hiện menu lựa chọn chức năng chương trình
void menu(TuDien &td);

//! Hàm thực hiện chức năng tìm kiếm từ điển
void timKiem(TuDien const &td);

//! Hàm thực hiện chức năng sửa đổi từ
void suaDoiTu(TuDien &td);

//! Hàm thực hiện chức năng thêm từ
void themTu(TuDien &td);

//! Hàm thực hiện chức năng xóa từ
void xoaTu(TuDien &td);

//! Hàm thực hiện chức năng lưu từ điển vào file
void luuTuDien(TuDien const &td);

//! Hàm thực hiện chức nằng nạp từ điển từ file
void napTuDien(TuDien &td);



//////////////////////////////////////////////////////
/////// Cài đặt các chức năng
//////////////////////////////////////////////////////

//! Hàm thực hiện chức nằng nạp từ điển từ file
/*!
 * Khi thực hiện sẽ yêu cầu người dùng nhập đường dẫn
 * đến file chứa từ điển. Định dạng từ điển phải tuân thủ
 * yêu cầu ở #TuDien::nhapTuFile.
 *
 * Nếu trong quá trình thao tác file bị lỗi thì sẽ thông báo ra.
 *
 * \param td: Đối tượng từ điển
 */
void napTuDien(TuDien &td) {
    std::string filename;
    std::cout << "\nNhap tu dien tu file:\n"
        "Nhap duong dan file tu dien (.quit de huy nhap): ";
    std::getline(std::cin, filename);
    if (filename == ".quit") return;
    try {
        td.nhapTuFile(filename);
        std::cout << "Nhap tu dien thanh cong!\n";
    } catch (std::exception &ex) {
        std::cerr << "Loi trong luc nhap tu dien\n"
                    << ex.what() << '\n';
    }
}


//! Hàm thực hiện chức năng tìm kiếm từ điển
/*!
 * Tìm kiểm từ tiếng Anh trong từ điển (nếu có) và in ra nghĩa
 * tiếng Việt của từ đó. Nếu không có sẽ báo lỗi.
 * Hàm này sẽ liên tục hỏi các từ cho đến khi người dùng nhập
 * chuỗi ".quit" thì mới trở về menu chính.
 *
 * \param td: Đối tượng từ điển
 */
void timKiem(TuDien const &td) {
    std::string tviet, tanh;
    while (true) {
        std::cout << "\nTim kiem trong tu dien:\n"
            "Nhap tu tieng Anh (nhap .quit de ket thuc): ";
        std::getline(std::cin, tanh);
        if (tanh == ".quit") break;
        if (td.timTu(tanh, tviet)) {
            std::cout << "Nghia cua tu \"" << tanh << 
                         "\" la : \"" << tviet << "\"\n";
        } else {
            std::cerr << "Loi: Khong co tu \"" << tanh << "\" trong tu dien\n";
        }
    }
}

//! Hàm thực hiện chức năng sửa đổi từ
/*!
 * Yêu cầu người dùng nhập từ tiếng Anh, nếu từ này chưa có trong từ
 * điển thì sẽ báo lỗi không tìm thấy, nếu có thì yêu cầu nhập nghĩa
 * tiếng Việt mới để sửa trong từ điển.
 *
 * Quá trình này được thực hiện liên tục cho đến khi nhập ".quit"
 *
 * \param td: Đối tượng từ điển
 */
void suaDoiTu(TuDien &td) {
    std::string tviet, tanh;
    while (true) {
        std::cout << "\nSua doi tu:\n"
            "Nhap tu tieng Anh can sua (nhap .quit de ket thuc): ";
        std::getline(std::cin, tanh);
        if (tanh == ".quit") break;
        if (td.timTu(tanh, tviet)) {
            std::cout << "Nghia cua tu \"" << tanh << 
                         "\" la : \"" << tviet << "\"\n";
            std::cout << "Sua thanh (nhap .quit de thoat): ";
            std::getline(std::cin, tviet);
            if (tviet == ".quit") continue;
            td.suaDoiTu(tanh, tviet);
        } else {
            std::cerr << "Loi: Khong co tu \"" << tanh << "\" trong tu dien\n";
        }
    }
}

//! Hàm thực hiện chức năng thêm từ
/*!
 * Yêu cầu người dùng nhập lần lượt nhập từ tiếng Anh và
 * nghĩa tiếng Việt tương ứng để thêm vào từ điển. Nếu
 * từ tiếng Anh đã có trong từ điển thì báo lỗi.
 *
 * Quá trình này được thực hiện liên tục cho đến khi nhập ".quit"
 *
 * \param td: Đối tượng từ điển
 */
void themTu(TuDien &td) {
    std::string tviet, tanh;
    while (true) {
        std::cout << "\nThem tu:\n"
            "Nhap tu tieng Anh can them (nhap .quit de ket thuc): ";
        std::getline(std::cin, tanh);
        if (tanh == ".quit") break;

        std::cout << "Nhap nghia tieng Viet (nhap .quit de thoat): ";
        std::getline(std::cin, tviet);
        if (tviet == ".quit") continue;

        if (!td.themTu(tanh, tviet))
            std::cerr << "Loi: Tu \"" << tanh << "\" da co trong tu dien\n";
    }
}

//! Hàm thực hiện chức năng xóa từ
/*!
 * Yêu cầu người dùng nhập vào từ tiếng Anh để xóa ra khỏi
 * từ điển. Nếu từ không có sẵn trong từ điển thì sẽ báo
 * lỗi.
 *
 * Quá trình này được thực hiện liên tục cho đến khi nhập ".quit"
 *
 * \param td: Đối tượng từ điển
 */
void xoaTu(TuDien &td) {
    std::string tanh;
    while (true) {
        std::cout << "\nXoa tu:\n"
            "Nhap tu tieng Anh can xoa (nhap .quit de ket thuc): ";
        std::getline(std::cin, tanh);
        if (tanh == ".quit") break;

        if (!td.xoaTu(tanh))
            std::cerr << "Loi: Tu \"" << tanh << "\" chua co trong tu dien\n";
    }
}

//! Hàm thực hiện chức năng lưu từ điển vào file
/*!
 * Hỏi người dùng đường dẫn đến file để lưu từ điển vào.
 * Nếu trong quá trình ghi file có lỗi thì sẽ báo lỗi.
 *
 * \param td: Đối tượng từ điển
 * \sa napTuDien
 */
void luuTuDien(TuDien const &td) {
    std::string filename;
    std::cout << "\nLuu vao file:\n"
        "Nhap duong dan file tu dien (.quit de huy nhap): ";
    std::getline(std::cin, filename);
    if (filename == ".quit") return;
    try {
        td.luuVaoFile(filename);
        std::cout << "Luu tu dien thanh cong!\n";
    } catch (std::exception &ex) {
        std::cerr << "Loi trong luc luu tu dien\n"
                    << ex.what() << '\n';
    }
}

//! Hàm hiện menu lựa chọn chức năng chương trình
/*!
 * Hiện menu lựa chọn các chức năng: Nạp từ điển (#napTuDien),
 * tìm kiếm (#timKiem), sửa đổi từ (#suaDoiTu), thêm từ
 * (#themTu), xóa từ (#xoaTu) và lưu từ điển (#luuTuDien).
 *
 * Mỗi khi thực hiện xong một chức năng thì chương trình lại
 * trở lại menu và cho phép lựa chọn một chức năng khác. Quá
 * trình lặp lại cho đến khi chọn chức năng thoát chương
 * trình trong menu.
 *
 * \param td: Đối tượng từ điển
 */
void menu(TuDien &td) {
    std::string input;
    while (true) {
        std::cout << "\n\nChon chuc nang:\n"
            "1. Nap tu dien tu file\n"
            "2. Tim kiem\n"
            "3. Sua doi tu\n"
            "4. Them tu moi\n"
            "5. Xoa tu\n"
            "6. Luu tu dien vao file\n"
            "99. Thoat chuong trinh\n"
            "Lua chon: ";
        std::getline(std::cin, input);
        int choice = std::atoi(input.data());
        std::cout << '\n';

        switch (choice) {
            case 1: napTuDien(td); break;
            case 2: timKiem(td); break;
            case 3: suaDoiTu(td); break;
            case 4: themTu(td); break;
            case 5: xoaTu(td); break;
            case 6: luuTuDien(td); break;
            case 99: return;
            default: break;
        }
    }
}

int main(int argc, char *argv[]) {
    TuDien td;
    if (argc >= 2) {
	std::string filename = argv[1];
	try {
	    td.nhapTuFile(filename);
	} catch (std::exception &ex) {
	}
    }

    menu(td);

    return 0;
}
