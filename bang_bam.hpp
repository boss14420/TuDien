/*
 * =====================================================================================
 *
 *       Filename:  bang_bam.hpp
 *
 *    Description:  Lớp bảng băm 
 *
 *        Version:  1.0
 *        Created:  10/20/2013 09:21:56 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __BANGBAM_HPP__
#define __BANGBAM_HPP__

#include <utility>
#include <vector>
#include <list>
#include <cmath>

/*
 * Lớp hàm băm
 */
template <typename Khoa>
struct HamBam {
public:
    HamBam(std::size_t sz = 11) : _numBucket(sz) {} 
    std::size_t bucket_count() const { return _numBucket; }
    void set_bucket(std::size_t count) { _numBucket = count; }

private:
    std::size_t _numBucket;
};

/*
 * Lớp bảng băm
 */

template <typename Khoa, typename T, typename HamBam, typename HamSoSanh>
class BangBam
{
public:
    typedef Khoa kieu_khoa;
    typedef T kieu_anh_xa;
    typedef std::pair<kieu_khoa, kieu_anh_xa> cap_gia_tri;
    typedef std::list<cap_gia_tri> danh_sach;
    typedef typename danh_sach::iterator iterator;
    typedef typename danh_sach::const_iterator const_iterator;

    BangBam(std::size_t so_phan_tu = 11);

    const_iterator tim(kieu_khoa const &khoa) const;
    iterator tim(kieu_khoa const &khoa);

    std::pair<iterator, bool> them(cap_gia_tri const &giaTri);

    iterator suaDoi(cap_gia_tri const &giaTri);

    bool xoa(kieu_khoa const &khoa);

    void xoaBangBam();

    float load_factor() const { return (float)_kichThuoc/ _bangBam.size(); }

    float max_load_factor() const { return _maxLoadFactor; }

    void rehash(std::size_t count);

    void reserve(std::size_t count);

    std::size_t kich_thuoc() const { return _kichThuoc; }

    iterator end() { return _bangBam.back().end(); }
    const_iterator end() const { return _bangBam.back().end(); }

    template <typename HamDuyet>
    void duyetBangBam(HamDuyet hamDuyet) const;

private:
    const_iterator 
        timKhoa(kieu_khoa const &khoa, std::size_t &bucket) const;

    iterator
        timKhoa(kieu_khoa const &khoa, std::size_t &bucket);

private:
    //! Lưu trữ các cặp giá trị (khóa, kiểu được ánh xạ) của bẳng băm
    std::vector<danh_sach> _bangBam;

    //! Số phần tử (số khóa) đang được lưu tại bảng băm
    std::size_t _kichThuoc;

    //! load factor lớn nhất được phép
    float _maxLoadFactor;

    //! hàm băm cho khóa
    HamBam _hamBam;

    //! hàm so sánh khóa
    HamSoSanh _hamSoSanh;
};

//
// Cài đặt phương thức cho lớp bảng băm
//

/*
 * Phương thức khởi tạo
 */
template <typename Khoa, typename T, typename HamBam, typename HamSoSanh>
BangBam<Khoa, T, HamBam, HamSoSanh>::BangBam(std::size_t preserve_kichThuoc)
    : _kichThuoc(0), _maxLoadFactor(.75), _hamBam(preserve_kichThuoc), _hamSoSanh()
{
    _bangBam.resize(_hamBam.bucket_count());
}

/*
 * Tìm khóa trong bảng băm, trả về iterator đến khóa tìm được
 * \param khoa: khóa cần tìm
 * \return iterator: iterator đến vị trí cặp giá trị tương ứng,
 * nếu không tìm thấy thì trả về end()
 * \sa timKhoa(), end()
 */
template <typename Khoa, typename T, typename HamBam, typename HamSoSanh>
typename BangBam<Khoa, T, HamBam, HamSoSanh>::iterator
BangBam<Khoa, T, HamBam, HamSoSanh>::tim(Khoa const &khoa) 
{
    std::size_t bucket;
    iterator it = timKhoa(khoa, bucket);
    if (it != _bangBam[bucket].end())
        return it;
    return end();
}

/*
 * Tìm khóa trong bảng băm, trả về iterator đến khóa tìm được.
 * Phiên bản dành cho đối tượng const.
 * \sa tim
 */
template <typename Khoa, typename T, typename HamBam, typename HamSoSanh>
typename BangBam<Khoa, T, HamBam, HamSoSanh>::const_iterator
BangBam<Khoa, T, HamBam, HamSoSanh>::tim(Khoa const &khoa) const
{
    std::size_t bucket;
    const_iterator it = timKhoa(khoa, bucket);
    if (it != _bangBam[bucket].end())
        return it;
    return end();
}

/*
 * Sửa giá trị trong bảng băm
 * \param giaTri: cặp (khóa, giá trị) cần sửa
 * \return iterator: iterator đến cặp giá trị tương ứng trong bảng băm,
 * nếu không tìm thấy thì trả về end()
 * \sa end()
 */
template <typename Khoa, typename T, typename HamBam, typename HamSoSanh>
typename BangBam<Khoa, T, HamBam, HamSoSanh>::iterator
BangBam<Khoa, T, HamBam, HamSoSanh>::suaDoi(std::pair<Khoa, T> const &giaTri)
{
    std::size_t bucket;
    iterator it = timKhoa(giaTri.first, bucket);
    if (it != _bangBam[bucket].end()) {
        it->second = giaTri.second;
        return it;
    }
    return end();
}


