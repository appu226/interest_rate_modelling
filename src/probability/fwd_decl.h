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


#ifndef INTEREST_RATE_MODELLING_FWD_DECL_H
#define INTEREST_RATE_MODELLING_FWD_DECL_H

#include <memory>

namespace irm {

    // path.h
    class IPath;
    typedef std::shared_ptr<const IPath> IPathCPtr;
    typedef std::shared_ptr<IPath> IPathPtr;

    // state.h
    class StateVariable;
    class IState;
    typedef std::shared_ptr<const IState> IStateCPtr;
    typedef std::shared_ptr<IState> IStatePtr;

    // time.h
    typedef double Time;
    class ITimeVector;
    typedef std::shared_ptr<const ITimeVector> ITimeVectorCPtr;
    typedef std::shared_ptr<ITimeVector> ITimeVectorPtr;

    // wiener_process.h
    class WienerProcess;
    typedef std::shared_ptr<const WienerProcess> WienerProcessCPtr;
    typedef std::shared_ptr<WienerProcess> WienerProcessPtr;

} // end namespace irm


#endif //INTEREST_RATE_MODELLING_FWD_DECL_H
