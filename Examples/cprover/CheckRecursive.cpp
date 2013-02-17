#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
// ::wali::cprover
#include "BplToPds.hpp"

#include <tr1/unordered_map>
#include <tr1/unordered_set>
#include <stack>

using namespace std;
using namespace wali::cprover;
using namespace wali::cprover::details;
using namespace wali::cprover::details::resolve_details;

typedef std::tr1::unordered_set< proc *> ProcSet;
typedef std::tr1::unordered_map< proc *, ProcSet > Callees;
typedef std::stack< proc * > ProcStack;
char * mainProc = "main";
Callees callees;

void printLoop(ProcStack& path){
  proc * p = path.top();
  path.pop();
  ProcStack loop;
  loop.push(p);
  while(path.top() != p){
    loop.push(path.top());
    path.pop();
  }
  loop.push(p);

  cout << "loop:" << endl;
  while(!loop.empty()){
    cout << "  " << loop.top()->f << endl;
    loop.pop();
  }
}

void collectReachable(proc * p, ProcSet& reachable)
{
  if(reachable.find(p) != reachable.end())
    return;
  reachable.insert(p);
  for(ProcSet::iterator pit = callees[p].begin(); pit != callees[p].end(); ++pit)
    collectReachable(*pit, reachable);
}

void dfs(ProcSet& visited, ProcSet& onPath, ProcSet& loopHeads, proc * p)
{
  if(onPath.find(p) != onPath.end()){
    loopHeads.insert(p);
    return;
  }
  if(visited.find(p) != visited.end())
    return;
  onPath.insert(p);
  visited.insert(p);
  if(callees.find(p) == callees.end())
    return;
  for(ProcSet::iterator pit = callees[p].begin(); pit != callees[p].end(); ++pit)
    dfs(visited, onPath, loopHeads, *pit);
  onPath.erase(p);
  return;
}

int main(int argc, char ** argv)
{
  string fname;
  if(argc >=2){
    stringstream s;
    s << argv[1];
    s >> fname;
  }else{
    cerr << "Usage: ./CheckRecursive input_file [entry function (default:main)]\n";
    return -1;
  }
  if(argc >= 3){
    stringstream s;
    delete mainProc;
    mainProc = argv[2];
  }
  
  cout << "Parsing program..." << endl;
  prog * pg = parse_prog(fname.c_str());

  cout << "Checking for recursion..." << endl;
  str_proc_ptr_hash_map name2proc;
  map_name_to_proc(name2proc, pg);
    
  for(str_proc_ptr_hash_map::iterator it = name2proc.begin(); it != name2proc.end(); ++it){
    stmt_ptr_proc_ptr_hash_map m;
    map_call_to_callee(m, name2proc, it->second);
    callees[it->second] = ProcSet();
    for(stmt_ptr_proc_ptr_hash_map::iterator pit = m.begin(); pit != m.end(); ++pit)
      callees[it->second].insert(pit->second);
  }

  proc * main;
  if(name2proc.find(mainProc) == name2proc.end()){
    cerr << "Could not find main proc [" << mainProc << "]" << endl;
    return -1;
  }else{
    main = name2proc[mainProc];
  }

  ProcSet visited;
  ProcSet onPath;
  ProcSet loopHeads;
  dfs(visited,onPath,loopHeads,main);
  if(loopHeads.size() == 0)
    cout << "Program not recursive" << endl;
  else{
    cout << "Program is recursive." << endl;
    cout << "Loop Heads: " << endl;
    for(ProcSet::const_iterator it = loopHeads.begin(); it != loopHeads.end(); ++it)
      cout << (*it)->f << " ";
    cout << endl;
    ProcSet reachableFromMain, reachableFromLoopHeads;
    collectReachable(main, reachableFromMain);
    for(ProcSet::iterator it = loopHeads.begin(); it != loopHeads.end(); ++it)
      collectReachable(*it, reachableFromLoopHeads);
    cout << "\% procs in a recursive loop: " 
      << 100 * double(reachableFromLoopHeads.size()) / double(reachableFromMain.size())
      << endl;
  }

  deep_erase_prog(&pg);
}
