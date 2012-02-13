#include "BinRelManager.hpp"
#include "BuddyExt.hpp"

#include "glog/logging.h"
#include "buddy/fdd.h"

using namespace  wali::domains::binrel;

// ////////////////////////////////////////
//     TRASH IN --- TRASH OUT            //
// ////////////////////////////////////////

//TODO: remove this comment, and to do that, put comments elsewhere.
namespace wali
{
  namespace domains
  {
    namespace binrel
    {

#define BOOLSIZE 2

      static BddInfo_t regAbool, regBbool, regAint, regBint;

      static bdd bddAnd, bddOr, bddNot;
      static bdd bddPlus, bddMinus, bddTimes, bddDiv;


      //It's a good habit to forward declare all the static functions in the
      //file so that there is an index and so that the contents of the file can
      //move around freely.

      static void reset();
      static bdd applyBinOp(bdd lexpr, bdd rexpr, bdd op, BddInfo_t regA,
          BddInfo_t regB);
      static bdd applyUnOp(bdd expr, bdd op, BddInfo_t regA);
      static bdd applyBinOpBool(bdd lexpr, bdd rexpr, bdd op);
      static bdd applyBinOpInt(bdd lexpr, bdd rexpr, bdd op);
      static bdd applyUnOpBool(bdd expr, bdd op);

      // ////////////////////////////////////////////////////////////////////////////
      // The interface assumes that the google logging library is initialized before
      // calling any of its functions.

      void addBoolVar(Voc& voc, std::string name)
      {
        BddInfo_t bi = new BddInfo;
        bi->maxVal = 2;
        voc[name] = bi;
      }

      void addIntVar(Voc& voc, std::string name, int size)
      {
        LOG_IF(WARNING, size < 2) 
          << "I haven't tested the library for int size less than 2";
        BddInfo_t bi = new BddInfo;
        bi->maxVal = size;
        voc[name] = bi;
      }

      static void reset()
      {
        //set all BddInfo_t objects to NULL. The BddInfo counts should drop to
        //0, and the objects should be deleted on their own.
        regAbool = NULL;
        regBbool = NULL;
        regAint = NULL;
        regBint = NULL;

        //reste all bdds
        bddAnd = bddfalse;
        bddOr = bddfalse;
        bddNot = bddfalse;
        bddPlus = bddfalse;
        bddMinus = bddfalse;
        bddTimes = bddfalse;
        bddDiv = bddfalse;
      }

