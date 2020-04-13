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



#include "experiment.h"

#include <iostream>

namespace irm {

  IExperiment::CPtr IExperiment::findExperiment(const std::string & exptName) {
    auto exptMap = getMap();
    auto expt = exptMap->find(exptName);
    if (expt == exptMap->end())
      throw std::runtime_error("Could not find experiment by the name '" + exptName + "'");
    else
      return expt->second;
  }

  bool IExperiment::registerExperiment(const std::string & name, const CPtr & expt) {
    ExptMapPtr exptMap = getMap();
    exptMap->insert(std::make_pair(name, expt));
    return true;
  }

  void IExperiment::printHelpStatic()
  {
    ExptMapPtr exptMap = getMap();
    std::cout << "Help for all available experiments:\n\n\n";
    for (auto exptIt = exptMap->cbegin(); exptIt != exptMap->cend(); ++exptIt)
    {
      std::cout << exptIt->first << "\n";
      exptIt->second->printHelp();
      std::cout << "\n\n\n";
    }
  }

  IExperiment::ExptMapPtr IExperiment::getMap() {
    static ExptMapPtr theMap(new ExptMap());
    return theMap;
  }

} // end namespace irm
