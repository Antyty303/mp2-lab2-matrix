//Отличия в: TDynamicVector: векторные operator +, -, *
//           TDynamicMatrix: перемещающее присваивание, * на вектор, матричные +, -, *
#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>
#include <type_traits>
#include <stdexcept>
#include <cstring> //memcmp

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

template<typename T>
    requires std::is_trivially_copyable_v<T>
class TDynamicVector
{
protected:
    size_t sz;
    T* pMem;
public:
    TDynamicVector(size_t size = 1);
    TDynamicVector(T* arr, size_t s);
    TDynamicVector(const TDynamicVector& v);
    TDynamicVector(TDynamicVector&& v) noexcept;
    ~TDynamicVector();

    TDynamicVector& operator=(const TDynamicVector& v);
    TDynamicVector& operator=(TDynamicVector&& v) noexcept;

    size_t size() const noexcept { return sz; }
    T* data() noexcept { return pMem; }
    const T* data() const noexcept { return pMem; }

    T& operator[](size_t ind) { return pMem[ind]; }
    const T& operator[](size_t ind) const { return pMem[ind]; }
    T& at(size_t ind);
    const T& at(size_t ind) const;

    bool operator==(const TDynamicVector& v) const noexcept;
    bool operator!=(const TDynamicVector& v) const noexcept;

    TDynamicVector operator+(T val) const;
    TDynamicVector operator-(T val) const;
    TDynamicVector operator*(T val) const;

    TDynamicVector operator+(const TDynamicVector& v) const;
    TDynamicVector operator-(const TDynamicVector& v) const;
    T operator*(const TDynamicVector& v) const;

    friend void swap(TDynamicVector& lhs, TDynamicVector& rhs) noexcept
    {
        std::swap(lhs.sz, rhs.sz);
        std::swap(lhs.pMem, rhs.pMem);
    }

    friend std::istream& operator>>(std::istream& istr, TDynamicVector& v)
    {
        for (size_t i = 0; i < v.sz; i++)
            istr >> v.pMem[i];
        return istr;
    }
    friend std::ostream& operator<<(std::ostream& ostr, const TDynamicVector& v)
    {
        for (size_t i = 0; i < v.sz; i++)
            ostr << v.pMem[i] << ' ';
        return ostr;
    }
};

