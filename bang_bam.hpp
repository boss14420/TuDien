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

/*! \file */

//! Lớp cơ sở của các hàm băm
/*!
 * Lớp này có sẵn một đối tượng là #_numBucket với 2 
 * phương thức #set_bucket và #bucket_count để thay đổi số lượng
 * bucket trong bảng băm. Giá trị của hàm băm sẽ được lấy modulo
 * theo #bucket_count trước khi được trả về.
 *
 * Các hàm băm thực sử sẽ kết thừa lớp này và định nghĩa thêm toán
 * tử gọi hàm (operator(Khoa)) để tính xem khóa đã cho nên được đặt
 * vào bucket nào trong bảng băm.
 *
 * \sa BangBam, HamBamChuoi
 */
template <typename Khoa>
struct HamBam {
public:
    //! Phương thức khởi tạo
    HamBam(std::size_t sz = 11) : _numBucket(sz) {} 

    //! Lấy số bucket hiện tại
    std::size_t bucket_count() const { return _numBucket; }

    //! Thay đổi số bucket
    void set_bucket(std::size_t count) { _numBucket = count; }

private:
    //! Số bucket
    std::size_t _numBucket;
};

//! Lớp bảng băm
/*!
 * Lớp BangBam biểu diễn một bảng băm ánh xạ từ kiểu Khoa đến T,
 * với giá trị băm của một khóa tính bởi đối tượng hàm thuộc lớp
 * HamBam, và các khóa so sánh bằng nhau bởi đối tượng hàm thuộc
 * lớp HamSoSanh.
 *
 * Trong lớp BangBam, các giá trị được lưu thành cặp (Khoa, T) trong các
 * danh sách liên kết. Mỗi danh sách liên kết tương ứng với một bucket
 * của #BangBam::_bangBam. Đối tượng #BangBam::_hamBam sẽ tính được khóa nào thì nên chèn vào
 * danh sách liên kết tương ứng với bucket nào.
 *
 * Lớp BangBam cũng có giới hạn #BangBam::max_load_factor(), là tỉ số giữa số phần tử
 * của BangBam (#BangBam::_kichThuoc) với số bucket của #BangBam::_bangBam để luôn nhỏ hơn một
 * mức nhất định. Nhờ đó mà có thể đảm bảo thời gian O(1) trên các thao tác.
 * Nếu tỉ lệ này quá lớn thì sẽ xác suất đụng độ (hai khóa khác
 * nhau thuộc cùng một bucket càng cao), ngược lại nếu tỉ lệ quá nhỏ thì sẽ
 * tốn chi phí bộ nhơ.
 *
 * Lớp BangBam có các phương thức cơ bản để thao tác trên bảng băm như:
 * #BangBam::tim(), #BangBam::suaDoi(), #BangBam::them(), #BangBam::xoa(), #BangBam::duyetBangBam().
 * Các thao tác tim(), suaDoi(), them(), xoa() đều có độ phức tạp tính toán
 * là hằng số O(1) trong trường hợp trung bình (khi các phần tử phân bố đều
 * trên các bucket) và O(_kichThuoc) trong trường hợp xấu nhất (các phần tử
 * đều nằm trên 1 bucket).
 *
 * Phương thức duyetBangBam() nhận một đối số là hàm, hoặc đối tượng hàm và
 * áp dụng hàm đó cho tất cả các giá trị của bảng băm. Hàm đầu vào này nhận
 * đầu vào là một cặp giá trị (#BangBam::cap_gia_tri) (khóa - giá trị).
 *
 * \sa HamBam
 */
template <typename Khoa, typename T, typename HamBam, typename HamSoSanh>
class BangBam
{
public:
    //! Kiểu khóa
    typedef Khoa kieu_khoa;

    //! Kiểu được ánh xạ từ kiểu khóa
    typedef T kieu_anh_xa;

    //! Cặp giá trị (#kieu_khoa, #kieu_anh_xa) được lưu trên các danh sách liên kết
    typedef std::pair<kieu_khoa, kieu_anh_xa> cap_gia_tri;

    //! Kiểu danh sách liên kết để lưu các cặp giá trị
    typedef std::list<cap_gia_tri> danh_sach;

    //! Bộ lặp đến cặp giá trị trong danh sách
    typedef typename danh_sach::iterator iterator;

    //! Bộ lặp hằng (không thay đổi giá trị) đến các cặp giá trị trong danh sách
    typedef typename danh_sach::const_iterator const_iterator;

public:
    //! Phương thức khởi tạo
    BangBam(std::size_t kichThuoc = 11);

    //! Tìm khóa trong bảng băm, trả về #iterator đến khóa tìm được
    const_iterator tim(kieu_khoa const &khoa) const;

    //! Tìm khóa trong bảng băm, trả về #iterator đến khóa tìm được
    iterator tim(kieu_khoa const &khoa);

    //! Thêm giá trị vào bảng băm 
    std::pair<iterator, bool> them(cap_gia_tri const &giaTri);

    //! Sửa giá trị trong bảng băm
    iterator suaDoi(cap_gia_tri const &giaTri);

    //! Xóa một khóa trong bảng băm
    bool xoa(kieu_khoa const &khoa);

    //! Xóa toàn bộ bảng bảm
    void xoaBangBam();
    
    //! Tính tỉ lệ số phần tử / số bucket hiện tại
    float load_factor() const { return (float)_kichThuoc/ _bangBam.size(); }

    //! #load_factor tối đa có thể chấp nhận
    float max_load_factor() const { return _maxLoadFactor; }

    //! Sắp xếp lại các phần tử bảng băm với số bucket thay đổi
    void rehash(std::size_t count);

