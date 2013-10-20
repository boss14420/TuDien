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

struct HamBamChuoi : HamBam<std::string> {
public:
    HamBamChuoi(std::size_t sz = 11) 
        : HamBam<std::string>(sz) 
    {} 

    // hàm băm FNV-1
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
    typedef BangBam<std::string, std::string, 
                    HamBamChuoi, SoSanhChuoi> bang_bam;
    typedef bang_bam::iterator iterator;
    typedef bang_bam::const_iterator const_iterator;
    typedef bang_bam::cap_gia_tri cap_gia_tri;

public:
    TuDien();

    bool timTu(std::string const &tanh, std::string &tviet) const;

    bool suaDoiTu(std::string const &tanh, std::string const &tviet);

    bool themTu(std::string const &tanh, std::string const &tviet);

    bool xoaTu(std::string const &tanh);

    void nhapTuFile(std::string const &filename, char delim = ':');

    void luuVaoFile(std::string const &filename, char delim = ':') const;

private:
    struct HamInTuDien {
        void operator() (cap_gia_tri const&);
        std::ostream& os;
    };

private:
    bang_bam _bangBam;
};


#endif // __TU_DIEN__
