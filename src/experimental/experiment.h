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

#include <memory>
#include <string>
#include <map>

namespace irm {

  class IExperiment {
    public:

      typedef std::shared_ptr<const IExperiment> CPtr;

      virtual int run(int argc, const char ** argv) const = 0;
      virtual void printHelp() const = 0;

      static CPtr findExperiment(const std::string & experimentName);
      static bool registerExperiment(const std::string & experimentName, const CPtr & expt);
      static void printHelpStatic();

    private:
      typedef std::map<std::string, CPtr> ExptMap;
      typedef std::shared_ptr<ExptMap> ExptMapPtr;
      
      static ExptMapPtr getMap();

  };

} // end namespace irm