template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicVector<T>::TDynamicVector(size_t size = 1) : sz(size)
{
    if (sz == 0)
        throw std::out_of_range("Vector size should be greater than zero");
    pMem = new T[sz]();
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicVector<T>::TDynamicVector(T* arr, size_t s) : sz(s)
{
    if (arr == nullptr) throw std::invalid_argument("Vector constructor requires non-nullptr arg");
    pMem = new T[sz];
    std::memcpy(pMem, arr, sz * sizeof(T));
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicVector<T>::TDynamicVector(const TDynamicVector& v)
{
    pMem = new T[v.sz];
    std::memcpy(pMem, v.pMem, v.sz * sizeof(T));
    sz = v.sz;
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicVector<T>::TDynamicVector(TDynamicVector&& v) noexcept
{
    pMem = v.pMem;
    sz = v.sz;

    v.pMem = nullptr;
    v.sz = 0;
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicVector<T>::~TDynamicVector()
{
    if (pMem) delete[] pMem;
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicVector<T>& TDynamicVector<T>::operator=(const TDynamicVector& v)
{
    if (&v == this) return *this;
    delete pMem;

    pMem = new T[v.sz];
    std::memcpy(pMem, v.pMem, v.sz * sizeof(T));
    sz = v.sz;

    return *this;
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicVector<T>& TDynamicVector<T>::operator=(TDynamicVector&& v) noexcept
{
    if (&v == this) return *this;
    delete pMem;

    pMem = v.pMem;
    sz = v.sz;

    v.pMem = nullptr;
    v.sz = 0;

    return *this;
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
T& TDynamicVector<T>::at(size_t ind)
{
    if (ind >= sz)
        throw std::out_of_range("Vector index out of range");
    return pMem[ind];
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
const T& TDynamicVector<T>::at(size_t ind) const
{
    if (ind >= sz)
        throw std::out_of_range("Vector index out of range");
    return pMem[ind];
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
bool TDynamicVector<T>::operator==(const TDynamicVector& v) const noexcept
{
    if (this == &v) return true;
    if (sz != v.sz) return false;

    return std::memcmp(pMem, v.pMem, sz * sizeof(T)) == 0;
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
bool TDynamicVector<T>::operator!=(const TDynamicVector& v) const noexcept
{
    return !(*this == v);
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicVector<T> TDynamicVector<T>::operator+(T val) const
{
    TDynamicVector<T> tmp(*this);
    for (size_t i = 0; i < sz; i++)
    {
        tmp.pMem[i] += val;
    }
    return tmp;
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicVector<T> TDynamicVector<T>::operator-(T val) const
{
    TDynamicVector<T> tmp(*this);
    for (size_t i = 0; i < sz; i++)
    {
        tmp.pMem[i] -= val;
    }
    return tmp;
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicVector<T> TDynamicVector<T>::operator*(T val) const
{
    TDynamicVector<T> tmp(*this);
    for (size_t i = 0; i < sz; i++)
    {
        tmp.pMem[i] *= val;
    }
    return tmp;
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicVector<T> TDynamicVector<T>::operator+(const TDynamicVector& v) const
{
    if (sz != v.sz)
        throw std::invalid_argument("TDynamicVector must have the same size for addition");

    TDynamicVector tmp(*this);
    for (size_t i = 0; i < sz; i++)
    {
        tmp.pMem[i] += v.pMem[i];
    }
    return tmp;
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicVector<T> TDynamicVector<T>::operator-(const TDynamicVector& v) const
{
    if (sz != v.sz)
        throw std::invalid_argument("TDynamicVector must have the same size for addition");

    TDynamicVector tmp(*this);
    for (size_t i = 0; i < sz; i++)
    {
        tmp.pMem[i] -= v.pMem[i];
    }
    return tmp;
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
T TDynamicVector<T>::operator*(const TDynamicVector& v) const
{
    if (sz != v.sz)
        throw std::invalid_argument("TDynamicVector must have the same size for product");

    T res = T{};
    for (size_t i = 0; i < sz; i++)
    {
        res += pMem[i] * v.pMem[i];
    }
    return res;
}



///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

template<typename T>
    requires std::is_trivially_copyable_v<T>
class TDynamicMatrix
{
protected:
    size_t rows;
    size_t cols;
    TDynamicVector<T> pMem;
public:
    TDynamicMatrix(size_t r = 1, size_t c = 1);
    TDynamicMatrix(const TDynamicMatrix& m);
    TDynamicMatrix(TDynamicMatrix&& m) noexcept;
    ~TDynamicMatrix() = default;

    TDynamicMatrix& operator=(const TDynamicMatrix& m);
    TDynamicMatrix& operator=(TDynamicMatrix&& m) noexcept;

    size_t get_rows() const noexcept { return rows; }
    size_t get_cols() const noexcept { return cols; }

    T* operator[](size_t ind) noexcept { return &pMem[ind * cols]; }
    const T* operator[](size_t ind) const noexcept { return &pMem[ind * cols]; }
    T* at(size_t ind);
    const T* at(size_t ind) const;

    bool operator==(const TDynamicMatrix& m) const noexcept;
    bool operator!=(const TDynamicMatrix& m) const noexcept;

    TDynamicMatrix operator*(const T& val) const;
    TDynamicVector<T> operator*(const TDynamicVector<T>& v) const;
    TDynamicMatrix operator+(const TDynamicMatrix& m) const;
    TDynamicMatrix operator-(const TDynamicMatrix& m) const;
    TDynamicMatrix operator*(const TDynamicMatrix& m) const;

    friend void swap(TDynamicMatrix& lhs, TDynamicMatrix& rhs) noexcept
    {
        std::swap(lhs.rows, rhs.rows);
        std::swap(lhs.cols, rhs.cols);
        swap(lhs.pMem, rhs.pMem);
    }

    friend std::istream& operator>>(std::istream& istr, TDynamicMatrix& m)
    {
        size_t new_rows = 0, new_cols = 0;
        istr >> new_rows >> new_cols;

        if (new_rows == 0 || new_cols == 0)
            throw std::out_of_range("The number of columns and rows of the Matrix must be greater than zero");

        m.rows = new_rows;
        m.cols = new_cols;
        m.pMem = TDynamicVector<T>(new_rows * new_cols);

        istr >> m.pMem;
        return istr;
    }

    friend std::ostream& operator<<(std::ostream& ostr, const TDynamicMatrix& m)
    {
        ostr << m.rows << ' ' << m.cols << '\n';

        for (size_t i = 0; i < m.rows; i++)
        {
            for (size_t j = 0; j < m.cols; j++)
            {
                ostr << m[i][j];
                if (j + 1 < m.cols) ostr << ' ';
            }
            if (i + 1 < m.rows) ostr << '\n';
        }
        return ostr;
    }
};

template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicMatrix<T>::TDynamicMatrix(size_t r = 1, size_t c = 1) : rows(r), cols(c)
{
    if (rows == 0 || cols == 0)
        throw std::out_of_range("The number of columns and rows of the Matrix must be greater than zero");
    pMem = TDynamicVector<T>(rows * cols);
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicMatrix<T>::TDynamicMatrix(const TDynamicMatrix& m) : rows(m.rows), cols(m.cols), pMem(m.pMem)
{
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicMatrix<T>::TDynamicMatrix(TDynamicMatrix&& m) noexcept : rows(m.rows), cols(m.cols), pMem(std::move(m.pMem))
{
    m.rows = 0;
    m.cols = 0;
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicMatrix<T>& TDynamicMatrix<T>::operator=(const TDynamicMatrix& m)
{
    if (this == &m) return *this;

    pMem = m.pMem;
    rows = m.rows;
    cols = m.cols;

    return *this;
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicMatrix<T>& TDynamicMatrix<T>::operator=(TDynamicMatrix&& m) noexcept
{
    if (this == &m) return *this;

    pMem = std::move(m.pMem);
    rows = m.rows;
    cols = m.cols;

    m.rows = 0;
    m.cols = 0;

    return *this;
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
T* TDynamicMatrix<T>::at(size_t ind)
{
    if (ind >= rows)
    {
        throw std::out_of_range("Matrix row index out of range");
    }
    return &pMem[ind * cols];
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
const T* TDynamicMatrix<T>::at(size_t ind) const
{
    if (ind >= rows)
    {
        throw std::out_of_range("Matrix row index out of range");
    }
    return &pMem[ind * cols];
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
bool TDynamicMatrix<T>::operator==(const TDynamicMatrix& m) const noexcept
{
    if (rows != m.rows || cols != m.cols) return false;
    return pMem == m.pMem;
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
bool TDynamicMatrix<T>::operator!=(const TDynamicMatrix& m) const noexcept
{
    if (rows != m.rows || cols != m.cols) return true;
    return !(pMem == m.pMem);
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicMatrix<T> TDynamicMatrix<T>::operator*(const T& val) const
{
    TDynamicMatrix<T> res(rows, cols);
    res.pMem = pMem * val;
    return res;
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicVector<T> TDynamicMatrix<T>::operator*(const TDynamicVector<T>& v) const
{
    if (cols != v.size())
        throw std::invalid_argument("Matrix columns must match vector size for multiplication");

    TDynamicVector<T> res(rows);

    for (size_t i = 0; i < rows; i++)
    {
        T sum = T{};
        for (size_t j = 0; j < cols; j++)
        {
            sum += (*this)[i][j] * v[j];
        }
        res[i] = sum;
    }
    return res;
}

template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicMatrix<T> TDynamicMatrix<T>::operator+(const TDynamicMatrix& m) const
{
    if (rows != m.rows || cols != m.cols)
        throw std::invalid_argument("Matrix dimensions must be equal for addition");

    TDynamicMatrix<T> res(rows, cols);
    res.pMem = pMem + m.pMem;
    return res;
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicMatrix<T> TDynamicMatrix<T>::operator-(const TDynamicMatrix& m) const
{
    if (rows != m.rows || cols != m.cols)
        throw std::invalid_argument("Matrix dimensions must be equal for subtraction");

    TDynamicMatrix<T> res(rows, cols);
    res.pMem = pMem - m.pMem;
    return res;
}
template<typename T>
    requires std::is_trivially_copyable_v<T>
TDynamicMatrix<T> TDynamicMatrix<T>::operator*(const TDynamicMatrix& m) const
{
    if (cols != m.rows)
        throw std::invalid_argument("Number of columns and rows of the matrices does not match for multiplication");

    TDynamicMatrix<T> res(rows, m.cols);
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < m.cols; j++)
        {
            T sum = T{};
            for (size_t k = 0; k < cols; k++)
            {
                sum += (*this)[i][k] * m[k][j];
            }
            res[i][j] = sum;
        }
    }
    return res;
}

#endif
