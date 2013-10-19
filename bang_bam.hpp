/*
 * =====================================================================================
 *
 *       Filename:  bang_bam.hh
 *
 *    Description:  header cua lop bang_bam
 *
 *        Version:  1.0
 *        Created:  10/19/2013 01:31:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __BANG_BAM__
#define __BANG_BAM__

#include <utility>
#include <list>
#include <vector>
#include <iterator>
#include <cmath>

template <typename Key, typename T, typename HamBam, typename SoSanh>
class BangBam;

/*
 * Lớp HashIterator, biểu diễn bộ lăp (iterator) cho lớp BangBam
 */
template <typename Key, typename T, typename HamBam, typename SoSanh>
class HashIterator : public std::iterator<std::bidirectional_iterator_tag,
                                          std::pair<Key, T> >
{
public:
    typedef Key key_type;
    typedef T mapped_type;
    typedef std::pair<key_type, mapped_type> value_type;
    typedef value_type& reference;
    typedef std::list<value_type> list_type;
    typedef HashIterator<Key, T, HamBam, SoSanh> this_type;

    HashIterator()
        : _bucket(0), _bangBam(NULL)
    {}

    HashIterator(std::size_t _bucket, 
                 typename list_type::iterator _it,
                 BangBam<Key, T, HamBam, SoSanh> *_bangBam)
        : _bucket(_bucket), _it(_it), _bangBam(_bangBam)
    {}

    value_type& operator*() const { return *_it; }
    value_type* operator->() const { return &(*_it); }

    this_type& operator++() { increment(); return *this; }
    this_type& operator--() { decrement(); return *this; }

    const this_type operator++(int) {
        this_type tmp = *this;
        increment();
        return tmp;
    }

    const this_type operator--(int) {
        this_type tmp = *this;
        decrement();
        return tmp;
    }

    bool operator==(this_type const &rhs) const {
        return _bangBam == rhs._bangBam && _bucket == rhs._bucket 
               && _it == rhs._it;
    }

    bool operator!=(this_type const &rhs) const {
        return !(*this == rhs);
    }

private:
    std::size_t _bucket;
    typename list_type::iterator _it;
    BangBam<Key, T, HamBam, SoSanh> *_bangBam;

    void increment();
    void decrement();
};

// Tăng bộ lặp lên 1
template <typename Key, typename T, typename HamBam, typename SoSanh>
void
HashIterator<Key, T, HamBam, SoSanh>::increment()
{
    // Nếu _it chưa phải kết thúc 1 bucket
    if (++_it != (_bangBam->_table)[_bucket].end())
        return;

    for (++_bucket; _bucket != _bangBam->_table.size(); ++_bucket) {
        // tìm 1 bucket không rỗng
        if (!_bangBam->_table[_bucket].empty()) {
            _it = _bangBam->_table[_bucket].begin();
            return;
        }
    }

    // nếu không tìm được
    --_bucket;
    _it = _bangBam->_table[_bucket].end();
}

// Giảm bộ lặp xuống 1
template <typename Key, typename T, typename HamBam, typename SoSanh>
void
HashIterator<Key, T, HamBam, SoSanh>::decrement()
{
    // Nếu _it không phải đầu 1 bucket
    if (_it != _bangBam->_table[_bucket].begin()) {
        --_it;
        return;
    }

    for (int i = _bucket - 1; i >= 0; --i) {
        if (!_bangBam->_table[i].empty()) {
            _it = _bangBam->table[i].end();
            --_it;
            _bucket = i;
            return;
        }
    }

    _it = _bangBam->_table[0].begin();
    --_it;
    _bucket = 0;
}

/*
 * Lớp hàm băm
 */
template <typename Key>
class _HamBam {
public:
    _HamBam() : _numBucket(11) {} 
    std::size_t bucketCount() const { return _numBucket; }
    void setBucket(std::size_t count) { _numBucket = count; }
private:
    std::size_t _numBucket;
};

/*
 * Lớp bảng băm
 */
template <typename Key, typename T, typename HamBam, 
          typename SoSanh = std::equal_to<Key> >
class BangBam {
public:
    typedef Key key_type;
    typedef T mapped_type;
    typedef std::pair<key_type, mapped_type> value_type;
    typedef value_type& reference;
    typedef std::list<value_type> list_type;
    typedef BangBam<Key, T, HamBam, SoSanh> this_type;

    typedef HashIterator<Key, T, HamBam, SoSanh> iterator;
    typedef const iterator const_iterator;
    friend class HashIterator<Key, T, HamBam, SoSanh>;
    //friend class iterator;

    BangBam (std::size_t preserve_size = 11) : _size(0), _soSanh()
    {
        _hamBam.setBucket(preserve_size);
        _table.resize(_hamBam.bucketCount());
    }

    const_iterator find (key_type const &key) const;
    iterator find (key_type const &key);

    std::pair<iterator, bool> insert (value_type const &value);

    mapped_type& operator[] (key_type const &key) {
        return (insert(std::make_pair(key, mapped_type())).first)->second;
    }

    bool erase (key_type const &key);

    void clear();

    void rehash (std::size_t count);

    void reserve (std::size_t count);

    std::size_t size() const { return _size; }

    iterator begin();
    iterator end() { 
        return iterator(_table.size() - 1, _table.back().end(), this);
    }
    const_iterator begin() const { 
        return const_cast<this_type*>(this)->begin(); 
    }
    const_iterator end() const {
        return const_cast<this_type*>(this)->end(); 
    }

private:
    typename list_type::iterator findElement(key_type const&, std::size_t &);