    //! Lấy chỗ trước cho các phần tử trong bảng băm
    void reserve(std::size_t count);

    //! Số phần tử hiện tại của bảng băm
    std::size_t kich_thuoc() const { return _kichThuoc; }

    //! Bộ lặp trỏ đến cuối bảng băm
    iterator end() { return _bangBam.back().end(); }

    //! Bộ lặp trỏ đến cuối bảng băm
    const_iterator end() const { return _bangBam.back().end(); }

    //! Duyệt qua các phần tử trong bảng băm
    template <typename HamDuyet>
    void duyetBangBam(HamDuyet hamDuyet) const;

private:
    //! Tìm khóa trong bảng băm
    const_iterator 
        timKhoa(kieu_khoa const &khoa, std::size_t &bucket) const;

    //! Tìm khóa trong bảng băm
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

//! Phương thức khởi tạo
/*!
 * \param kichThuoc: Số bucket ban đầu được khởi tạo cho bảng băm
 */
template <typename Khoa, typename T, typename HamBam, typename HamSoSanh>
BangBam<Khoa, T, HamBam, HamSoSanh>::BangBam(std::size_t kichThuoc)
    : _kichThuoc(0), _maxLoadFactor(.75), _hamBam(kichThuoc), _hamSoSanh()
{
    _bangBam.resize(_hamBam.bucket_count());
}

//! Tìm khóa trong bảng băm, trả về #iterator đến khóa tìm được
/*!
 * Độ phức tạp tính toán: O(1) với trường hợp trung bình,
 * O(#_kichThuoc) trong trường hợp xấu nhất.
 *
 * \param khoa: khóa cần tìm
 * \return #iterator: iterator đến vị trí cặp giá trị tương ứng,
 * nếu không tìm thấy thì trả về end()
 * \sa timKhoa(), end(), tim(Khoa const&) const
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

//! Tìm khóa trong bảng băm, trả về #iterator đến khóa tìm được
/*!
 * Phiên bản dành cho đối tượng const.
 * \sa tim(Khoa const&)
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

//! Sửa giá trị trong bảng băm
/*!
 * Độ phức tạp tính toán: O(1) với trường hợp trung bình,
 * O(#_kichThuoc) trong trường hợp xấu nhất.
 *
 * \param giaTri: cặp (khóa, giá trị) cần sửa
 * \return #iterator: iterator đến cặp giá trị tương ứng trong bảng băm,
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


//! Thêm giá trị vào bảng băm 
/*!
 * Nếu việc thêm vào làm #load_factor() của bảng băm vượt quá giá 
 * trị #max_load_factor() thì hash lại toàn bộ bảng băm
 * với nhiều bucket hơn.
 *
 * Độ phức tạp tính toán: O(1) với trường hợp trung bình,
 * O(#_kichThuoc) trong trường hợp xấu nhất. Trong trường
 * hợp cần phải rehash, độ phức tạp là O(_kichThuoc).
 *
 * \param giaTri: cặp (khóa, giá trị) cần sửa
 * \return std::pair<#iterator, bool>: nếu trong bảng chưa có cặp giá trị với
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

//! Xóa một khóa trong bảng băm
/*!
 * Độ phức tạp tính toán: O(1) với trường hợp trung bình,
 * O(#_kichThuoc) trong trường hợp xấu nhất.
 *
 * \param khoa: khóa cần xóa
 * \return bool: xóa thành công hay không (khóa cần xóa có tồn tại trong
 * bảng trước đó hay không)
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

//! Xóa toàn bộ bảng bảm
template <typename Khoa, typename T, typename HamBam, typename HamSoSanh>
void
BangBam<Khoa, T, HamBam, HamSoSanh>::xoaBangBam()
{
    _bangBam.clear();
    _kichThuoc = 0;
}


//! Tìm khóa trong bảng băm
/*!
 * Phương thức này khác với #tim() ở chỗ nó còn trả về bucket có chứa
 * khóa cần tìm. Ngoài ra nếu không tìm thấy thì nó không trả về #end()
 * của bảng băm mà trả về end() của bucket sẽ dùng để chứa khóa.
 *
 * Độ phức tạp tính toán: O(1) với trường hợp trung bình,
 * O(#_kichThuoc) trong trường hợp xấu nhất.
 *
 * \param khoa: khóa cần tìm
 * \param bucket: vị trí của khóa trong bảng băm, được gán sau khi phương
 * thức thực hiện xong
 * \return biến lặp đến vị trí của khóa trong danh sách
 * kết ở bucket, nết không thấy thì trả về end() của bucket
 * \sa tim, timKhoa(Khoa const&, std::size_t &) const
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


//! Tìm khóa trong bảng băm
/*!
 * Dành cho đối tượng hằng
 * \sa tim, timKhoa(Khoa const&, std::size_t &)
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



//! Sắp xếp lại các phần tử bảng băm với số bucket thay đổi
/*!
 * Độ phức tạp tính toán: O(#_kichThuoc)
 *
 * \param count: số bucket mới
 * \sa reserve
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


//! Lấy chỗ trước cho các phần tử trong bảng băm
/*!
 * Hàm sẽ thay đổi số bucket của bảng băm sao cho nó có thể chứa
 * được count phần tử mà không cần phải #rehash lại cho đến lúc đó.
 * \param count: số phần tử
 * \sa rehash
 */
template <typename Khoa, typename T, typename HamBam, typename HamSoSanh>
void
BangBam<Khoa, T, HamBam, HamSoSanh>::reserve (std::size_t count)
{
    rehash((std::size_t) std::ceil(count / max_load_factor()) + 1);
}


//! Duyệt qua các phần tử trong bảng băm
/*!
 * Độ phức tạp tính toán: O(#_kichThuoc)
 *
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
