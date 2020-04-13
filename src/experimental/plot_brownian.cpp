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

#include "plot_brownian.h"

#include <matplotlibcpp.h>
namespace plt = matplotlibcpp;

#include <probability/time.h>
#include <probability/wiener_process.h>
#include <probability/state.h>
#include <probability/path.h>

#include <sstream>

namespace irm {

  // register
  bool regsterPlotBrownian = irm::IExperiment::registerExperiment("PlotBrownian", std::make_shared<PlotBrownian const>());

  int PlotBrownian::run(int, const char **) const 
  {
    const Time t0 = 0;
    const int numTimes = 100;
    const double timeStep = .03;
    const double initialW = 0;
    const double mu = 1;
    const double sigma = .2;
    const int seed = 624;
    const int numPaths = 5;

    auto timeVector = ITimeVector::createUniform(t0, timeStep, numTimes);
    WienerProcess process(timeVector, initialW);
    StateVariable W(0);

    auto gbf = 
      [=](Time t, double w)
      {
        double drift = mu - .5*sigma*sigma;
        return std::exp(drift*t + sigma*w);
      };
    auto geometricBrownianDefn = 
      [=](Time t, const IState & state)
      {
        double w = state.getValue(W);
        return gbf(t, w);
      };
    StateVariable IGB(0);
    auto igbDrift =
      [&](Time, const IState & state) {
        double igbPrev = state.getValue(IGB);
        return igbPrev * mu;
      };
    auto igbVol =
      [&](Time, const IState & state) {
        double igbPrev = state.getValue(IGB);
        return igbPrev * sigma;
      };

    StateVariable GB = process.addDerivedStateVariable(geometricBrownianDefn, gbf(t0, initialW));
    IGB = process.addItoIntegralProcess(igbDrift, igbVol, gbf(t0, initialW));

    std::default_random_engine dre(seed);
    std::vector<double> x, y;
    x.reserve(timeVector->getNumTimes());
    for (int i = 0; i < timeVector->getNumTimes(); ++i)
      x.push_back(timeVector->getTimeAtIndex(i));

    for (int ipath = 0; ipath < numPaths; ++ipath)
    {
      auto path = process.generatePath(dre);

      y.resize(path->getNumTimes(), 0);
      for (int i = 0; i < path->getNumTimes(); ++i)
        y[i] = path->getStateAtIndex(i).getValue(GB);
      std::stringstream gbname;
      gbname << "GB_" << ipath;
      plt::named_plot(gbname.str(), x, y);

      for (int i = 0; i < path->getNumTimes(); ++i)
        y[i] = path->getStateAtIndex(i).getValue(IGB);
      std::stringstream igbname;
      igbname << "IGB_" << ipath;
      plt::named_plot(igbname.str(), x, y);

    }
    plt::title("Geometric Brownian (GB) and Ito GB sample paths");
    plt::legend();
    plt::show();

    //for (int i = 0; i < x.size() && i < y.size(); ++i)
    //  std::cout << x[i] << ", " << y[i] << "\n";
    //std::cout << std::endl;

    return 0;
  }


} // end namespace irm
