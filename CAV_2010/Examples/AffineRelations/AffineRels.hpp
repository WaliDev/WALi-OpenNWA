//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2002-2005, Gogul Balakrishnan, 
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
#ifndef __AFFINE_RELS_H
#define __AFFINE_RELS_H

#include <stdio.h>
#include <vector>
#include "ARConfig.hpp"
#include "ModuleSpace.hpp"
#if 0 //def USE_AFFINERELS_CLASS_WITH_VSA
#	include "AffineRelsVsaMacros.h"
#endif

namespace AR {

    enum StackDeltaError { CONSTVAL, NONCONSTVAL, UNREACHABLE, APPROX };

    //----------------------------------
    /// A class that holds a set of
    /// affine relations.
    //----------------------------------
    class AffineRels {
        public:
            typedef int  AffRelCoeff_t;
            typedef int* AffRel_t;


            /// Default constructor.
            AffineRels();	

            /// Construct an AffineRels which is NullSpace(*pVS).
            AffineRels(ModuleSpace* pVs);

            /// Destructor.
            ~AffineRels();

            /// Clear the set.
            void clear();

            /// Copy the set of affine relations in rhs.
            void copy(AffineRels* rhs);

            /// Return the number of relations.
            unsigned nrOfRels() const;

            /// Insert the given affine relation.
            void insert(AffRel_t affrel);

            /// Get the affine relation at given index.
            AffRel_t getAffineRel(unsigned int relNr) const;

            /// Return whether the relation has a non-zero coefficient for the variable.
            bool hasVar(int relNr,int varNr) const;

            /// Return whether the relation has non-integer coefficients.
            bool hasNonIntegerCoeffs(int relNr) const;

            /// The number of variables with non-zero coefficients (excludes const).
            unsigned nNonZeroCoeffs(int relNr) const;

            /// Print the set of affine relations with given variable names.  
            void prettyPrint(FILE* fp, char** varNames) const;

            /// Print the set of affine relations with given variable names. 
            std::ostream& prettyPrint(std::ostream &out, char** varNames) const;

            /// Update global stats about affine relations.
            void updateARStats() const;

            /// A vector space = span{I}.
            static ModuleSpace* idspace;		


#ifdef USE_AFFINERELS_CLASS_WITH_VSA
            /// Get the affine relations for the given node.
            static AffineRels* getAffineRels(nCFGnode* );

            /// Compute affine relations for control nodes in program.
            static void initAffineRelations();
#endif

        private:


            /// A set of affine relations. 
            /// \note A single affine relation is represented by an 
            /// array of coefficients. 
            std::vector<AffRel_t> affRels; 


    }; // class AffineRels


} // namespace AR

#endif
