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

#include <string>
#include <vector>
#include <list>

struct HamBamChuoi {
public:
    HamBamChuoi(std::size_t sz = 11) : _numBucket(sz) {} 
    std::size_t bucketCount() const { return _numBucket; }
    void setBucket(std::size_t count) { _numBucket = count; }

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

private:
    std::size_t _numBucket;
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
    typedef std::string key_type;
    typedef std::string mapped_type;
    typedef std::pair<key_type, mapped_type> value_type;
    typedef std::list<value_type> list_type;
    typedef std::vector<list_type> bang_bam;

    TuDien(std::size_t preserve_size = 11);

    bool timTu(std::string const &tanh, std::string &tviet) const;

    bool suaDoiTu(std::string const &tanh, std::string const &tviet);

    bool themTu(std::string const &tanh, std::string const &tviet);

    bool xoaTu(std::string const &tanh);

    void nhapTuFile(std::string const &filename, char delim = ':');

    void luuVaoFile(std::string const &filename, char delim = ':') const;

    std::size_t size() const { return _size; }

private:
    list_type::const_iterator 
        timKhoa(std::string const &key, std::size_t &bucket) const;

    list_type::iterator
        timKhoa(std::string const &key, std::size_t &bucket);

    float load_factor() const { return (float)_size / _bangBam.size(); }

    float max_load_factor() const { return _max_load_factor; }

    void rehash(std::size_t count);

    void reserve(std::size_t count);

private:
    bang_bam _bangBam;
    std::size_t _size;
    float _max_load_factor;
    HamBamChuoi _hamBam;
    SoSanhChuoi _soSanh;
};


#endif // __TU_DIEN__
