#include "opennwa/Nwa.hpp"
#include "opennwa/NestedWord.hpp"
#include "opennwa/construct/intersect.hpp"
#include "opennwa/construct/complement.hpp"

#include "opennwa/query/language.hpp"

namespace opennwa {
  namespace query {

    bool
    languageContains(Nwa const & nwa, NestedWord const & word)
    {
      return nwa.isMemberNondet(word);
    }


    bool
    languageSubsetEq(Nwa const & first, Nwa const & second)
    {
      Nwa second_copy = second;
        
      // We have to synchronize alphabets first
      for (Nwa::SymbolIterator sym = first.beginSymbols();
           sym != first.endSymbols(); ++sym)
      {
        second_copy.addSymbol(*sym);
      }
        
      //Check L(a1) contained in L(a2) by checking 
      //if L(a1) intersect (complement L(a2)) is empty.
      NwaRefPtr comp = construct::complement(second_copy);
      NwaRefPtr inter = construct::intersect(first, *comp);
      //inter.intersect(first, comp); //L(a1) intersect (complement L(a2))

      return languageIsEmpty(*inter);
    }

      
    bool
    languageIsEmpty(Nwa const & nwa)
    {
      return nwa._private_isEmpty_();
    }


    bool
    languageEquals(Nwa const & first, Nwa const & second)
    {
      //The languages accepted by two NWAs are equivalent if they are both contained
      //in each other, ie L(a1) contained in L(a2) and L(a2) contained in L(a1).
      bool first_in_second = query::languageSubsetEq(first, second);
      bool second_in_first = query::languageSubsetEq(second, first);

      return (first_in_second && second_in_first );
    }
      
  }
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:
