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


#pragma warning (disable: 4786)
#pragma warning (disable: 4005)
#pragma warning (disable: 4099)
#pragma warning (disable: 4800)

#include <cstdio>
#include "ModuleSpace.hpp"
#include "ARConfig.hpp"
#include "AffineRels.hpp"
#include <cmath>

namespace AR {

    unsigned int ModuleSpace::curMemUsage = 0;
    unsigned int ModuleSpace::maxMemUsage = 0;
    int ModuleSpace::svd_vec_size = 0;

    //
    // Access given matrix as a linear array
    //
#ifdef VSA_ARA_USE_CNCL_MATRICES
#define MATRIX_PTR_OF(m) (m.getMatrix())
#define linearAccess(m, indx)   ((m)[(indx)])
    typedef Matrix* RemoveElems_t;
    void inline deallocMatrix(ModularMatrix& m) { }
    void inline deallocMatrix(Matrix& m) { }

#else
#define MATRIX_PTR_OF(m) (m)
#define linearAccess(m, indx)   ((m)[(indx)])
    typedef int* RemoveElems_t;
    void inline deallocMatrix(ModularMatrix &m) {delete []m.m;}
    void inline deallocMatrix(int* m) {delete []m;}

#endif

    void inline deallocMatrix(ModularMatrixInt& m) {
        delete []m.m;
    }


    bool debug_module_space = false;
#define MODULE_DBGS(stmts) do{ if(debug_module_space) { stmts; } }while(0)
    std::ostream& modDbgFile = std::cerr;

    void print_mat(const int *A, int n, const char *s) {
        modDbgFile<<s<<":\n";
        int i,j,k=0;
        for(i=0;i<n;i++) {
            for(j=0;j<n;j++,k++) {
                modDbgFile<<A[k]<<" ";
            }
            modDbgFile<<"\n";
        }
        modDbgFile.flush();
    }

#ifdef VSA_ARA_USE_CNCL_MATRICES
    void print_mat(Matrix &A, int n, const char *s) {
        print_mat(A.getMatrix(),n,s);
    }
#endif

    ////////////////////////////////////////////////////
    // M E T H O D S
    ////////////////////////////////////////////////////

    //-----------------------------------
    // gcd(a, b)
    //----------------------------------
    inline  unsigned int GCD(unsigned int a, unsigned int b) {
        int tmp;

        //
        // Note:
        //  gcd(a, 0) = a
        //  gcd(0, a) = a
        //  gcd(0, 0) = 0
        //
        while(b != 0) {
            tmp = b;
            b = a % b;
            a = tmp;
        }

        return a;
    }

    unsigned int multiplicative_inverse(unsigned int d) {
        unsigned int xn ,t;
        assert(d % 2 == 1);

        xn = d;
        while(true) {
            t = d*xn;
            if(t == 1) return xn;
            xn = xn*(2-t);
        }
    }



#ifdef VSA_ARA_USE_CNCL_MATRICES
    ModularMatrix ModuleSpace::copyModularMatrix(const ModularMatrix &m) {
        return m;
    }
    ModularMatrix ModuleSpace::createModularMatrix(const Matrix &m) {
        ModularMatrix ret(m,Nsq);
        return ret;
    }
#else
    ModularMatrix ModuleSpace::copyModularMatrix(const ModularMatrix &m) {
        int *md = new int[Nsq];
        memcpy(md,m.m,Nsq*sizeof(int));
        ModularMatrix ret(md,m.leading_index, m.leading_rank);
        return ret;
    }
    ModularMatrix ModuleSpace::createModularMatrix(const int *m) {
        int *md = new int[Nsq];
        memcpy(md,m,Nsq*sizeof(int));
        ModularMatrix ret(md,Nsq);
        return ret;
    }
#endif


    //---------------------------------------------------
    // Construct 
    //---------------------------------------------------
    ModuleSpace::ModuleSpace(int m, bool id) {
        N=m;
        Nsq = N*N;
        basisUptodate=true;
        if(id) {
            setIdspace();
        }
    }


    //---------------------------------------------------
    // Copy constructor
    //---------------------------------------------------
    ModuleSpace::ModuleSpace(const ModuleSpace& rhs) {

        N = rhs.N;
        Nsq = rhs.Nsq;
        basisUptodate = rhs.basisUptodate;

        MSConstIterator beg, end;
        beg = rhs.basis.begin();
        end = rhs.basis.end();

        while(beg!=end) {
            insertMatrix(*beg);
            beg++;
        }

    }

    //---------------------------------------------------
    // dimension
    //---------------------------------------------------
    unsigned int ModuleSpace::moduleRank() {
        if(!basisUptodate)
            findBasis();

        MSConstIterator beg,end;
        beg = basis.begin();
        end = basis.end();

        unsigned int rank = 0;
        while(beg != end) {
            rank += (*beg).leading_rank;
            beg++;
        }
        return rank + MAX_POWER*(Nsq - basis.size());
    }


    //---------------------------------------------------
    // Destructor 
    //---------------------------------------------------
    ModuleSpace::~ModuleSpace() {
        MSIterator beg,end;
        beg=basis.begin();
        end=basis.end();
        while(beg!=end) {
            deallocMatrix(*beg);
            beg++;
        }
    }

    //---------------------------------
    // Is this a zero vector space?
    //---------------------------------
    bool ModuleSpace::isEmpty() {
        // FIXME: Finding the basis is not necessary for most
        // of the cases   
        if(!basisUptodate)
            findBasis();
        return (basis.size()==0);
    }


    //----------------------------------------------------------------
    // Find if a variable gets incremented by a constant value in the weight
    //----------------------------------------------------------------
    //
    // This function basically solves to see if the affine relation
    // r = r_0 + x holds after the weight provided the affine relation
    // r_0 = r holds before the weight -- and x has a unique solution
    //
    // constval is an "out" value. The return value says "True, it is
    // a constant", or "False, it is not a constant".
    //
    // Note: In the Modular domain, a decrement by 4 and increment by
    // (2^32 - 4) is indistinguishable.
    //----------------------------------------------------------------
    bool ModuleSpace::isVarConstant(unsigned int var, int &constval) {
        if(var >= N) return false;

        if(!basisUptodate) {
            findBasis();
        }
        MSIterator beg, end;

        beg = basis.begin();
        end = basis.end();
        // First try to find one solution
        int sol;
        bool found = false;
        while(beg != end && !found) {
            for(int i = 0; i < N && !found; i++) {
                int a = (*beg).m[i*N];
                if( a % 2 == 1) {
                    int b = (*beg).m[i*N + var];
                    if(i == var) b -= (*beg).m[0];
                    sol = (int)multiplicative_inverse(a) * b;
                    found = true;
                }
            }
            beg++;
        }
        if(!found) { // solution does not exist or is not unique
            return false;
        }
        // Now check if the solution holds for all equations
        beg = basis.begin();
        while(beg != end) {
            for(int i = 0; i < N; i++) {
                int a = (*beg).m[i*N];
                int b = (*beg).m[i*N + var];
                if(i == var) b -= (*beg).m[0];
                if(a*sol!=b) {
                    return false; // No solution
                }
            }
            beg++;
        }
        constval = sol;
        return true;

    }

