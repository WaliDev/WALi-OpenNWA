//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2007, Gogul Balakrishnan, Thomas Reps
// University of Wisconsin, Madison.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//   1. Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//   2. Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in
//      the documentation and/or other materials provided with the
//      distribution.
//
//   3. Neither the name of the University of Wisconsin, Madison nor the
//      names of its contributors may be used to endorse or promote
//      products derived from this software without specific prior
//      written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// e-mail: bgogul@cs.wisc.edu, reps@cs.wisc.edu
//
//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2007, an unpublished work by GrammaTech, Inc.
// ALL RIGHTS RESERVED
//
// This software is furnished under a license and may be used and
// copied only in accordance with the terms of such license and the
// inclusion of the above copyright notice.  This software or any
// other copies thereof may not be provided or otherwise made
// available to any other person.  Title to and ownership of the
// software is retained by GrammaTech, Inc.
//
// GrammaTech proprietary and confidential.
//
// Not to be redistributed or disclosed without a prior written
// consent from GrammaTech, Inc.
//
//////////////////////////////////////////////////////////////////////////////
// Dictionary implementation based on Applicative-AVL trees
//-----------------------------------------------------------------


#ifndef DICTIONARY_GUARD
#define DICTIONARY_GUARD

#include "vsa_sysdep.h" // defines VSA_IMP_TYPENAME__
#include <limits.h>
#include <iostream>


#if defined(X86_PROFILE_NEW_DELETE)
#   include <new.h>
#   include <typeinfo.h>
//  Hacks don't get much uglier than the following:
//  - There are several different heap profiling tricks being
//    applied (when various macros are defined).
//  - Some of them do something like
//      #define new new(__FILE__,__line__)
//    to redirect calls to new to a version that can record
//    useful file and line information.
//  - However, the above #define-d "new" cannot appear in
//    the declaration (prototype) of our overloaded new operator.
//  - Therefore, here, I'm defining a new macro "decl_new" to
//    always contain exactly the un-expanded token "new", and
//    leaving "new" to be a possibly-expanded macro.  The idea
//    is to use "decl_new" in the declarations of the operators,
//    and let "new" be used at calls to "new".
//  - The following is the only trick I can think of to set
//    decl_new to be an unexpanded "new" token, while leaving
//    "new" to be whatever it was defined to be.
#   if defined(new)
#      define new_new new
#      undef new
#      define decl_new new
#      define new new_new
#   else // !defined(new)
#      define decl_new new
#   endif // !defined(new)
#endif //defined(X86_PROFILE_NEW_DELETE)

#define PER_BB_PICKLE
#if defined(PER_BB_PICKLE) // {
//- dictflags are currently used by:
//  - VSApickler: one "visited" bit
//  - VSApickler: one bit to distinguish "global" AVL nodes
#   define DICTFLAG_VISITED 0x01
#   define DICTFLAG_GLOBAL  0x02
typedef unsigned char dictflags_t;
typedef unsigned char height_t;
#else // } if !defined(PER_BB_PICKLE) {
typedef unsigned short height_t;
#endif // } if !defined(PER_BB_PICKLE)
typedef unsigned short refcount_t;

static const refcount_t maxRefCount = USHRT_MAX;

template <class Key_t, class Datum_t, class KeyComp_t>
class DictNode_T;

enum STATUS { DICT_NOT_FOUND, DICT_FOUND };
#if _MSC_VER > 1200
   #include "RIntXCong.h"
#endif


//------------------------------------------------------------------------
// Default comparator
//
//Notes:
//	Clients can use this default dictionary comparator or define a
// comparator on their own. But the comparator should have a static
// keycompare method as shown below
//------------------------------------------------------------------------
template <class Key_t> 
class DictComp_T{
public:
	static int keycompare(const Key_t& k1, const Key_t& k2) {
		if(k1==k2) 
			return 0;
		if(k1>k2)
			return 1;
		else
			return -1;
	}
};

template <class T>
bool dataComparator(const T& t1, const T& t2) {return t1 == t2;}


//----------------------------------------------------------------------------
// Dictionary implementation (Based on applicative AVL trees)
//-----------------------------------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t=DictComp_T<Key_t> >
class Dictionary_T {
	friend std::ostream& operator<< (std::ostream&  out, const Dictionary_T<Key_t, Datum_t, KeyComp_t>& d);

public:

	//---------------------------------------
	// Constructors
	//---------------------------------------
	Dictionary_T();														// Default constructor
	
