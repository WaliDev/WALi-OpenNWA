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


#ifndef AR_MODULE_SPACE_GUARD
#define AR_MODULE_SPACE_GUARD

#include <cstdio>

#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include "ref_ptr.h"

#define VSA_ARA_USE_CNCL_MATRICES 

#define MAX_POWER 32
// a * 2^w
#define MULT_BY_POWER_OF_2(a,w) (((w) == MAX_POWER) ? 0 : ((a) << (w)))
// a / 2^w
// Note: while using this to find the invertible part of a number, 
// --> 0 = 2^32  * 1 <--
// and this macro should not be used
#define DIV_BY_POWER_OF_2(a,w) (((w) == MAX_POWER) ? 0 : ((a) >> (w)))

#ifdef VSA_ARA_USE_CNCL_MATRICES
#include "Matrix.hpp"
#endif

namespace AR {

    class AffineRels;

    //#include "swyx/src/common/bit_set.hpp"

    // Find the highest power (r) of 2 dividing n
    inline unsigned int compute_rank(int x) {
        unsigned int y;
        unsigned int n;

        if (x == 0) return MAX_POWER;
        n = 31;
        y = x << 16; if (y != 0) {n = n - 16; x = y; }
        y = x << 8;  if (y != 0) {n = n -  8; x = y; }
        y = x << 4;  if (y != 0) {n = n -  4; x = y; }
        y = x << 2;  if (y != 0) {n = n -  2; x = y; }
        y = x << 1;  if (y != 0) {n = n -  1; }
        return n;
    }


    // find the multiplicative inverse
    unsigned int multiplicative_inverse(unsigned int d);


    //-------------------------------------------------
    //
    //-------------------------------------------------

    struct ModularMatrix {
#ifdef VSA_ARA_USE_CNCL_MATRICES
        Matrix m;
#else
        int *m;
#endif

        const int operator [] (const int indx) const{
            return m[indx];
        }
        unsigned int leading_index;
        unsigned int leading_rank;

        // Methods
        ModularMatrix(const ModularMatrix &_m) {
            m = _m.m;
            leading_index = _m.leading_index;
            leading_rank = _m.leading_rank;
        }
        ModularMatrix operator =(const ModularMatrix &_m) {
            if (this != &_m) {
                m = _m.m;
                leading_index = _m.leading_index;
                leading_rank = _m.leading_rank;
            }
            return *this;
        }
#ifdef VSA_ARA_USE_CNCL_MATRICES
        ModularMatrix(const Matrix &_m, int n) {
            m = _m;
            int i=0;
            while(i < n && m[i] == 0) {
                i++;
            }
            if(i==n) {
                leading_index = n;
                leading_rank = MAX_POWER;
            } else {
                leading_index = i;
                leading_rank = compute_rank(m[i]);
            }
        }
        ModularMatrix(const Matrix &_m, unsigned int _li, unsigned int _lr) {
            m = _m;
            leading_index = _li;
            leading_rank = _lr;
        }
#else
        ModularMatrix(int *_m, int n) {
            m = _m;
            int i=0;
            while(i < n && m[i] == 0) {
                i++;
            }
            if(i==n) {
                leading_index = n;
                leading_rank = MAX_POWER;
            } else {
                leading_index = i;
                leading_rank = compute_rank(m[i]);
            }
        }
        ModularMatrix(int *_m, unsigned int _li, unsigned int _lr) {
            m = _m;
            leading_index = _li;
            leading_rank = _lr;
        }
#endif
    };


    struct ModularMatrixInt {
        int *m;
        unsigned int leading_index;
        unsigned int leading_rank;

        ModularMatrixInt(const ModularMatrixInt &_m) {
            m = _m.m;
            leading_index = _m.leading_index;
            leading_rank = _m.leading_rank;
        }
        ModularMatrixInt operator =(const ModularMatrixInt &_m) {
            if (this != &_m) {
                m = _m.m;
                leading_index = _m.leading_index;
                leading_rank = _m.leading_rank;
            }
            return *this;
        }