    std::vector<list_type> _table; 
    std::size_t _size;
    HamBam _hamBam;
    SoSanh _soSanh;
};


// cài đặt phương thức
//

template <typename Key, typename T, typename HamBam, typename SoSanh>
typename BangBam<Key, T, HamBam, SoSanh>::list_type::iterator
BangBam<Key, T, HamBam, SoSanh>::findElement(
            typename BangBam<Key, T, HamBam, SoSanh>::key_type const &key,
            std::size_t &bucket)
{
    bucket = _hamBam(key);
    typename list_type::iterator li = _table[bucket].begin();
    for (; li != _table[bucket].end(); ++li)
        if (_soSanh(li->first, key))
            return li;
    return li;
}


/*
 * Tìm kiếm phần tử có khóa cho trước
 * \param key: khóa cần tìm
 * \return: iterator đến cặp (key, mapped) cần tìm
 */
template <typename Key, typename T, typename HamBam, typename SoSanh>
typename BangBam<Key, T, HamBam, SoSanh>::const_iterator 
BangBam<Key, T, HamBam, SoSanh>::find ( 
    typename BangBam<Key, T, HamBam, SoSanh>::key_type const &key ) const
{
    return const_cast<this_type*>(this)->find(key);
}

template <typename Key, typename T, typename HamBam, typename SoSanh>
typename BangBam<Key, T, HamBam, SoSanh>::iterator 
BangBam<Key, T, HamBam, SoSanh>::find ( 
    typename BangBam<Key, T, HamBam, SoSanh>::key_type const &key )
{
    std::size_t bucket;
    typename list_type::iterator li = findElement(key, bucket);

    if (li != _table[bucket].end())
        return iterator(bucket, li, this);

    // Nếu không tìm thấy thì trả về cuối danh sách liên kết
    return end();
}

/*
 * Chèn vào bảng băm. Nếu đã có phần tử với khóa tương ứng trong bảng 
 * thì không chèn.
 * \param value_type: cặp (key, mapped) cần chèn
 * \return cặp (iterator, bool), với iterator là iterator đến cặp 
 * giá trị với khóa = key, bool chỉ việc chèn có thành công hay không.
 */
template <typename Key, typename T, typename HamBam, typename SoSanh>
std::pair<typename BangBam<Key, T, HamBam, SoSanh>::iterator, bool>
BangBam<Key, T, HamBam, SoSanh>::insert ( 
        typename BangBam<Key, T, HamBam, SoSanh>::value_type const &value)
{
    std::size_t bucket;
    typename list_type::iterator li = findElement(value.first, bucket);

    if (li != _table[bucket].end())
        return std::make_pair(iterator(bucket, li, this), false);
    
    li = _table[bucket].insert(li, value);
    ++_size;
    return std::make_pair(iterator(bucket, li, this), true);
}

/*
 * Xóa hoàn toàn bảng băm
 */
template <typename Key, typename T, typename HamBam, typename SoSanh>
void
BangBam<Key, T, HamBam, SoSanh>::clear()
{
    _table.clear();
    _size = 0;
}

/*
 * Xóa phần tử khỏi bảng băm
 * \param key: khóa tương ứng phần tử cần xóa
 * \return : xóa thành công hay không
 */
template <typename Key, typename T, typename HamBam, typename SoSanh>
bool
BangBam<Key, T, HamBam, SoSanh>::erase ( 
        typename BangBam<Key, T, HamBam, SoSanh>::key_type const &key)
{
    std::size_t bucket;
    typename list_type::iterator li = findElement(key, bucket);

    if (li != _table[bucket].end()) {
        _table[bucket].erase(li);
        --_size;
        return true;
    }
    return false;
}

/*
 * rehash
 * \param count: số bucket
 */
template <typename Key, typename T, typename HamBam, typename SoSanh>
void
BangBam<Key, T, HamBam, SoSanh>::rehash(std::size_t count)
{
    _hamBam.setBucket(count);
    if (!_size) {
        _table.resize(_hamBam.bucketCount());
        return;
    }

    typename list_type::iterator li;
    std::vector<list_type> new_table (_hamBam.bucketCount());
    for (std::size_t i = 0; i != _table.size(); ++i) {
        li = _table[i].begin();
        for (; li != _table[i].end(); ++li) {
            std::size_t bucket = _hamBam(li->first);
            new_table[bucket].push_back(*li);
        }
    }
    _table.swap(new_table);
}


/*
 * reserve
 */
template <typename Key, typename T, typename HamBam, typename SoSanh>
void
BangBam<Key, T, HamBam, SoSanh>::reserve(std::size_t count)
{
    //rehash(std::ceil(count / max_load_factor()));
}


/*
 * Trả về bộ lặp đến phần tử đầu bảng băm
 */
template <typename Key, typename T, typename HamBam, typename SoSanh>
typename BangBam<Key, T, HamBam, SoSanh>::iterator
BangBam<Key, T, HamBam, SoSanh>::begin() 
{
    if (_size == 0) return end();

    for (std::size_t i = 0; i != _table.size(); ++i) {
        if (!_table[i].empty())
            return iterator(i, _table[i].begin(), this);
    }

    return end();
}

#endif // __BANG_BAM__
