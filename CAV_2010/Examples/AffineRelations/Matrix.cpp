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


#include "ARConfig.hpp"
#include "Matrix.hpp"
#include <iostream>


namespace AR {
    //
    // Initialize the hashtable used for canoncial matrices
    //
    RCMatrix::CnclMats* RCMatrix::cnclMats = NULL;
    Matrix::ProdCache* Matrix::prodCache = NULL;
    const float Matrix::cacheEntryThreshold = 0.90f;        // 
    const Matrix::ProdCache::size_type Matrix::maxProdCacheSize = 100000;  // Max size of cache
    MatrixPair::MatrixPairUsage_t Matrix::transAccessCnt = 1;      //
    // Some stats
    unsigned int Matrix::cacheHits = 0;
    unsigned int Matrix::cacheAccesses = 0;
    unsigned int Matrix::nMats = 0;




    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    // O v e r l o a d e d  < <  O p e r a t o r s 
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    // R C M A T R I X   I M P L E M E N T A T I O N 
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    //
    // Decrease ref count
    //
    void RCMatrix::decrRef()	{
        assert(count > 0);
        count = (satRef?count:--count);
        if(!count) {
            if(cnclMats)
                cnclMats->erase(MatrixDimPair_t(mat, N));
            delete this;
        }
    }

    //
    // Increase ref count
    //
    void RCMatrix::incrRef() { 
        if(!satRef) {
            if(count == UINT_MAX) 
                satRef = true; 
            else
                count++;
        }
        return;
    }



    //---------------------------------------------- 
    // Compute the haskey for a given matrix
    //----------------------------------------------
    RCMatrix::MatrixHkey_t RCMatrix::computeHashKey(const int* p, unsigned _N) {
        // FIXME: this should be made better?
        unsigned int rv;
        int i = 0;
        rv = _N;
        const int* matPtr = p;
        unsigned NbyN = _N * _N;

        // const row
        //
        for(i = 0; i < _N; ++i) {
            rv = 997 * rv + *matPtr;
            matPtr++;
        }
        // diagonal 
        matPtr = p;
        for(i = 0; i < _N; ++i) {
            rv = 997 * rv + *matPtr;
            matPtr += (_N + 1);
        }
        /*
           for(i =0; i < NbyN; ++i) {
           rv = 997 * rv + *matPtr;
           matPtr++;
           }
           */
        return rv;
    }



    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    // M A T R I X  I M P L E M E N T A T I O N 
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    //-------------------------------------
    // Get the canonical matrix
    //-------------------------------------
    RCMatrix* RCMatrix::getCnclMatrix(const int* p, unsigned _N) {
        if(!cnclMats) {
            // Default settings...
            cnclMats = new RCMatrix::CnclMats(100000);
        }
        // Nick's Hash
        RCMatrix::MatrixDimPair_t pn(p, _N);
        CnclMats::iterator it = cnclMats->find(pn);
        // insert it if not there
        if( it == cnclMats->end() ) {
            RCMatrix* m = new RCMatrix( p, _N );
            cnclMats->insert( CnclMats::value_type( MatrixDimPair_t(m->mat, m->N), m ) );
            m->incrRef();
            return m;
        }
        else {
            RCMatrix* m = (*it).second;
            m->incrRef();
            return m;
        }

    }

    //-------------------------------------
    // Delete (cleanup) the table of canonical matrices
    //-------------------------------------
    void RCMatrix::deleteCnclMatrices() {
        if(cnclMats){		
            delete cnclMats;
            cnclMats = NULL;
        }
    }

    void Matrix::clearCaches() {
        RCMatrix::deleteCnclMatrices();
        if(prodCache){
            delete Matrix::prodCache;
            prodCache = 0;
        }
    }

    void Matrix::printMatrixStats(FILE* fp) {
        fprintf(fp, "Cache Behaviour:\n");
        fprintf(fp, "  Hits:\t%u\n", cacheHits);
        fprintf(fp, "  Accesses:\t%u\n", cacheAccesses);
        if(cacheAccesses)
            fprintf(fp, "  Hit Rate:\t%.2f\n", (float)cacheHits/cacheAccesses);
        fprintf(fp, "Canonical Mats:\n");
        fprintf(fp, "Total Nr Of Matrices:\t%u\n", nMats);
        fprintf(fp, "CnclMatrixTable:");
        /*
           if(RCMatrix::cnclMats)
           RCMatrix::cnclMats->PrintHashDist(fp);
           fprintf(fp, "Product Cache:");
           if(prodCache)
           prodCache->PrintHashDist(fp);
           */
    }


    //-------------------
    // Print
    //-------------------
    void Matrix::Print(std::ostream & out) const
    {
        int x = 0;
        out << "[" << std::endl;
        for(int i = 0; i < getDim(); i++) {
            out << "  ";
            for(int j = 0; j < getDim(); j++) {
                out << rcmat->mat[x] << ", ";
                x++;
            }
            out << std::endl;
        }
        out << "]";
        return;
    }

