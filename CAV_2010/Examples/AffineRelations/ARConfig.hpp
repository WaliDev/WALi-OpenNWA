//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2007, Gogul Balakrishnan
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
// e-mail: bgogul@cs.wisc.edu
//
//////////////////////////////////////////////////////////////////////////////


#ifndef __A_R_CONFIG_H
#define __A_R_CONFIG_H

#include <string>
#include <vector>
#include <algorithm>
#include "ref_ptr.h"

#define arEAX   1  ///< Variable number for register eax.
#define arEBX   2  ///< Variable number for register ebx.
#define arECX   3  ///< Variable number for register ecx.
#define arEDX   4  ///< Variable number for register edx.
#define arESI   5  ///< Variable number for register esi.
#define arEDI   6  ///< Variable number for register edi.
#define arEBP   7  ///< Variable number for register ebp.
#define arESP   8  ///< Variable number for register esp.
#define arEAX0  9  ///< Variable number for register eax0.
#define arEBX0  10 ///< Variable number for register ebx0.
#define arECX0  11 ///< Variable number for register ecx0.
#define arEDX0  12 ///< Variable number for register edx0.
#define arESI0  13 ///< Variable number for register esi0.
#define arEDI0  14 ///< Variable number for register edi0.
#define arEBP0  15 ///< Variable number for register ebp0.
#define arESP0  16 ///< Variable number for register esp0.
#define arLAST  8  ///< The last variable number.

/// Number of variables for affine relations. 
#define NVARS          (arLAST)

namespace AR {

    /// The array of variable names for rendering affine relations.
    extern char* arVarNames[];

    // ClassIDs associated with the registers used in affine relations.
    // (Same order as the list of names above).
    //extern int arVarClassIDs[];


	typedef unsigned dim_t;
	const dim_t dim = NVARS + 1;
	extern unsigned long long nVarDist[AR::dim]; ///< Distribution of the number of variables in affine relations
	                                           ///< (Gives an idea of how complex the affine rels are).
	
	extern unsigned long long nDimDist[AR::dim]; ///< Distribution on the dimensionality of the vector space.

	class NameSpace {
	public:
		NameSpace();

		/// constructor
		NameSpace(dim_t _nNames, char* _names[]);

		/// copy constructor
		NameSpace(const NameSpace& rhs) : 
            /*syms(rhs.syms),*/ names(rhs.names), count(0) {}

		/// assignment
		NameSpace& operator =(const NameSpace& rhs) {
			if(this == &rhs)
				return *this;
			//syms  = rhs.syms;
			names = rhs.names;
			return *this;
		}

		void clear();

		// Get the textual representation of sym
		//static std::string getName(Symbol* sym);

		// add a symbol to the namespace
		//void add(Symbol* sym);

		// add a name to the namespace (set corresponding symbol to NULL)
		void add(const std::string& name);

		/// Get the index for the given symbol. 
		/// Return -1 if not found.
		////int getIndex(const Symbol* sym) const;

		/// get the index for the given name.
		int getIndex(const std::string& name) const;

		/// Get the aloc at the given index.
        //Symbol* getAloc(dim_t n) const;

		/// Get the name of the aloc at the given index
		std::string operator [](dim_t n) const;
				
		/// destructor
		~NameSpace() {}

		/// Get the dimension of the namespace;
		dim_t getDimension() const { 
            return names.size();
        }

		RefCounter count; // For ref_ptr class
	private:
		//std::vector<Symbol*>     syms;
		std::vector<std::string> names;
#if _MSC_VER > 1200
    public:
#endif
		//static Symbol* unknownSym;
	};
	typedef ref_ptr<NameSpace> NameSpacePtr;

	extern NameSpacePtr x86RegNames;


	const unsigned VER_WPDS       = 0;
	const unsigned VER_EWPDS      = 1;
	const unsigned VER_WPDS_CHEAT = 2;		
	const unsigned VER_NR         = 3;		
	
	extern unsigned whichWpdsVersion;
#ifdef USE_EWPDS
	extern bool constructingEWPDS;
#endif

	void initialize();
} // namespace

#define VSA_RESTORE_REGS_IN_AFFREL 1


#endif
