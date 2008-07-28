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
#pragma warning (disable: 4786)
#pragma warning (disable: 4005)
#pragma warning (disable: 4786)
#pragma warning (disable: 4099)
#pragma warning (disable: 4800)


#include <iostream>
#include <cmath>
#if 0 //def USE_AFFINERELS_CLASS_WITH_VSA
#	include "cmdline.h"
#	include "wpdspp_ar_interface.h"
#	include "wpdsBBInterface.h"
#	include "Debug.h"
#endif
#include "AffineRels.hpp"
#include "ModuleSpace.hpp"

namespace AR {


    // Idspace 
    ModuleSpace *AffineRels::idspace = new ModuleSpace(AR::dim, true);


#ifdef USE_AFFINERELS_CLASS_WITH_VSA

    void AffineRels::initAffineRelations() {
#ifdef BASIC_BLOCKS_OPTION
        if(flag_basic_blocks)
#endif
            initAffRelWpdspp_BB();
#ifdef BASIC_BLOCKS_OPTION
        else
            initAffRelWpdspp();
#endif
    }


    //--------------------------------------------
    /// Determine the set of affine relations that
    /// hold at the given CFGnode node.
    ///
    /// @param node The cfg node of interest.
    /// @return An AffineRels object that reprsents the set of affine relations.
    ///
    //--------------------------------------------
    AffineRels *AffineRels::getAffineRels(nCFGnode *node) {

#ifdef BASIC_BLOCKS_OPTION
        if(flag_basic_blocks)
#endif
            return wpdsppGetAffineRels_BB(node);
#ifdef BASIC_BLOCKS_OPTION
        else
            return wpdsppGetAffineRels(node);
#endif
    }

#endif

    //----------------------------------
    /// Default constructor. 
    //----------------------------------
    AffineRels::AffineRels() {
    }

    //-------------------------------------------------------
    // 
    //-------------------------------------------------------
    AffineRels::AffineRels(ModuleSpace *pVs) {
        pVs->findAffineRelations(*this);
    }

    //-------------------------------------------------------
    //
    //-------------------------------------------------------
    void AffineRels::clear() {
        affRels.clear();
    }

    //-------------------------------------------------------
    /// @return Number of affine relations.
    //-------------------------------------------------------
    unsigned AffineRels::nrOfRels() const {
        return affRels.size();
    }

    //-------------------------------------------------------
    // copy rhs to this.
    //-------------------------------------------------------
    void AffineRels::copy(AffineRels *rhs) {
        affRels.clear();
        for(unsigned i=0;i<rhs->nrOfRels();++i) {
            insert(rhs->getAffineRel(i));
        }
    }

    //-------------------------------------------------------
    // 	Insert an affine relation.
    //-------------------------------------------------------
    void AffineRels::insert(AffRel_t affrel) {
        AffRel_t rel=new AffRelCoeff_t[AR::dim];
        for(int i = 0; i < AR::dim; ++i) {
            rel[i]=affrel[i];
        }
        affRels.push_back(rel);
    }

    //-------------------------------------------------------
    /// @param relNr The index of the relation sought for.
    /// @return An array of coefficients for that 
    ///  particular affine relation.
    /// @pre 0<=relNr<affRels.size()
    //-------------------------------------------------------
    AffineRels::AffRel_t AffineRels::getAffineRel(unsigned int relNr) const {
        return affRels[relNr];
    }

    //---------------------------------------------------------
    /// @param relNr The index of the relation.
    /// @param varNr The variable number.
    /// @return Whether the coefficient of the variable is non-zero.
    /// @pre 0<=relNr<affRels.size(), 1<=varNr<=NVARS
    //---------------------------------------------------------
    bool AffineRels::hasVar(int relNr,int varNr) const{
        AffRel_t ar = affRels[relNr];
        return(ar[varNr]!=0);
    }

    //-----------------------------------------
    // Does this affine relation have non-integer
    // coefficients.
    ///
    /// @param relNr The relation number.
    /// @pre 0<= relNr < this->size()
    //-----------------------------------------
    bool AffineRels::hasNonIntegerCoeffs(int relNr) const{

        AffRel_t ar = affRels[relNr];

        bool hasDouble=false;

        for(unsigned varNr=0;varNr < AR::dim;++varNr) {
            if(ceil(ar[varNr]) != ar[varNr]) {
                hasDouble=true;
                break;
            }
        }
        return hasDouble;
    }

    //---------------------------------------------------------------------------
    /// The number of variables with non-zero coefficients (excludes const).
    ///
    /// @param relNr The relation number.
    /// @pre 0<= relNr < this->size()
    //---------------------------------------------------------------------------
    unsigned AffineRels::nNonZeroCoeffs(int relNr) const {
        unsigned cnt = 0;
        AffRel_t ar = affRels[relNr];
        for(unsigned varNr = 1;varNr < AR::dim;++varNr) {
            if( ar[varNr] != 0) {
                cnt++;
            }
        }
        return cnt;

    }

    //-------------------------------------------------------
    /// Destructor. 
    //-------------------------------------------------------
    AffineRels::~AffineRels() {
        for(unsigned i=0;i<affRels.size();++i) 
            delete [] affRels[i];
        affRels.clear();



    }



    //-------------------------------------------------------
    /// @param fp The output file.
    /// @param varNames An array of null-terminated strings
    ///  that will be used as the variable names.
    //-------------------------------------------------------
    void AffineRels::prettyPrint(FILE *fp, char **varNames) const{
        for(unsigned i=0;i<affRels.size();++i)  {
            fprintf(fp,"%d ",affRels[i][0]);
            for(int j=1;j<AR::dim;++j) { 
                if(affRels[i][j]!=0)
                    fprintf(fp," + %d*%s",affRels[i][j],varNames[j]);
            }
            fprintf(fp,"\n");
        }
    }


    //-------------------------------------------------------
    /// @param fp The output file.
    /// @param varNames An array of null-terminated strings
    ///  that will be used as the variable names.
    //-------------------------------------------------------
    std::ostream& AffineRels::prettyPrint(std::ostream &out, char **varNames) const {
        std::ios::fmtflags oldsettings=out.flags();
        out.setf(std::ios::fixed, std::ios::floatfield);
        out.precision(2);
        for(unsigned i=0;i<affRels.size();++i)  {
            out << affRels[i][0] << " ";

            for(int j=1;j<AR::dim;++j) {
                if(affRels[i][j]!=0)
                    out <<" + " << affRels[i][j]<<"*" << varNames[j];

            }
            out << std::endl;
        }
        out.flags(oldsettings);
        return out;
    }

    //------------------------------------------------
    /// Update global stats about affine relations.
    //-------------------------------------------------
    void AffineRels::updateARStats() const{
        //- Update the dimension distribution
        AR::nDimDist[nrOfRels()]++;

        //- Update the number of variable distribution
        //
        for(unsigned i = 0; i < nrOfRels(); ++i) {
            AR::nVarDist[nNonZeroCoeffs(i)]++;	
        }

        return;
    }

} // namespace AR