    //----------------------------------------------------------------
    // Compute the weakest precondition of an affine expression
    // returns the pre-"value" of that expression, and a
    // precondition "cond" for that value to hold
    // 
    // if(cond) then the value of "value" before this transformer 
    // equals the value of "in" after the transformer
    //
    // The procedure can "give up" by returning 0, meaning that
    // the precondition is "unknown". This will happen when the
    // coefficient of "x_0" (the value of "in") is even (and hence
    // there is no good way of representing that equation inside "value".
    // However, if the coeff. of x_0 divides all other coeffs (when it
    // is even), then this is done and the return value is 2.
    //
    // Return value:
    // 0 : unknown
    // 1 : exact
    // 2:  approx
    //----------------------------------------------------------------
    int ModuleSpace::expressionPrecondition(AffineRels &in, AffineRels &value, AffineRels &cond) {
        int n = in.nrOfRels();
        int i,j;

        if(n > 1 || n == 0)
            return 0; // There must be exactly one affine expression 

        value.clear();
        cond.clear();

        AffineRels::AffRel_t a = in.getAffineRel(0);
        if(!basisUptodate) {
            findBasis();
        }

        // Compute M.a for each M in basis
        MSIterator beg, end;
        beg = basis.begin();
        end = basis.end();

#ifndef VSA_ARA_USE_CNCL_MATRICES // Not handling this for now
#error
#endif

        MatrixSetInt intBasis; 

        for(; beg != end; beg++) {
            int *m = new int[N+1];
            for(i=0;i<N;i++) {
                m[i] = 0;
                for(j=0;j<N;j++) {
                    m[i] += (*beg).m[(i*N)+j] * a[j];
                }
            }
            m[N] = -1 * (*beg).m[0];

            intBasis.push_back(ModularMatrixInt(m, N+1));
        }
        // Find basis for these affine expression
        findBasis(&intBasis, N+1, true);
        // Now insert into the answer
        MSIntIterator begi, endi;
        begi = intBasis.begin();
        endi = intBasis.end();

        AffineRels::AffRel_t to_insert_only = new AffineRels::AffRelCoeff_t[N];

        int retval = 1;
        for(; begi != endi && (retval != 0); begi++) {
            if((*begi).m[N] == 0) {
                memcpy(to_insert_only, (*begi).m, sizeof(int) * N);
                cond.insert(to_insert_only);
            } else {
                int di = ((*begi).m[N] > 0)? (*begi).m[N] : -(*begi).m[N];
                di = multiplicative_inverse(di >> compute_rank(di));
                unsigned int gcdOfCoeffs = 0;
                for(i=0;i<N+1;i++) {
                    int temp = di * (*begi).m[i];
                    gcdOfCoeffs = GCD(temp > 0 ? temp : -temp, gcdOfCoeffs);
                }

                (*begi).m[N] *= di;

                if((*begi).m[N] == gcdOfCoeffs || -(*begi).m[N] == gcdOfCoeffs) { // phew
                    // Divide through by gcd (this is only safe, because we're not allowed
                    // to divide through by even numbers

                    int sign = ((*begi).m[N] > 0)? -1 : 1;
                    for(i=0;i<N;i++) {
                        to_insert_only[i] = sign * ((di * (*begi).m[i]) / gcdOfCoeffs);
                    }
                    value.insert(to_insert_only);
                    if(gcdOfCoeffs % 2 == 0) {
                        retval = 2;
                    }
                } else { // give up!
                    retval = 0;
                }
            }
        }
        // cleanup
        begi = intBasis.begin();
        endi = intBasis.end();

        for(; begi != endi; begi++) {	
            delete [] (*begi).m;
        }
        intBasis.clear();
        delete [] to_insert_only;

        if(retval == 0) {
            value.clear();
            cond.clear();
        }

        return retval;
    }


    //----------------------------------------------------------------
    // Find the basis of the matrix set using Gaussian Elimination
    //----------------------------------------------------------------
    //
    // Note: Any vector space in F of finite dimension 'n' is isomorphic
    // to the co-ordinate space F^n.  We can consider the matrix as a 
    // nRows*nCols vector and find the basis
    //
    //----------------------------------------------------------------
    void ModuleSpace::findBasis(MatrixSet *pBasis) {

        //
        // Note:- This function is overloaded. 
        //
        //   If pBasis is NULL, then we are trying to obtain the
        // basis set so we should free the space occupied by the
        // linearly dependent matrices. Otherwise we  are using 
        // this for checking if some thing is a subspace of another.
        //

        if(!pBasis) {
            pBasis  = &basis;
        }
        if(pBasis->size() == 0) return;

        // copy into an int-basis and deallocate pBasis
        MatrixSetInt intBasis;  
        MSIterator beg, end;
        MSIntIterator begi, endi;

        beg = pBasis->begin();
        end = pBasis->end();
        while(beg != end) {
            int *mat;
#ifdef VSA_ARA_USE_CNCL_MATRICES
            mat = new int[Nsq];
            memcpy(mat, MATRIX_PTR_OF((*beg).m), Nsq * sizeof(int));
            deallocMatrix(*beg);
#else
            mat = (*beg).m;
#endif
            intBasis.push_back( ModularMatrixInt(mat,(*beg).leading_index, (*beg).leading_rank));
            beg++;
        }

        findBasis(&intBasis, Nsq);

        // now copy back into pBasis
        pBasis->clear();
        begi = intBasis.begin();
        endi = intBasis.end();
        while(begi != endi) {
#ifdef VSA_ARA_USE_CNCL_MATRICES
            Matrix m((*begi).m, N);
            delete [] (*begi).m;   // Matrix makes a copy for itself
#else
            int *m = (*begi).m;
#endif
            pBasis->push_back(ModularMatrix(m, (*begi).leading_index, (*begi).leading_rank));
            begi++;
        }
        intBasis.clear();

        if(pBasis == &basis) {
            basisUptodate = true;
        }

    }

    void ModuleSpace::findBasis(std::vector<int *> &pBasis, int vec_size, int clear_mem) {
        std::vector<int *>::iterator beg,end;
        MSIntIterator begi, endi;

        MatrixSetInt intBasis;  

        beg = pBasis.begin();
        end = pBasis.end();
        for(; beg != end; beg++) {
            intBasis.push_back(ModularMatrixInt(*beg, vec_size));
        }
        findBasis(&intBasis, vec_size, clear_mem);
        pBasis.clear();

        begi = intBasis.begin();
        endi = intBasis.end();
        for(;begi != endi; begi++) {
            pBasis.push_back((*begi).m);
        }
    }

