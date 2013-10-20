/*
 * =====================================================================================
 *
 *       Filename:  tu_dien2.hh
 *
 *    Description:  Lớp Từ điển
 *
 *        Version:  1.0
 *        Created:  10/19/2013 02:59:36 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __TU_DIEN__
#define __TU_DIEN__

#include "bang_bam.hpp"
#include <string>
#include <iosfwd>

/*! \file */

//! Lớp hàm băm dành cho chuỗi
/*!
 * \sa HamBam
 */
struct HamBamChuoi : HamBam<std::string> {
public:
    //! Phương thức khởi tạo
    HamBamChuoi(std::size_t sz = 11) 
        : HamBam<std::string>(sz) 
    {} 

    //! Hàm băm chuỗi
    /*!
     * Sử dụng hàm FNV-1 thao tác lên các ký tự trong chuỗi.
     * \param str: chuỗi cần băm
     * \return std::size_t: giá trị băm. Đảm bảo 
     * nhỏ hơn #HamBam::bucket_count()
     */
    std::size_t operator() (std::string const &str) const {
        unsigned long long h = 14695981039346656037ULL;
        std::string::const_iterator si = str.begin();
        for (; si != str.end(); ++si) {
            h *= 1099511628211ULL;
            h ^= static_cast<unsigned long long>(std::tolower(*si));
        }
        return h % bucket_count();
    }
};


//! Lớp hàm so sánh chuỗi
struct SoSanhChuoi {

    //! Hàm so sánh chuỗi
    /*!
     * Hai chuỗi được coi là bằng nhau nếu tất cả các ký tự đều giống
     * nhau, không phần biệt hoa - thường.
     * \param s1: chuỗi thứ nhất
     * \param s2: chuỗi thứ hai
     * \return bool: hai chuỗi có bằng nhau hay không
     */
    bool operator() (std::string const &s1, std::string const &s2) const {
        if (s1.size() != s2.size()) return false;

        std::string::const_iterator si1 = s1.begin(), si2 = s2.begin();
        for (; si1 != s1.end(); ++si1, ++si2) {
            if (std::tolower(*si1) != std::tolower(*si2))
                return false;
        }
        return true;
    }
};


//! Lớp từ điển
/*!
 * Biểu diễn cho một từ điển với một ngôn ngữ nguồn và 
 * ngôn ngữ đích.  
 * Từ điển hỗ trợ các thao tác tìm kiếm từ (#TuDien::timTu), sửa 
 * đổi từ (#TuDien::suaDoiTu), xóa tử (#TuDien::xoaTu), #TuDien::themTu(). Ngoài ra
 * từ điển còn có thể được lưu vào file (#TuDien::luuVaoFile) hoặc
 * nhập từ file (#TuDien::nhapTuFile).
 *
 * Từ điển này sử dụng #BangBam để lưu trử các từ nên các thao
 * tác cơ bản có thể thực hiện được trong thời gian hằng số O(1)
 * trong trương hợp trung bình.
 *
 * \sa BangBam
 */
class TuDien {
public:
    //! Kiểu bảng băm dùng để lưu trữ
    typedef BangBam<std::string, std::string, 
                    HamBamChuoi, SoSanhChuoi> bang_bam;

    //! Kiểu bộ lặp trở đến các cặp (từ, nghĩa)
    typedef bang_bam::iterator iterator;

    //! Kiể bộ lặp hằng (không thay đổi được giá trị)
    typedef bang_bam::const_iterator const_iterator;

    //! Cặp giá trị (từ, nghĩa)
    typedef bang_bam::cap_gia_tri cap_gia_tri;

public:
    //! Phương thức khởi tạo
    TuDien();

    //! phương thức tìm nghĩa của từ trong từ điển
    bool timTu(std::string const &tanh, std::string &tviet) const;

    //! phương thức sửa đổi nghĩa của từ trong từ điển
    bool suaDoiTu(std::string const &tanh, std::string const &tviet);

    //! phương thức thêm từ vào từ điển
    bool themTu(std::string const &tanh, std::string const &tviet);

    //! phương thức xóa từ trong từ điển
    bool xoaTu(std::string const &tanh);

    //! Nhập từ điển từ file
    void nhapTuFile(std::string const &filename, char delim = ':');

    //! Lưu từ điển vào file
    void luuVaoFile(std::string const &filename, char delim = ':') const;

private:
    //! Lớp đối tượng hàm in nghĩa của từ
    /*
     * \sa luuVaoFile
     */
    struct HamInTuDien {
        //! Hàm in cặp giá trị (từ - nghĩa)
        void operator() (cap_gia_tri const&);

        //! Luồng để in ra
        std::ostream& os;
    };

private:
    //! Bảng băm lưu nội dung của từ điển
    bang_bam _bangBam;
};


#endif // __TU_DIEN__
