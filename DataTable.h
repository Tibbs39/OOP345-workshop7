// Name:  Kenneth Yue
// Seneca Student ID: 127932176
// Seneca email:  kyue3@myseneca.ca
// Date of completion: Nov 2, 2018
//
// I confirm that the content of this file is created by me,
// with exception of the parts provided to me by my professor

#ifndef W7_DATATABLE_H
#define W7_DATATABLE_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <algorithm>

namespace w7 {
/*     template <typename T = int>
    class Pair {
        T xVal;
        T yVal;

        // constructor 
        Pair() {} // default constructor
        Pair(const T& x, const T& y) : xVal(x), yVal(y) {}

        // return X
        T x() { return xVal; }

        // return Y
        T y() { return yVal; }
    }; */

    template <typename T>
    class DataTable {
        // internal pair class
        template <typename T>
        class Pair {
            T xVal;
            T yVal;

            public:
                // constructor 
                Pair() {} // default constructor
                Pair(const T& x, const T& y) : xVal(x), yVal(y) {}

                // return X
                T x() { return xVal; }
                // return Y
                T y() { return yVal; }
        };

        int width;
        int decPrec;

        // a vector of pairs
        std::vector<Pair<T>> dataset;
        // constructor (filestream, field width, decimal precision)

        public:
            // mean
            T mean() const {
                T sum = std::accumulate(dataset.begin(), dataset.end(), 0, [](const T& a, const Pair<T>& b) -> T {
                    return a + b.x();
                });

                return sum / dataset.size();
            }

            // sigma
            T sigma() const {

            }

            // median
            T median() const {
                // sort the data set
                // check if length even or odd
                // if even, get element at(length / 2) and element at(length / 2 + 1)
                // then the sum of two elements divided by 2

                // if odd, get element at(length + 1) divided by 2
            }

            // regression
            void regression(T& slope, T& y_intercept) const {

            }

            // display the data set
            void display(std::ostream& os) const {
                os << std::setw(width) << "x" << std::setw(width) << "y" << std::endl;

                std::for_each(dataset.begin(), dataset.end(), [](const Pair<T>& a) {
                    os << std::setw(width) << std::setprecision(decPrec) << a.x()
                       << std::setw(width) << std::setprecision(decPrec) << a.y() << std::endl;
                });


            }

            // return the size of the data set
            size_t getSize() const {
                return dataset.size();
            }
    };
}


#endif