    void ModuleSpace::findBasis(MatrixSetInt *pBasis, int vec_size, int clear_mem) {
        // if clear_mem is true then zero-vectors will be deallocated before returning the result

        assert(pBasis != NULL); // cannot pass in a null basis here
        if(pBasis->size() == 0) return;

        int *li = new int[vec_size+1]; // li[i] is the matrix having leading entry at i
        int d,dprime,r,xi,rprime,yi,n,i,j;

        for(xi=0;xi<=vec_size;xi++) {
            li[xi] = -1;
        }

        // Triangularization
        n = pBasis->size();

        MODULE_DBGS({
                MSIntIterator end;
                MSIntIterator beg;
                beg = pBasis->begin();
                end = pBasis->end();
                while(beg != end) {
                print_mat((*beg).m,(int)(sqrt((double) vec_size) + 0.5),"pBasis");
                beg++;
                }
                });
        if( (*pBasis)[0].leading_index != (unsigned) vec_size) {
            li[(*pBasis)[0].leading_index] = 0;
        }

        // triangularize i^th vector
        for(i = 1; i < n; i++) {
            ModularMatrixInt x((*pBasis)[i]);

            yi = li[x.leading_index];
            xi = i;
            while(yi != -1) {
                ModularMatrixInt y((*pBasis)[yi]);

                // now x and y have the same leading index
                r = x.leading_rank;
                d = DIV_BY_POWER_OF_2(x.m[x.leading_index], r);
                rprime = y.leading_rank;
                dprime = DIV_BY_POWER_OF_2(y.m[y.leading_index], rprime);

                if(rprime <= r) {
                    // x = dprime*x - 2^{r - rprime} *d*y
                    int t = MULT_BY_POWER_OF_2(d,(r- rprime));
                    bool set = false;
                    for(j = x.leading_index; j < vec_size; j++) {
                        x.m[j] = dprime*x.m[j] - t*y.m[j];
                        if(x.m[j] != 0 && !set) {
                            x.leading_index = j;
                            set = true;
                        }
                    }
                    if(!set) {// i.e. x = 0
                        x.leading_index = vec_size;
                        x.leading_rank = MAX_POWER;
                    }
                    else 
                        x.leading_rank = compute_rank(x.m[x.leading_index]);

                    (*pBasis)[xi] = x;
                    yi = li[x.leading_index];
                } else {
                    // y = d*y - 2^{rprime - r}*dprime*x
                    int t = MULT_BY_POWER_OF_2(dprime,(rprime - r));
                    bool set = false;
                    for(j = y.leading_index; j < vec_size; j++) {
                        y.m[j] = d*y.m[j] - t*x.m[j];
                        if(y.m[j] != 0 && !set) {
                            y.leading_index = j;
                            set = true;
                        }
                    }
                    if(!set) {// i.e. y = 0
                        y.leading_index = vec_size;
                        y.leading_rank = MAX_POWER;
                    }
                    else
                        y.leading_rank = compute_rank(y.m[y.leading_index]);

                    li[x.leading_index] = xi;	
                    (*pBasis)[yi] = y;
                    xi = yi;
                    x = y;
                    yi = li[y.leading_index];
                }
            }
            if(x.leading_index != (unsigned) vec_size) {
                li[x.leading_index] = xi;
            }
        }

        MODULE_DBGS({
                MSIntIterator end;
                MSIntIterator beg;
                beg = pBasis->begin();
                end = pBasis->end();
                while(beg != end) {
                print_mat((*beg).m,(int)(sqrt((double) vec_size) + 0.5),"pBasis");
                beg++;
                }
                });

        if(clear_mem) {
            // Now, get rid of 0 vectors
            MSIntIterator beg,end;
            beg	= pBasis->begin();
            end = pBasis->end();
            while(beg != pBasis->end()) {
                if((*beg).leading_index == (unsigned) vec_size) {
                    deallocMatrix(*beg);
                    beg = pBasis->erase(beg);
                } else
                    beg++;
            }
            assert(pBasis->size() <= (unsigned) vec_size);
        }

        delete [] li;
    }

    //----------------------------------------------------
    // Find NULL space. Returns a NxN matrix whose columns
    // are the null-space basis vectors.
    //----------------------------------------------------
    int *ModuleSpace::findNullSpace() {
        if(!basisUptodate) {
            findBasis();
        }
        MODULE_DBGS(prettyPrint(modDbgFile));

        int i,j;
        MatrixSetInt tmpBasis;
        MSIterator beg, end;
        MSIntIterator begi, endi;

        beg = basis.begin();
        end = basis.end();
        while(beg != end) {
            // create a vector for each row of the matrix
            for(i = 0; i < N; i++) {
                int *mat = new int[N];
                memcpy(mat, MATRIX_PTR_OF((*beg).m) + (i*N), N*sizeof(int));
                MODULE_DBGS({
                        modDbgFile << "Pushing : ";
                        for(j=0;j<N;j++) {
                        modDbgFile<<mat[j]<<" ";
                        }
                        modDbgFile<<"\n";
                        });
                tmpBasis.push_back(ModularMatrixInt(mat,N));
            }
            beg++;
        }
        findBasis(&tmpBasis, N);
        assert(tmpBasis.size() <= (unsigned) N);
        // make a single matrix out of what we got
        int *A = new int[Nsq];
        int *Rinv = new int[Nsq];
        int *Linv = new int[Nsq];
        int n = tmpBasis.size();
        for(i=0; i<n; i++) {
            memcpy(A + (i*N), tmpBasis[i].m, N*sizeof(int));
        }
        for(i=n; i<N; i++) {
            memset(A + (i*N), 0, N*sizeof(int));
        }
        // Now do a modular SVD
        MODULE_DBGS(print_mat(A,N,"A"));

        modular_svd(A,Rinv,Linv,N);

        MODULE_DBGS({  
                print_mat(A,N,"A");
                print_mat(Rinv,N,"Rinv");
                print_mat(Linv,N,"Linv");
                });

        // convert A into e -- generating set for the solution (Lem 2, Gp 4)
        int *tmp = A;
        for(i=0;i < N;i++) {
            *tmp = MULT_BY_POWER_OF_2(1,(MAX_POWER - compute_rank(*tmp)));
            tmp += N+1;
        }
        // Premultiply by Rinv
        tmp = multiplyMatrices(Rinv, A);
        MODULE_DBGS(print_mat(tmp,N,"tmp"));
        delete [] A;
        A = tmp;

        // cleanup
        delete [] Rinv;
        delete [] Linv;
        begi = tmpBasis.begin();
        endi = tmpBasis.end();
        while(begi != endi) {
            delete [] (*begi).m;
            begi++;
        }

        return A;
    }


    //-------------------------------------------------
    // Find affine relations
    //-------------------------------------------------
    void ModuleSpace::findAffineRelations(AffineRels &affRels) {
        affRels.clear();
        int *A = findNullSpace();
        int i,j;

        // Insert non-zero affine relations only
        int dim = 0;
        for(i = 0; i < N; i++) {
            int is_zero = true;
            for(j = 0; j < N; j++) {
                if(A[j * N + i] != 0) {
                    is_zero = false;
                }
            }
            if(!is_zero) {
                dim++;
            }
        }
        // Infeasible!
        if(dim == N)
            return;

        // Insert non-zero affine relations only
        AffineRels::AffRel_t to_insert_only = new AffineRels::AffRelCoeff_t[N];
        for(i=0;i<N;i++) {
            int is_zero = true;
            unsigned int gcdOfCoeffs = 0;
            for(j=0;j<N;j++) {
                to_insert_only[j] = A[j*N+i];
                gcdOfCoeffs = GCD(A[j*N+i] > 0 ? A[j*N+i] : -A[j*N+i], gcdOfCoeffs);
                if(A[j*N+i] != 0) {
                    is_zero = false;
                }
            }
            if(!is_zero) {
                // Divide throughout by the (positive) odd common factor
                gcdOfCoeffs /= (1 << compute_rank(gcdOfCoeffs));
                for(j=0;j<N;j++) 
                    to_insert_only[j]/= (int)(gcdOfCoeffs);
                affRels.insert(to_insert_only);
            }
        }

        // cleanup
        delete [] to_insert_only;
        delete [] A;
    }

    /////////////////////////////////////////////////
    // new = this (join) op2
    /////////////////////////////////////////////////
    ModuleSpace *ModuleSpace::join(ModuleSpace *op2) {
        MSIterator beg,end;
        ModuleSpace *result = new ModuleSpace(N);
        beg = basis.begin();
        end = basis.end();
        while(beg!=end) {
            result->insertMatrix(*beg);
            beg++;
        }
        beg = op2->basis.begin();
        end = op2->basis.end();
        while(beg!=end) {
            result->insertMatrix(*beg);
            beg++;
        }

        result->findBasis();
        return result;
    }

#ifdef ___TSL
    /////////////////////////////////////////////////
    // new = this (join) op2
    /////////////////////////////////////////////////
    ModuleSpace *ModuleSpace::join_wo_elimination(ModuleSpace *op2) {
        MSIterator beg,end;
        ModuleSpace *result = new ModuleSpace(N);
        beg = basis.begin();
        end = basis.end();
        while(beg!=end) {
            result->insertMatrix(*beg);
            beg++;
        }
        beg = op2->basis.begin();
        end = op2->basis.end();
        while(beg!=end) {
            result->insertMatrix(*beg);
            beg++;
        }
        return result;
    }
#endif