	Dictionary_T(DictNode_T<Key_t, Datum_t, KeyComp_t> *n);				// Constructor
	
	Dictionary_T(const Key_t& k, 
				 const Datum_t& v, 
				 const Dictionary_T<Key_t, Datum_t, KeyComp_t>& l,  
				 const Dictionary_T<Key_t, Datum_t, KeyComp_t>& r);		// Constructor

	Dictionary_T(const Key_t& k, const Datum_t& v);						// Constructor

	Dictionary_T(const Dictionary_T<Key_t, Datum_t, KeyComp_t>& d);		// Copy constructor
	

	void clear();														// Delete all entries
	//--------------------------------
	// Destructor
	//--------------------------------	
	~Dictionary_T();													// Destructor

	//---------------------------------
	// Operators
	//---------------------------------
	Dictionary_T<Key_t, Datum_t, KeyComp_t>&  
		operator= (const Dictionary_T<Key_t, Datum_t, KeyComp_t>& d);  // Assignment operator


	bool			NoEntries() const;			// Has no entries?
	const Key_t&	Key()		const;			// Key at root
	const Datum_t&	Datum()		const;			// Datum at root
	
	Dictionary_T<Key_t, Datum_t, KeyComp_t> Left()		const;	// Left child
	Dictionary_T<Key_t, Datum_t, KeyComp_t> Right()		const;	// Right child
	Dictionary_T<Key_t, Datum_t, KeyComp_t> RightMost() const;	// Rightmost child
	height_t								Height()	const;	// Max of height of the children

    // Insertion
    Dictionary_T<Key_t, Datum_t, KeyComp_t> 
        Insert(const Key_t& k, const Datum_t& v) const;     // Return a dictionary obtained by inserting (k, v)

    Dictionary_T<Key_t, Datum_t, KeyComp_t>                 // Return a dictionary obtained by inserting (k, v)
        InsertWithCheck(const Key_t& k, const Datum_t& v,   // after first checking if k already maps to v
                        bool (*dataComp)(const Datum_t&d1, const Datum_t& d2) = NULL
                       ) const;                             // (Default comparator is shallow bitwise comparison)

    // Instead of calling Insert or InsertWithCheck directly,
    // it may be useful to use one of these macros, which can
    // then be modified to use deep vs shallow comparison,
    // to always turn on/off checks, to add stats collection, etc.
	//
	// InsertWithCheck is meant to be an _optimization_ of Insert, 
	// where if a to-be-inserted item is found to already exist 
	// (according to the dataComp function), we bypass the log-space 
	// cost of inserting an item into the applicative AVL tree. Using 
	// a shallow comparison that sometimes fails to recognize deep equality 
	// only means that on those occasions we do not perform the optimization.
	//
	// NOTES ON CURRENT VERSION:
    //  - INSERT_WITH_CHECK: does *shallow* comparison (discards cf argument)
#define INSERT_WITH_CHECK(key, val, cf) InsertWithCheck(key, val)
    //  - INSERT_NO_CHECK: no lookup.
#define INSERT_NO_CHECK(key, val, cf) Insert(key, val)

    
	Dictionary_T<Key_t, Datum_t, KeyComp_t> 
		Delete(const Key_t& k) const;							// Return a dictionary obtained by deleting (k, *)
	
	STATUS IsKeyInDict(const Key_t& t) const;  // Is the given key in the dictionary?
	STATUS Lookup(const Key_t& k, Datum_t& v) const;			// Search for a given k, update v
	STATUS Lookup(const Key_t& k, Key_t& origK, Datum_t& v) const;	// Search for a given k, update origK & v
#if defined(PER_BB_PICKLE) // {
    dictflags_t Flags() const { return myEntries ? myEntries->flags : 0; } // Return the flags
#endif // } defined(PER_BB_PICKLE)
	DictNode_T<Key_t, Datum_t, KeyComp_t> *myEntries;

	//----------------------------------
	// Iterator
	//----------------------------------
	class iterator{
		public:

			const Key_t& Key() const {
				return myEntry->key;
			}

			const Datum_t& Datum() const {
				return myEntry->datum;
			}

			const bool IsRootShared(const iterator& rhs) const  {
				return (myEntry == rhs.myEntry);
			}

			iterator():elemStack(NULL), stackPos(-1), myEntry(NULL), stackSize(0) {
				
			}