/*
 * Thêm giá trị vào bảng băm. Nếu việc thêm vào làm load factor của
 * bảng băm vượt quá giá trị max_load_factor() thì hash lại toàn bộ bảng băm
 * với bucket_count lớn hơn.
 * \param giaTri: cặp (khóa, giá trị) cần sửa
 * \return std::pair<iterator, bool>: nếu trong bảng chưa có cặp giá trị với
 * khóa đã cho thì trả về iterator đến khóa đó và false, nếu chưa có thì trả về
 * địa chỉ đến khóa mới thêm vào và true
 * \sa load_factor(), max_load_factor()
 */
template <typename Khoa, typename T, typename HamBam, typename HamSoSanh>
std::pair<typename BangBam<Khoa, T, HamBam, HamSoSanh>::iterator, bool>
BangBam<Khoa, T, HamBam, HamSoSanh>::them(std::pair<Khoa, T> const &giaTri)
{
    std::size_t bucket;
    iterator it = timKhoa(giaTri.first, bucket);
    if (it != _bangBam[bucket].end()) {
        return std::make_pair(it, false);
    }

    ++_kichThuoc;
    if (load_factor() > max_load_factor()) {
        rehash(_bangBam.size() * 2 + 1);
        bucket = _hamBam(giaTri.first);
        it = _bangBam[bucket].end();
    }

    it = _bangBam[bucket].insert(it, giaTri);
    return std::make_pair(it, true);
}

/*
 * Xóa một khóa trong bảng băm
 * \param khoa: khóa cần xóa
 * \return bool: xóa thành công hay không (khóa cần xóa có tồn tại trong
 * bảng trước đó hay không
 */
template <typename Khoa, typename T, typename HamBam, typename HamSoSanh>
bool
BangBam<Khoa, T, HamBam, HamSoSanh>::xoa(Khoa const &khoa)
{
    std::size_t bucket;
    iterator it = timKhoa(khoa, bucket);
    if (it != _bangBam[bucket].end()) {
        --_kichThuoc;
        _bangBam[bucket].erase(it);
        return true;
    }

    return false;
}

/*
 * Xóa toàn bộ bảng bảm
 */
template <typename Khoa, typename T, typename HamBam, typename HamSoSanh>
void
BangBam<Khoa, T, HamBam, HamSoSanh>::xoaBangBam()
{
    _bangBam.clear();
    _kichThuoc = 0;
}


/*
 * Tìm khóa trong bảng băm
 * \param khoa: khóa cần tìm
 * \bucket: vị trí của khóa trong bảng băm, được gán sau khi phương
 * thức thực hiện xong
 * \return biến lặp đến vị trí của khóa trong danh sách
 * kết ở bucket, nết không thấy thì trả về end() của bucket
 * \sa tim
 */
template <typename Khoa, typename T, typename HamBam, typename HamSoSanh>
typename BangBam<Khoa, T, HamBam, HamSoSanh>::iterator
BangBam<Khoa, T, HamBam, HamSoSanh>::timKhoa(
            Khoa const &khoa, std::size_t &bucket) 
{
    bucket = _hamBam(khoa);
    iterator it = _bangBam[bucket].begin();
    for (; it != _bangBam[bucket].end(); ++it) {
        if (_hamSoSanh(it->first, khoa))
            return it;
    }

    // Nếu không tìm thấy thì trả về end của bucket
    return it;
}


/*
 * Tìm khóa trong bảng băm, dành cho đối tượng hằng
 * \sa timKhoa
 */
template <typename Khoa, typename T, typename HamBam, typename HamSoSanh>
typename BangBam<Khoa, T, HamBam, HamSoSanh>::const_iterator
BangBam<Khoa, T, HamBam, HamSoSanh>::timKhoa( 
                Khoa const &khoa, std::size_t &bucket) const
{
    bucket = _hamBam(khoa);
    const_iterator it = _bangBam[bucket].begin();
    for (; it != _bangBam[bucket].end(); ++it) {
        if (_hamSoSanh(it->first, khoa))
            return it;
    }

    // Nếu không tìm thấy thì trả về end của bucket
    return it;
}



/*
 * Thay đổi số lượng bucket của bảng băm, đồng thời 
 * tính lại hash của tất cả các phần tử
 */
template <typename Khoa, typename T, typename HamBam, typename HamSoSanh>
void
BangBam<Khoa, T, HamBam, HamSoSanh>::rehash (std::size_t count)
{
    _hamBam.set_bucket(count);
    if (!_kichThuoc) {
        _bangBam.resize(_hamBam.bucket_count());
        return;
    }

    std::vector<danh_sach> bangBamMoi (_hamBam.bucket_count());
    const_iterator it;
    for (std::size_t i = 0; i != _bangBam.size(); ++i) {
        for (it = _bangBam[i].begin(); it != _bangBam[i].end(); ++it)
            bangBamMoi[_hamBam(it->first)].push_back(*it);
    }
    _bangBam.swap(bangBamMoi);
}


/*
 * Lấy chỗ trước cho ít nhất count bucket trong bảng băm
 */
template <typename Khoa, typename T, typename HamBam, typename HamSoSanh>
void
BangBam<Khoa, T, HamBam, HamSoSanh>::reserve (std::size_t count)
{
    rehash((std::size_t) std::ceil(count / max_load_factor()) + 1);
}


/*
 * Duyệt qua các phần tử trong bảng băm
 * \param hamDuyet: hàm được áp dụng lên các phần tử trong bảng băm
 */
template <typename Khoa, typename T, typename HamBam, typename HamSoSanh>
template <typename HamDuyet>
void 
BangBam<Khoa, T, HamBam, HamSoSanh>::duyetBangBam(HamDuyet hamDuyet) const
{
    for (std::size_t i = 0; i != _bangBam.size(); ++i) {
        const_iterator it = _bangBam[i].begin();
        for (; it != _bangBam[i].end(); ++it)
            hamDuyet(*it);
    }
}

#endif // __BANGBAM_HPP__
