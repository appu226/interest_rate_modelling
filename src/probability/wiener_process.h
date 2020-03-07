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


#ifndef INTEREST_RATE_MODELLING_WIENER_PROCESS_H
#define INTEREST_RATE_MODELLING_WIENER_PROCESS_H

#include "fwd_decl.h"

#include <vector>
#include <functional>

namespace irm {


    /**
     * class WienerProcess
     * Describes a wiener process and all its derived variables.
     * User can add a definition for a derived random variable, and get back an index.
     * The index is the position of the value of the derived random variable in a generate state.
     * A derived random variable can depend on previously defined random variables using their indices.
     * A derived random variable can also be an Ito process.
     */
    class WienerProcess {
    public:

        /** Constructor
         *
         * @param timeVector The time points in the generate state space.
         * @param initialValue The initial value of the wiener process.
         */
        WienerProcess(ITimeVectorCPtr timeVector, double initialValue);

        /**
         * StateFunction: T x \Omega -> \Re
         */
        typedef std::function< double( Time, const IState & ) > StateFunction;


        /** Function to add a state variable whose value is defined by other variables in the current state.
         * A derived state variable is a random variable that is a function of
         * previously defined random variables in the state.
         * @param variableDefinition Function from currently generated state to the value of the variable.
         * @param initialValue The initial value of the variable at the start of all paths.
         * @return Returns the index of the newly defined random variable in the state space.
         */
        StateVariable addDerivedStateVariable(StateFunction variableDefinition, double initialValue);

        /** Function to add an Ito process X defined as [ dX   =   dtMultipler * dt   +   dwMultiplier * dW ]
         *
         * @param drift The nultiplier to dt, defined on the previous state
         * @param volatility  The multiplier to dW, defined on the previous state
         * @param initialValue The initial value of the variable at the start of all paths.
         * @return Returns the index of the newly defined random variable in the generate state space.
         */
        StateVariable addItoIntegralProcess(StateFunction drift, StateFunction volatility, double initialValue);

        /**
         * Function to generate a single path of all the random variable in the state
         * @tparam RandomNumberGenerator The type of the random number generator
         * @param randomNumberGenerator The random number generator (eg. std::default_random_engine) for generating the path
         * @return Returns a path, which an indexed set states.
         *         The index of each state in the path
         *           will match the index of the corresponding time point
         *           provided in the constructor of the WienerProcess.
         *         The index of each value in the state
         *           will match the index of the random variable
         *           added to the WienerProcess.
         */
        template<typename RandomNumberGenerator>
        IPathCPtr generatePath(RandomNumberGenerator & randomNumberGenerator) const;

    private:


        // helper struct
        struct StateVariableDefn {
            StateFunction currentStateFunction;
            StateFunction drift;
            StateFunction volatility;
        };
        typedef std::shared_ptr<const StateVariableDefn> StateVariableDefnCPtr;


        // helper functions
        int getRequiredNumberOfSamples() const;
        IPathCPtr generatePath(std::vector<double> brownianSamples) const;


        // member variables
        ITimeVectorCPtr m_timeVector;
        std::vector<double> m_initialValue;
        std::vector<StateVariableDefnCPtr> m_stateVariableDefns;
    }; // end class WienerStateSpace



} // end namespace irm


#include "wiener_process_template_defn.h"


#endif //INTEREST_RATE_MODELLING_WIENER_PROCESS_H