			iterator(const iterator& rhs) {
				stackSize = rhs.stackSize;
				elemStack = new DictNode_T<Key_t, Datum_t, KeyComp_t>*[stackSize];
				for(int i = 0; i < stackSize; ++i) {
					elemStack[i] = rhs.elemStack[i];
				}
				myEntry  = rhs.myEntry;
				stackPos = rhs.stackPos;
			}

			iterator& operator = (const iterator& rhs) {
				// avoid self-copy
				if(this == &rhs) return *this;

				delete [] elemStack;
				stackSize = rhs.stackSize;
				elemStack = new DictNode_T<Key_t, Datum_t, KeyComp_t>*[stackSize];
				for(int i = 0; i < stackSize; ++i) {
					elemStack[i] = rhs.elemStack[i];
				}
				myEntry  = rhs.myEntry;
				stackPos = rhs.stackPos;
				return *this;
			}


			~iterator() {
				if(elemStack)
					delete []elemStack;
			}

			// Move to the next element
			void operator ++(int dum) {
				/// STACK EAGERLY?
				/*
				if((myEntry = pop())) {
					stackLeftElems(myEntry->right.myEntries);
				}
				*/
				/// STACK LAZILY
				// (necessary for bypassing right subtrees)
				stackLeftElems(myEntry->right.myEntries);
				myEntry = pop();
			}

			// Move by skipping the right tree altogether 
			void advanceWORightTree() {
				/// Important: ++ should stack nodes lazily
				myEntry = pop();
			}

			// Reached the end of the dictionary?
			bool end() const{
				return (myEntry == NULL);
			}

		private:
			DictNode_T<Key_t, Datum_t, KeyComp_t>*	myEntry;
			DictNode_T<Key_t, Datum_t, KeyComp_t>**	elemStack;
			int stackPos; unsigned stackSize;

			// Initialize the iterator with a particular stack size
			// and the tree whose root is "root"
			void initialize(int size, DictNode_T<Key_t, Datum_t, KeyComp_t>* root) {
				if(elemStack)
					delete []elemStack;
				stackSize = size;
				elemStack = new DictNode_T<Key_t, Datum_t, KeyComp_t>*[stackSize];
				stackLeftElems(root);
				myEntry = pop();
				//(STACK EAGERLY)this->operator++(0);
			}
			
			// Pop the top of the stack
			DictNode_T<Key_t, Datum_t, KeyComp_t>* pop() {
				if(stackPos == -1)
					return NULL;
				return elemStack[stackPos--];
			}

			// Stack the left children of elem (incl. elem)
			void stackLeftElems(DictNode_T<Key_t, Datum_t, KeyComp_t>* elem) {
				while(elem) {
					elemStack[++stackPos] = elem; // Push
					elem = elem->left.myEntries;
				}
			}

			friend class Dictionary_T<Key_t, Datum_t, KeyComp_t>;
	};

	// Iterators
	//Dictionary_T<Key_t, Datum_t, KeyComp_t>::iterator *First() const;
	void begin(VSA_IMP_TYPENAME__ Dictionary_T<Key_t, Datum_t, KeyComp_t>::iterator& ) const;
	inline bool IsRootShared(const Dictionary_T<Key_t, Datum_t, KeyComp_t>& rhs) const;
	
	// Compare two dictionaries. 
	// Use sharing to short-cut the comparisons operations.
	bool IsEqual(const Dictionary_T<Key_t, Datum_t, KeyComp_t>& rhs,
				 bool (*dataComp)(const Datum_t&d1, const Datum_t& d2) = dataComparator<Datum_t>) const;
	

	
	bool IsEmpty() const;
	bool IsSizeOne() const;
	unsigned int Size() const;
	void Print(std::ostream&  out, unsigned int depth) const;
 private:
	Dictionary_T<Key_t, Datum_t, KeyComp_t> Bal(const Key_t& k, 
												const Datum_t& d, 
												const Dictionary_T<Key_t, Datum_t, KeyComp_t>& l, 
												const Dictionary_T<Key_t, Datum_t, KeyComp_t>& r) const;
};

//---------------------------------------
// Dictnode
//---------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t=DictComp_T<Key_t> >
class DictNode_T {
 public:
	DictNode_T(Key_t k, Datum_t d, Dictionary_T<Key_t, Datum_t, KeyComp_t> l, Dictionary_T<Key_t, Datum_t, KeyComp_t> r, height_t h);
	DictNode_T(Key_t k, Datum_t d, Dictionary_T<Key_t, Datum_t, KeyComp_t> l, Dictionary_T<Key_t, Datum_t, KeyComp_t> r);
	DictNode_T(Key_t k, Datum_t d);
	~DictNode_T<Key_t, Datum_t, KeyComp_t>();
	void IncrRef();
	void DecrRef();

#if defined(X86_PROFILE_NEW_DELETE)
	void* operator decl_new(size_t);
	void  operator delete(void*);
	void* operator decl_new[] (size_t);
//	void  operator delete[] (void*);
#endif //defined(X86_PROFILE_NEW_DELETE)

