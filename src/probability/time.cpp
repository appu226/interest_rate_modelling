/*

Copyright 2020 Parakram Majumdar

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "time.h"

namespace {


    class TimeVector : public irm::ITimeVector {
    public:
        TimeVector(std::vector<irm::Time> data) :
                m_data(std::move(data)) {}


        int getNumTimes() const override {
            return m_data.size();
        }

        irm::Time getTimeAtIndex(int i) const override {
            return m_data[i];
        }

    private:
        std::vector<irm::Time> m_data;
    };


    class RegularIntervals : public irm::ITimeVector {
    public:
        RegularIntervals(double start, int numTimes, double delta) :
                m_start(start),
                m_numTimes(numTimes),
                m_delta(delta) {}

        int getNumTimes() const override {
            return m_numTimes;
        }

        double getTimeAtIndex(int i) const override {
            return m_start + m_delta * i;
        }


    private:
        double m_start;
        int m_numTimes;
        double m_delta;
    };

} // end anonymous namespace



namespace irm {

    ITimeVector::CPtr ITimeVector::createFromTimeVector(std::vector<double> timeVector) {
        return std::make_shared<TimeVector>(std::move(timeVector));
    }

    ITimeVector::CPtr ITimeVector::createRegularInterval(double start, int numTimes, double delta) {
        return std::make_shared<RegularIntervals>(start, numTimes, delta);
    }

} // end namespace irm