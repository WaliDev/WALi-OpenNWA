#ifndef _WALI_CFG_HPP_
#define _WALI_CFG_HPP_

/*
 * @author Akash Lal
 */

/*
 * This file includes an abstract class for a Control Flow Graph (CFG). A CFG is represented
 * as a graph over CFGNodes and CFGEdges. 
 *
 * This abstract class is meant to provide just enough interface to be able to 
 * construct a WPDS.
 */

#include "wali/Key.hpp"
#include "wali/SemElem.hpp"
#include "wali/MergeFn.hpp"
#include <set>

class CFGNode;
class CFGEdge;
class CFG;

// Abstract class denoting a CFG node
class CFGNode {

  public:

  // Return a unique identifier for the CFG node
  virtual int getId() const = 0;

  // Return a WPDS key that is unique for the CFG node. A sample
  // implementation for this method is provided using the getId
  // method. However, wali provides several ways of obtaining keys:
  // see [... NAK?]
  virtual wali::Key getWpdsKey() const {
    return wali::getKey(getId());

    /* To use a priority-based key (see TODO) one can use the following code: 
     * return wali::getKey( wali::getKey(getPriorityNumber()), wali::getKey(getId()) ); 
     *
     * This assume the presence of a function "int CFGNode::getPriorityNumber()" that
     * returns a (possibily non-unique) priority for the CFGNode.
     */
  }

  // Return the set of outgoing edges
  //std::set<CFGEdge *> getOutgoingEdges() = 0;

  virtual ~CFGNode() {}
};

// Abstract class denoting a CFG edge
class CFGEdge {

  // The source and target nodes of the edge
  CFGNode *src, *tgt;
  // If this edge is a call edge, then callee is the called
  // procedure. For simplicity, we assume that a call edge
  // has only one callee. (The presence of multiple callees
  // do not pose any challenge specific to WPDSs.)
  CFG* callee;

public:

  // Return the source node of the edge
  CFGNode *getSource() const {
    return src;
  }

  // Return the target node of the edge
  CFGNode *getTarget() const {
    return tgt;
  }

  // Is this edge a call edge?
  virtual bool isCall() const = 0;

  // Get callee, if this is a call edge
  CFG *getCallee() const {
    if(!isCall()) return 0;
    return callee;
  }

  // Return the weight (or abstract transformer) associated
  // with the CFG edge. The convention for call edges is that
  // this function should provide the transformer associated with
  // just the call instruction, not the called procedure.
  virtual wali::sem_elem_t getWeight() const = 0;

  // Return the merge function associated with the edge. This is only
  // invoked for call edges.
  virtual wali::merge_fn_t getMergeFn() const = 0;

  virtual ~CFGEdge() {}
};

// Abstract class for denoting a CFG. We assume that a CFG has a 
// unique entry node (that has no predecessors) and a unique exit
// node (that has no successors).
class CFG {

  // Entry and exit nodes of the CFG
  CFGNode *entryNode, *exitNode;

public:

  // Return the entry node of the CFG
  CFGNode *getEntry() const {
    return entryNode;
  }

  // Return the exit node of the CFG
  CFGNode *getExit() const {
    return exitNode;
  }

  // Return the set of all CFG edges contained in this CFG
  virtual const std::set<CFGEdge *> & getEdges() const = 0;

  virtual ~CFG() {}
};

#endif // _WALI_CFG_HPP_