	unsigned int Size() const;

 private:
	Key_t key;
	Datum_t datum;
	Dictionary_T<Key_t, Datum_t, KeyComp_t> left, right;
#if defined(PER_BB_PICKLE) // {
    dictflags_t flags;
#endif // } defined(PER_BB_PICKLE)
	height_t height;
	refcount_t refCount;

#ifdef EXPLICIT_PICKLE_REFCOUNT_FIELD_IN_DICT
	unsigned int pickle_refcount;	// used by VSApickler
#endif

	friend class VSApickler;
	friend class Dictionary_T<Key_t, Datum_t, KeyComp_t>;
	friend class Dictionary_T<Key_t, Datum_t, KeyComp_t>::iterator;
};

//----------------------------------------
// Size
//----------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
unsigned int DictNode_T<Key_t, Datum_t, KeyComp_t>::Size() const {
	return 1 + left.Size() + right.Size();
}

//---------------------------------------------------
// Constructors/Destructor 
//---------------------------------------------------

//---------------------------------------------------
// Default constructor
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
Dictionary_T<Key_t, Datum_t, KeyComp_t>::Dictionary_T()
  : myEntries(NULL)
{
}


//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
Dictionary_T<Key_t, Datum_t, KeyComp_t>::Dictionary_T(DictNode_T<Key_t, Datum_t, KeyComp_t> *n)
  : myEntries(n)
{
	
  if (myEntries != NULL) {
	 myEntries->IncrRef();
  }
}

//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
Dictionary_T<Key_t, Datum_t, KeyComp_t>::Dictionary_T(const Key_t& k, 
													  const Datum_t& v, 
													  const Dictionary_T<Key_t, Datum_t, KeyComp_t>& l, 
													  const Dictionary_T<Key_t, Datum_t, KeyComp_t>& r)
  : myEntries(new DictNode_T<Key_t, Datum_t, KeyComp_t>(k,v,l,r))
{
  	myEntries->IncrRef();
}

//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
Dictionary_T<Key_t, Datum_t, KeyComp_t>::Dictionary_T(const Key_t& k, const Datum_t& v)
  : myEntries(new DictNode_T<Key_t, Datum_t, KeyComp_t>(k,v))
{
	myEntries->IncrRef();
}

//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
Dictionary_T<Key_t, Datum_t, KeyComp_t>::Dictionary_T(const Dictionary_T<Key_t, Datum_t, KeyComp_t>& d)
{
  myEntries = d.myEntries;
  if (myEntries != NULL) {
    myEntries->IncrRef();
  }
}


//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
void Dictionary_T<Key_t, Datum_t, KeyComp_t>::clear()
{
  if (myEntries != NULL) {
    myEntries->DecrRef();
	myEntries = NULL;
  }
  return;
}

//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
Dictionary_T<Key_t, Datum_t, KeyComp_t>::~Dictionary_T()
{
  if (myEntries != NULL) {
    myEntries->DecrRef();
  }
}

//---------------------------------------------------
// Overloaded assignment
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
Dictionary_T<Key_t, Datum_t, KeyComp_t>& Dictionary_T<Key_t, Datum_t, KeyComp_t>::operator= (const Dictionary_T<Key_t, Datum_t, KeyComp_t>& d)
{
  if (this != &d)      // don't assign to self!
  {
    DictNode_T<Key_t, Datum_t, KeyComp_t> *temp = myEntries;
    myEntries = d.myEntries;
    if (myEntries != NULL) {
      myEntries->IncrRef();
    }
	
    if (temp != NULL) {
      temp->DecrRef();
    }
  }
  return *this;    	
}