    /////////////////////////////////////////////////
    // Are the spaces equal? (approximate operation -- see isExactEqual)
    /////////////////////////////////////////////////
    bool ModuleSpace::isEqual(ModuleSpace *op2) {

        // Incomparable
        if(op2->N!=this->N)
            return false;

        if(op2->isEmpty()) {
            return isEmpty();
        }

        // Ensure that the basis has been determined
        if(!basisUptodate)
            findBasis();
        if(!op2->basisUptodate)
            op2->findBasis();

        // Cheap hack -- is unsafe but *should* work
        // because of way equality is used
        if(op2->moduleRank() != this->moduleRank())
            return false;
        else return true;

        // Actual equailty method -- its still just safe 
        // and not exact. 

        return (this->isSubspace(op2) && op2->isSubspace(this));
    }

    /////////////////////////////////////////////////
    // Are the spaces equal? (exact operation)
    /////////////////////////////////////////////////
    bool ModuleSpace::isExactEqual(ModuleSpace *op2) {

        return (isStrictSubspace(op2)==0);
    }

    bool ModuleSpace::isExactEqual(std::vector<const int *> &lhs, std::vector<const int *> &rhs, int vec_size) {
        return (isStrictSubspace(lhs,rhs,vec_size) == 0);
    }

    //////////////////////////////////////////////////////
    // calculate new=op2 (o) this;
    ///////////////////////////////////////////////////////
    ModuleSpace *ModuleSpace::compose(ModuleSpace *op2) {
        MSIterator thisBeg,thisEnd,op2Beg,op2End;
        ModuleSpace *result=new ModuleSpace(N);

        // Multiply all possible combinations of matrices from
        // the basis set of the vector spaces
        thisBeg = basis.begin();
        thisEnd = basis.end();
        while(thisBeg != thisEnd) {

            op2Beg = op2->basis.begin();
            op2End = op2->basis.end();
            while(op2Beg != op2End) {
                ModularMatrix mm(multiplyMatrices((*op2Beg).m, (*thisBeg).m), Nsq);
                result->basis.push_back(mm);
                op2Beg++;
            }
            thisBeg++;
        }

        //
        // Find the basis so that we can work on a finite set always
        //
        result->findBasis();
        return result;
    }

#ifdef ___TSL
    //////////////////////////////////////////////////////
    // calculate new=op2 (o) this;
    ///////////////////////////////////////////////////////
    ModuleSpace *ModuleSpace::compose_wo_elimination(ModuleSpace *op2) {
        MSIterator thisBeg,thisEnd,op2Beg,op2End;
        ModuleSpace *result=new ModuleSpace(N);

        // Multiply all possible combinations of matrices from
        // the basis set of the vector spaces
        thisBeg = basis.begin();
        thisEnd = basis.end();
        while(thisBeg != thisEnd) {

            op2Beg = op2->basis.begin();
            op2End = op2->basis.end();
            while(op2Beg != op2End) {
                ModularMatrix mm(multiplyMatrices((*op2Beg).m, (*thisBeg).m), Nsq);
                result->basis.push_back(mm);
                op2Beg++;
            }
            thisBeg++;
        }
        return result;
    }
#endif

#ifdef ___TSL
    ///////////////////////////////////////////////////////
    // calculate set_of_columns = set_of_matrix * a column;
    //    - vector<int>: column
    //    - TSL_FIXME: make this cleaner
    ///////////////////////////////////////////////////////
    std::vector<std::vector<int> > ModuleSpace::multiply(std::vector<int> col) {
        assert(col.size() == N);
        MSIterator thisBeg,thisEnd,op2Beg,op2End;
        std::vector<std::vector<int> > result;

        std::vector<int> special;
        // Multiply the given column to each matrix from
        // the basis set of the vector spaces
        thisBeg = basis.begin();
        thisEnd = basis.end();
        while(thisBeg != thisEnd) {
            std::vector<int> ans;
            Matrix m = (*thisBeg).m;
            if(m(0,0) == 0) {
                for(int i=0; i<N; i++) {
                    int sum = 0;
                    for(int j=0; j<N; j++) {
                        sum += col[j] * m(i, j);
                    }
                    ans.push_back(sum);
                }
                result.push_back(ans);
            }
            else {
                for(int i=0; i<N; i++) {
                    int sum = 0;
                    for(int j=0; j<N; j++) {
                        sum += col[j] * m(i, j);
                    }
                    special.push_back(sum);
                }
            }
            thisBeg++;
        }

        std::vector<std::vector<int> > result2;
        result2.push_back(special);
        std::vector<std::vector<int> >::iterator it;
        for(it=result.begin(); it!=result.end(); it++) {
            result2.push_back(*it);
        }

        return result2;
    }

    ref_ptr<ModuleSpace> ModuleSpace::to9by9(const ref_ptr<ModuleSpace> & a) {
        assert(a->getDimensionOfPointSpace() == 11);
        ref_ptr<ModuleSpace> result = new ModuleSpace(9);

        MSIterator thisBeg,thisEnd;
        std::vector<int> special;
        // Multiply the given column to each matrix from
        // the basis set of the vector spaces
        thisBeg = a->basis.begin();
        thisEnd = a->basis.end();
        while(thisBeg != thisEnd) {
            Matrix m = (*thisBeg).m; // m must be 11 x 11
            int* mat = new int[9*9];
            int n = 0;
            for(int i=0; i<11; i++) {
                for(int j=0; j<11; j++) {
                    if(i == 1 || j == 1 || i == 10 || j == 10) {
                        // discard NullReg32 and EIP row and column
                    }
                    else {
                        mat[n++] = m(i,j);
                    }
                }
            }
            Matrix new_m(mat, 9);
            delete [] mat;
            result->insertMatrix(new_m);
            thisBeg++;
        }
        return result;
    }

#endif


#ifdef VSA_ARA_USE_CNCL_MATRICES
    Matrix ModuleSpace::multiplyMatrices(const Matrix& op1, const Matrix& op2) const {
        return Matrix::multiply(op1, op2);
    }
#endif 
    ///////////////////////////////////////////////////////
    // Multiply two matrices
    //////////////////////////////////////////////////////
    int *ModuleSpace::multiplyMatrices(const int *op1,const int *op2) const {

        int *multResult=new int[N*N];

        int i,j,k;
        for(i=0;i<N;++i) {
            for(j=0;j<N;++j) {
                multResult[i*N+j]=0;
                for(k=0;k<N;++k) {
                    //UNOPT:multResult[i*N+j]=multResult[i*N+j]+op1[i*N+k]*op2[k*N+j];
                    multResult[i*N+j]+=op1[i*N+k]*op2[k*N+j];
                }
            }
        }
        return multResult;
    }

