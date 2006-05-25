//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2002-2004, Gogul Balakrishnan, Thomas Reps
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
// e-mail: bgogul@cs.wisc.edu, reps@cs.wisc.edu
//////////////////////////////////////////////////////////////////////////////
#include "ARConfig.h"
#include "Matrix.h"
#include <assert.h>
#include <iostream>

const int RCMatrix::N = AR::dim;
const int RCMatrix::NbyN = N * N;

//
// Initialize the hashtable used for canoncial matrices
//
RCMatrix::CnclMats* RCMatrix::cnclMats = 0;//new Matrix::CnclMats(1007);
Matrix::ProdCache* Matrix::prodCache = 0;
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
			cnclMats->erase(mat);
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
RCMatrix::MatrixHkey_t RCMatrix::computeHashKey(const int* p) {
	// FIXME: this should be made better?
	unsigned int rv;
	int i = 0;
	rv = 0;
	const int* matPtr = p;
	for(i =0; i < RCMatrix::NbyN; ++i) {
		rv = 997 * rv + *matPtr;
		matPtr++;
	}
  	return rv;
}



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// M A T R I X  I M P L E M E N T A T I O N 
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//-------------------------------------
// Get the canonical matrix
//-------------------------------------
RCMatrix* RCMatrix::getCnclMatrix(const int* p) {
	if(!cnclMats)
		cnclMats = new RCMatrix::CnclMats(100000);
	// Nick's Hash
	CnclMats::iterator it = cnclMats->find(p);
	// insert it if not there
	if( it == cnclMats->end() ) {
		RCMatrix* m = new RCMatrix( p );
		cnclMats->insert( CnclMats::value_type( m->mat, m ) );
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
        //TODO?: iterate through and decrRef each entry in cnclMats!
        delete cnclMats;
        cnclMats = 0;
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
	for(int i = 0; i < RCMatrix::N; i++) {
		out << "  ";
		for(int j = 0; j < RCMatrix::N; j++) {
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
	int x = 0;
	fprintf(fp, "[\n");
	for(int i = 0; i < RCMatrix::N; i++) {
		fprintf(fp, "  ");
		for(int j = 0; j < RCMatrix::N; j++) {
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
Matrix::Matrix()
{
  int *tempMatrix = new int[RCMatrix::NbyN];

  int *p = tempMatrix;
  for(int i = 0; i < RCMatrix::N; i++) {
    for(int j = 0; j < RCMatrix::N; j++) {
      if (i == j) {
        *p = 1;
      }
      else {
        *p = 0;
      }
      p++;
    }
  }
  rcmat = RCMatrix::getCnclMatrix(tempMatrix);
  delete tempMatrix;
  nMats++;
}


//------------------------------------
// Return the identity matrix
//------------------------------------
Matrix Matrix::mkId() {
  static Matrix m;   // Default constructor creates the identity matrix
  return m;
}


//-------------------------------------
// Return a transposed matrix.
// ------------------------------------
Matrix Matrix::transpose(const Matrix& m) {
  static int *p = new int[RCMatrix::NbyN];
  int *q = m.rcmat->mat;
  for (int i = 0; i < RCMatrix::N; i++) {
    for (int j = 0; j < RCMatrix::N; j++) {
      p[i * RCMatrix::N + j] = q[j * RCMatrix::N + i];
    }
  }
  return Matrix(p);
}


//-------------------
// Add two matrices
//-------------------
Matrix Matrix::add(const Matrix& mat1, const Matrix& mat2) {
  static int *result = new int[RCMatrix::NbyN];
  int i, j;
  for (i = 0; i < RCMatrix::N; i++) {
    for (j = 0; j < RCMatrix::N; j++) {
      result[i * RCMatrix::N + j] = mat1[i * RCMatrix::N + j] + mat2[i * RCMatrix::N + j];
    }
  }
  return Matrix(result);
}

//------------------------------------
// Find the difference of two matrices
//------------------------------------
Matrix Matrix::subtract(const Matrix& mat1, const Matrix& mat2) {
  static int *result = new int[RCMatrix::NbyN];
  int i, j;
  for (i = 0; i < RCMatrix::N; i++) {
    for (j = 0; j < RCMatrix::N; j++) {
      result[i * RCMatrix::N + j] = mat1[i * RCMatrix::N + j] - mat2[i * RCMatrix::N + j];
    }
  }
  return Matrix(result);
}

//------------------------
// Multiply two matrices
//------------------------
Matrix Matrix::multiply(const Matrix& mat1, const Matrix& mat2) {

#ifdef NO_PROD_CACHING
	static int *multResult=new int[RCMatrix::NbyN];
	multiplyMatrices(multResult, mat1.rcmat->mat, mat2.rcmat->mat);
	Matrix *m = new Matrix(multResult);
	return m;
#else

	if(!prodCache)
		prodCache = new ProdCache(200000);
	
	
	static int *multResult=new int[RCMatrix::NbyN];
	cacheAccesses++;
	
	Matrix* m;
	MatrixPair pair(mat1.rcmat, mat2.rcmat);
	ProdCache::iterator it = prodCache->find(pair);
	// insert it if not there
	if( it == prodCache->end() ) {
		// Not in cache
		multiplyMatrices(multResult, mat1.rcmat->mat, mat2.rcmat->mat);
		m = new Matrix(multResult);
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
void Matrix::multiplyMatrices(int* multResult, const int *op1,const int *op2) {
	
	int i,j,k;
	for(i = 0; i < RCMatrix::N; ++i) {
		for(j = 0; j < RCMatrix::N; ++j) {
			multResult[i * RCMatrix::N + j]=0;
			for(k = 0; k < RCMatrix::N; ++k) {
				multResult[i * RCMatrix::N + j] += op1[i * RCMatrix::N + k] * op2[k * RCMatrix::N + j];
			}
		}
	}
}