//---------------------------------------------------
// Insert <k,v> into dictionary, after first checking
// whether <k,v> is already in the dictionary.
// The third argument is the comparator for the value;
// if it is null, a shallow bitwise comparison (with
// memcmp) is used.
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
Dictionary_T<Key_t, Datum_t, KeyComp_t>
    Dictionary_T<Key_t, Datum_t, KeyComp_t>::InsertWithCheck(const Key_t& k,
                                                             const Datum_t& v,
                                                             bool (*dataComp)(const Datum_t&d1, const Datum_t& d2)
                                                             ) const
{
    Datum_t v_old;
    if((this->Lookup(k, v_old) == DICT_FOUND)
        && ((dataComp == NULL) ? !memcmp(&v, &v_old, sizeof(Datum_t))
                               : (dataComp(v, v_old)))){
        return *this;
    }
    return this->Insert(k,v);
}


//---------------------------------------------------
// Insert <k,v> into dictionary, forcibly applying
// the update (and rebalancing) regardless of whether
// <k,v> is already in the dictionary.
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
Dictionary_T<Key_t, Datum_t, KeyComp_t>
    Dictionary_T<Key_t, Datum_t, KeyComp_t>::Insert(const Key_t& k, const Datum_t& v) const
{
  if (myEntries == NULL)
    return Dictionary_T<Key_t, Datum_t, KeyComp_t>(k,v);
  int temp = KeyComp_t::keycompare(k, Key());
  if (temp < 0)
    return Bal(Key(), Datum(), Left().Insert(k,v), Right());
  else if (temp == 0)
    return Bal(k,v,Left(),Right());
  else
    return Bal(Key(), Datum(), Left(), Right().Insert(k,v));
}


//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
Dictionary_T<Key_t, Datum_t, KeyComp_t> Dictionary_T<Key_t, Datum_t, KeyComp_t>::Delete(const Key_t& k) const
{
  if (myEntries == NULL) return NULL;
  int temp = KeyComp_t::keycompare(k, Key());
  if (temp < 0)
    return Bal(Key(), Datum(), Left().Delete(k), Right());
  else if (temp == 0)
  {
    if ((myEntries->left).NoEntries()) return myEntries->right;
    if ((myEntries->right).NoEntries()) return myEntries->left;
    Dictionary_T<Key_t, Datum_t, KeyComp_t> rmost = Left().RightMost();
    return Bal(rmost.Key(), rmost.Datum(), Left().Delete(rmost.Key()), Right());
  }
  else
    return Bal(Key(), Datum(), Left(), Right().Delete(k));
}

//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
STATUS Dictionary_T<Key_t, Datum_t, KeyComp_t>::IsKeyInDict(const Key_t& k) const 
{
  if (myEntries == NULL)
    return DICT_NOT_FOUND;
  int temp = KeyComp_t::keycompare(k, Key());
  if (temp < 0) {
    if (Left().NoEntries())
      return DICT_NOT_FOUND;
    else
      return Left().IsKeyInDict(k);
  }
  else if (temp == 0)
  {
    return DICT_FOUND;
  }
  else {
    if (Right().NoEntries())
      return DICT_NOT_FOUND;
    else
      return Right().IsKeyInDict(k);
  }
}

//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
STATUS Dictionary_T<Key_t, Datum_t, KeyComp_t>::Lookup(const Key_t& k, Datum_t& v) const
{
  if (myEntries == NULL)
    return DICT_NOT_FOUND;
  int temp = KeyComp_t::keycompare(k, Key());
  if (temp < 0) {
    if (Left().NoEntries())
      return DICT_NOT_FOUND;
    else
      return Left().Lookup(k,v);
  }
  else if (temp == 0)
  {
    v = myEntries->datum;
    return DICT_FOUND;
  }
  else {
    if (Right().NoEntries())
      return DICT_NOT_FOUND;
    else
      return Right().Lookup(k,v);
  }
}



//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
STATUS Dictionary_T<Key_t, Datum_t, KeyComp_t>::Lookup(const Key_t& k, Key_t& origK, Datum_t& v) const
{
  if (myEntries == NULL)
    return DICT_NOT_FOUND;

  int temp = KeyComp_t::keycompare(k, Key());
  if (temp < 0) {
    if (Left().NoEntries())
      return DICT_NOT_FOUND;
    else
      return Left().Lookup(k, origK, v);
  }
  else if (temp == 0)
  {
    origK = myEntries->key;
    v = myEntries->datum;
    return DICT_FOUND;
  }
  else {
    if (Right().NoEntries())
      return DICT_NOT_FOUND;
    else
      return Right().Lookup(k, origK, v);
  }
}

//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
Dictionary_T<Key_t, Datum_t, KeyComp_t> Dictionary_T<Key_t, Datum_t, KeyComp_t>::RightMost() const
{
  if (Right().NoEntries())
    return *this;
  else
    return (myEntries->right).RightMost();
}