        ModularMatrixInt(int *_m, int n) {
            m = _m;
            int i=0;
            while(i < n && m[i] == 0) {
                i++;
            }
            if(i==n) {
                leading_index = n;
                leading_rank = MAX_POWER;
            } else {
                leading_index = i;
                leading_rank = compute_rank(m[i]);
            }
        }
        ModularMatrixInt(int *_m, unsigned int _li, unsigned int _lr) {
            m = _m;
            leading_index = _li;
            leading_rank = _lr;
        }

    };

    class ModularTransformer;
    //-------------------------------------------------
    //
    //-------------------------------------------------

    class ModuleSpace {

        friend class ModularTransformer;
        friend class ModularVectors;
        private:
        // Some types
        typedef std::vector<ModularMatrix>                     MatrixSet;
        typedef std::vector<ModularMatrix>::iterator           MSIterator;
        typedef std::vector<ModularMatrix>::const_iterator     MSConstIterator;
        typedef std::vector<ModularMatrix>::reverse_iterator   MSReverseIterator;

        typedef std::vector<ModularMatrixInt>                     MatrixSetInt;
        typedef std::vector<ModularMatrixInt>::iterator           MSIntIterator;
        public:


        //
        // Constructors and destructors
        //
        ModuleSpace(int m, bool id=false);   // Creates an empty module space of m x m Matrices
        ModuleSpace(const ModuleSpace& op2);
        ~ModuleSpace();      // Destructor

        //
        // Properties of the affine space
        //
        bool isEmpty();               // Is this vector space empty?
        bool isEqual(ModuleSpace *op2);     // Are the vector spaces this and op2 equal?
        unsigned getDimensionOfPointSpace() {return N;}

        //
        // Operations 
        //
        ModuleSpace *compose(ModuleSpace *op2);   // new=op2 (o) this
        ModuleSpace *join(ModuleSpace *op2);      // new=op2 |_| this
        ModuleSpace *diff(ModuleSpace *op2);      // new=this - op2
#ifdef USE_GMM
        ModuleSpace *quasione();                  // new = quasioneValue(this)
#endif
        void setIdspace();                        // Set to the identity space, i.e., {Id}
        // the space generated by the identity basis


        // Exact operations for checking equality and subset
        bool isExactEqual(ModuleSpace *op2);
        static bool isExactEqual(std::vector<const int *> &lhs, std::vector<const int *> &rhs, int vec_size);
        bool isExactSubspace(ModuleSpace *op2);                           // Exact subspace operation
        static bool isExactSubspace(std::vector<const int *> &lhs, std::vector<const int *> &rhs, int vec_size);

        // Back-end for the above operations
        int isStrictSubspace(ModuleSpace *op2);
        static int isStrictSubspace(std::vector<const int *> &lhs, std::vector<const int *> &rhs, int vec_size);

        // Operation on NullSpace of matrices
        int isStrictNullSubspace(ModuleSpace *op2);

        // call-site transformer
        ModuleSpace *call_site_transform(const int *m1, const int *m2);
#ifdef VSA_ARA_USE_CNCL_MATRICES 
        ModuleSpace* callerSaveXform(int saveReg, int restReg) const;
        ModuleSpace* call_site_transform(const Matrix &m1, const Matrix &m2);
#endif

        void insertMatrix(const ModularMatrix &); // Insert the NxN matrix pointed. Allocates memory for matrix
        ModularMatrix copyModularMatrix(const ModularMatrix &);  // Allocates memory for matrix
#ifdef VSA_ARA_USE_CNCL_MATRICES 
        ModularMatrix createModularMatrix(const Matrix &);
        void insertMatrix(const Matrix &);        // Insert the NxN matrix pointed. Allocates memory for matrix
        void insertMatrix(const int *);           // Insert the NxN matrix pointed. Allocates memory for matrix
#else
        ModularMatrix createModularMatrix(const int *);
        void insertMatrix(const int *);           // Insert the NxN matrix pointed. Allocates memory for matrix
#endif

