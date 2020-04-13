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



#include <iostream>
#include <string>

#include "experiment.h"
#include "simple_plot.h"

namespace irm {
  // structure to hold the parsed command line options
  struct ExptCli 
  {
    public:
      std::string experiment; // which experiment to run
      int experimentArgc;     // number of arguments being sent to the experiment
      const char ** experimentArgv; // arguments being sent to the experiment

      // parse the command line options
      ExptCli(int argc, const char ** argv) 
        : experiment(),
          experimentArgc(0),
          experimentArgv(NULL)
      {
        for(int iarg = 1; iarg < argc; ++iarg)
        {
          std::string arg = argv[iarg];
          if (arg == "--name")
          {
            if (++iarg == argc)
              parsingError("Expecting experiment name after flag --name");
            experiment = argv[iarg];
          } 
          else if (arg == "--help")
          {
            printHelp();
            std::exit(0);
          } 
          else if(arg == "--")
          {
            experimentArgc = argc  - iarg - 1;
            experimentArgv = argv + iarg + 1;
            break;
          }
          else
          {
            parsingError("Unexpected flag '" + arg + "'");
          }
        }
      }

      static void printHelp()
      {
        std::cout << "Single project for running all experiments\n"
                  << "Usage:\n"
                  << "\texperiment --name <name of experiment> -- <options for the experiment>\n"
                  << "\texperiment --help\n\n\n"
                  << std::endl;
        IExperiment::printHelpStatic();
      }

      static void parsingError(const std::string & error)
      {
        std::cerr << "Parsing error: " << error << std::endl;
        printHelp();
        std::exit(1);
      }
  };

}

int main(int argc, const char ** argv)
{
  irm::ExptCli cliOptions(argc, argv);
  irm::IExperiment::CPtr experiment = irm::IExperiment::findExperiment(cliOptions.experiment);
  return experiment->run(cliOptions.experimentArgc, cliOptions.experimentArgv);
}