//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
bool Dictionary_T<Key_t, Datum_t, KeyComp_t>::NoEntries() const
{
  return (myEntries == NULL);
}

//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
const Key_t& Dictionary_T<Key_t, Datum_t, KeyComp_t>::Key() const
{
	assert(myEntries!=NULL);
	return myEntries->key;
}

//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
const Datum_t& Dictionary_T<Key_t, Datum_t, KeyComp_t>::Datum() const
{
	assert(myEntries!=NULL);
    return myEntries->datum;
}

//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
Dictionary_T<Key_t, Datum_t, KeyComp_t> Dictionary_T<Key_t, Datum_t, KeyComp_t>::Left() const
{
  if (myEntries == NULL)
    return Dictionary_T<Key_t, Datum_t, KeyComp_t>(NULL);
  else
    return myEntries->left;
}

//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
Dictionary_T<Key_t, Datum_t, KeyComp_t> Dictionary_T<Key_t, Datum_t, KeyComp_t>::Right() const
{
  if (myEntries == NULL)
    return Dictionary_T<Key_t, Datum_t, KeyComp_t>(NULL);
  else
    return myEntries->right;
}

//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
height_t Dictionary_T<Key_t, Datum_t, KeyComp_t>::Height() const
{
  if (myEntries == NULL)
    return 0;
  else
    return myEntries->height;
}


// Given two Dictionaries, l and r, whose heights are within two,
// create a balanced Dictionary_T<Key_t, Datum_t, KeyComp_t> from k, d, l, and r.
//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
Dictionary_T<Key_t, Datum_t, KeyComp_t> Dictionary_T<Key_t, Datum_t, KeyComp_t>::Bal(const Key_t& k, const Datum_t& d, const Dictionary_T<Key_t, Datum_t, KeyComp_t>& l, const Dictionary_T<Key_t, Datum_t, KeyComp_t>& r) const
{
  if (WithinOne(l.Height(), r.Height()))
    return Dictionary_T<Key_t, Datum_t, KeyComp_t>(k,d,l,r);
  else if (l.Height() > r.Height())
  { // l.Height() >= 2
    if (l.Left().Height() >= l.Right().Height()) {
      // cout << "Single rotation at " << k << ": " << d << endl;
      return Dictionary_T<Key_t, Datum_t, KeyComp_t>(l.Key(), l.Datum(), l.Left(), new DictNode_T<Key_t, Datum_t, KeyComp_t>(k,d,l.Right(),r));
    }
    else {
      // cout << "Double rotation at " << k << ": " << d << endl;
      return Dictionary_T<Key_t, Datum_t, KeyComp_t>(l.Right().Key(),
                        l.Right().Datum(),
                        Dictionary_T<Key_t, Datum_t, KeyComp_t>(l.Key(), l.Datum(), l.Left(), l.Right().Left()),
                        Dictionary_T<Key_t, Datum_t, KeyComp_t>(k, d, l.Right().Right(), r)
                       );
    }
  }
  else
  { // r.Height() >= 2
    if (r.Right().Height() >= r.Left().Height()) {
      // cout << "Single rotation at " << k << ": " << d << endl;
      return Dictionary_T<Key_t, Datum_t, KeyComp_t>(r.Key(), r.Datum(), new DictNode_T<Key_t, Datum_t, KeyComp_t>(k,d,l,r.Left()), r.Right());
    }
    else {
      // cout << "Double rotation at " << k << ": " << d << endl;
      return Dictionary_T<Key_t, Datum_t, KeyComp_t>(r.Left().Key(),
                        r.Left().Datum(),
                        Dictionary_T<Key_t, Datum_t, KeyComp_t>(k,d,l,r.Left().Left()),
                        Dictionary_T<Key_t, Datum_t, KeyComp_t>(r.Key(), r.Datum(), r.Left().Right(), r.Right())
                       );
    }
  }
}
//---------------------------------------------------
// Iterators
//---------------------------------------------------

//---------------------------------------------------
// Begin
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
void Dictionary_T<Key_t, Datum_t, KeyComp_t>::begin(VSA_IMP_TYPENAME__ Dictionary_T<Key_t, Datum_t, KeyComp_t>::iterator& iter) const {
	iter.initialize(Height(), myEntries);
}