    /////////////////////////////////////////////////////////
    // Insert a matrix
    /////////////////////////////////////////////////////////
    void ModuleSpace::insertMatrix(const ModularMatrix& f) {
        ModularMatrix fd = copyModularMatrix(f);
        basis.push_back(fd);
        basisUptodate=false;
        //affRelUptodate=false;
    }
#ifdef VSA_ARA_USE_CNCL_MATRICES
    void ModuleSpace::insertMatrix(const Matrix& f) {
        basis.push_back(createModularMatrix(f));
        basisUptodate=false;
        //affRelUptodate=false;
    }
    void ModuleSpace::insertMatrix(const int *f) {
        Matrix fm = Matrix(f, N);
        basis.push_back(createModularMatrix(fm));
        basisUptodate=false;
        //affRelUptodate=false;
    }
#else
    void ModuleSpace::insertMatrix(const int *f) {
        basis.push_back(createModularMatrix(f));
        basisUptodate=false;
        //affRelUptodate=false;
    }

#endif
    /////////////////////////////////////////
    // set id space
    ////////////////////////////////////////
    void ModuleSpace::setIdspace() {

        MSIterator beg,end;
        beg = basis.begin();
        end = basis.end();
        while(beg != end) {
            deallocMatrix(*beg);
            beg++;
        }
        basis.clear();

        int *idMatrix=new int[Nsq];

        int *tmp = idMatrix;
        for(int i = 0; i < N; ++i) {
            for(int j = 0; j < N; ++j) {
                if(i == j)
                    *tmp = 1;
                else 
                    *tmp = 0;
                tmp++;
            }
        }
        insertMatrix(idMatrix);
        delete [] idMatrix;
        /*
           basis.insert(idMatrix);
           basisUptodate=true;
           */


        //affRelUptodate=false;

        return ;
    }


    ///////////////////////////////////////////
    //
    ///////////////////////////////////////////
    void ModuleSpace::prettyPrint(FILE *fp, unsigned nTabs) {
        MSIterator beg,end;

        if(!basisUptodate)
            findBasis();

        beg=basis.begin();
        end=basis.end();
        fprintf(fp,"\n");
        while(beg!=end) {
            //PERFORM_INDENTATION(fp, nTabs);
            fprintf(fp,"[ ");
            int k=0;

            for(int i=0;i<N;++i) {
                //PERFORM_INDENTATION(fp, nTabs);
                for(int j=0;j<N;++j,++k)  {
                    fprintf(fp,"%d\t",((*beg).m)[k]);
                }
                fprintf(fp,"\n");
            }
            //PERFORM_INDENTATION(fp, nTabs);
            fprintf(fp,"] \n");
            beg++;
        }
        //PERFORM_INDENTATION(fp, nTabs);
        fprintf(fp,"#");
        fflush(fp);
    }

    void ModuleSpace::prettyPrint(std::ostream &out) {
        MSIterator beg,end;

        if(!basisUptodate)
            findBasis();

        beg=basis.begin();
        end=basis.end();

        out << std::endl;
        while(beg!=end) {
            // out << "~ " ;
            out << "[ ";
            int k=0;
            for(int i=0;i<N;++i) {
                for(int j=0;j<N;++j,++k)  {
                    out << linearAccess((*beg).m, k);
                    //out << "\t";
                    out << "   ";
                }
                out<<"\n";
            }
            out<< "] \n";
            beg++;
        }
        out <<"#"<<std::endl;
    }

    void ModuleSpace::printAsText(std::ostream &out) {
        MSIterator beg,end;

        if(!basisUptodate)
            findBasis();

        beg=basis.begin();
        end=basis.end();

        if(beg == end) {
            out << "zero";
            return;
        }

        // Find the matrix with 1 as leading entry
        const int *oneMat = NULL;
        for(; beg != end; beg++) {
            const int *mat = MATRIX_PTR_OF((*beg).m);
            if(mat[0] == 1) {
                oneMat = mat;
                break;
            }
        }

        if(oneMat == NULL) {
            out << "Cannot prettyPrint, no matrix has a leading 1\n";
            out.flush();
            assert(0); // can remove this assertion if there is a valid way in
            // which the above condition can occur
            prettyPrint(out);
        }

        beg=basis.begin();
        end=basis.end();

        int *temp = new int[N*N];
        bool printedLast = false;
        bool printed = false;
        bool r;
        int i;
        for(; beg != end; beg++) {
            const int *mat = MATRIX_PTR_OF((*beg).m);
            if(printedLast) out << " v ";
            if(mat == oneMat) {
                r = printMatAsText(mat, out);
            } else {
                for(i=0;i<N*N;i++) {
                    temp[i] = mat[i] + oneMat[i];
                }
                r = printMatAsText(temp,out);
            }
            printedLast = !r;
            printed = printed | (!r);
        }
        if(!printed) out << "id";
        delete [] temp;
    }

    bool ModuleSpace::printMatAsText(const int *mat, std::ostream &out) {
        int i,j;

        // First make sure that first column is (1 0 0 0 0 ...)^t
        assert(mat[0] == 1);
        for(i=1;i<N;i++) {
            assert(mat[i*N] == 0);
        }

        // Now start printing, column by column
        bool isId = true;
        bool printedLast = false;
        for(j=1;j<N;j++) {
            if(printedLast) {
                out << " ^ ";
            }
            bool r = printMatColumnAsText(mat, j, out);
            isId = r && isId;
            printedLast = !r;
        }

        return isId;
    }

    bool ModuleSpace::printMatColumnAsText(const int *mat, int col, std::ostream &out) {
        bool isId = true;
        int i;
        // check if column is ID
        for(i=0;i<N && isId;i++) {
            if(i == col) {
                if(mat[i*N+col] != 1) isId = false;
            } else {
                if(mat[i*N+col] != 0) isId = false;
            }
        }

        if(isId) return true;

        out << arVarNames[col] << ":=";
        out << mat[col];

        for(i=1;i<N;i++) {
            if(mat[i*N+col] == 0) continue;
            out << " + ";
            if(mat[i*N+col] != 1) out << mat[i*N+col] << "*";
            out << arVarNames[i];
        }

        return false;
    }


#ifdef ___TSL
    void ModuleSpace::prettyPrint_wo_elimination(std::ostream &out) {
        MSIterator beg,end;

        beg=basis.begin();
        end=basis.end();

        out << std::endl;
        while(beg!=end) {
            // out << "~ " ;
            out << "[ ";
            int k=0;
            for(int i=0;i<N;++i) {
                for(int j=0;j<N;++j,++k)  {
                    out << linearAccess((*beg).m, k);
                    //out << "\t";
                    out << "   ";
                }
                out<<"\n";
            }
            out<< "] \n";
            beg++;
        }
        out <<"#"<<std::endl;
    }
#endif


    //----------------------------------------------------
    // is 'this' a subspace of 'op2'? (approximate operation)
    //----------------------------------------------------
    bool ModuleSpace::isSubspace(ModuleSpace *op2) {

        //
        // Note: 
        //     isSubspace() is a private method and it is 
        //  called only by isEqual. So the matrix sets will
        //  be the bases of the respective spaces. So they 
        //  need not be recalculated.
        //
        ModuleSpace tmp(N);

        MSIterator beg,end;
        beg = basis.begin();
        end = basis.end();
        while(beg != end) {
            tmp.insertMatrix(*beg);
            beg++;
        }

        beg = op2->basis.begin();
        end = op2->basis.end();
        while(beg != end) {
            tmp.insertMatrix(*beg);
            beg++;
        }

        tmp.findBasis();

        if(tmp.moduleRank() == moduleRank())
            return true;
        else      
            return false;
    }

    //-------------------------------------------------------
    // is 'this' a strict subspace of 'op2'? (exact operation)
    /// @return 
    ///   0 - if equal
    ///  -1 - if this < op2
    ///  +1 - if this > op2 || this, rhs are incomparable.
    //-------------------------------------------------------
    int ModuleSpace::isStrictSubspace(ModuleSpace *op2) {

        // Incomparable
        if(op2->N!=this->N)
            return false;


        if(!basisUptodate) {
            findBasis();
        }
        if(!op2->basisUptodate) {
            op2->findBasis();
        }

        // convert into 'const int *'
        std::vector<const int *> lhs;
        std::vector<const int *> rhs;

        convertBasisToConstInt(lhs);
        op2->convertBasisToConstInt(rhs);

        return isStrictSubspace(lhs, rhs, Nsq);
    }

