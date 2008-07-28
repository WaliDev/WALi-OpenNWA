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


#include "AffineRels.hpp"
#include "ARConfig.hpp"
#include "Matrix.hpp"

namespace AR {

    // Variable Names for use in rendering the affine relations.
    char* arVarNames[] = {"1","eax","ebx","ecx","edx","esi","edi","ebp","esp"};

    // ClassIDs associated with the registers used in affine relations.
    // (Same order as the list of names above).
    //int arVarClassIDs[] = {0,
    //    ID_register_eax, ID_register_ebx, ID_register_ecx, ID_register_edx,
    //    ID_register_esi, ID_register_edi, ID_register_ebp, ID_register_esp};


    char* regNames[] = {"eax","ebx","ecx","edx","esi","edi","ebp","esp"};

    //char *arVarNames[]={"1","eax","ebx","ecx","edx","esi","edi","ebp","esp",
    //                        "eax0","ebx0","ecx0","edx0","esi0","edi0","ebp0","esp0"};


	/// Distribution of the number of variables in affine relations
	/// (Gives an idea of how complex the affine rels are).
	unsigned long long nVarDist[AR::dim] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; 
	                                         
	/// Distribution on the dimensionality of the vector space.
	unsigned long long nDimDist[AR::dim] = {0, 0, 0, 0, 0, 0, 0, 0, 0};


	// x86RegNames
	NameSpacePtr x86RegNames;
	
	void initialize() {
		x86RegNames = new NameSpace(AR::dim - 1, regNames);
		return;
	}
	

	unsigned whichWpdsVersion = VER_EWPDS;
#ifdef USE_AKASH_EWPDS
	bool constructingEWPDS = false;
#endif
    //Symbol* NameSpace::unknownSym = (Symbol*)1;
	//---------------------------------
	/// Default constructor.	
	//---------------------------------
	NameSpace::NameSpace() {
        //syms.push_back(NULL);
		names.push_back("CONST$");
	}

	void NameSpace::clear() {
		//syms.clear();
		names.clear();
		//syms.push_back(NULL);
		names.push_back("CONST$");
	}

	/// constructor
	NameSpace::NameSpace(dim_t _nNames, char* _names[]):count(0) {
		//syms.push_back(NULL);
		names.push_back("CONST$");
		for(dim_t i = 0; i < _nNames; ++i) {
			//Symbol* sym = GlobalSymbolTable->LookupSymbol(_names[i]);
			//assert(sym);
			//syms.push_back(sym);
			names.push_back(_names[i]);
		}
	}

//    std::string NameSpace::getName(Symbol* sym)  {
//		std::string name(sym->GetNameNoBaseIndex());
//		if(sym->GetBaseIndex()) {
//			name += MAKE_STRING("$" << sym->GetBaseIndex());
//		}
//		return name;
//	}
//	

//---------------------------------
/// add symbol
//---------------------------------
//    void NameSpace::add(Symbol* sym)  {
//		assert(sym != unknownSym);		
//		// not already there?
//		if(getIndex(sym) == -1) {
//			//syms.push_back(sym);
//			names.push_back(getName(sym));
//		}
//		return;
//
//	}

	//---------------------------------
	/// add name
	//---------------------------------
	void NameSpace::add(const std::string& name)  {
		// not already there?
		if(getIndex(name) == -1) {
			//syms.push_back(unknownSym);
			names.push_back(name);
		}
		return;

	}

    /*
	/// Get the index for the given symbol. 
	/// Return -1 if not found.
	int NameSpace::getIndex(const Symbol* sym) const {
		assert(sym != unknownSym);
		std::vector<Symbol*>::const_iterator elem = 
			std::find(syms.begin(), syms.end(), sym);

		// not there?
		if(elem == syms.end()) 
			return -1;
		else
			return elem - syms.begin();
	}
    */


	/// Get the index for the given name.
	/// Return -1 if not found.
	int NameSpace::getIndex(const std::string& name) const {
		std::vector<std::string>::const_iterator elem = 
			std::find(names.begin(), names.end(), name);

		// not there?
		if(elem == names.end()) 
			return -1;
		else
			return elem - names.begin();
	}

    /*
	/// Get the aloc at the given index.
	Symbol* NameSpace::getAloc(dim_t n) const {
		assert(n >= 0 && n < syms.size()); 
		return syms[n];
	}
    */

	/// Get the name of the aloc at the given index
	std::string NameSpace::operator [](dim_t n) const {
		assert(n >= 0 && n < names.size()); 
		return names[n];
	}

} // namespace AR
