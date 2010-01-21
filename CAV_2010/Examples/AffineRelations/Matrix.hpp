//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2007, Gogul Balakrishnan, Akash Lal, Thomas Reps
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
// e-mail: bgogul@cs.wisc.edu, akash@cs.wisc.edu, reps@cs.wisc.edu
//
//////////////////////////////////////////////////////////////////////////////


#ifndef __MATRIX_H
#define __MATRIX_H


#include <stdio.h>
#include "HashMap.h"
#include <climits>
#include <iostream>

namespace AR {
    class Matrix;
    class MatrixPair;

    //----------------------------------------
    /// Matrix w/ reference counting.
    //----------------------------------------
    class RCMatrix {
        private:
            ///
            unsigned N;    // = AR::dim;
            unsigned NbyN; // = N*N;

        public:
            typedef unsigned long MatrixHkey_t;
            static MatrixHkey_t computeHashKey(const int*, unsigned _N);	// Compute hash of the matrix

            void decrRef();	// Decrease ref count
            void incrRef();	// Increase ref count

            const int* getMatrix() const { 
                return mat; 
            }

            static unsigned getCnclTableSize() {
                return cnclMats->size();
            }

            static RCMatrix* getCnclMatrix(const int*, unsigned _N);		// Return the canonical matrix
            static void deleteCnclMatrices();

        private:


            MatrixHkey_t key;	// Hash Key

            int* mat;			// Matrix
            unsigned count;		// Reference count
            bool satRef;		// Reference count has saturated?


            // Constructor
            RCMatrix(const int* _mat, unsigned _N):count(0), satRef(false), N(_N), NbyN(_N*_N)
        {mat = copyIntArray( _mat ); key = computeHashKey(mat, N);}	

            // Destructor
            ~RCMatrix()					
            { if(mat) delete [] mat; }

            inline int * copyIntArray( const int *p )	
            {			
                int *copy = new int[NbyN];
                for( int i=0; i < NbyN ; i++ )
                    copy[i] = p[i];
                return copy;
            }

            typedef std::pair<const int*, unsigned> MatrixDimPair_t;

            //
            // Canonical matrix table
            //
            struct EqArr {
                bool operator()(const MatrixDimPair_t& l, const MatrixDimPair_t& r ) const {
                    // are the dimensions equal?
                    if(l.second != r.second)
                        return false;

                    unsigned NSq = l.second * l.second;
                    for( unsigned i = 0 ; i < NSq ; i++ )
                        if( l.first[i] != r.first[i] )
                            return false;
                    return true;
                }
            };

            struct HashArr {
                unsigned long operator()(const MatrixDimPair_t& rcm) const {
                    return computeHashKey(rcm.first, rcm.second);
                }
            };
            typedef wpds::HashMap<MatrixDimPair_t, RCMatrix*, HashArr, EqArr> CnclMats;

            static CnclMats* cnclMats;		// Canonical matrix table

            // Friends
            friend struct EqArr;
            friend class Matrix;
            friend class MatrixPair;
    }; // class RCMatrix

    // Pair of matrices
    class MatrixPair {
        public:
            typedef unsigned long MatrixPairHkey_t;
            typedef unsigned long MatrixPairUsage_t;
#define	MATRIX_PAIR_MAX_USAGE_CNT UINT_MAX

            inline MatrixPair(RCMatrix* _one, RCMatrix* _two):mOne(_one), mTwo(_two), usageCnt(1)
        {mOne->incrRef(); mTwo->incrRef();}
            inline MatrixPair(const MatrixPair& rhs):mOne(rhs.mOne), mTwo(rhs.mTwo), usageCnt(rhs.usageCnt)
        {mOne->incrRef(); mTwo->incrRef();}
            inline ~MatrixPair() 
            {mOne->decrRef(); mTwo->decrRef();}
            inline MatrixPair& operator=(const MatrixPair& rhs) 
            {
                if (this != &rhs) { // watch for aliasing
                    mOne = rhs.mOne;
                    mTwo = rhs.mTwo;
                    usageCnt = rhs.usageCnt; 
                    mTwo->incrRef();
                    mOne->incrRef();
                }
                return *this;
            }