      const Voc initialize(int bddMemSize, int cacheSize, Voc voc)
      {
        reset();
        const Voc retVoc = BinRel::initialize(bddMemSize, cacheSize, voc);

        regAbool = NULL;
        regBbool = NULL;
        for(
            Voc::const_iterator iter = retVoc.begin(); 
            iter != retVoc.end();
            ++iter
           ){
          if(regBbool != NULL)
            break;
          if((iter->second)->maxVal == BOOLSIZE){
            if(regAbool == NULL)
              regAbool = iter->second;
            else
              regBbool = iter->second;
          }
        }

        regAint = NULL;
        regBint = NULL;
        for(
            Voc::const_iterator iter = retVoc.begin(); 
            iter != retVoc.end();
            ++iter
           ){
          if(regBint != NULL)
            break;
          if((iter->second)->maxVal > BOOLSIZE){
            if(regAint == NULL)
              regAint = iter->second;
            else
              regBint = iter->second;
          }
        }

        if(regAbool == NULL || regBbool == NULL){
          LOG(INFO) << "[binrel::initialize] I did not find enough Boolean vars"
            << "\nAnd(...) will not be functional.";
        }else{
          bddAnd =
            (fdd_ithvar(regAbool->baseRhs,1) & 
             fdd_ithvar(regBbool->baseRhs,1) &
             fdd_ithvar(regAbool->baseExtra,1))
            |
            (fdd_ithvar(regAbool->baseRhs,1) & 
             fdd_ithvar(regBbool->baseRhs,0) &
             fdd_ithvar(regAbool->baseExtra,0))
            |
            (fdd_ithvar(regAbool->baseRhs,0) & 
             fdd_ithvar(regBbool->baseRhs,1) &
             fdd_ithvar(regAbool->baseExtra,0))
            |
            (fdd_ithvar(regAbool->baseRhs,0) & 
             fdd_ithvar(regBbool->baseRhs,0) &
             fdd_ithvar(regAbool->baseExtra,0));
        }

        if(regAbool == NULL || regBbool == NULL){
          LOG(INFO) << "[binrel::initialize] I did not find enough Boolean vars"
            << "\nOr(...) will not be functional.";
        }else{
          bddOr =
            (fdd_ithvar(regAbool->baseRhs,1) & 
             fdd_ithvar(regBbool->baseRhs,1) &
             fdd_ithvar(regAbool->baseExtra,1))
            |
            (fdd_ithvar(regAbool->baseRhs,1) & 
             fdd_ithvar(regBbool->baseRhs,0) &
             fdd_ithvar(regAbool->baseExtra,1))
            |
            (fdd_ithvar(regAbool->baseRhs,0) & 
             fdd_ithvar(regBbool->baseRhs,1) &
             fdd_ithvar(regAbool->baseExtra,1))
            |
            (fdd_ithvar(regAbool->baseRhs,0) & 
             fdd_ithvar(regBbool->baseRhs,0) &
             fdd_ithvar(regAbool->baseExtra,0));
        }

        if(regAbool == NULL){
          LOG(INFO) << "[binrel::initialize] I did not find enough Boolean vars"
            << "\nNot(...) will not be functional.";
        }else{
          bddNot =
            (fdd_ithvar(regAbool->baseRhs,1) & 
             fdd_ithvar(regAbool->baseExtra,0))
            |
            (fdd_ithvar(regAbool->baseRhs,0) & 
             fdd_ithvar(regAbool->baseExtra,1));
        }

        int maxVal=0;
        if(regAint != NULL)
          maxVal = regAint->maxVal;

        if(regAint == NULL || regBint == NULL){
          LOG(INFO) << "[binrel::initialize] I did not find enough int vars"
            << "\nPlus(...) will not be functional.";
        }else{
          bddPlus = bddfalse;
          for(int i=0; i<maxVal; ++i){
            for(int j=0; j<maxVal; ++j){
              int k = (i + j) % maxVal;
              bddPlus = bddPlus |
                (fdd_ithvar(regAint->baseRhs,i) &
                 fdd_ithvar(regBint->baseRhs,j) &
                 fdd_ithvar(regAint->baseExtra,k));
            }
          }
        }

        if(regAint == NULL || regBint == NULL){
          LOG(INFO) << "[binrel::initialize] I did not find enough int vars"
            << "\nMinus(...) will not be functional.";
        }else{
          bddMinus = bddfalse;
          for(int i=0; i<maxVal; ++i){
            for(int j=0; j<maxVal; ++j){
              int k = (i - j + maxVal) % maxVal;
              bddMinus = bddMinus |
                (fdd_ithvar(regAint->baseRhs,i) &
                 fdd_ithvar(regBint->baseRhs,j) &
                 fdd_ithvar(regAint->baseExtra,k));
            }
          }
        }

        if(regAint == NULL || regBint == NULL){
          LOG(INFO) << "[binrel::initialize] I did not find enough int vars"
            << "\nTimes(...) will not be functional.";
        }else{
          bddTimes = bddfalse;
          for(int i=0; i<maxVal; ++i){
            for(int j=0; j<maxVal; ++j){
              int k = (i * j) % maxVal;
              bddTimes = bddTimes |
                (fdd_ithvar(regAint->baseRhs,i) &
                 fdd_ithvar(regBint->baseRhs,j) &
                 fdd_ithvar(regAint->baseExtra,k));
            }
          }
        }

        if(regAint == NULL || regBint == NULL){
          LOG(INFO) << "[binrel::initialize] I did not find enough int vars"
            << "\nDiv(...) will not be functional.";
        }else{
          bddDiv = bddfalse;
          for(int i=0; i<maxVal; ++i){
            for(int j=0; j<maxVal; ++j){
              int k;
              if(j == 0)
                //arbitrary
                k = 0;
              else
                k = (int) i / j;
              bddDiv = bddDiv |
                (fdd_ithvar(regAint->baseRhs,i) &
                 fdd_ithvar(regBint->baseRhs,j) &
                 fdd_ithvar(regAint->baseExtra,k));
            }
          }
        }
        return retVoc;
      }

