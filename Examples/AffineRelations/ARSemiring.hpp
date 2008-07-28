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


#ifndef AR_ARSEMIRING_GUARD
#define AR_ARSEMIRING_GUARD

#include "ModuleSpace.hpp"
#include <iostream>

//----------------------------------
// For Nick's wpds implementation
//----------------------------------
//-----------------------------------
// ARSemiring class for wpds
//-----------------------------------

namespace AR {

    class ARSemiring {
        public:
            // toggle exact equality
            static bool exact_equality;

            //---------------------
            // Constructors 
            //---------------------
            ARSemiring(ModuleSpace* , unsigned = 0);	
            ~ARSemiring();

            //-----------------------------
            // semiring one and zero
            //-----------------------------
            static ARSemiring* one();
            static ARSemiring* zero();

            //---------------------------------
            // semiring operations
            //---------------------------------
            ARSemiring* extend(ARSemiring *) ;
            ARSemiring* combine(ARSemiring *);
            bool		equal(ARSemiring *) const;

            ARSemiring* quasiOne() const;
            ARSemiring* diff(ARSemiring *) const;

            //----------------------------
            // Reference count
            //-----------------------------
            RefCounter count;

            //------------------------------------
            // output
            //------------------------------------
            std::ostream & print(std::ostream &) const;

            /// Return the pointer 
            /// NOTE: It is dangerous to lose control of a pointer, when it is
            /// ref counted. The safe thing would be to return "const ModuleSpace*" here. 
            /// However, most of the ModuleSpace functions (even queries like isEmpty()) 
            /// need to invoke findBasis() that changes the object. Therefore, it is not 
            /// possible to return "const ModuleSpace*" here! Be careful about how you use 
            /// the pointer.
            ///
            ModuleSpace *vs();
        private:
            static ARSemiring *_zero;
            static ARSemiring *_one;
            typedef ref_ptr<ModuleSpace> ModuleSpacePtr;
            ModuleSpacePtr p_vs;

    }; // class ARSemiring

} // namespace AR

#endif  // AR_SEMIRING_GUARD