            inline MatrixPairHkey_t unresHash() const 
            { return  ((unsigned)mOne << 16) | ((unsigned)mTwo & 0xffff);
                //return 997 * ((unsigned)mOne >> 2) + ((unsigned)mTwo >> 2); 
            }
            inline MatrixPairHkey_t Hash(unsigned int k)  const 
            { return (unresHash() % k); }
            inline bool operator == (const MatrixPair& rhs) const  
            { return rhs.mOne == mOne && rhs.mTwo == mTwo;}
            inline void resetUsageCnt() 
            { usageCnt = 0;}
            inline void incUsageCnt() { 
                if(usageCnt != MATRIX_PAIR_MAX_USAGE_CNT)
                    usageCnt++;
            }
            inline static void incUsageCnt(MatrixPairUsage_t& cnt) {
                if(cnt != MATRIX_PAIR_MAX_USAGE_CNT)
                    cnt++;
            }
            inline MatrixPairUsage_t getUsageCnt() { 
                return usageCnt;
            }


        private:
            RCMatrix* mOne;
            RCMatrix* mTwo;
            MatrixPairUsage_t usageCnt;
    }; // class MatrixPair



    std::ostream& operator<< (std::ostream & out, const Matrix &m);

    // Wrapper class
    class Matrix {
        public:
            Matrix() {rcmat = NULL;}
            Matrix(unsigned N);
            inline explicit Matrix(const int* _mat, unsigned N)
            {rcmat = RCMatrix::getCnclMatrix(_mat, N); nMats++;}
            inline explicit Matrix(const Matrix* rhs) 
            {rcmat = rhs->rcmat; rcmat->incrRef(); nMats++;}
            inline Matrix(const Matrix& rhs) 
            {rcmat = rhs.rcmat; rcmat->incrRef(); nMats++;}
            inline explicit Matrix(RCMatrix* rcm) 
            {rcmat = rcm; rcmat->incrRef(); nMats++;}
            inline ~Matrix() 
            {rcmat->decrRef();}
            inline Matrix& operator= (const Matrix& rhs) 
            {
                if (this != &rhs) { // watch for aliasing
                    if(rcmat) 
                        rcmat->decrRef();
                    rcmat = rhs.rcmat;
                    rcmat->incrRef();
                    nMats++;
                }
                return *this;
            }

            inline bool operator ==(const Matrix& rhs) const	// operator ==
            { return rcmat == rhs.rcmat;}	
            inline const int* getMatrix() const		// get the integer matrix pointer
            {return rcmat->mat;}
            inline int operator[](unsigned x) const		        // get the xth element
            {return rcmat->mat[x];}
            inline int operator()(unsigned r, unsigned c) const	// get the [r,c]th element
            {return rcmat->mat[r * rcmat->N + c];}
            inline unsigned getDim() const {return rcmat->N;}
            void Print(std::ostream & out) const;
            void prettyPrint(FILE* fp) const;
            static Matrix mkId(unsigned N);
            static Matrix transpose(const Matrix& m);
            static Matrix add(const Matrix& mat1, const Matrix& mat2);
            static Matrix subtract(const Matrix& mat1, const Matrix& mat2);
            static Matrix multiply(const Matrix& mat1, const Matrix& mat2);
            static void printMatrixStats(FILE* fp);
            static void clearCaches();
        private:
            RCMatrix* rcmat;
            static void multiplyMatrices(int*, const int* mat1, const int* mat2, unsigned N);
            static unsigned int cacheHits, cacheAccesses, nMats;
        public:
            struct EqPair {
                bool operator ()(const MatrixPair& l, const MatrixPair& r) const {
                    return l == r;
                }
            };
            struct HashPair {
                unsigned long operator () (const MatrixPair& l) const {
                    return l.unresHash();
                }
            };

            typedef wpds::HashMap<MatrixPair, RCMatrix*, HashPair, EqPair> ProdCache;

            static const ProdCache::size_type maxProdCacheSize;  // Max size of cache
            static const float cacheEntryThreshold;              // Min fraction of accesses 
            static MatrixPair::MatrixPairUsage_t transAccessCnt; //
            static ProdCache* prodCache;

            friend class MatrixPair;
    }; // class Matrix

} // namespace AR

#endif

