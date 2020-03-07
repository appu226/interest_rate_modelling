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

#include "path.h"

#include "time.h"
#include "state.h"


namespace {

    using namespace irm;

    class PathFromStateVector : public IPath {
    public:

        PathFromStateVector(
                ITimeVectorCPtr timeVector,
                int stateSize,
                std::vector<IStatePtr> stateVector) :
                m_timeVector(timeVector),
                m_stateSize(stateSize),
                m_stateVector(std::move(stateVector))
        { }

        int getNumTimes() const override {
            return m_timeVector->getNumTimes();
        }

        int getStateSize() const override {
            return m_stateSize;
        }

        Time getTimeAtIndex(int timeIndex) const override {
            return m_timeVector->getTimeAtIndex(timeIndex);
        }

        const IState & getStateAtIndex(int timeIndex) const override {
            return *m_stateVector.at(timeIndex);
        }

        IState & getStateAtIndex(int timeIndex) override {
            return *m_stateVector.at(timeIndex);
        }

    private:
        ITimeVectorCPtr m_timeVector;
        int m_stateSize;
        std::vector<IStatePtr> m_stateVector;
    }; // end class PathVectorImpl

} // end anonymous namespace


namespace irm {

    IPathPtr IPath::createZeroPath(ITimeVectorCPtr timeVector, int stateSize) {
        int numTimes = timeVector->getNumTimes();
        std::vector<IStatePtr> stateVector;
        stateVector.reserve(numTimes);
        for (int i = 0; i < numTimes; ++i)
            stateVector.push_back(IState::createZeroState(stateSize));
        return std::make_shared<PathFromStateVector>(timeVector, stateSize, stateVector);
    }

} // end namespace irm