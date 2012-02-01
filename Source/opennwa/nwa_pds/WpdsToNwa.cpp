#include "opennwa/Nwa.hpp"
#include "opennwa/nwa_pds/conversions.hpp"

using wali::wpds::WPDS;
using wali::wpds::Rule;
using wali::wpds::WpdsRules;

namespace opennwa
{
  namespace nwa_pds
  {
    NwaRefPtr WpdsToNwa( const WPDS & pds )
    {
      NwaRefPtr nwa(new Nwa());
      WpdsToNwa(*nwa, pds);
      return nwa;
    }

      
    void WpdsToNwa( Nwa & nwa, const WPDS & pds )
    {
      //TODO: check this!

      nwa.clear();

      std::map<State,State> call_return;

      WpdsRules rules = WpdsRules(); //FIXME
      pds.for_each(rules);

      //Step Rules:
      // For each <p,n_1> -w-> <p',n_2> in delta_1,
      // add ((p,n_1), n_1, (p',n_2)) to delta_i
      for( std::set<Rule>::iterator it = rules.stepRules.begin();
           it != rules.stepRules.end(); it++ )
      {
        nwa.addInternalTrans(getKey(it->from_state(),it->from_stack()), //from
                             it->from_stack(),                          //sym
                             getKey(it->to_state(),it->to_stack1()));   //to
      }

      //Call Rules:
      // For each <p,n_c> -w-> <p',e r_c> in delta_2,
      // add ((p,n_c), n_c, (p',e)) to delta_c, and
      // add (p,n_c) => r_c to the call-return map 
      for( std::set<Rule>::iterator it = rules.pushRules.begin();
           it != rules.pushRules.end(); it++ )
      {
        nwa.addCallTrans(getKey(it->from_state(),it->from_stack()),   //from
                         it->from_stack(),                           //sym
                         getKey(it->to_state(),it->to_stack1()));    //to

        call_return.insert(std::pair<State,State>(getKey(it->from_state(),it->from_stack()),  //call
                                                  it->to_stack2()));                          //ret
      }


      // Return Rules:
      // For each <p,x> -w-> <p',*> in delta_0 and
      //          (p'',n_c) => r_c in the call-return map,
      // add ((p,x),(p'',n_c),x,(p',r)) to delta_r 
      for( std::set<Rule>::iterator it = rules.popRules.begin();
           it != rules.popRules.end(); it++ )
      {
        for( std::map<State,State>::iterator cr_it = call_return.begin();
             cr_it != call_return.end(); cr_it++ )
        {
          nwa.addReturnTrans(getKey(it->from_state(),it->from_stack()),   //from
                             cr_it->first,                               //pred
                             it->from_stack(),                           //sym
                             getKey(it->to_state(),cr_it->second));      //to
        }
      }
    }

      

  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