      std::ostream& dumpManagerData(std::ostream& o)
      {
        o << "BinRelManager dump:" << std::endl;
        regAbool->print(o << "regAbool: ") << std::endl;
        regBbool->print(o << "regBbool: ") << std::endl;
        regAint->print(o << "regAint: ") << std::endl;
        regBint->print(o << "regBint: ") << std::endl;

        o << "bddAnd: " << fddset << bddAnd << std::endl;
        o << "bddOr: " << fddset << bddOr << std::endl;
        o << "bddNot: " << fddset << bddNot << std::endl;
        o << "bddPlus: " << fddset << bddPlus << std::endl;
        o << "bddMinus: " << fddset << bddMinus << std::endl;
        o << "bddTimes: " << fddset << bddTimes << std::endl;
        o << "bddDiv: " << fddset << bddDiv << std::endl;

        return o;
      }

      bdd From(std::string var)
      {
        //TODO: This header should be placed in all functions below.
        bdd ret = bddfalse;
        if(!BinRel::is_initialized()){
          LOG(ERROR) << "From called before initializing BinRel";
          return bddfalse;
        }
        const BddInfo_t bi = (*BinRel::getVoc().find(var)).second;
        if(bi->maxVal == BOOLSIZE)
          ret = biimp(bi->maxVal, bi->baseLhs, regAbool->baseRhs);
        else
          ret = biimp(bi->maxVal, bi->baseLhs, regAint->baseRhs);

        return ret;
      }

      bdd True()
      {
        return fdd_ithvar(regAbool->baseRhs,1);
      }

      bdd False()
      {
        return fdd_ithvar(regAbool->baseRhs,0);
      }

      bdd Const(int val)
      {
        if(val >= regAint->maxVal){
          LOG(ERROR) << "[Const] Attempted to create a constant value larger "
            << "than maxVal";
          return bddfalse;
        }
        return fdd_ithvar(regAint->baseRhs, val);
      }

      bdd NonDet()
      {
        return bddtrue;
      }

      static bdd applyBinOp(bdd lexpr, bdd rexpr, bdd op, BddInfo_t regA,
          BddInfo_t regB)
      { 
        bddPair *regA2regB = bdd_newpair();
        fdd_setpair(regA2regB, regA->baseRhs, regB->baseRhs);
        rexpr = bdd_replace(rexpr, regA2regB);
        bdd_freepair(regA2regB);

        lexpr = lexpr & rexpr;

        //IMP: fddsets are unioned by & of the representing bdds (Not | as I had
        //earlier expected.
        lexpr = bdd_relprod(
            lexpr,
            op, 
            fdd_ithset(regA->baseRhs) & fdd_ithset(regB->baseRhs)
            );

        bddPair *regAExtra2Rhs = bdd_newpair();
        fdd_setpair(regAExtra2Rhs, regA->baseExtra, regA->baseRhs);
        lexpr = bdd_replace(lexpr, regAExtra2Rhs);
        bdd_freepair(regAExtra2Rhs);

        return lexpr;
      }

      static bdd applyUnOp(bdd expr, bdd op, BddInfo_t regA)
      {
        expr = bdd_relprod(
            expr,
            op,
            fdd_ithset(regA->baseRhs)
            );

        bddPair *regAExtra2Rhs = bdd_newpair();
        fdd_setpair(regAExtra2Rhs, regA->baseExtra, regA->baseRhs);
        expr = bdd_replace(expr, regAExtra2Rhs);
        bdd_freepair(regAExtra2Rhs);

        return expr;
      }

      static bdd applyBinOpBool(bdd lexpr, bdd rexpr, bdd op)
      {
        return applyBinOp(lexpr,rexpr,op,regAbool,regBbool);
      }

      static bdd applyBinOpInt(bdd lexpr, bdd rexpr, bdd op)
      {
        return applyBinOp(lexpr,rexpr,op,regAint,regBint);
      }

      static bdd applyUnOpBool(bdd expr, bdd op)
      {
        return applyUnOp(expr,op,regAbool);
      }

      bdd And(bdd lexpr, bdd rexpr)
      {
        return applyBinOpBool(lexpr, rexpr, bddAnd);      
      }

      bdd Or(bdd lexpr, bdd rexpr)
      {
        return applyBinOpBool(lexpr, rexpr, bddOr);      
      }

      bdd Not(bdd expr)
      {
        return applyUnOpBool(expr, bddNot);      
      }

      bdd Plus(bdd lexpr, bdd rexpr)
      {
        return applyBinOpInt(lexpr, rexpr, bddPlus);
      }

      bdd Minus(bdd lexpr, bdd rexpr)
      {
        return applyBinOpInt(lexpr, rexpr, bddMinus);
      }