    //-------------------
    // Print
    //-------------------
    void Matrix::prettyPrint(FILE* fp) const
    {
        unsigned N = getDim();
        int x = 0;
        fprintf(fp, "[\n");
        for(int i = 0; i < N; i++) {
            fprintf(fp, "  ");
            for(int j = 0; j < N; j++) {
                fprintf(fp, "%d, ", rcmat->mat[x]);
                x++;
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "]");
        return;
    }


    std::ostream& operator<< (std::ostream & out, const Matrix &m)
    {
        m.Print(out);
        return(out);
    }

    //---------------------------------
    // Default constructor
    //
    // Creates the identity matrix
    //---------------------------------
    Matrix::Matrix(unsigned N)
    {
        int *tempMatrix = new int[N * N];

        int *p = tempMatrix;
        for(unsigned i = 0; i < N; i++) {
            for(unsigned j = 0; j < N; j++) {
                if (i == j) {
                    *p = 1;
                }
                else {
                    *p = 0;
                }
                p++;
            }
        }
        rcmat = RCMatrix::getCnclMatrix(tempMatrix, N);
        delete tempMatrix;
        nMats++;
    }


    //------------------------------------
    // Return the identity matrix
    //------------------------------------
    Matrix Matrix::mkId(unsigned N) {
        static unsigned curN = 0;
        static Matrix* m = NULL;
        if(curN == N) {
            return *m;
        }
        curN = N;
        delete m;
        m = new Matrix(N);
        return *m;
        //static Matrix m(N);   // Default constructor creates the identity matrix
        //return m;
    }


    //-------------------------------------
    // Return a transposed matrix.
    // ------------------------------------
    Matrix Matrix::transpose(const Matrix& m) {
        unsigned N = m.getDim();

        // allocate space for result
        static unsigned lastN = 0;
        static int *p = NULL;
        if(lastN < N) {
            p = new int[N * N];
            lastN = N;
        }

        int *q = m.rcmat->mat;
        for (unsigned i = 0; i < N; i++) {
            for (unsigned j = 0; j < N; j++) {
                p[i * N + j] = q[j * N + i];
            }
        }
        return Matrix(p, N);
    }


    //-------------------
    // Add two matrices
    //-------------------
    Matrix Matrix::add(const Matrix& mat1, const Matrix& mat2) {
        assert(mat1.getDim() == mat2.getDim());
        unsigned N = mat1.getDim();

        // allocate space for result
        static unsigned lastN = 0;
        static int *result = NULL;
        if(lastN < N) {
            result = new int[N * N];
            lastN = N;
        }

        unsigned i, j;
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                result[i * N + j] = mat1[i * N + j] + mat2[i * N + j];
            }
        }
        return Matrix(result, N);
    }

    //------------------------------------
    // Find the difference of two matrices
    //------------------------------------
    Matrix Matrix::subtract(const Matrix& mat1, const Matrix& mat2) {
        assert(mat1.getDim() == mat2.getDim());
        unsigned N = mat1.getDim();

        // allocate space for result
        static unsigned lastN = 0;
        static int *result = NULL;
        if(lastN < N) {
            result = new int[N * N];
            lastN = N;
        }

        unsigned i, j;
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                result[i * N + j] = mat1[i * N + j] - mat2[i * N + j];
            }
        }
        return Matrix(result, N);
    }

    //------------------------
    // Multiply two matrices
    //------------------------
    Matrix Matrix::multiply(const Matrix& mat1, const Matrix& mat2) {
        assert(mat1.getDim() == mat2.getDim());

        unsigned N = mat1.getDim();

        // allocate space for result
        static unsigned lastN = 0;
        static int *multResult = NULL;
        if(lastN < N) {
            multResult = new int[N * N];
            lastN = N;
        }

#ifdef NO_PROD_CACHING
        multiplyMatrices(multResult, mat1.rcmat->mat, mat2.rcmat->mat, N);
        Matrix *m = new Matrix(multResult, mat1.getDim());
        return m;
#else

        if(!prodCache)
            prodCache = new ProdCache(200000);


        cacheAccesses++;

        Matrix* m;
        MatrixPair pair(mat1.rcmat, mat2.rcmat);
        ProdCache::iterator it = prodCache->find(pair);
        // insert it if not there
        if( it == prodCache->end() ) {
            // Not in cache
            multiplyMatrices(multResult, mat1.rcmat->mat, mat2.rcmat->mat, N);
            m = new Matrix(multResult, N);
            prodCache->insert( ProdCache::value_type( pair, m->rcmat ) );
            m->rcmat->incrRef();	// Account for the fact that rcmat is in prodCache
            // FIXME: Need a policy to remove matrices in prodCache
        }
        else {
            cacheHits++;

            m = new Matrix((*it).second);
            (*it).first.incUsageCnt();
            assert(m->rcmat->mat);
        }

#ifdef VSA_ARA_USE_LFU_POLICY	
        // Throw away least-frequently used cache entries 
        MatrixPair::incUsageCnt(transAccessCnt);
        if(prodCache->size() >= maxProdCacheSize) {
            std::vector<MatrixPair> toBeRemoved;

            // Mark entries whose usage is below threshold
            ProdCache::iterator beg = prodCache->begin();
            ProdCache::iterator end = prodCache->end();
            while(beg != end) {
                if(((float)(*beg).first.getUsageCnt()/transAccessCnt) < cacheEntryThreshold) {
                    toBeRemoved.push_back((*beg).first);
                }
                beg++;
            }

            // Remove the marked entries
            for(unsigned i = 0; i < toBeRemoved.size(); ++i) {
                ProdCache::iterator it = prodCache->find(toBeRemoved[i]);
                // NOTE: it cannot be end
                (*it).second->decrRef(); // Update reference count for RCmatrix 
                prodCache->erase(it);
            }
            transAccessCnt = 1;
        }
#endif
        // FIXME: A bit clumsy here . . .
        Matrix retM = *m;
        delete m;
        return retM;
#endif // NO_PROD_CACHING
    }



    //----------------------------------
    // Multiply two matrices (Helper)
    //----------------------------------
    void Matrix::multiplyMatrices(int* multResult, const int *op1,const int *op2, unsigned N) {

        unsigned i, j, k;
        for(i = 0; i < N; ++i) {
            for(j = 0; j < N; ++j) {
                multResult[i * N + j]=0;
                for(k = 0; k < N; ++k) {
                    multResult[i * N + j] += op1[i * N + k] * op2[k * N + j];
                }
            }
        }
    }

} // namespace AR
