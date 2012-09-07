#include "wali/graph/NewtonLogger.hpp"


using namespace std;
using namespace wali::graph;

NewtonLogger::NewtonLogger(string n) :
  name(n),
  newtonSolverTime(0),
  sccCount(0),
  sccComputationTime(-1),

  nExtendGraphCreation(0),
  nCombineGraphCreation(0),
  nTensorGraphCreation(0),
  nDetensorGraphCreation(0),
  nTransposeGraphCreation(0),

  nExtendSaturation(0),
  nCombineSaturation(0),
  nTensorSaturation(0),
  nDetensorSaturation(0),
  nTransposeSaturation(0),

  nExtendRest(0),
  nCombineRest(0),
  nTensorRest(0),
  nDetensorRest(0),
  nTransposeRest(0),
  
  inGraphCreation(false),
  inSaturation(false)
{}

void NewtonLogger::printLog()
{
  double acc;
  int acci;

  cout << name << endl << "======================" << endl;
  cout << "Newton stats: " << endl;
  cout << "Total time: " << newtonSolverTime << endl;
  cout << "Total scc computation time: " << sccComputationTime << endl;
  
  acc = 0;
  for(unsigned i=0; i < intraGraphCreationTimes.size(); ++i)
    acc += intraGraphCreationTimes[i];
  cout << "Total IntraGraph creation time: " << acc << endl;

  acc = 0;
  for(unsigned i=0; i < tarjanTimes.size(); ++i)
    acc += tarjanTimes[i];
  cout << "Total RegExp creation time: " << acc << endl;

  acci = 0;
  for(unsigned i=0; i < numNewtonSteps.size(); ++i)
    acci += numNewtonSteps[i];
  cout << "Total number of Newton steps: " << acci << endl;

  cout << "Newton steps per scc: ";
  for(unsigned i=0; i < numNewtonSteps.size(); ++i)
    cout << numNewtonSteps[i] << " ";
  cout << endl;

  acc = 0;
  for(unsigned i=0; i < newtonSolverTimes.size(); ++i)
    acc += newtonSolverTimes[i];
  cout << "Total Newton solver time: " << acc << endl;

  acc = 0;
  for(unsigned i=0; i < evaluateRootsTimes.size(); ++i)
    acc += evaluateRootsTimes[i];
  cout << "Total RegExp evaluation time: " << acc << endl;

  acc = 0;
  for(unsigned i=0; i < findChangedNodesTimes.size(); ++i)
    acc += findChangedNodesTimes[i];
  cout << "Total changed nodes search time: " << acc << endl;

  acc = 0;
  for(unsigned i=0; i < findChangedEdgesTimes.size(); ++i)
    acc += findChangedEdgesTimes[i];
  cout << "Total changed edges search time: " << acc << endl;

  acc = 0;
  for(unsigned i=0; i < updateEdgesTimes.size(); ++i)
    acc += updateEdgesTimes[i];
  cout << "Total edge update time: " << acc << endl;

  acc = 0;
  for(unsigned i=0; i < miscTimes.size(); ++i)
    acc += miscTimes[i];
  cout << "Total miscellaneous measured time: " << acc << endl;

  cout << "#######################################" << endl;
}