    // precondition: findBasis has already been called on lhs and rhs 
    int ModuleSpace::isStrictSubspace(std::vector<const int *> &lhs, std::vector<const int *> &rhs, int vec_size) {
        if(isExactSubspace(lhs,rhs,vec_size)) {
            if(isExactSubspace(rhs,lhs,vec_size))
                return 0;
            return -1;
        }
        return 1;
    }
    //--------------------------------------------------------------------
    // is NULL('this') a strict subspace of NULL('op2')? (exact operation)
    /// @return 
    ///   0 - if equal
    ///  -1 - if this < op2
    ///  +1 - if this > op2 || this, rhs are incomparable.
    //--------------------------------------------------------------------
    int ModuleSpace::isStrictNullSubspace(ModuleSpace *op2) {
        if(isExactNullSubspace(op2)) {
            if(op2->isExactNullSubspace(this))
                return 0;
            return -1;
        }
        return 1;
    }		


    //----------------------------------------------------
    // is 'this' a subspace of 'op2'? (exact operation)
    //----------------------------------------------------
    bool ModuleSpace::isExactSubspace(ModuleSpace *op2) {
        if(!basisUptodate) {
            findBasis();
        }
        if(!op2->basisUptodate) {
            op2->findBasis();
        }

        // convert into 'const int *'
        std::vector<const int *> lhs;
        std::vector<const int *> rhs;

        convertBasisToConstInt(lhs);
        op2->convertBasisToConstInt(rhs);

        return isExactSubspace(lhs,rhs,Nsq);
    }

