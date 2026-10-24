//Отличия в: operator +, -, * в TDynamicVector
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
    TDynamicVector(size_t size = 1) : sz(size)
    {
        if (sz == 0)
            throw std::out_of_range("Vector size should be greater than zero");
        pMem = new T[sz]();
    }
    TDynamicVector(T* arr, size_t s) : sz(s)
    {
        if (arr == nullptr) throw std::invalid_argument("TDynamicVector constructor requires non-nullptr arg");
        pMem = new T[sz];
        std::memcpy(pMem, arr, sz * sizeof(T));
    }
    TDynamicVector(const TDynamicVector& v)
    {
        pMem = new T[v.sz];
        std::memcpy(pMem, v.pMem, v.sz * sizeof(T));
        sz = v.sz;
    }
    TDynamicVector(TDynamicVector&& v) noexcept
    {
        pMem = v.pMem;
        sz = v.sz;

        v.pMem = nullptr;
        v.sz = 0;
    }
    ~TDynamicVector()
    {
        if (pMem) delete[] pMem;
    }
    TDynamicVector& operator=(const TDynamicVector& v)
    {
        if (&v == this) return *this;
        delete pMem;

        pMem = new T[v.sz];
        std::memcpy(pMem, v.pMem, v.sz * sizeof(T));
        sz = v.sz;

        return *this;
    }
    TDynamicVector& operator=(TDynamicVector&& v) noexcept
    {
        if (&v == this) return *this;
        delete pMem;

        pMem = v.pMem;
        sz = v.sz;

        v.pMem = nullptr;
        v.sz = 0;

        return *this;
    }

    size_t size() const noexcept { return sz; }

    // индексация
    T& operator[](size_t ind)
    {
        return pMem[ind];
    }
    const T& operator[](size_t ind) const
    {
        return pMem[ind];
    }
    // индексация с контролем
    T& at(size_t ind)
    {
        if (ind >= sz)
            throw std::out_of_range("TDynamicVector index out of range");
        return pMem[ind];
    }
    const T& at(size_t ind) const
    {
        if (ind >= sz)
            throw std::out_of_range("TDynamicVector index out of range");
        return pMem[ind];
    }

    // сравнение
    bool operator==(const TDynamicVector& v) const noexcept
    {
        if (this == &v) return true;
        if (sz != v.sz) return false;

        return std::memcmp(pMem, v.pMem, sz * sizeof(T)) == 0;
    }
    bool operator!=(const TDynamicVector& v) const noexcept
    {
        return !(*this == v);
    }

    // скалярные операции
    TDynamicVector operator+(T val)
    {
        TDynamicVector<T> tmp(*this);
        for (size_t i = 0; i < sz; i++)
        {
            tmp.pMem[i] += val;
        }
        return tmp;
    }
    TDynamicVector operator-(double val)
    {
        TDynamicVector<T> tmp(*this);
        for (size_t i = 0; i < sz; i++)
        {
            tmp.pMem[i] -= val;
        }
        return tmp;
    }
    TDynamicVector operator*(double val)
    {
        TDynamicVector<T> tmp(*this);
        for (size_t i = 0; i < sz; i++)
        {
            tmp.pMem[i] *= val;
        }
        return tmp;
    }

    // векторные операции
    TDynamicVector operator+(const TDynamicVector& v)
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
    TDynamicVector operator-(const TDynamicVector& v)
    {
        if (sz != v.sz)
            throw std::invalid_argument("TDynamicVector must have the same size for subtraction");

        TDynamicVector tmp(*this);
        for (size_t i = 0; i < sz; i++)
        {
            tmp.pMem[i] -= v.pMem[i];
        }
        return tmp;
    }
    T operator*(const TDynamicVector& v) noexcept
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

    friend void swap(TDynamicVector& lhs, TDynamicVector& rhs) noexcept
    {
        std::swap(lhs.sz, rhs.sz);
        std::swap(lhs.pMem, rhs.pMem);
    }

    // ввод/вывод
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

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

template<typename T>
//requires std::is_trivially_copyable_v<T>
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>>
{
    using TDynamicVector<TDynamicVector<T>>::pMem;
    using TDynamicVector<TDynamicVector<T>>::sz;
public:
    TDynamicMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(s)
    {
        for (size_t i = 0; i < sz; i++)
            pMem[i] = TDynamicVector<T>(sz);
    }

    using TDynamicVector<TDynamicVector<T>>::operator[];

    // сравнение
    bool operator==(const TDynamicMatrix& m) const noexcept
    {
        return false;
    }

    // матрично-скалярные операции
    TDynamicMatrix<T> operator*(const T& val)
    {
        return TDynamicMatrix<T>{};
    }

    // матрично-векторные операции
    TDynamicVector<T> operator*(const TDynamicVector<T>& v)
    {
        return TDynamicVector<T>{};
    }

    // матрично-матричные операции
    TDynamicMatrix operator+(const TDynamicMatrix& m)
    {
        // можно ли написать реализацию, используя operator+ вектора?
        return TDynamicMatrix<T>{};
    }
    TDynamicMatrix operator-(const TDynamicMatrix& m)
    {
        return TDynamicMatrix<T>{};
    }
    TDynamicMatrix operator*(const TDynamicMatrix& m)
    {
        return TDynamicMatrix<T>{};
    }

    // ввод/вывод
    friend std::istream& operator>>(std::istream& istr, TDynamicMatrix& v)
    {
        return istr;
    }
    friend std::ostream& operator<<(std::ostream& ostr, const TDynamicMatrix& v)
    {
        return ostr;
    }
};

#endif
