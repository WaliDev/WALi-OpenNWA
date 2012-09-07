#ifndef wali_graph__NEWTON_LOGGER_H
#define wali_graph__NEWTON_LOGGER_H


#include "wali/ref_ptr.hpp"
#include "wali/Countable.hpp"
#include "wali/util/Timer.hpp"

#include <vector>
#include <string>

// Switch on logging in newton solver
#define NEWTON_LOGGING 1

namespace wali 
{
  namespace graph
  {

    struct NewtonLogger : public Countable
    {
      //Timers used as needed
      std::string name;
      util::Timer *t1, *t2, *t3, *t4, *t5, *t6, *t7;

      double newtonSolverTime;
      int sccCount;
      double sccComputationTime;
      std::vector<double> sccTimes;
      std::vector<double> intraGraphCreationTimes;
      std::vector<double> tarjanTimes;
      std::vector<double> newtonSolverTimes;

      std::vector<int> numNewtonSteps;
      std::vector<double> newtonStepTimes;
      std::vector<double> evaluateRootsTimes;
      std::vector<double> findChangedNodesTimes;
      std::vector<double> findChangedEdgesTimes;
      std::vector<double> updateEdgesTimes;
      std::vector<double> miscTimes;

      int nExtendGraphCreation;
      int nCombineGraphCreation;
      int nTensorGraphCreation;
      int nDetensorGraphCreation;
      int nTransposeGraphCreation;

      int nExtendSaturation;
      int nCombineSaturation;
      int nTensorSaturation;
      int nDetensorSaturation;
      int nTransposeSaturation;

      int nExtendRest;
      int nCombineRest;
      int nTensorRest;
      int nDetensorRest;
      int nTransposeRest;

      //internal
      bool oldMeasureAndReport;
      bool inGraphCreation;
      bool inSaturation;

      NewtonLogger(std::string name);
      void printLog();
    };

