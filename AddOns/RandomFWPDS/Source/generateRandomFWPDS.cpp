// ::wali::wpds::fwpds
#include "wali/wpds/fwpds/FWPDS.hpp"

// ::wali
#include "wali/Key.hpp"
#include "wali/KeySpace.hpp"
#include "wali/SemElem.hpp"
#include "wali/MergeFn.hpp"

// ::std
#include <string>
#include <vector>
#include <cstdlib>

#include "generateRandomFWPDS.hpp"

using namespace wali::wpds::fwpds;
using namespace wali::wpds;


// Data
// Array that stores keys for all procedure entries.
static wali::Key * entries;
// Array that stores keys for all procedure exits.
static wali::Key * exits;
// Name of the pds state to use
static wali::Key pdsState;

static int curKey;
static int pCall;
static int pSplit;
// end::Data

Names::Names()
{
  entries = NULL;
  exits = NULL;
}
Names::~Names()
{
  delete [] entries;
  delete [] exits;
}
// /Forward declaration of functions.
static void genproc(FWPDS& pds, const Conf& conf, int procnum, int remNodes, int
    remSplits, int remCalls, std::ostream *o, int tabstop);
static wali::Key genblock(FWPDS& pds, const Conf& conf, wali::Key curhead, int
    remNodes, int remSplits, int remCalls, std::ostream *o, int tabstop);

/**
  * Generate a random FWPDS with the configuration given by conf
  * @detail ...
  * @see struct Conf
  **/
void randfwpds(FWPDS& pds, const Conf& conf, Names& names, 
    std::ostream *o, double _pCall, double _pSplit)
{
  srand(time(NULL));
  //For debugging
  //srand(1);

  wali::clearKeyspace();
  curKey=0;
  pCall = (int)(_pCall * 100);
  pSplit = (int)(_pSplit * 100);

  pdsState = wali::getKeySpace()->getKey(curKey++);
  //Generate Keys for all procedure entries and exits.  
  entries = new wali::Key[conf.numprocs];
  exits = new wali::Key[conf.numprocs];
  for(int i=0;i<conf.numprocs; ++i){
    entries[i] = wali::getKeySpace()->getKey(curKey++);
    exits[i] = wali::getKeySpace()->getKey(curKey++);
  }


  if(o)
    *o << "#######################################\n";
  int baseNodes = (int) conf.numnodes / (conf.numprocs+3);
  int baseSplits = (int) conf.numsplits / (conf.numprocs+3);
  int baseCalls = (int) conf.numcalls / (conf.numprocs+3);
  int remNodes = conf.numnodes - baseNodes*conf.numprocs;
  int remSplits = conf.numsplits - baseSplits*conf.numprocs;
  int remCalls = conf.numcalls - baseCalls*conf.numprocs;
  for(int i=0; i < conf.numprocs; ++i){
    int nodes = rand() % remNodes + 1;
    int calls = rand() % remCalls;
    int splits = rand() % remSplits;
    remNodes -= nodes;
    if(remNodes <= 0) remNodes = 1;
    remCalls -= calls;
    remSplits -= splits;
    genproc(pds,conf,i,baseNodes+nodes,baseSplits+splits,baseCalls+calls,o,2);
    if(o)
      *o << "*******************************\n";
  }

  names.pdsState = pdsState;
  names.entries = new wali::Key[conf.numprocs];
  names.exits = new wali::Key[conf.numprocs];
  for(int i=0;i<conf.numprocs;++i){
    names.entries[i] = entries[i];
    names.exits[i] = exits[i];
  }

  delete [] entries;
  delete [] exits;

  //TODO: What about errors tags?
}

static void genproc(FWPDS& pds, const Conf& conf, int procnum, int remNodes, int
    remSplits, int remCalls, std::ostream *o, int tabspace)
{
  if(remCalls < 1) remCalls = 1;
  if(remNodes < 1) remNodes = 1;
  if(remSplits < 1) remSplits = 1;

  //In order to keep modulus related code clean,
  //bump values up by one, and stop at 1
  remCalls++;
  remNodes++;
  remSplits++;
  wali::Key curhead = entries[procnum];

  //remNodes +=3; //To avoid corner casees.
  if(o){
    *o << "proc[" << procnum << "(entry:"<< entries[procnum] 
      <<", exit:"<<exits[procnum] <<")]:\n";
    *o << "DEBUG: remNodes:" << remNodes-1 << " remSplits:" 
      << remSplits-1 << " remCalls:" << remCalls-1 << "\n";
  }
  while(remNodes > 1 || remCalls > 1 || remSplits > 1){
    int nodes,calls,splits;
    if(remNodes < 1)
      nodes = 0;
    else 
      nodes = rand() % remNodes;
    if(remCalls < 1)
      calls = 0;
    else
      calls = rand() % remCalls;
    if(remSplits < 1)
      splits = 0;
    else
      splits = rand() % remSplits;
    remNodes -= nodes;
    remCalls -= calls;
    remSplits -= splits;
    curhead = genblock(pds,conf,curhead,nodes,splits,calls,o,tabspace+2);
  }
  //The last edge to the exit node
  pds.add_rule(
      pdsState,
      curhead,
      pdsState,
      exits[procnum],
      conf.randomWt()
    );
  if(o){
    for(int j=0;j<tabspace+2;++j)
      *o<< " ";
    *o << wali::key2str(curhead) << "\n";
    for(int j=0;j<tabspace+2;++j)
      *o<< " ";
    *o << wali::key2str(exits[procnum]) << "\n";
  }
  //We add an edge from start to end, to make sure 
  //there is a path through the function without 
  //further calls
  pds.add_rule(
      pdsState,
      entries[procnum],
      pdsState,
      exits[procnum],
      conf.randomWt()
    );
  if(o){
    for(int j=0;j<tabspace;++j)
      *o<< " ";
    *o << "Default branch: " << wali::key2str(entries[procnum]) <<
      "->" << wali::key2str(exits[procnum]) << "\n";
  }
  //Return from the functions, except for the first function
  if(procnum != 0){
  pds.add_rule(
      pdsState,
      exits[procnum],
      pdsState,
      conf.randomWt()
    );  
  }
}

