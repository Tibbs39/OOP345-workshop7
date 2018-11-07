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
    template <typename T = double>
    class DataTable {
        int width;
        int decPrec;
        
        // a vector of pairs
        std::vector<T> xVals;
        std::vector<T> yVals;

        // passes func to std::accumulate
        template <typename Func>
        T calcSum(std::vector<T> dataset, Func func) const {
            return std::accumulate(dataset.begin(), dataset.end(), T(0), func);
        }

        T calcSum(std::vector<T> dataset) const {
            return std::accumulate(dataset.begin(), dataset.end(), T(0));
        }

        public:
            // constructor (filestream, field width, decimal precision)
            DataTable(std::ifstream& ifs, const int& fw, const int& prec) : width(fw), decPrec(prec) { 
                std::string buffer;
                int ln_cnt = 0;
                while (!ifs.eof()) {
                    std::getline(ifs, buffer);
                    if (buffer.length() > 0)
                        ++ln_cnt;
                }

                ifs.clear();
                ifs.seekg(0, ifs.beg);

                T tmpX, tmpY;
                for (int i = 0; i < ln_cnt; i++) {
                    ifs >> tmpX >> tmpY;
                    if (ifs.fail()) {
                        std::string err = "Invalid Format";
                        throw err;
                    }
                    xVals.push_back(tmpX);
                    yVals.push_back(tmpY);
                }
            }
            
            // mean of Y values
            T mean() const {
                return calcSum(yVals) / getSize();
            }

            // standard deviation of Y values
            T sigma() const {
                // calculate mean & store in a variable
                T avg = mean();
                
                // get sum of for each (element minus mean)^2
                T sum = calcSum(yVals, [&](const T& a, const T& b) -> T {
                    return a + (b - avg) * (b - avg);
                });
                // divide by num of elements minus 1
                sum = sum / (getSize() - 1);

                // square root everything
                sum = sqrt(sum);

                return sum;
            }

            // median of y-values
            T median() const {
                // sort the data set
                std::vector<T> temp = yVals;
                std::sort(temp.begin(), temp.end());

                // if size is odd, it should round to the correct element cause the type is an integer
                return temp[temp.size() / 2];
            }

            // regression
            void regression(T& slope, T& y_intercept) const {
                // calculate required sums
                T sum_x = calcSum(xVals);

                T sum_y = calcSum(yVals);

                T sum_xy = std::inner_product(xVals.begin(), xVals.end(), yVals.begin(), T(0));

                T sum_xSq = calcSum(xVals, [](const T& a, const T& b) -> T {
                    return a + b * b;
                });

                T n = getSize(); // n = size

                // calculate slope
                slope = ( (n * sum_xy) - (sum_x * sum_y) ) / ( (n * sum_xSq) - (sum_x * sum_x) );

                // calculate y-intercept
                y_intercept = (sum_y - slope * sum_x) / n;
            } 

            // display the data set
            void display(std::ostream& os) const {
                os << std::setw(width) << "x" << std::setw(width) << "y" << std::endl;

                os << std::fixed << std::setprecision(decPrec);

                for (size_t i = 0; i < getSize(); ++i) {
                    os << std::setw(width) << xVals[i]
                       << std::setw(width) << yVals[i] << std::endl;
                }
            }

            // return the size of the data set
            size_t getSize() const {
                return xVals.size();
            }
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const DataTable<T>& src) {
        src.display(os);

        return os;
    }
}

#endif