        void findBasis(MatrixSet *p=0);       // Find the basis 
        static void findBasis(MatrixSetInt *pBasis, int vec_size, int clear_mem=true);
        static void findBasis(std::vector<int *> &pBasis, int vec_size, int clear_mem=true); // vector interface

        void findAffineRelations(AffineRels &affRels);      // Find the affine relationships that hold
        bool isVarConstant(unsigned int var, int &constval);// Find if a variable has a constant value
        int expressionPrecondition(AffineRels &in, AffineRels &value, AffineRels &cond);

        // Ref counting
        RefCounter count;

        ///////////////////////////
        // Output
        ///////////////////////////
        void prettyPrint(FILE *fp, unsigned nTabs = 0);              // Pretty print the basis vectors
        void prettyPrint(std::ostream &out);                         // Pretty print the basis vectors
        void printAsText(std::ostream &out);                         // Pretty print the basis vectors as text
#ifdef ___TSL
        void prettyPrint_wo_elimination(std::ostream &out);          // Pretty print the basis vectors
#endif

        unsigned int moduleRank();

        static unsigned int curMemUsage;
        static unsigned int maxMemUsage;

#ifdef ___TSL
        ModuleSpace *compose_wo_elimination(ModuleSpace *op2);   // new=op2 (o) this
        ModuleSpace *join_wo_elimination(ModuleSpace *op2);      // new=op2 |_| this
        std::vector<std::vector<int> > multiply(std::vector<int> col);

        // temporary: to compare results to Akash's
        static ref_ptr<ModuleSpace> to9by9(const ref_ptr<ModuleSpace> & a);

        MatrixSet getBasis() const { return basis; }
#endif

        private:
        ///////////////////////////
        //Data members
        ///////////////////////////
        int N;                    // Dimesion of the matrices 
        int Nsq;                  // N*N

        MatrixSet basis;          // The set of matrices that constitute the basis
        // Empty set implies the zero vector space 

        bool basisUptodate;       // Is the basis set up-to-date?

        ///////////////////////////
        // Helper methods
        ///////////////////////////
        int* findNullSpace();
#ifdef VSA_ARA_USE_CNCL_MATRICES
        Matrix multiplyMatrices(const Matrix& op1, const Matrix& op2) const;
#endif 
        int *multiplyMatrices(const int *op1, const int *op2) const;      // Multiplies the NxN matrix op1 by the NxN 
        bool isSubspace(ModuleSpace *op2);                                // Is op2 a subspace of this? 
        bool isExactNullSubspace(ModuleSpace *op2);
        bool printMatAsText(const int *mat, std::ostream &out);
        bool printMatColumnAsText(const int *mat, int col, std::ostream &out);

        void convertBasisToConstInt(std::vector<const int *> &b);
        // modular_svd is not thread-safe. It access a static member svd_vec_size
        static void modular_svd(int *A, int *Rinv, int *Linv, int A_vec_size);
        static void col_swap(int *A, int pos, int c1, int c2);
        static void col_mult(int *A, int pos, int col, int d);
        static void col_sub(int *A, int pos, int col1, int col2, int x);
        static void row_swap(int *A, int pos, int r1, int r2);
        static void row_mult(int *A, int pos, int row, int d);
        static void row_sub(int *A, int pos, int row1, int row2, int x);
        static void find_pivot(int *A, int pos, int &pivot_posi, int &pivot_posj);
        static bool is_smallest_row(int *A, int pos, int &i, int &j);
        static bool is_smallest_col(int *A, int pos, int &i, int &j);
        static int svd_vec_size;

        // 
        // Memory management routines
        //
        static int *allocMemBlock(int);
        static void deallocMemBlock(int *, int);

        //MatMemMgr<int> *memMgr;

    };

} // namespace AR



#endif  // AR_MODULE_SPACE_GUARD