      bdd Times(bdd lexpr, bdd rexpr)
      {
        return applyBinOpInt(lexpr, rexpr, bddTimes);
      }

      bdd Div(bdd lexpr, bdd rexpr)
      {
        return applyBinOpInt(lexpr, rexpr, bddDiv);
      }

      bdd Assign(std::string var, bdd expr)
      {
        BddInfo_t bi;
        Voc voc = BinRel::getVoc();
        if(voc.find(var) == voc.end()){
          LOG(WARNING) << "[BinRelManager::Assign] Unknown Variable";
          return bddfalse;
        }else{
          bi = BinRel::getVoc().find(var)->second;
        }

        //redundant?
        bddPair *regARhs2Extra = bdd_newpair();
        if(bi->maxVal == BOOLSIZE)
          fdd_setpair(
              regARhs2Extra,
              regAbool->baseRhs,
              regAbool->baseExtra
              );
        else
          fdd_setpair(
              regARhs2Extra,
              regAint->baseRhs,
              regAint->baseExtra
              );
        expr = bdd_replace(expr,regARhs2Extra);
        bdd_freepair(regARhs2Extra);
        //up to here.

        bddPair *regA2var = bdd_newpair();
        if(bi->maxVal == BOOLSIZE)
          fdd_setpair(
              regA2var,
              regAbool->baseExtra,
              bi->baseRhs
              );
        else
          fdd_setpair(
              regA2var,
              regAint->baseExtra,
              bi->baseRhs
              );
        expr = bdd_replace(expr, regA2var);
        bdd_freepair(regA2var);

        bdd c = bddtrue;
        for(
            Voc::const_iterator iter = voc.begin();
            iter != voc.end();
            ++iter)
        {
          if(var != iter->first)
            c = c & biimp(iter->second->maxVal, (iter->second)->baseLhs,
                (iter->second)->baseRhs);
        }
        return expr & c;
      }

      bdd Assume(bdd expr1, bdd expr2)
      {
        const Voc voc = BinRel::getVoc();

        bddPair *regARhs2Extra = bdd_newpair();
        if(regAbool != NULL)
          fdd_setpair(
              regARhs2Extra,
              regAbool->baseRhs,
              regAbool->baseExtra
              );
        if(regAint != NULL)
          fdd_setpair(
              regARhs2Extra,
              regAint->baseRhs,
              regAint->baseExtra
              );
        expr1 = bdd_replace(expr1, regARhs2Extra);
        bdd_freepair(regARhs2Extra);

        bddPair *regARhs2BExtra = bdd_newpair();
        if(regAbool != NULL)
          fdd_setpair(
              regARhs2BExtra,
              regAbool->baseRhs,
              regBbool->baseExtra
              );
        if(regAint != NULL)
          fdd_setpair(
              regARhs2BExtra,
              regAint->baseRhs,
              regBint->baseExtra
              );
        expr2 = bdd_replace(expr2, regARhs2BExtra);
        bdd_freepair(regARhs2BExtra);

        bddPair *baseLhs2Rhs = bdd_newpair();
        for(Voc::const_iterator iter = voc.begin(); iter != voc.end(); ++iter){
          fdd_setpair(
              baseLhs2Rhs,
              (iter->second)->baseLhs,
              (iter->second)->baseRhs
              );
        }
        expr2 = bdd_replace(expr2, baseLhs2Rhs);
        bdd_freepair(baseLhs2Rhs);

        bdd equate = bddtrue;
        for(Voc::const_iterator iter = voc.begin(); iter != voc.end(); ++iter){
          equate = equate &
            biimp(
                iter->second->maxVal,
                iter->second->baseLhs,
                iter->second->baseRhs
                );
        }
        if(regAbool != NULL)
          equate = equate &
            biimp(regAbool->maxVal,
                regAbool->baseExtra,
                regBbool->baseExtra
                );
        if(regAint != NULL)
          equate = equate &
            biimp(regAint->maxVal,
                regAint->baseExtra,
                regBint->baseExtra
                );

        bdd ret = expr1 & expr2 & equate;

        if(regAbool != NULL){
          ret = bdd_exist(ret, fdd_ithset(regAbool->baseExtra));
          ret = bdd_exist(ret, fdd_ithset(regBbool->baseExtra));
        }
        if(regAint != NULL){
          ret = bdd_exist(ret, fdd_ithset(regAint->baseExtra));
          ret = bdd_exist(ret, fdd_ithset(regBint->baseExtra));
        }

        return ret;
      }
    }
  }
}

