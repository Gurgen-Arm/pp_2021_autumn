// Copyright 2021 Trevogin Kirill
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <mpi.h>
#include <random>
#include <vector>
#ifndef MODULES_TASK_3_TREVOGIN_K_CANON_CANON_H_
#define MODULES_TASK_3_TREVOGIN_K_CANON_CANON_H_

#define UINT64_T uint64_t

template <class T>
class matrix {
private:
    T* m;
    int numRows, numColums;

public:
    matrix();
    matrix(const matrix<T>& c);
    matrix(const int _numRows, const int _numColums);
    ~matrix();

    T* operator[](const int i);
    const T* operator[](const int i) const;
    matrix<T> operator*(const matrix<T>& c);
    matrix<T> operator+(const matrix<T>& c);
    matrix<T>& operator=(const matrix<T>& c);
    bool operator==(const matrix<T>& c);
    template <class T2>
    friend std::ostream& operator<<(std::ostream& o, const matrix<T2>& c);

    void fillMatrix(int (*generateFunciton)(int, int, int*),
        int value1, int value2, int baseFeedbackValue);
    int getNumRows() const { return numRows; }
    int getNumColums() const { return numColums; }
    T* data() { return m; }
    /*const*/ T* data() const { return /*reinterpret_cast<const T*>(*/ m /*)*/; }
    void prepareSpace(int _numRows, int _numColums);
};

template <class T>
matrix<T>::matrix()
{
    m = nullptr;
    numRows = numColums = 0;
}

template <class T>
matrix<T>::matrix(const matrix<T>& c)
{
    numRows = c.numRows;
    numColums = c.numColums;
    if (numRows * numColums == 0) {
        m = nullptr;
    } else {
        m = new T[static_cast<UINT64_T>(numRows) * numColums]; // C26451
        for (int i = 0; i < numRows * numColums; i++)
            m[i] = c.m[i];
    }
}

template <class T>
matrix<T>::matrix(const int _numRows, const int _numColumns)
{
    numRows = _numRows;
    numColums = _numColumns;
    m = new T[static_cast<UINT64_T>(numRows) * numColums]; // C26451
    for (int i = 0; i < numRows * numColums; m[i++] = 0) { }
}

template <class T>
matrix<T>::~matrix()
{
    numRows = numColums = 0;
    if (m != nullptr) {
        delete[] m;
        m = nullptr;
    }
}

template <class T>
T* matrix<T>::operator[](const int i)
{
    return m + static_cast<UINT64_T>(i) * numColums; // C26451
}

template <class T>
const T* matrix<T>::operator[](const int i) const
{
    return reinterpret_cast<const T*>(m + static_cast<UINT64_T>(i) * numColums); // C26451
}

template <class T>
matrix<T> matrix<T>::operator*(const matrix<T>& c)
{
    matrix<T> res(numRows, c.numColums);
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < c.numColums; j++) {
            res[i][j] = 0;
            for (int k = 0; k < numColums; k++) {
                res[i][j] += (*this)[i][k] * c[k][j];
            }
        }
    }
    return res;
}

template <class T>
matrix<T> matrix<T>::operator+(const matrix<T>& c)
{
    matrix<T> res(numRows, numColums);
    if (numRows != c.numRows || numColums != c.numColums)
        return res;
    for (int i = 0; i < numRows; i++)
        for (int j = 0; j < numColums; j++)
            res[i][j] = (*this)[i][j] + c[i][j];
    return res;
}

template <class T>
matrix<T>& matrix<T>::operator=(const matrix<T>& c)
{
    if (this == &c)
        return *this;
    if (m != nullptr)
        delete[] m;
    numRows = c.numRows;
    numColums = c.numColums;
    m = new T[static_cast<UINT64_T>(numRows) * numColums];
    for (int i = 0; i < numRows * numColums; i++)
        m[i] = c.m[i];
    return *this;
}

template <class T>
bool matrix<T>::operator==(const matrix<T>& c)
{
    if (numRows != c.numRows || numColums != c.numColums)
        return false;
    for (int i = 0; i < numRows * numColums; i++)
        if (m[i] != c.m[i])
            return false;
    return true;
}

template <class T>
std::ostream& operator<<(std::ostream& o, const matrix<T>& c)
{
    for (int i = 0; i < c.getNumRows(); i++) {
        for (int j = 0; j < c.getNumColums(); j++) {
            o << c[i][j] << '\t';
        }
        o << std::endl;
    }
    return o;
}

template <class T>
void matrix<T>::fillMatrix(int (*generateFunciton)(int, int, int*), int value1, int value2, int baseFeedbackValue)
{
    int feedback = baseFeedbackValue;
    for (int i = 0; i < numRows * numColums; i++)
        m[i] = static_cast<T>(generateFunciton(value1, value2, &feedback));
}

template <class T>
void matrix<T>::prepareSpace(int _numRows, int _numColums)
{
    if (m != nullptr)
        delete[] m;
    m = nullptr;
    numRows = _numRows;
    numColums = _numColums;
    m = new T[static_cast<UINT64_T>(numRows) * numColums];
}

// can generate in range [-maxValue, -minValue] && [minValue, maxValue] IN: minValue, maxValue > 0
int generateRngValue(int minValue, int maxValue, int* feedback);

// generates: {minValue + feedback, minValue + feedback + 1, minValue + feedback + 2, ...}
int generateRisingValue(int minValue, int empty, int* feedback);

// generates: {maxValue - feedback, maxValue - feedback - 1, maxValue - feedback - 2, ...}
int generateFallingValue(int empty, int maxValue, int* feedback);

// generates: v1, v2, v1, v2, v1, v2, v1, v2, ... with rule feedback % 2 == 0 -> v1 else v2
int generateJumpingValue(int v1, int v2, int* feedback);

// parallel Cannon's multiplication
matrix<double> cannonsMultiplication(matrix<double>* A, matrix<double>* B);


#endif // MODULES_TASK_3_TREVOGIN_K_CANON_CANON_H_