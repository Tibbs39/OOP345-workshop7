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
#include <fstream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>

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

    template <typename T = double>
    class DataTable {
        /* internal class Pair (should I not make this a nested class??) */
        template <typename T2 = T>
        class Pair {
            T2 xVal;
            T2 yVal;

            public:
                // constructor 
                Pair(const T2& x, const T2& y) : xVal(x), yVal(y) {}

                // return X
                T2 x() const { return xVal; }
                // return Y
                T2 y() const { return yVal; }
        }; // end of internal class Pair

        int width;
        int decPrec;
        
        // a vector of pairs
        std::vector<Pair<T>> dataset;

        // passes func to std::accumulate
        template <typename Func>
        T calcSum(Func func) const {
            //T init = 0;
            return std::accumulate(dataset.begin(), dataset.end(), T(0), func);
        }

        public:
            // constructor (filestream, field width, decimal precision)
            DataTable(std::ifstream& ifs, const int& fw, const int& prec) : width(fw), decPrec(prec) { 
                std::string buffer;
                int ln_cnt = 0;
                while (!ifs.eof()) {
                    std::getline(ifs, buffer);
                    ++ln_cnt;
                }

                ifs.clear();
                ifs.seekg(0, ifs.beg);

                T tmpX, tmpY;
                for (int i = 0; i < ln_cnt-1; i++) {
                    ifs >> tmpX >> tmpY;
                    Pair<T> tmpXY(tmpX, tmpY);
                    dataset.push_back(tmpXY);
                }
            }
            
            // mean of Y values
            T mean() const {
                T sum = calcSum([](const T& a, const Pair<T>& b) -> T {
                    return a + b.y();
                });

                return sum / dataset.size();
            }

            // standard deviation of Y values
            T sigma() const {
                // calculate mean & store in a variable
                T avg = mean();
                
                /* auto lambda = [&](const T& a, const Pair<T>& b) -> T {
                    return a + pow(b.y() - avg, 2);
                };

                T sum = calcSum(lambda); */
                // get sum of for each (element minus mean)^2
                T sum = calcSum([&](const T& a, const Pair<T>& b) -> T {
                    return a + pow(b.y() - avg, 2);
                });
                // divide by num of elements minus 1
                sum = sum / (dataset.size() - 1);

                // square root everything
                sum = sqrt(sum);

                return sum;
            }

            // median
            T median() const {
                // sort the data set
                std::vector<Pair<T>> temp = dataset;
                std::sort(temp.begin(), temp.end(), [](const Pair<T>& a, const Pair<T>& b) -> bool {
                    return a.y() < b.y();
                });

                /* NOTE: 
                    Is this the correct median calculation? I thought if length was even
                    the median should be the average of the two middle elements of the data set.
                */

                // if length = odd,  get element at( (length + 1) / 2 )
                // if length = even, get element at( length / 2 )
                int i = (temp.size() + (temp.size() % 2) ) / 2;
                
                
                return temp[i].y();
            }

            // regression
            void regression(T& slope, T& y_intercept) const {
                // calculate required sums
                T sum_x = calcSum([](const T& a, const Pair<T>& b) -> T {
                    return (a + b.x());
                });

                T sum_y = calcSum([](const T& a, const Pair<T>& b) -> T {
                    return a + b.y();
                });

                T sum_xy = calcSum([](const T& a, const Pair<T>& b) -> T {
                    return a + b.x() * b.y();
                });

                T sum_xSq = calcSum([](const T& a, const Pair<T>&b) -> T {
                    return a + pow(b.x(), 2);
                });

                T n = dataset.size(); // n = size

                // calculate slope
                slope = ( (n * sum_xy) - (sum_x * sum_y) ) / ( (n * sum_xSq) - pow(sum_x,2) );

                // calculate y-intercept
                y_intercept = (sum_y - slope * sum_x) / n;
            } 

            // display the data set
            void display(std::ostream& os) const {
                os << std::setw(width) << "x" << std::setw(width) << "y" << std::endl;

                os << std::fixed << std::setprecision(decPrec);

                std::for_each(dataset.begin(), dataset.end(), [&](const Pair<T>& src) {
                    os << std::setw(width) << src.x()
                       << std::setw(width) << src.y() << std::endl;
                });
            }

            // return the size of the data set
            size_t getSize() const {
                return dataset.size();
            }
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const DataTable<T>& src) {
        src.display(os);

        return os;
    }
}


#endif