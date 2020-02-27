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

#include <iostream>
#include <cassert>

#include <probability/state.h>
#include <probability/time.h>

void testState();

void testTime();

#define info(x) std::cout << "[test_probability] " << x << std::endl

int main() {
    info("starting");
    testState();
    testTime();
    info("SUCCESS");
    return 0;
}


void testState() {
    using namespace irm;
    const double tolerance = 1e-13;
    std::vector<double> data{0.3, 0.4, 0.5};
    IState::CPtr state = IState::createFromDoubleVector(data);
    assert(state->getNumValues() == 3);
    for (int i = 0; i < data.size(); ++i) {
        assert(std::abs(state->getValueAtIndex(i) - data[i]) < tolerance);
    }
}


void testTime() {
    using namespace irm;
    const double tolerance = 1e-13;
    std::vector<irm::Time> data{0.3, 0.4, 0.5};
    irm::ITimeVector::CPtr vec = irm::ITimeVector::createFromTimeVector(data);
    irm::ITimeVector::CPtr reg = irm::ITimeVector::createRegularInterval(
            data[0], data.size(), (data.back() - data.front()) / (data.size() - 1));

    assert(vec->getNumTimes() == data.size());
    assert(reg->getNumTimes() == data.size());

    for (int i = 0; i < data.size(); ++i) {
        assert(std::abs(vec->getTimeAtIndex(i) - data[i]) < tolerance);
        assert(std::abs(reg->getTimeAtIndex(i) - data[i]) < tolerance);
    }
}