static wali::Key genblock(FWPDS& pds, const Conf& conf, wali::Key curhead, int
    remNodes, int remSplits, int remCalls, std::ostream *o, int tabspace)
{

  wali::Key endhead,endhead1,endhead2,nexthead;
  int nodes1,nodes2,splits1,splits2,calls1,calls2,fn;
  int choice;
  while(remNodes > 0 || remCalls > 0 || remSplits > 0){
    choice = rand() % 100;
    if(choice < pCall){
      if(remCalls < 1) continue;
      nexthead = wali::getKeySpace()->getKey(curKey++);
      fn = rand() % conf.numprocs;
      if(o){
        for(int j=0;j<tabspace;++j)
          *o<< " ";
        *o << wali::key2str(curhead) << "[CALL TO PROC(" << fn << ")[" << entries[fn] <<"]" << "\n";
      }
      pds.add_rule(
          pdsState,
          curhead,
          pdsState,
          entries[fn],
          nexthead,
          conf.randomWt(),
          new wali::MergeFn(conf.randomWt())
          );
      curhead = nexthead;
      remCalls--;
    }else if(choice < pCall + pSplit){
      if(remSplits < 1) continue;
      remSplits--;

      if(remCalls < 1){
        calls1 = calls2 = 0;
      }else{
        calls1 = rand() % remCalls;
        remCalls -= calls1;
        if(remCalls < 1)
          calls2 = 0;
        else
          calls2 = rand() % remCalls;
      }
      remCalls -= calls2;

      if(remNodes < 1){
        nodes1 = nodes2 = 0;
      }else{
        nodes1 = rand() % remNodes;
        remNodes -= nodes1;
        if(remNodes < 1)
          nodes2 = 0;
        else
          nodes2 = rand() % remNodes;
      }
      remNodes -= nodes2;

      if(remSplits < 1){
        splits1 = splits2 = 0;
      }else{
        splits1 = rand() % remSplits;
        remSplits -= splits1;
        if(remSplits < 1)
          splits2 = 0;
        else
          splits2 = rand() % remSplits;
      }
      remSplits -= splits2;

      if(o){
        for(int j=0;j<tabspace;++j)
          *o<< " ";
        *o << wali::key2str(curhead) << "[BRANCH 1]" << "\n";
      }
      nexthead = wali::getKeySpace()->getKey(curKey++);
      pds.add_rule(
          pdsState,
          curhead,
          pdsState,
          nexthead,
          conf.randomWt()
          );
      endhead1 = genblock(pds,conf,nexthead,nodes1,splits1,calls1,o,tabspace+2);
      if(o){
        for(int j=0;j<tabspace+2;++j)
          *o<< " ";
        *o << wali::key2str(endhead1)<< "\n";
      }

      if(o){
        for(int j=0;j<tabspace;++j)
          *o<< " ";
        *o << wali::key2str(curhead) << "[BRANCH 2]" << "\n";
      }
      nexthead = wali::getKeySpace()->getKey(curKey++);
      pds.add_rule(
          pdsState,
          curhead,
          pdsState,
          nexthead,
          conf.randomWt()
          );
      endhead2 = genblock(pds,conf,nexthead,nodes1,splits1,calls1,o,tabspace+2);
      if(o){
        for(int j=0;j<tabspace+2;++j)
          *o<< " ";
        *o << wali::key2str(endhead2)<< "\n";
      }

      endhead =  wali::getKeySpace()->getKey(curKey++);
      pds.add_rule(
          pdsState,
          endhead1,
          pdsState,
          endhead,
          conf.randomWt()
          );
      pds.add_rule(
          pdsState,
          endhead2,
          pdsState,
          endhead,
          conf.randomWt()
          );
      curhead = endhead;
    }else{
      nexthead = wali::getKeySpace()->getKey(curKey++);
      if(o){
        for(int j=0;j<tabspace;++j)
          *o<< " ";
        *o << wali::key2str(curhead) << "\n";
      }
      pds.add_rule(
          pdsState,
          curhead,
          pdsState,
          nexthead,
          conf.randomWt()
          );
      curhead = nexthead;
      remNodes--;
    }
  }
  return curhead;
}



