#pragma once

#include <string>
#include <vector>
#include "point.hpp"

template<class T>
struct Overlay {
    using iterator = typename std::vector<T>::iterator;

    int getXSize() const;
    int getYSize() const;

    bool check(int x, int y) const;

    T& get(int x, int y);
    const T& get(int x, int y) const;

    T& operator()(int x, int y);
    const T& operator()(int x, int y) const;

    T& operator()(point p) { return (*this)(p.first, p.second); }

    void resize(int x, int y);

    void clear() {
        memset(data.data(), 0, data.size() * sizeof(T));
    }

    iterator begin() { return data.begin(); }
    iterator end() { return data.end(); }

    int xof(iterator ptr) { return (ptr - data.begin()) % xsz; }
    int yof(iterator ptr) { return (ptr - data.begin()) / xsz; }
    point pointof(iterator ptr) { return { xof(ptr), yof(ptr) }; }

    Overlay(int x, int y);

    int xsz;
    int ysz;
    std::vector<T> data;
};

template<class T>
Overlay<T>::Overlay(int x, int y) : xsz(x), ysz(y), data(xsz*ysz) {}

template<class T>
int Overlay<T>::getXSize() const
{
    return xsz;
}

template<class T>
int Overlay<T>::getYSize() const
{
    return ysz;
}

template <class T>
bool Overlay<T>::check(int x, int y) const {
    return (x >= 0 && x < xsz) && (y >= 0 && y < ysz);
}

template <class T>
T& Overlay<T>::get(int x, int y) { return data[xsz*y + x]; }

template <class T>
const T& Overlay<T>::get(int x, int y) const
{
    return data[xsz*y + x];
}

template <class T>
T& Overlay<T>::operator()(int x, int y)
{
    return get(x, y);
}
template <class T>
const T& Overlay<T>::operator()(int x, int y) const
{
    return get(x, y);
}

template <class T>
void Overlay<T>::resize(int x, int y) { xsz = x; ysz = y; data.resize(x*y); }
