/*
 * =====================================================================================
 *
 *       Filename:  tu_dien.cc
 *
 *    Description:  Cài đặt lớp từ điển
 *
 *        Version:  1.0
 *        Created:  10/19/2013 03:29:38 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include "tu_dien.hh"
#include <fstream>
#include <functional>
#include <cctype>
#include <locale>
#include <algorithm>
#include <cmath>

/*
 * Phương thức khởi tạo
 */
TuDien::TuDien() : _bangBam()
{}

/*
 */


/*
 * phương thức tìm nghĩa của từ trong từ điển
 * \param tanh: từ tiếng Anh
 * \param tviet: từ tiếng Việt tương ứng
 * \return: tanh có trong từ điển hay không
 */
bool
TuDien::timTu(std::string const &tanh, std::string &tviet) const
{
    const_iterator it = _bangBam.tim(tanh);
    if (it != _bangBam.end()) {
        tviet = it->second;
        return true;
    }
    return false;
}


/*
 * phương thức sửa đổi nghĩa của từ trong từ điển
 * \param tanh: từ tiếng Anh
 * \param tviet: từ tiếng Việt tương ứng
 * \return: tanh có trong từ điển hay không
 */
bool
TuDien::suaDoiTu(std::string const &tanh, std::string const &tviet)
{
    return _bangBam.suaDoi(std::make_pair(tanh, tviet)) != _bangBam.end();
}


/*
 * phương thức sửa đổi nghĩa của từ trong từ điển
 * \param tanh: từ tiếng Anh
 * \param tviet: từ tiếng Việt tương ứng
 * \return: thêm có thành công hay không
 */
bool
TuDien::themTu(std::string const &tanh, std::string const &tviet)
{
    return _bangBam.them(std::make_pair(tanh, tviet)).second;
}


/*
 * phương thức xóa từ trong từ điển
 * \param tanh: từ tiếng Anh
 * \return: tanh có trong từ điển hay không
 */
bool
TuDien::xoaTu(std::string const &tanh)
{
    return _bangBam.xoa(tanh);
}


/*
 * Xoá khoảng trắng bên trái
 */
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

/*
 * Xoá khoảng trắng bên phải
 */
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

/*
 * Xoá khoảng trắng 2 bên
 */
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

/*
 * Nhập từ điển từ file
 * \param filename: file từ điển
 * \param delim: phân cách giữa từ tiếng Anh và tiếng Việt
 */
void
TuDien::nhapTuFile(std::string const &filename, char delim)
{
    std::ifstream ifs;
    ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    ifs.open(filename.data());

    _bangBam.xoaBangBam();

    std::size_t soTu;
    std::string tanh, tviet;
    ifs >> soTu;
    ifs >> std::skipws;
    _bangBam.reserve(soTu);
    while (soTu--) {
        std::getline(ifs, tanh, delim);
        std::getline(ifs, tviet);
        _bangBam.them(std::make_pair(trim(tanh), trim(tviet)));
    }
    ifs.close();
}


/*
 * Lưu từ điển vào file
 * \param filename: file từ điển
 * \param delim: phân cách giữa từ tiếng Anh và tiếng Việt
 * \return
 */
void
TuDien::luuVaoFile(std::string const &filename, char delim) const
{
    std::ofstream ofs;
    ofs.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    ofs.open(filename.data());

    ofs << _bangBam.kich_thuoc() << '\n';
    _bangBam.duyetBangBam((HamInTuDien){ofs});
    ofs.close();
}

void
TuDien::HamInTuDien::operator() (TuDien::cap_gia_tri const &giaTri)
{
    os << giaTri.first << " : " << giaTri.second << '\n';
}
