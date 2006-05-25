//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2002-2004, Gogul Balakrishnan, 
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
//////////////////////////////////////////////////////////////////////////////
#ifndef __Z_A_S_SEMIRING_H
#define __Z_A_S_SEMIRING_H

#include "VectorSpace.h"
#include <iostream>

//----------------------------------
// For Nick's wpds implementation
//----------------------------------
//-----------------------------------
// Semiring class for wpds
//-----------------------------------

class VSSemiring {
public:
	//---------------------
	// Constructors 
	//---------------------
	VSSemiring(VectorSpace *,unsigned=0);	
	~VSSemiring();

	//-----------------------------
	// semiring one and zero
	//-----------------------------
	static VSSemiring *one();
	static VSSemiring *zero();

	//---------------------------------
	// semiring operations
	//---------------------------------
	VSSemiring *extend(VSSemiring *) ;
	VSSemiring *combine(VSSemiring *);
	bool		equal(VSSemiring *) const;

	VSSemiring*  quasiOne() const;
#ifdef DWPDS
	VSSemiring *diff(VSSemiring *) const;
#endif
	//----------------------------
	// Reference count
	//-----------------------------
	unsigned int count;
	
	//------------------------------------
	// output
	//------------------------------------
	std::ostream & print(std::ostream &) const;

	VectorSpace *vs();

    VSSemiring* parse_element( const char* buf );
private:
	static VSSemiring *_zero;
	static VSSemiring *_one;
	VectorSpace *p_vs;
};

#endif
