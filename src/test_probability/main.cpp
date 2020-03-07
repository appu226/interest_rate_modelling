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

#include <probability/path.h>
#include <probability/state.h>
#include <probability/time.h>
#include <probability/wiener_process.h>

void testPath();
void testState();
void testTime();
void testWienerProcess();


#define info(x) std::cout << "[test_probability] " << x << std::endl



bool doubleEquals(double x, double y, double absTol = 1e-100) {
    double diff = std::abs(x - y);
    return diff < absTol;
}





int main() {
    info("Starting");
    testPath();
    testState();
    testTime();
    testWienerProcess();
    info("SUCCESS");
    return 0;
}



void testPath() {
    using namespace irm;
    info("testPath");
    int numTimes = 3;
    Time t0 = 0, dt = .1;
    auto tv = ITimeVector::createUniform(t0, dt, numTimes);
    int stateSize = 2;
    auto path = IPath::createZeroPath(tv, stateSize);
    assert(path->getNumTimes() == numTimes);
    assert(path->getStateSize() == stateSize);
    for (int it = 0; it < numTimes; ++it) {
        assert(path->getStateAtIndex(it).getNumValues() == stateSize);
        assert(std::static_pointer_cast<const IPath>(path)->getStateAtIndex(it).getNumValues() == stateSize);
        assert(path->getTimeAtIndex(it) == tv->getTimeAtIndex(it));
        for (int iv = 0; iv < stateSize; ++iv) {
            double newValue = it + iv * numTimes;
            doubleEquals(path->getStateAtIndex(it).getValue(StateVariable(iv)), 0);
            path->getStateAtIndex(it).setValue(StateVariable(iv), newValue);
            doubleEquals(path->getStateAtIndex(it).getValue(StateVariable(iv)), newValue);
        }
    }
}


void testState() {
    using namespace irm;
    info("testState");
    auto zeroState = IState::createZeroState(5);
    assert(zeroState);
    assert(zeroState->getNumValues() == 5);
    for (int i = 0; i < 5; ++i) {
        StateVariable si(i);
        assert(doubleEquals(zeroState->getValue(si), 0.0));
        zeroState->setValue(si, i);
        assert(doubleEquals(zeroState->getValue(si), i));
    }


    std::vector<double> customValues{1, 2.71828, 3.14159};
    auto customState = IState::createFromVector(customValues);
    assert(customState->getNumValues() == customValues.size());
    for (int i = 0; i < customValues.size(); ++i) {
        StateVariable si(i);
        assert(doubleEquals(customState->getValue(si), customValues[i]));
        customState->setValue(si, i);
        assert(doubleEquals(customState->getValue(si), i));
    }
}


void testTime() {
    using namespace irm;
    info("testTime");
    Time t0 = 0, dt = .1;
    int nt = 4;
    auto uniformTimeVector = ITimeVector::createUniform(t0, dt, nt);
    assert(uniformTimeVector->getNumTimes() == nt);
    for (int it = 0; it < nt; ++it)
    {
        assert(doubleEquals(uniformTimeVector->getTimeAtIndex(it), t0 + dt * it));
        uniformTimeVector->setTimeAtIndex(it, it);
        assert(doubleEquals(uniformTimeVector->getTimeAtIndex(it), it));
    }

    std::vector<Time> tv {0, .1, .2, 3.14};
    auto customTimeVector = ITimeVector::createFromVector(tv);
    assert(customTimeVector->getNumTimes() == tv.size());
    for (int it = 0; it < tv.size(); ++it)
    {
        assert(doubleEquals(customTimeVector->getTimeAtIndex(it), tv[it]));
        customTimeVector->setTimeAtIndex(it, it);
        assert(doubleEquals(customTimeVector->getTimeAtIndex(it), it));
    }
}


void testWienerProcess() {
    info("testWienerProcess");
    using namespace irm;
    const double w0 = 0;
    const Time t0 = 0;
    const Time dT = 1e-3;
    const int numTimes = 1e3;
    WienerProcess process(ITimeVector::createUniform(t0, dT, numTimes), w0);
    StateVariable W(0);

    // f1 = w^2 - t
    auto f1_func = [&](double t, const IState & X) {
        double w = X.getValue(W);
        return w * w - t;
    };
    StateVariable f1 = process.addDerivedStateVariable(f1_func, w0 - t0);


    // df2 = 2WdW
    auto f2_drift = [](double, const IState &) -> double {
        return 0;
    };
    auto f2_vol = [&](double, const IState & X) -> double {
        double w = X.getValue(W);
        return 2 * w;
    };
    StateVariable f2 = process.addItoIntegralProcess(f2_drift, f2_vol, w0 - t0);

    // f3 = f1 - f2
    auto f3_func = [&](double, const IState & X) -> double {
        return X.getValue(f1) - X.getValue(f2);
    };
    StateVariable f3 = process.addDerivedStateVariable(f3_func, 0);


    // generate paths and compute avg values of f1 and f2
    const int numPaths = 1e2;
    int seed;
    std::default_random_engine dre(1984);
    double f3Sum = 0;
    for (int ipath = 0; ipath < numPaths; ++ipath) {
        auto path = process.generatePath(dre);
        const IState & finalState = path->getStateAtIndex(numTimes - 1);
        double f3Val = finalState.getValue(f3);
        f3Sum += f3Val;
        // info(f3Val);
    }
    double f3Avg = f3Sum / numPaths;
    // info("f3Avg = " << f3Avg);
    assert(std::abs(f3Avg) < 1e-1);

} // end function testWienerProcess