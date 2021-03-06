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

    using namespace irm;

    class TimeVectorFromVector : public ITimeVector {
    public :
        int getNumTimes() const override {
            return m_timePoints.size();
        }

        Time getTimeAtIndex(int timeIndex) const override {
            return m_timePoints.at(timeIndex);
        }

        void setTimeAtIndex(int timeIndex, Time t) override {
            m_timePoints[timeIndex] = t;
        }

        TimeVectorFromVector(std::vector<double> timeVector) :
          m_timePoints(std::move(timeVector))
        { }

    private:
        std::vector<Time> m_timePoints;
    };

} // end anonymous namespace


namespace irm {

    ITimeVectorPtr ITimeVector::createFromVector(std::vector<Time> t)
    {
        return std::make_shared<TimeVectorFromVector>(std::move(t));
    }

    ITimeVectorPtr ITimeVector::createUniform(Time start, Time dt, int numTimes)
    {
        std::vector<Time> tv;
        tv.reserve(numTimes);
        for (int i = 0; i < numTimes; ++i)
            tv.push_back(start + dt * i);
        return std::make_shared<TimeVectorFromVector>(std::move(tv));
    }


} // end namespace irm