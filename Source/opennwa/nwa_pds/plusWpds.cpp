#include <sstream>
#include <cstring>

#include "opennwa/Nwa.hpp"
#include "opennwa/nwa_pds/plusWpds.hpp"

using wali::wpds::WPDS;
using wali::wpds::Rule;
using wali::wpds::WpdsRules;

namespace opennwa
{
  namespace nwa_pds
  {

    WPDS
    plusWpds( Nwa const & nwa, const WPDS & base )
    {
      typedef ::opennwa::details::TransitionStorage Trans;
      
      //Q: do implicit transitions to the stuck state affect this result?
      //A: it changes the states that are reachable
      Nwa copy = nwa;

      State stuck = getKey("stuck");
      while (copy.isState(stuck)) {
        stuck = getKey(stuck, getKey("'"));
      }
      
      copy.realizeImplicitTrans(stuck);

      WPDS result;

      WpdsRules rules = WpdsRules(); //FIXME

      base.for_each(rules);

      //Step Rules:
      for(  std::set<Rule>::iterator it = rules.stepRules.begin();
            it != rules.stepRules.end(); it++ )
      {
        //<p,n_1> -w-> <p',n_2> in delta_1, (q,n_1,q') in delta_i
        Key sym = it->from_stack();
        //look for internal transitions in the NWA with symbol 'sym'
        for( Nwa::InternalIterator iit = copy.beginInternalTrans();
             iit != copy.endInternalTrans(); iit++ )
        {
          if( sym == Trans::getInternalSym(*iit) )
          {
            //<(p,q),n_1> -w-> <(p',q'),n_2>
            Key from = getKey(it->from_state(),Trans::getSource(*iit));
            Key to = getKey(it->to_state(),Trans::getTarget(*iit));
            result.add_rule(from,sym,to,it->to_stack1(),it->weight());
          }
        }
      }

      //Call Rules:
      for(  std::set<Rule>::iterator it = rules.pushRules.begin();
            it != rules.pushRules.end(); it++ )
      {
        //<p,n_c> -w-> <p',e r_c> in delta_3, (q_c,n_c,q) in delta_c
        Key sym = it->from_stack();
        //look for call transitions in the NWA with symbol 'sym'
        for( Nwa::CallIterator cit = copy.beginCallTrans();
             cit != copy.endCallTrans(); cit++ )
        {
          if( sym == Trans::getCallSym(*cit) )
          {
            //<(p,q_c),n_c> -w-> <(p',q),e (r_c,q_c)>
            Key from = getKey(it->from_state(),Trans::getCallSite(*cit));
            Key to = getKey(it->to_state(),Trans::getEntry(*cit));
            Key stack2 = getKey(it->to_stack2(),Trans::getCallSite(*cit));
            result.add_rule(from,sym,to,it->to_stack1(),stack2,it->weight());
          }
        }
      }

      //Return Rules:
      for( std::set<Rule>::iterator it = rules.popRules.begin();
           it != rules.popRules.end(); it++ )
      {
        //<p,x> -w-> <p',*> in delta_0, (q_r,q_c,x,q) in delta_r
        Key sym = it->from_stack();
        //look for return transitions in the NWA with symbol 'sym'
        for( Nwa::ReturnIterator rit = copy.beginReturnTrans();
             rit != copy.endReturnTrans(); rit++ )
        {
          if( sym == Trans::getReturnSym(*rit) )
          {
            //<(p,q_r),x> -w-> <(p',q_r^x),*>
            Key from = getKey(it->from_state(),Trans::getExit(*rit));
            Key to = getKey(it->to_state(),getKey(Trans::getExit(*rit),sym));
            result.add_rule(from,sym,to,it->weight());
            //For all r_c in delta_2
            for( std::set<Rule>::iterator dit = rules.pushRules.begin();
                 dit != rules.pushRules.end(); dit++ )
            {
              //<(p',q_r^x),(r_c,q_c)> -1-> <(p',q),r_c>
              from = getKey(it->to_state(),getKey(Trans::getExit(*rit),sym));
              sym = getKey(dit->to_stack2(),Trans::getCallSite(*rit));
              to = getKey(it->to_state(),Trans::getReturnSite(*rit));
              result.add_rule(from,sym,to,dit->to_stack2(),it->weight()->one());
            }
          }
        }
      }
      return result;
    }


  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

