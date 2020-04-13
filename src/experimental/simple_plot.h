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



#pragma once

#include "experiment.h"

#include <matplotlibcpp.h>
namespace plt = matplotlibcpp;

namespace irm {

  class SimplePlot : public IExperiment {
    public:
      int run(int argc, const char ** argv) const override {
        plt::plot({1,3,2,4});
        plt::show();
        return 0;
      }

      void printHelp() const override {
        std::cout << "A simple experiment to test integration with the matplotlibcpp library" << std::endl;
      }
  };

  // register
  bool regsterSimplePlot = irm::IExperiment::registerExperiment("SimplePlot", std::make_shared<SimplePlot const>());

} // end namespace irm

