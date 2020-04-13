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


#include "wiener_process.h"

#include "state.h"
#include "time.h"
#include "path.h"

namespace irm {



    WienerProcess::WienerProcess(
            ITimeVectorCPtr timeVector,
            double initialValue):
            m_timeVector(timeVector),
            m_initialValue(1, initialValue),
            m_stateVariableDefns()
    { }



    StateVariable WienerProcess::addDerivedStateVariable(
            StateFunction variableDefinition,
            double initialValue)
    {
        StateVariable nextIndex(m_initialValue.size());
        m_initialValue.push_back(initialValue);
        auto svd = std::make_shared<StateVariableDefn>();
        svd->currentStateFunction = variableDefinition;
        m_stateVariableDefns.push_back(svd);
        return nextIndex;
    }

    StateVariable WienerProcess::addItoIntegralProcess(
            StateFunction drift,
            StateFunction volatility,
            double initialValue)
    {
        StateVariable nextIndex(m_initialValue.size());
        m_initialValue.push_back(initialValue);
        auto svd = std::make_shared<StateVariableDefn>();
        svd->drift = drift;
        svd->volatility = volatility;
        m_stateVariableDefns.push_back(svd);
        return nextIndex;
    }


    int WienerProcess::getRequiredNumberOfSamples() const {
        return m_timeVector->getNumTimes() - 1;
    }

    IPathCPtr WienerProcess::generatePath(std::vector<double> brownianSample) const {
        // generate empty path
        auto path = IPath::createZeroPath(m_timeVector, m_initialValue.size());

        // set initial state
        int stateSize = m_initialValue.size();
        for (int i = 0; i < stateSize; ++i)
            path->getStateAtIndex(0).setValue(StateVariable(i), m_initialValue[i]);


        // loop over time incrementally to generate the rest of the path
        int numTimes = m_timeVector->getNumTimes();
        Time t = (numTimes > 0 ? m_timeVector->getTimeAtIndex(0) : 0);
        Time tprev = t;
        StateVariable xW(0);
        for (int it = 1; it < numTimes; ++it)
        {
            // advance the state
            const IState & prevState = path->getStateAtIndex(it - 1);
            IState & curState = path->getStateAtIndex(it);

            // get the next wiener value
            tprev = t;
            t = m_timeVector->getTimeAtIndex(it);
            Time dt = t - tprev;
            int ibrownian = it - 1;
            double dW = brownianSample[ibrownian] *  std::sqrt(dt);
            curState.setValue(xW, prevState.getValue(xW) + dW);

            // compute all the derived variables
            int nsvd = m_stateVariableDefns.size();
            for (int isvd = 0; isvd < nsvd; ++isvd)
            {
                StateVariable x = StateVariable(isvd + 1);
                auto svd = m_stateVariableDefns.at(isvd);

                // variable is a function of the current state
                if (svd->currentStateFunction)
                    curState.setValue(x, svd->currentStateFunction(t, curState));

                // variable is an ito process
                // incrementing on the previous state value
                // based on the drift and volatility
                else if (svd->drift || svd->volatility){
                    double prevValue = prevState.getValue(x);
                    double driftIncrement = 0;
                    double volIncrement = 0;
                    if (svd->drift) {
                        driftIncrement = dt * svd->drift(t, prevState);
                    }
                    if (svd->volatility) {
                        volIncrement = dW * svd->volatility(t, prevState);
                    }
                    curState.setValue(x, prevValue + driftIncrement + volIncrement);
                }
            }

        }

        return path;
    }


}