//----------------------------------------
// Size
//----------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
unsigned int Dictionary_T<Key_t, Datum_t, KeyComp_t>::Size() const {
	if(myEntries)
		return 1 + myEntries->left.Size() + myEntries->right.Size();
	else
		return 0;

}

//----------------------------------------
// Is the root of the AVL tree shared?
//----------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
inline bool Dictionary_T<Key_t, Datum_t, KeyComp_t>::IsRootShared(const Dictionary_T<Key_t, Datum_t, KeyComp_t>& rhs) const {
	return (myEntries == rhs.myEntries);
}


//----------------------------------------
// Is the tree equal to rhs
//----------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
bool Dictionary_T<Key_t, Datum_t, KeyComp_t>::IsEqual(const Dictionary_T<Key_t, Datum_t, KeyComp_t>& rhs,
													  bool (*dataComp)(const Datum_t&d1, const Datum_t& d2)
													  ) const {
	
	// First compare the pointers
	if(this->IsRootShared(rhs))
		return true;

	// The root pointers are not equal
	// compare all the entries.
	iterator thisIter, rhsIter; 
	this->begin(thisIter);
	rhs.begin(rhsIter);
	while(!thisIter.end() && !rhsIter.end()) {

		// If the trees rooted at the nodes
		// pointed to by the iterators 
		// are the same, skip each right tree
		// completely.
		if(thisIter.IsRootShared(rhsIter)) {
			thisIter.advanceWORightTree();
			rhsIter.advanceWORightTree();
			continue;			
		}

		int cmp = KeyComp_t::keycompare(thisIter.Key(), rhsIter.Key());
		
		// Keys not equal? return false.
		if(cmp) 
			return false;
			
		// Data not equal? return false.
		if(!(*dataComp)(thisIter.Datum(), rhsIter.Datum()))
			return false;
		
		// Advance the iterators
		thisIter++;
		rhsIter++;
	}
	// Have both the iterators reached the
	// end (i.e. are the sizes of dictionaries 
	// equal?).
	if(thisIter.end() && rhsIter.end())
		return true;
	else
		return false;
}


//----------------------------------------
// Is empty?
//----------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
bool Dictionary_T<Key_t, Datum_t, KeyComp_t>::IsEmpty() const {
	return (myEntries == NULL);
}

//----------------------------------------
// Is the size equal to one?
//----------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
bool Dictionary_T<Key_t, Datum_t, KeyComp_t>::IsSizeOne() const {
	// Empty?
	if(!myEntries) 
		return false;

	// Both the left and right children are empty?
	return (myEntries->left.NoEntries() && myEntries->right.NoEntries());
}

//---------------------------------------------------
// Print
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
void Dictionary_T<Key_t, Datum_t, KeyComp_t>::Print(std::ostream&  out, unsigned int depth) const
{
    if (myEntries == NULL) {
        out << "NULL ";
    }
    else {
        if (!(myEntries->left).NoEntries())
            (myEntries->left).Print(out, depth+1);
        for (unsigned int i = 0; i < depth; i++)
            out << "  ";
        out << "height: " << (int)(myEntries->height)
            << ", key: " << (myEntries->key)
            << ", datum: " << (myEntries->datum)
            << ", refCount: " << myEntries->refCount << " ";

        if (!(myEntries->right).NoEntries())
            (myEntries->right).Print(out, depth+1);
    }
}

static int WithinOne(unsigned int i, unsigned int j)
{
  return(i==j || i+1==j || j+1==i);
}

//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
std::ostream& operator<< (std::ostream&  out, const Dictionary_T<Key_t, Datum_t, KeyComp_t>& d)
{
  d.Print(out, 0);
  return(out);
}


//********************************************************************
// DictNode_T<Key_t, Datum_t, KeyComp_t>
//********************************************************************
//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
DictNode_T<Key_t, Datum_t, KeyComp_t>::DictNode_T(Key_t k, Datum_t d, Dictionary_T<Key_t, Datum_t, KeyComp_t> l, Dictionary_T<Key_t, Datum_t, KeyComp_t> r, height_t h)
  : key(k), datum(d), left(l), right(r),
#if defined(PER_BB_PICKLE) // {
    flags(0),
#endif // } defined(PER_BB_PICKLE)
    height(h), refCount(0)
{
#ifdef EXPLICIT_PICKLE_REFCOUNT_FIELD_IN_DICT
    pickle_refcount = 0;
#endif
}

