//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2007, Gogul Balakrishnan, Akash Lal
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
// e-mail: bgogul@cs.wisc.edu, akash@cs.wisc.edu
//
//////////////////////////////////////////////////////////////////////////////


#pragma warning (disable: 4786)

#include "Semiring.hpp"
#include "ModuleSpace.hpp"
#include "AffineRels.hpp"

//-------------------------------------------
// For Nick's wpds implementation
//--------------------------------------------
//---------------------------------------------------------------
// semiring for wpds
//----------------------------------------------------------------
VSSemiring* VSSemiring::_one=new VSSemiring(new ModuleSpace(AR::dim,true), 1);
VSSemiring* VSSemiring::_zero=new VSSemiring(new ModuleSpace(AR::dim), 1);

//--------------------------------------
// Constructor
//--------------------------------------
VSSemiring::VSSemiring(ModuleSpace *_p_vs,unsigned c):p_vs(_p_vs), count(c) {

}

//--------------------------------------
// Destructor
//--------------------------------------
VSSemiring::~VSSemiring() {
}

//-----------------------------------------
// return the zero element of the semring
//-----------------------------------------
VSSemiring *VSSemiring::zero() {
	return _zero;
}

//-----------------------------------------
// return one of the semring
//-----------------------------------------
VSSemiring *VSSemiring::one() {
	return _one;
}

//----------------------------
//
//----------------------------
ModuleSpace *VSSemiring::vs() {
	// FIXME: It is dangerous to lose control of a pointer, when it is
	// ref counted. The safe thing would be to return "const ModuleSpace*" here. 
	// However, most of the ModuleSpace functions (even queries like isEmpty()) 
	// need to invoke findBasis() that changes the object. Therefore, it is not 
	// possible to return "const ModuleSpace*" here! Be careful about how you use 
	// the pointer.
	return p_vs.get_ptr();
}

//-----------------------------------------
// extend
//-----------------------------------------
VSSemiring *VSSemiring::extend(VSSemiring *op2) {
	
	/*
	if(op2 == VSSemiring::_one) {
		if( this == VSSemiring::_one )
			return this;
		else
			return new VSSemiring(new ModuleSpace(*this->p_vs));
	}

	if(this == VSSemiring::_one) {
		if( op2 == VSSemiring::_one )
			return op2;
		else
			return new VSSemiring(new ModuleSpace(*op2->p_vs));
	}
	*/
	return new VSSemiring(op2->p_vs->compose(p_vs.get_ptr()));
}

//-----------------------------------------
// combine
//-----------------------------------------
VSSemiring *VSSemiring::combine(VSSemiring *op2) {
	/*
	if( op2 == VSSemiring::_zero ) {
		if( this == VSSemiring::_zero )
			return this;
		else
			return new VSSemiring(new ModuleSpace(*this->p_vs));
	}
	
	if( this == VSSemiring::_zero ) {
		if( op2 == VSSemiring::_zero )
			return op2;
		else
			return new VSSemiring(new ModuleSpace(*op2->p_vs));
	}
	*/
	return new VSSemiring(this->p_vs->join(op2->p_vs.get_ptr()));
}


VSSemiring* VSSemiring::quasiOne() const{
	return VSSemiring::one();
}

#ifdef DWPDS
//---------------------------------------------
// this-op2
//---------------------------------------------

VSSemiring *VSSemiring::diff(VSSemiring *op2) const {
	ModuleSpace* diffValue=this->p_vs->diff(op2->p_vs.get_ptr());	
	return new VSSemiring(diffValue);
}

#endif

//-----------------------------------------
// are the two elements equal?
//-----------------------------------------
bool VSSemiring::equal(VSSemiring *op2) const {
	bool isequal=p_vs->isEqual(op2->p_vs.get_ptr());
	return isequal;
}




//-----------------------------------------
// print to the given ostream
//-----------------------------------------
std::ostream &VSSemiring::print(std::ostream &os) const {
	if(this==_one) {
		os << "O #\n";
		return os;
	}
	if(this==_zero) {
		os << "Z #\n";
		return os;
	}
	p_vs->prettyPrint(os);
	return os;
}
