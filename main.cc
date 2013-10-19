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

void menu(TuDien &td);

void napTuDien(TuDien &td);
void timKiem(TuDien const &td);
void suaDoiTu(TuDien &td);
void themTu(TuDien &td);
void xoaTu(TuDien &td);
void luuTuDien(TuDien const &td);

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
