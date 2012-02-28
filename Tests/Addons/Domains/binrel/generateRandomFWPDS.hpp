// ::wali::wpds::fwpds
#include "wali/wpds/fwpds/FWPDS.hpp"

// ::wali
#include "wali/Key.hpp"
#include "wali/SemElem.hpp"

// ::std
#include <string>
#include <vector>
#include <fstream>

using namespace wali::wpds::fwpds;
using namespace wali::wpds;

typedef wali::sem_elem_t (* RANDOMWT) ();

/**
  * Used to pass configuration parameters to the random FWPDS generation
  * function.
  * @see randfwpds(...)
  */
struct Conf{
  //! pointer to a function that generates random weights
  RANDOMWT randomWt;
  //! The number of procedures to be generated
  int numprocs;
  //! The number of callsites to be generated
  int numcalls;
  //! The number of cfg nodes to be generated [loosely followed]
  int numnodes;
  //! The number of two-way splits to be generated.
  int numsplits;
  //! Number of error points to be generated
  int numerrs;
};

/**
  * Used to return the important names to the function  calling randfwpds(...)
  */
struct Names{
  wali::Key pdsState;
  wali::Key *entries;
  wali::Key *exits;
  std::vector< wali::Key > errs;
};

/**
  * Generate a random FWPDS with the configuration given by conf
  * @detail ...
  * @see struct Conf
  **/
void randfwpds(FWPDS& pds, const Conf& conf, Names& names, 
    std::ostream *o = NULL, float pCall = 0.45, float pSplit = 0.45);

