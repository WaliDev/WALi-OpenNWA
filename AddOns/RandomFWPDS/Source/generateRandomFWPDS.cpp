// ::wali::wpds
#include "wali/wpds/WPDS.hpp"

// ::wali
#include "wali/Key.hpp"
#include "wali/KeySpace.hpp"
#include "wali/SemElem.hpp"
#include "wali/MergeFn.hpp"

// ::std
#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>

#include "generateRandomFWPDS.hpp"

using namespace wali::wpds;
using namespace std;

string RandomPdsGen::getName(unsigned curKey)
{
  stringstream ss;
  ss << "Node__" << curKey;
  return ss.str();
}

RandomPdsGen::RandomPdsGen(wtgen_t r, int np, int nc, int nn, int ns, int ne, double pc, double ps, unsigned s) :
  Countable(),
  randomWt(r),
  numprocs(np),
  numcalls(nc),
  numnodes(nn),
  numsplits(ns),
  numerrs(ne),
  pCall(pc*100),
  pSplit(ps*100),
  seed(s)
{}

RandomPdsGen::~RandomPdsGen()
{
  entries.clear();
  exits.clear();
}

/**
  * Generate a random FWPDS with the configuration given by conf
  * @detail ...
  * @see struct Conf
  **/
void RandomPdsGen::get(WPDS& pds, Names& names, std::ostream * o)
{
  if(seed != 0)
    srand(seed);
  else
    srand(time(NULL));

  wali::clearKeyspace();
  unsigned curKey=1;

  pdsState = wali::getKeySpace()->getKey(getName(curKey++));
  //Generate Keys for all procedure entries and exits.  
  entries.clear();
  exits.clear();
  for(int i=0;i<numprocs; ++i){
    entries.push_back(wali::getKeySpace()->getKey(getName(curKey++)));
    exits.push_back(wali::getKeySpace()->getKey(getName(curKey++)));
  }


  if(o)
    *o << "#######################################\n";
  int baseNodes = (int) numnodes / (numprocs+3);
  int baseSplits = (int) numsplits / (numprocs+3);
  int baseCalls = (int) numcalls / (numprocs+3);
  int remNodes = numnodes - baseNodes*numprocs;
  int remSplits = numsplits - baseSplits*numprocs;
  int remCalls = numcalls - baseCalls*numprocs;
  for(int i=0; i < numprocs; ++i){
    int nodes = rand() % remNodes + 1;
    int calls = rand() % remCalls;
    int splits = rand() % remSplits;
    remNodes -= nodes;
    if(remNodes <= 0) remNodes = 1;
    remCalls -= calls;
    remSplits -= splits;
    genproc(pds,i,curKey,baseNodes+nodes,baseSplits+splits,baseCalls+calls,o,2);
    if(o)
      *o << "*******************************\n";
  }

  names.pdsState = pdsState;
  names.entries.clear();
  names.exits.clear();
  for(int i=0;i<numprocs;++i){
    names.entries.push_back(entries[i]);
    names.exits.push_back(exits[i]);
  }

  //TODO: What about errors tags?
}

void RandomPdsGen::genproc(
    WPDS& pds, 
    int procnum, 
    unsigned& curKey,
    int remNodes, 
    int remSplits, 
    int remCalls, 
    std::ostream *o, 
    int tabspace
    )
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
    *o << "proc[" << procnum << "(entry:"<< wali::key2str(entries[procnum])
      <<", exit:"<< wali::key2str(exits[procnum]) <<")]:\n";
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
    curhead = genblock(pds,curhead,curKey,nodes,splits,calls,o,tabspace+2);
  }
  //The last edge to the exit node
  pds.add_rule(
      pdsState,
      curhead,
      pdsState,
      exits[procnum],
      (*randomWt)()
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
      (*randomWt)()
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
      (*randomWt)()
    );  
  }
}

wali::Key RandomPdsGen::genblock(
    WPDS& pds, 
    Key curhead, 
    unsigned& curKey,
    int remNodes, 
    int remSplits, 
    int remCalls, 
    std::ostream *o, 
    int tabspace
    )
{

  wali::Key endhead,endhead1,endhead2,nexthead;
  int nodes1,nodes2,splits1,splits2,calls1,calls2,fn;
  int choice;
  while(remNodes > 0 || remCalls > 0 || remSplits > 0){
    choice = rand() % 100;
    if(choice < pCall){
      if(remCalls < 1) continue;
      nexthead = wali::getKeySpace()->getKey(getName(curKey++));
      fn = rand() % numprocs;
      if(o){
        for(int j=0;j<tabspace;++j)
          *o<< " ";
        *o << wali::key2str(curhead) << "[CALL TO PROC(" << fn << ")[" << wali::key2str(entries[fn]) 
          <<"] stk:" << wali::key2str(nexthead) << "\n";
      }
      pds.add_rule(
          pdsState,
          curhead,
          pdsState,
          entries[fn],
          nexthead,
          (*randomWt)()
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
      nexthead = wali::getKeySpace()->getKey(getName(curKey++));
      pds.add_rule(
          pdsState,
          curhead,
          pdsState,
          nexthead,
          (*randomWt)()
          );
      endhead1 = genblock(pds,nexthead,curKey,nodes1,splits1,calls1,o,tabspace+2);
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
      nexthead = wali::getKeySpace()->getKey(getName(curKey++));
      pds.add_rule(
          pdsState,
          curhead,
          pdsState,
          nexthead,
          (*randomWt)()
          );
      endhead2 = genblock(pds,nexthead,curKey,nodes1,splits1,calls1,o,tabspace+2);
      if(o){
        for(int j=0;j<tabspace+2;++j)
          *o<< " ";
        *o << wali::key2str(endhead2)<< "\n";
      }

      endhead =  wali::getKeySpace()->getKey(getName(curKey++));
      pds.add_rule(
          pdsState,
          endhead1,
          pdsState,
          endhead,
          (*randomWt)()
          );
      pds.add_rule(
          pdsState,
          endhead2,
          pdsState,
          endhead,
          (*randomWt)()
          );
      curhead = endhead;
    }else{
      nexthead = wali::getKeySpace()->getKey(getName(curKey++));
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
          (*randomWt)()
          );
      curhead = nexthead;
      remNodes--;
    }
  }
  return curhead;
}



