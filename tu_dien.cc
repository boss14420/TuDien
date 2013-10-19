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

/*
 * Phương thức khởi tạo
 */
TuDien::TuDien()
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
    std::size_t bucket;
    list_type::const_iterator it = timKhoa(tanh, bucket);
    if (it != _bangBam[bucket].end()) {
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
    std::size_t bucket;
    list_type::iterator it = timKhoa(tanh, bucket);
    if (it != _bangBam[bucket].end()) {
        it->second = tviet;
        return true;
    }
    return false;
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
    std::size_t bucket;
    list_type::iterator it = timKhoa(tanh, bucket);
    if (it != _bangBam[bucket].end())
        return false;

    ++_size;
    if (load_factor() > max_load_factor()) {
        rehash(_size * 2 + 1);
        bucket = _hamBam(tanh);
    }
    _bangBam[bucket].insert(std::make_pair(tanh, tviet));
    return true;
}


/*
 * phương thức xóa từ trong từ điển
 * \param tanh: từ tiếng Anh
 * \param tviet: từ tiếng Việt tương ứng
 * \return: tanh có trong từ điển hay không
 */
bool
TuDien::xoaTu(std::string const &tanh)
{
    std::size_t bucket;
    list_type::iterator it = timKhoa(tanh, bucket);
    if (it == _bangBam[bucket].end())
        return false;

    _bangBam[bucket].erase(it);
    --_size;
    return true;
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
 * \return
 */
void
TuDien::nhapTuFile(std::string const &filename, char delim)
{
    std::ifstream ifs;
    ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    ifs.open(filename.data());

    _bangBam.clear();
    _size = 0;

    std::size_t soTu;
    std::string tanh, tviet;
    ifs >> soTu;
    ifs >> std::skipws;
    reserve(soTu);
    while (soTu--) {
        std::getline(ifs, tanh, delim);
        std::getline(ifs, tviet);
        themTu(tanh, tviet);
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

    ofs << _size << '\n';
    list_type::iterator it;
    for (std::size_t i = 0; i != _bangBam.size(); ++i) {
        for (it = _bangBam[i].begin(); it != _bangBam[i].end(); ++it) {
            ofs << it->first << " : " << it->second << '\n';
        }
    }
    ofs.close();
}


/*
 * Tìm khóa trong bảng băm
 * \param key: từ khóa cần tìm
 * \bucket: vị trí của khóa trong bảng băm
 * \return biến lặp đến vị trí của khóa trong danh sách
 * kết ở bucket
 */

TuDien::list_type::iterator 
TuDien::timKhoa(std::string const &key, std::size_t &bucket)
{
    bucket = _hamBam(key);
    list_type::iterator it = _bangBam[bucket].begin();
    for (; it != _bangBam[bucket].end(); ++it) {
        if (_soSanh(it->first, key))
            return it;
    }

    // Nếu không tìm thấy thì trả về end của bucket
    return it;
}
