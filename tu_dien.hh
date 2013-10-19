/*
 * =====================================================================================
 *
 *       Filename:  tu_dien.hpp
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

struct HamBamChuoi : _HamBam<std::string> {
    // hàm băm FNV-1
    std::size_t operator() (std::string const &str) const {
        unsigned long long h = 14695981039346656037ULL;
        std::string::const_iterator si = str.begin();
        for (; si != str.end(); ++si) {
            h *= 1099511628211ULL;
            h ^= static_cast<unsigned long long>(std::tolower(*si));
        }
        return h % bucketCount();
    }
};

struct SoSanhChuoi {
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

/*
 * Lớp từ điển
 */
class TuDien {
public:
    typedef BangBam<std::string, std::string, HamBamChuoi, SoSanhChuoi> 
                bang_bam;

    TuDien();

    bool timTu(std::string const &tanh, std::string &tviet) const;

    bool suaDoiTu(std::string const &tanh, std::string const &tviet);

    bool themTu(std::string const &tanh, std::string const &tviet);

    bool xoaTu(std::string const &tanh);

    void nhapTuFile(std::string const &filename, char delim = ':');

    void luuVaoFile(std::string const &filename, char delim = ':') const;

private:
    bang_bam _bangBam;
};


#endif // __TU_DIEN__
