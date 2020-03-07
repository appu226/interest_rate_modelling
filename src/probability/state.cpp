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


#include "state.h"

namespace {

    using namespace irm;

    class StateFromVector: public IState {
    public:
        StateFromVector(std::vector<double> values) :
          m_values(std::move(values))
        { }

        int getNumValues() const override {
            return m_values.size();
        }

        double getValue(StateVariable x) const override {
            return m_values[x.index];
        }

        void setValue(StateVariable x, double value) override {
            m_values[x.index] = value;
        }

    private:
        std::vector<double> m_values;
    }; // end class StateFromVector

} // end anonymous namespace

namespace irm {
    IStatePtr IState::createZeroState(int stateSize){
        return std::make_shared<StateFromVector>(std::vector(stateSize, 0.0));
    }

    IStatePtr IState::createFromVector(std::vector<double> value) {
        return std::make_shared<StateFromVector>(std::move(value));
    }
} // end namespace irm