    /*
       bool ModuleSpace::isExactSubspace(ModuleSpace *op2, int vec_size) {

       if(!basisUptodate) {
       findBasis();
       }
       if(!op2->basisUptodate) {
       op2->findBasis();
       }

       MSIterator beg, end;

// copy into (int*) space for modular_svd
int *A = new int[vec_size*vec_size];
int *Rinv = new int[vec_size*vec_size];
int *Linv = new int[vec_size*vec_size];
int i,j,k;

assert(op2->basis.size() <= vec_size);

beg = op2->basis.begin();
end = op2->basis.end();
j=0;
while(beg != end) {
const int *mat = MATRIX_PTR_OF((*beg).m);
for(i=0;i<vec_size;i++) {
A[i*vec_size + j] = mat[i];
}
beg++;
j++;
}
// fill up the rest of A with zeros
for(k=j;k<vec_size;k++) {
for(i=0;i<vec_size;i++) {
A[i*vec_size + k] = 0;
}
}
// Do svd
modular_svd(A,Rinv,Linv,vec_size);
// Compute b = (Linv m) for each m in this->basis and verify
// rank(A_ii) <= rank(b_i)
bool subset = true;
beg = basis.begin();
end = basis.end();
while(beg != end && subset) {
const int *mat = MATRIX_PTR_OF((*beg).m);
int b_i;
for(i=0;i<vec_size && subset;i++) {
b_i = 0;
for(j=0;j<vec_size;j++) {
b_i += Linv[i*vec_size + j] * mat[j];
}
if(compute_rank(A[i*vec_size+i]) > compute_rank(b_i)) {
subset = false;
}
}
beg++;
}
delete [] A;
delete [] Linv;
delete [] Rinv;

return subset;

}
*/

// lhs a subset of rhs? (vector version)
// precondition: findBasis has already been called
bool ModuleSpace::isExactSubspace(std::vector<const int *> &lhs, std::vector<const int *> &rhs, int vec_size) {

    std::vector<const int *>::iterator beg, end;

    // copy into (int*) space for modular_svd
    int *A = new int[vec_size*vec_size];
    int *Rinv = new int[vec_size*vec_size];
    int *Linv = new int[vec_size*vec_size];
    int i,j,k;

    assert(rhs.size() <= vec_size);

    beg = rhs.begin();
    end = rhs.end();
    j=0;
    while(beg != end) {
        const int *mat = *beg;
        for(i=0;i<vec_size;i++) {
            A[i*vec_size + j] = mat[i];
        }
        beg++;
        j++;
    }
    // fill up the rest of A with zeros
    for(k=j;k<vec_size;k++) {
        for(i=0;i<vec_size;i++) {
            A[i*vec_size + k] = 0;
        }
    }
    // Do svd
    modular_svd(A,Rinv,Linv,vec_size);
    // Compute b = (Linv m) for each m in this->basis and verify
    // rank(A_ii) <= rank(b_i)
    bool subset = true;
    beg = lhs.begin();
    end = lhs.end();
    while(beg != end && subset) {
        const int *mat = *beg;
        int b_i;
        for(i=0;i<vec_size && subset;i++) {
            b_i = 0;
            for(j=0;j<vec_size;j++) {
                b_i += Linv[i*vec_size + j] * mat[j];
            }
            if(compute_rank(A[i*vec_size+i]) > compute_rank(b_i)) {
                subset = false;
            }
        }
        beg++;
    }
    delete [] A;
    delete [] Linv;
    delete [] Rinv;

    return subset;
}

//-------------------------------------------------------------
// is NULL('this') a subspace of NULL('op2')? (exact operation)
//-------------------------------------------------------------
bool ModuleSpace::isExactNullSubspace(ModuleSpace *op2) {

    int *N1 = findNullSpace();
    int *N2 = op2->findNullSpace();
    MODULE_DBGS(print_mat(N1, N, "N1"));
    MODULE_DBGS(print_mat(N2, N, "N2"));

    int *Rinv = new int[Nsq];
    int *Linv = new int[Nsq];
    int i,j,k;

    // Do svd
    modular_svd(N2,Rinv,Linv,N);

    MODULE_DBGS(print_mat(N2, N, "N2"));
    MODULE_DBGS(print_mat(Linv, N, "Linv"));
    MODULE_DBGS(print_mat(Rinv, N, "Rinv"));

    // Compute b = (Linv m) for each column m in N1 and verify
    // rank(N2_ii) <= rank(b_i)
    bool subset = true;
    for(k=0; k<N && subset; k++) {  // pick k^th col of N1
        int b_i;
        for(i=0;i<N && subset;i++) {
            b_i = 0;
            for(j=0;j<N;j++) {
                b_i += Linv[i*N + j] * N1[j*N + k];
            }
            if(compute_rank(N2[i*N+i]) > compute_rank(b_i)) {
                subset = false;
            }
        }
    }
    delete [] N1;
    delete [] N2;
    delete [] Linv;
    delete [] Rinv;

    return subset;
}

/*
// return (this - op2)
//
ModuleSpace *ModuleSpace::diff(ModuleSpace *op2) {

if(!basisUptodate) {
findBasis();
}
if(!op2->basisUptodate) {
op2->findBasis();
}

MSIterator beg, end;

// copy into (int*) space for modular_svd
int *A = new int[Nsq*Nsq];
int *Rinv = new int[Nsq*Nsq];
int *Linv = new int[Nsq*Nsq];
int i,j,k;

beg = op2->basis.begin();
end = op2->basis.end();
j=0;
while(beg != end) {
const int *mat = MATRIX_PTR_OF((*beg).m);
for(i=0;i<Nsq;i++) {
A[i*Nsq + j] = mat[i];
}
beg++;
j++;
}
// fill up the rest of A with zeros
for(k=j;k<Nsq;k++) {
for(i=0;i<Nsq;i++) {
A[i*Nsq + k] = 0;
}
}
// Do svd
modular_svd(A,Rinv,Linv,Nsq);
ModuleSpace *res = new ModuleSpace(N);
// Compute b = (Linv m) for each m in this->basis and verify
// rank(A_ii) <= rank(b_i)
beg = basis.begin();
end = basis.end();
while(beg != end) {
const int *mat = MATRIX_PTR_OF((*beg).m);
int b_i;
bool subset = true;
for(i=0;i<Nsq && subset;i++) {
b_i = 0;
for(j=0;j<Nsq;j++) {
b_i += Linv[i*Nsq + j] * mat[j];
}
if(compute_rank(A[i*Nsq+i]) > compute_rank(b_i)) {
subset = false;
}
}
if(!subset) 
res->insertMatrix(mat);
beg++;
}
delete [] A;
delete [] Linv;
delete [] Rinv;

return res;
}
*/

// convert basis to vector<const int *>
void ModuleSpace::convertBasisToConstInt(std::vector<const int *> &b) {
    MSIterator beg, end;

    beg = basis.begin();
    end = basis.end();

    for(; beg != end; beg++) {
        b.push_back(MATRIX_PTR_OF((*beg).m));
    }
}

// return (this - op2)
ModuleSpace *ModuleSpace::diff(ModuleSpace *op2) {

    if(!basisUptodate) {
        findBasis();
    }
    if(!op2->basisUptodate) {
        op2->findBasis();
    }

    // copy op2 and this into an int-basis
    MatrixSetInt intBasis;  
    MSIterator beg, end;
    MSIntIterator begi, endi;

    beg = op2->basis.begin();
    end = op2->basis.end();
    while(beg != end) {
        int *mat;
        mat = new int[Nsq];
        memcpy(mat, MATRIX_PTR_OF((*beg).m), Nsq * sizeof(int));
        intBasis.push_back( ModularMatrixInt(mat,(*beg).leading_index, (*beg).leading_rank));
        beg++;
    }
    beg = basis.begin();
    end = basis.end();
    while(beg != end) {
        int *mat;
        mat = new int[Nsq];
        memcpy(mat, MATRIX_PTR_OF((*beg).m), Nsq * sizeof(int));
        intBasis.push_back( ModularMatrixInt(mat,(*beg).leading_index, (*beg).leading_rank));
        beg++;
    }
    findBasis(&intBasis, Nsq, false); // do not clear zero-vectors as they indicate the result

    ModuleSpace *res = new ModuleSpace(N);
    int i = 0;
    int size_op2 = op2->basis.size();
    int size_this = basis.size();

    for(i = size_op2; i < (size_op2 + size_this); i++) {
        if(intBasis[i].leading_index != Nsq) {
            res->insertMatrix(basis[i - size_op2].m);
        }
    }
    // clear intBasis
    begi = intBasis.begin();
    endi = intBasis.end();
    while(begi != endi) {
        delete [] (*begi).m;
        begi++;
    }

    intBasis.clear();
    // Sanity check: res \combine op2 = this \combine op2
    /*ModuleSpace *lhs = res->join(op2);
      ModuleSpace *rhs = join(op2);
      if(!lhs->isExactSubspace(rhs) || !rhs->isExactSubspace(lhs)) {
      _asm int 3;
      MODULE_DBGS(prettyPrint(modDbgFile));
      MODULE_DBGS(op2->prettyPrint(modDbgFile));
      }
      delete lhs;
      delete rhs;
      */
    return res;
}


#ifdef USE_GMM
ModuleSpace *ModuleSpace::quasione() {
    // TODO
    return NULL;
}
#endif

//////////////////////////////////////////////////////
// call site transformer
///////////////////////////////////////////////////////
#ifdef VSA_ARA_USE_CNCL_MATRICES
//--------------------------------------------
/// Set up this vector space so that when
/// pre-composed with any other space
/// allows values for saveReg to flow through
/// to restReg and other regs are just 
/// left as such. (MAKES SENSE?)
//---------------------------------------------
ModuleSpace* ModuleSpace::callerSaveXform(int saveReg, int restReg) const{
    int* tmpMat = new int[N * N];
    int Nsq = N * N;
    ModuleSpace *result = new ModuleSpace(N);
    MSConstIterator beg, end;
    beg = basis.begin();
    end = basis.end();
    while(beg != end) {

        // 
        {
            for(int i = 0; i < Nsq; ++i) {
                tmpMat[i] = (*beg)[i];
            }
        }

        //- Set the correct save/restore 
        //
        for(int i = 0; i < N; ++i) {
            tmpMat[i * N + restReg] = 0;
        }
        tmpMat[saveReg * N + restReg] = 1;

        result->insertMatrix(tmpMat);
        beg++;
    }

    result->findBasis();

    delete[] tmpMat;
    return result;
}

//---------------------------------------------------------------------
/// Transformer for call-site
///
/// \param m1 The transformer for enter-of-callee->exit-of-callee edge.
/// \param m2 The transformer along call->return-site edge.
//---------------------------------------------------------------------
ModuleSpace *ModuleSpace::call_site_transform(const Matrix &m1, const Matrix &m2) {
    MSIterator thisBeg,thisEnd;
    ModuleSpace *result=new ModuleSpace(N);
    int i;   
    // Multiply all possible combinations of matrices from
    // the basis set of the vector spaces
    thisBeg = basis.begin();
    thisEnd = basis.end();
    while(thisBeg != thisEnd) {
        int w = ((*thisBeg).m)[0];
        // t1 = thisBeg \times m1
        Matrix t1 = Matrix::multiply((*thisBeg).m, m1);
        // t2 =  w \times m2
        int *t2_val = new int[Nsq];
        for(i=0;i<Nsq;i++) {
            t2_val[i] = w * m2[i];
        }
        Matrix t2(t2_val, N);
        delete [] t2_val; // FIXME: is this correct?
        Matrix transformed = Matrix::add(t1, t2);

        result->basis.push_back(ModularMatrix(transformed,Nsq));
        thisBeg++;
    }

    //
    // Find the basis so that we can work on a finite set always
    //
    result->findBasis();
    return result;
}
#else
ModuleSpace *ModuleSpace::call_site_transform(const int *m1, const int *m2) {
    MSIterator thisBeg,thisEnd,op2Beg,op2End;
    ModuleSpace *result=new ModuleSpace(N);
    int i;   

    // Multiply all possible combinations of matrices from
    // the basis set of the vector spaces
    thisBeg = basis.begin();
    thisEnd = basis.end();
    while(thisBeg != thisEnd) {
        int w = ((*thisBeg).m)[0];
        // t1 = thisBeg \times m1
        int *t1 = multiplyMatrices((*thisBeg).m, m1);
        // t2 =  w \times m2 + t1
        int *t2 = new int[Nsq];
        for(i=0;i<Nsq;i++) {
            t2[i] = (w * m2[i]) + t1[i];
        }
        delete [] t1; 

        result->basis.push_back(ModularMatrix(t2,Nsq));
        thisBeg++;
    }

    //
    // Find the basis so that we can work on a finite set always
    //
    result->findBasis();
    return result;
}
#endif

///////////////////////////////////////////////////////////////
// Modular-SVD (A = LDR). We need only D and Rinv and Linv
///////////////////////////////////////////////////////////////

// Modifies A into D. Memory for Rinv, Linv already allocated
void ModuleSpace::modular_svd(int *A, int *Rinv, int *Linv, int A_vec_size) {
    int i,j,pos,r,k,d,rprime,dprime,x;
    svd_vec_size = A_vec_size; // set class data member

    // Set Rinv, Linv = ID
    int *tmp = Rinv;
    int *tmp2 = Linv;
    for(i=0; i < svd_vec_size; i++) {
        for(j=0; j < svd_vec_size; j++, tmp++, tmp2++) {
            if(i==j) {
                *tmp = 1;
                *tmp2 = 1;
            } else {
                *tmp = 0;
                *tmp2 = 0;
            }
        }
    }
    // The guassian part
    for(pos = 0; pos < svd_vec_size - 1; pos++) {
        MODULE_DBGS(print_mat(Rinv, svd_vec_size, "Rinv"));
        MODULE_DBGS(print_mat(Linv, svd_vec_size, "Linv"));

        find_pivot(A,pos,i,j);
        r = compute_rank(A[i*svd_vec_size+j]);
        // bring pivot to diagonal
        col_swap(A, pos, pos, j);
        row_swap(A, pos, pos, i);
        // Do column operations on Rinv, row operations on Linv
        col_swap(Rinv, 0, pos, j);
        row_swap(Linv, 0, pos, i);

        i = pos; j = pos; // pivot has moved
        if(r == MAX_POWER) { // Row and Column are already zero
            continue;
        }
        // Zero out rows
        d = DIV_BY_POWER_OF_2(A[i*svd_vec_size+j],r);
        for(k = pos + 1; k < svd_vec_size; k++) {
            rprime = compute_rank(A[k*svd_vec_size + j]);
            if(rprime == MAX_POWER) continue; // already zero

            assert(r <= rprime);
            dprime = DIV_BY_POWER_OF_2(A[k*svd_vec_size + j],rprime);
            x = MULT_BY_POWER_OF_2(dprime, rprime - r);

            row_mult(A, pos, k, d);
            row_sub(A, pos, k, i, x);

            row_mult(Linv, 0, k, d);
            row_sub(Linv, 0, k, i, x);

            assert(A[k*svd_vec_size + j] == 0);
        }
        // Now do column operations: They're simple because the j^th column 
        // is now {2^r * d , 0 , 0 .. , 0}^t -- so we just zero out the
        // i^th row but we need to do col operations on Rinv
        tmp = A + (i*svd_vec_size+j+1);
        for(k = pos + 1; k < svd_vec_size; k++) {
            // We need to change Rinv
            rprime = compute_rank(*tmp);
            if(rprime == MAX_POWER) {
                tmp++;
                continue; // already zero
            }

            assert(r <= rprime);
            dprime = DIV_BY_POWER_OF_2((*tmp),rprime);
            x = MULT_BY_POWER_OF_2(dprime,rprime - r);
            col_mult(A, pos, k, d);
            col_mult(Rinv, 0, k, d);
            col_sub(Rinv, 0, k, j, x);

            *tmp = 0;
            tmp++;
        }
    }
}

// In place column operation: Swap
void ModuleSpace::col_swap(int *A, int pos, int c1, int c2) {
    if(c1 == c2) return;
    int i,j,k,tmp;
    i = pos*svd_vec_size + c1, j = pos*svd_vec_size + c2; 
    for(k=pos; k < svd_vec_size; k++) {
        tmp = A[i];
        A[i] = A[j];
        A[j] = tmp;
        i += svd_vec_size;
        j += svd_vec_size;
    }
}

// In place column operation: col = d * col
void ModuleSpace::col_mult(int *A, int pos, int col, int d) {
    if (d==1) return;
    int i=pos*svd_vec_size + col, j;
    for(j=pos;j<svd_vec_size;j++) {
        A[i] *= d;
        i += svd_vec_size;
    }
}

// In place column operation: col1 = col1 - x*col2
void ModuleSpace::col_sub(int *A, int pos, int col1, int col2, int x) {
    if(x==0) return;
    int i=pos*svd_vec_size + col1, j=pos*svd_vec_size + col2, k;
    for(k=pos;k<svd_vec_size;k++) {
        A[i] -= x*A[j];
        i+=svd_vec_size;
        j+=svd_vec_size;
    }
}

// In place row operation: Swap
void ModuleSpace::row_swap(int *A, int pos, int r1, int r2) {
    if(r1 == r2) return;
    int i,j,k,tmp;
    i = r1*svd_vec_size + pos, j = r2*svd_vec_size + pos; 
    for(k=pos; k < svd_vec_size; k++) {
        tmp = A[i];
        A[i] = A[j];
        A[j] = tmp;
        i ++;
        j ++;
    }
}

// In place row operation: row = d * row
void ModuleSpace::row_mult(int *A, int pos, int row, int d) {
    if (d==1) return;
    int i=row*svd_vec_size + pos, j;
    for(j=pos;j<svd_vec_size;j++) {
        A[i] *= d;
        i ++;
    }
}

// In place row operation: row1 = row1 - x*row2
void ModuleSpace::row_sub(int *A, int pos, int row1, int row2, int x) {
    if(x==0) return;
    int i=row1*svd_vec_size + pos, j=row2*svd_vec_size + pos, k;
    for(k=pos;k<svd_vec_size;k++) {
        A[i] -= x*A[j];
        i++;
        j++;
    }
}

#ifndef NORMAL_PIVOT_FINDING
// Finds pivot in A[pos:N-1, pos:N-1]: minimum rank element in a row and column
void ModuleSpace::find_pivot(int *A, int pos, int &pivot_posi, int &pivot_posj) {
    if(pos >= svd_vec_size) return; // raise error ?
    int i = pos,j = pos;
    is_smallest_row(A,pos,i,j);
    // find smallest in a row and check if its the smallest in its column
    // and repeat
    bool done = false;
    while(!done) {
        if(is_smallest_col(A,pos,i,j)) {
            done = true;
        } else if(is_smallest_row(A,pos,i,j)) {
            done = true;
        }
    }
    pivot_posi = i;
    pivot_posj = j;
}
#else
void ModuleSpace::find_pivot(int *A, int pos, int &pivot_posi, int &pivot_posj) {
    if(pos >= svd_vec_size) return;
    int i,j,orig_i;
    for(i=pos;i<svd_vec_size;i++) {
        j = pos; orig_i = i;
        is_smallest_row(A,pos,i,j);
        if(is_smallest_col(A,pos,i,j)) {
            pivot_posi = i;
            pivot_posj = j;
            return;
        } else
            i = orig_i;
    }
    // we are bound to have found a pivot by now
    assert(0);
}
#endif

// is (i,j) the smallest element in A(i,pos:N-1) ? If yes, return true
// else return the smallest element back through i,j
// precondition: pos <= i,j < N
bool ModuleSpace::is_smallest_row(int *A, int pos, int &i, int &j) {
    int *tmp = A + (i*svd_vec_size+pos) ;
    int min = compute_rank(A[i*svd_vec_size + j]), r;
    int minpos = j;
    for(int k = pos; k < svd_vec_size; k++) {
        if((r = compute_rank(*tmp)) < min) {
            min = r;
            minpos = k;
        }
        tmp++;
    }
    if(minpos == j) return true;
    j = minpos;
    return false;
}

// is (i,j) the smallest element in A(pos:N-1,j) ? If yes, return true
// else return the smallest element back through i,j
// precondition: pos <= i,j < N
bool ModuleSpace::is_smallest_col(int *A, int pos, int &i, int &j) {
    int *tmp = A + (pos*svd_vec_size+j) ;
    int min = compute_rank(A[i*svd_vec_size + j]), r;
    int minpos = i;
    for(int k = pos; k < svd_vec_size; k++) {
        if((r = compute_rank(*tmp)) < min) {
            min = r;
            minpos = k;
        }
        tmp+=svd_vec_size;
    }
    if(minpos == i) return true;
    i = minpos;
    return false;
}

} // namespace AR