//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
DictNode_T<Key_t, Datum_t, KeyComp_t>::DictNode_T(Key_t k, Datum_t d, Dictionary_T<Key_t, Datum_t, KeyComp_t> l, Dictionary_T<Key_t, Datum_t, KeyComp_t> r)
  : key(k), datum(d), left(l), right(r),
#if defined(PER_BB_PICKLE) // {
    flags(0),
#endif // } defined(PER_BB_PICKLE)
    refCount(0)
{
  height_t l_height = l.Height();
  height_t r_height = r.Height();
  height = 1 + ( (l_height >= r_height) ? l_height : r_height );
#ifdef EXPLICIT_PICKLE_REFCOUNT_FIELD_IN_DICT
    pickle_refcount = 0;
#endif
}

//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
DictNode_T<Key_t, Datum_t, KeyComp_t>::DictNode_T(Key_t k, Datum_t d)
  : key(k), 
	datum(d), 
	left(Dictionary_T<Key_t, Datum_t, KeyComp_t>()), 
	right(Dictionary_T<Key_t, Datum_t, KeyComp_t>()),
#if defined(PER_BB_PICKLE) // {
    flags(0),
#endif // } defined(PER_BB_PICKLE)
	height(1), 
	refCount(0)
{
#ifdef EXPLICIT_PICKLE_REFCOUNT_FIELD_IN_DICT
    pickle_refcount = 0;
#endif
}


//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
DictNode_T<Key_t, Datum_t, KeyComp_t>::~DictNode_T()
{
  // Destructors for left and right called automatically,
  // which will decrement the reference counts for left-myEntries
  // and right->myEntries (assuming they are non-NULL).
}

//---------------------------------------------------
// For profiling new/deletes
//---------------------------------------------------
#if defined(X86_PROFILE_NEW_DELETE)
template <class Key_t, class Datum_t, class KeyComp_t>
void* DictNode_T<Key_t, Datum_t, KeyComp_t>::operator decl_new(size_t s)
{
	assert(s == sizeof(DictNode_T<Key_t, Datum_t, KeyComp_t>));
	nHeapProfiler::getCounter(typeid(DictNode_T<Key_t, Datum_t, KeyComp_t>).name(),
						  0).increment(1, sizeof(DictNode_T<Key_t, Datum_t, KeyComp_t>));
	nHeapProfiler::getCounter("all_dictnodes", 0).increment(1, sizeof(DictNode_T<Key_t, Datum_t, KeyComp_t>));
	return ::operator new(s);
}
#endif //defined(X86_PROFILE_NEW_DELETE)

//---------------------------------------------------
// For profiling new/deletes
//---------------------------------------------------
#if defined(X86_PROFILE_NEW_DELETE)
template <class Key_t, class Datum_t, class KeyComp_t>
void  DictNode_T<Key_t, Datum_t, KeyComp_t>::operator delete(void* o)
{
	nHeapProfiler::getCounter(typeid(DictNode_T<Key_t, Datum_t, KeyComp_t>).name(),
						  0).increment(-1, -sizeof(DictNode_T<Key_t, Datum_t, KeyComp_t>));
	nHeapProfiler::getCounter("all_dictnodes", 0).increment(-1, -sizeof(DictNode_T<Key_t, Datum_t, KeyComp_t>));
	::operator delete(o);
}
#endif //defined(X86_PROFILE_NEW_DELETE)

//---------------------------------------------------
// For profiling new/deletes
//---------------------------------------------------
#if defined(X86_PROFILE_NEW_DELETE)
template <class Key_t, class Datum_t, class KeyComp_t>
void* DictNode_T<Key_t, Datum_t, KeyComp_t>::operator decl_new[] (size_t s)
{
#define ALLOCATING_ARRAY_OF_DICTNODES 0
	assert(ALLOCATING_ARRAY_OF_DICTNODES);
#undef ALLOCATING_ARRAY_OF_DICTNODES
	return ::operator new[](s);
}
#endif //defined(X86_PROFILE_NEW_DELETE)

//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
void DictNode_T<Key_t, Datum_t, KeyComp_t>::IncrRef()
{
  if (refCount < maxRefCount) {
    refCount++;
  }
}

//---------------------------------------------------
// 
//---------------------------------------------------
template <class Key_t, class Datum_t, class KeyComp_t>
void DictNode_T<Key_t, Datum_t, KeyComp_t>::DecrRef()
{
  if (refCount < maxRefCount) {
    refCount -= 1;
    if (refCount == 0) {
      delete this;
    }
  }
}

//------------------------------------
// Dictionary iterator
//-----------------------------------



#endif