    typedef ref_ptr<struct NewtonLogger> newton_logger_t;

#ifdef NEWTON_LOGGING

#define BEGIN_NEWTON_SOLVER(obj) \
    if(!(obj == NULL)) { \
      obj->sccCount = -1; \
      obj->oldMeasureAndReport = util::Timer::measureAndReport; \
      util::Timer::measureAndReport = false; \
      obj->t1 = new util::Timer("", std::cout); \
    }

#define END_NEWTON_SOLVER(obj) \
    if(!(obj == NULL)) { \
      obj->newtonSolverTime = obj->t1->elapsed(); \
      util::Timer::measureAndReport = obj->oldMeasureAndReport; \
      delete obj->t1; \
    }

#define BEGIN_SCC_COMPUTATION(obj) \
    if(!(obj == NULL)) { \
      obj->t2 = new util::Timer("", std::cout); \
    }

#define END_SCC_COMPUTATION(obj, num_scc) \
    if(!(obj == NULL)) { \
      obj->sccComputationTime = obj->t2->elapsed(); \
      delete obj->t2; \
      obj->sccCount = num_scc; \
    }

#define BEGIN_SCC_SOLVER(obj) \
    if(!(obj == NULL)) { \
      obj->t2 = new util::Timer("", std::cout); \
    }

#define END_SCC_SOLVER(obj) \
    if(!(obj == NULL)) { \
      obj->sccTimes.push_back(obj->t2->elapsed()); \
      delete obj->t2; \
    }

#define BEGIN_INTRAGRAPH_CREATION(obj) \
    if(!(obj == NULL)) { \
      obj->inGraphCreation = true; \
      obj->t3 = new util::Timer("", std::cout); \
    }

#define END_INTRAGRAPH_CREATION(obj) \
    if(!(obj == NULL)) { \
      obj->inGraphCreation = false; \
      obj->intraGraphCreationTimes.push_back(obj->t3->elapsed()); \
      delete obj->t3; \
    }

#define BEGIN_TARJAN(obj) \
    if(!(obj == NULL)) { \
      obj->t3 = new util::Timer("", std::cout); \
    }

#define END_TARJAN(obj) \
    if(!(obj == NULL)) { \
      obj->tarjanTimes.push_back(obj->t3->elapsed()); \
      delete obj->t3; \
    }

#define BEGIN_NEWTON_SOLUTION(obj) \
    if(!(obj == NULL)) { \
      obj->inSaturation = true; \
      obj->t3 = new util::Timer("", std::cout); \
      obj->numNewtonSteps.push_back(0); \
    }

#define END_NEWTON_SOLUTION(obj) \
    if(!(obj == NULL)) { \
      obj->inSaturation = false; \
      obj->newtonSolverTimes.push_back(obj->t3->elapsed()); \
      delete obj->t3; \
    }


#define BEGIN_NEWTON_STEP(obj) \
    if(!(obj == NULL)) { \
      obj->t4 = new util::Timer("", std::cout); \
      obj->numNewtonSteps[obj->numNewtonSteps.size() - 1] = obj->numNewtonSteps[obj->numNewtonSteps.size() - 1] + 1; \
    }

#define END_NEWTON_STEP(obj) \
    if(!(obj == NULL)) { \
      obj->newtonStepTimes.push_back(obj->t4->elapsed()); \
      delete obj->t4; \
    }

#define BEGIN_EVALUATE_ROOTS(obj) \
    if(!(obj == NULL)) { \
      obj->t5 = new util::Timer("", std::cout); \
    }

#define END_EVALUATE_ROOTS(obj) \
    if(!(obj == NULL)) { \
      obj->evaluateRootsTimes.push_back(obj->t5->elapsed()); \
      delete obj->t5; \
    }


#define BEGIN_FIND_CHANGED_NODES(obj) \
    if(!(obj == NULL)) { \
      obj->t5 = new util::Timer("", std::cout); \
    }

#define END_FIND_CHANGED_NODES(obj) \
    if(!(obj == NULL)) { \
      obj->findChangedNodesTimes.push_back(obj->t5->elapsed()); \
      delete obj->t5; \
    }

#define BEGIN_FIND_CHANGED_EDGES(obj) \
    if(!(obj == NULL)) { \
      obj->t5 = new util::Timer("", std::cout); \
    }

#define END_FIND_CHANGED_EDGES(obj) \
    if(!(obj == NULL)) { \
      obj->findChangedEdgesTimes.push_back(obj->t5->elapsed()); \
      delete obj->t5; \
    }

#define BEGIN_UPDATE_EDGES(obj) \
    if(!(obj == NULL)) { \
      obj->t5 = new util::Timer("", std::cout); \
    }

#define END_UPDATE_EDGES(obj) \
    if(!(obj == NULL)) { \
      obj->updateEdgesTimes.push_back(obj->t5->elapsed()); \
      delete obj->t5; \
    }

#define BEGIN_MISC(obj) \
    if(!(obj == NULL)) { \
      obj->t6 = new util::Timer("", std::cout); \
    }

#define END_MISC(obj) \
    if(!(obj == NULL)) { \
      obj->miscTimes.push_back(obj->t6->elapsed()); \
      delete obj->t6; \
    }

#define EXTEND(obj) \
    if(!(obj == NULL)) { \
      if(obj->inGraphCreation) \
        nExtendGraphCreation++; \
      else if(obj->inSaturation) \
        nExtendSaturation++; \
      else \
        nExtendRest++; \
    }

#define COMBINE(obj) \
    if(!(obj == NULL)) { \
      if(obj->inGraphCreation) \
        nCombineGraphCreation++; \
      else if(obj->inSaturation) \
        nCombineSaturation++; \
      else \
        nCombineRest++; \
    }

#define TENSOR(obj) \
    if(!(obj == NULL)) { \
      if(obj->inGraphCreation) \
        nTensorGraphCreation++; \
      else if(obj->inSaturation) \
        nTensorSaturation++; \
      else \
        nTensorRest++; \
    }

#define DETENSOR(obj) \
    if(!(obj == NULL)) { \
      if(obj->inGraphCreation) \
        nDetensorGraphCreation++; \
      else if(obj->inSaturation) \
        nDetensorSaturation++; \
      else \
        nDetensorRest++; \
    }

#define TRANSPOSE(obj) \
    if(!(obj == NULL)) { \
      if(obj->inGraphCreation) \
        nTransposeGraphCreation++; \
      else if(obj->inSaturation) \
        nTransposeSaturation++; \
      else \
        nTransposeRest++; \
    }


#define PRINT_NEWTON_LOG(obj) \
    if(!(obj == NULL)) { \
      obj->printLog();\
    }

#else //#ifdef NEWTON_LOGGING
#define SKIP 

#define BEGIN_NEWTON_SOLVER(obj) SKIP
#define END_NEWTON_SOLVER(obj) SKIP
#define BEGIN_SCC_COMPUTATION(obj) SKIP
#define END_SCC_COMPUTATION(obj, num_scc) SKIP
#define BEGIN_SCC_SOLVER(obj) SKIP
#define END_SCC_SOLVER(obj) SKIP
#define BEGIN_INTRAGRAPH_CREATION(obj) SKIP
#define END_INTRAGRAPH_CREATION(obj) SKIP
#define BEGIN_TARJAN(obj) SKIP
#define END_TARJAN(obj) SKIP
#define BEGIN_NEWTON_SOLUTION(obj) SKIP
#define END_NEWTON_SOLUTION(obj) SKIP

#define BEGIN_NEWTON_STEP(obj) SKIP
#define END_NEWTON_STEP(obj) SKIP
#define BEGIN_EVALUATE_ROOTS(obj) SKIP
#define END_EVALUATE_ROOTS(obj) SKIP
#define BEGIN_FIND_CHANGED_NODES(obj) SKIP
#define END_FIND_CHANGED_NODES(obj) SKIP
#define BEGIN_FIND_CHANGED_EDGES(obj) SKIP
#define END_FIND_CHANGED_EDGES(obj) SKIP
#define BEGIN_UPDATE_EDGES(obj) SKIP
#define END_UPDATE_EDGES(obj) SKIP
#define BEGIN_MISC(obj) SKIP
#define END_MISC(obj) SKIP

#define EXTEND SKIP
#define COMBINE SKIP
#define TENSOR SKIP
#define DETENSOR SKIP
#define TRANSPOSE SKIP

#define PRINT_NEWTON_LOG(obj) SKIP
#endif //#ifdef NEWTON_LOGGING

  }
}


#endif //#ifdef wali_graph__NEWTON_LOGGER_H
