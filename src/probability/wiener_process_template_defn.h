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

#ifndef INTEREST_RATE_MODELLING_WIENER_PROCESS_TEMPLATE_DEFN_H
#define INTEREST_RATE_MODELLING_WIENER_PROCESS_TEMPLATE_DEFN_H

#include "wiener_process.h"

#include <random>



namespace irm {


    template<typename RandomNumberGenerator>
    IPathCPtr WienerProcess::generatePath(RandomNumberGenerator & rng) const
    {
        std::normal_distribution nd;
        std::vector<double> brownianSample;
        int numBrownianSamples = getRequiredNumberOfSamples();
        brownianSample.reserve(numBrownianSamples);
        for (int i = 0; i < numBrownianSamples; ++i)
            brownianSample.push_back(nd(rng));
        return generatePath(std::move(brownianSample));
    }


} // end namespace irm

#endif //INTEREST_RATE_MODELLING_WIENER_PROCESS_TEMPLATE_DEFN_H
