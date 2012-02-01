#ifndef wali_nwa_NESTED_WORD_HPP
#define wali_nwa_NESTED_WORD_HPP

///
/// @author Evan Driscoll
///

#include "wali/Countable.hpp"
#include "opennwa/NwaFwd.hpp"
#include <vector>
#include <algorithm>

namespace opennwa
{
  /// This class represents a single nested word.
  ///
  /// The word can be unbalanced left or unbalanced right, even
  /// though our NWAs do not have support for unbalanced-right
  /// words.
  ///
  /// The representation used by this class is closer to that of a
  /// word in a visibly-pushdown language. It holds the linear
  /// contents of a word, but does not store the nesting relation
  /// explicitly. Instead, each position in the word is annotated
  /// with whether it is an internal, call, or return position. The
  /// nesting relation is induced by the matchings between calls and
  /// returns.
  class NestedWord : public wali::Countable
  {
  public:
    /// Each position in the nested word has a symbol and a type.
    ///
    struct Position
    {
      /// The type of a position: either a call position, internal
      /// position, or return position.
      enum Type {
        CallType, InternalType, ReturnType
      };
        
      /// The symbol at this position
      ///
      Symbol symbol;

      /// The type (call/internal/return) of this position
      ///
      Type type;
        
      /// Constructs a Position object with the given symbol and
      /// type
      Position(Symbol sym, Type ty) : symbol(sym), type(ty) {}

      /// Returns whether two positions are equal
      ///
      bool operator==(Position const & p) const
      {
        return symbol == p.symbol && type == p.type;
      }
    };
      
  private:
    std::vector<Position> word;
      
  public:
    /// An iterator to allow traversal through the word. (Don't
    /// depend on this concrete type specifically, but I guarantee
    /// it will be a random access iterator.)
    typedef std::vector<Position>::const_iterator const_iterator;
      
    /// Appends the position 'p' (symbol & type) to the end of this
    /// nested word
    void append(Position p)
    {
      word.push_back(p);
    }


    /// Appends the given symbol to the end of this nested word in a
    /// call position.
    void appendCall(Symbol sym)     { append(Position(sym, Position::CallType)); }
    /// Appends the given symbol to the end of this nested word in an
    /// internal position.
    void appendInternal(Symbol sym) { append(Position(sym, Position::InternalType)); }
    /// Appends the given symbol to the end of this nested word in a
    /// return position.
    void appendReturn(Symbol sym)   { append(Position(sym, Position::ReturnType)); }


    /// Returns an iterator to the first position in this nested
    /// word.
    const_iterator begin() const
    {
      return word.begin();
    }
      
    /// Returns an iterator to one position past the end of this
    /// nested word.
    const_iterator end() const
    {
      return word.end();
    }

    ///
    /// Returns the number of positions in this nested word
    size_t size() const
    {
      return word.size();
    }

    ///
    /// Returns whether the two nested words are equal
    bool operator==(NestedWord const & other) const
    {
      if (word.size() != other.word.size()) {
        return false;
      }
      else {
        return std::equal(word.begin(), word.end(), other.word.begin());
      }
    }

    ///
    /// Returns whether two nested words are unequal
    bool operator!=(NestedWord const & other) const
    {
      return !(*this == other);
    }
      
  };


  typedef ref_ptr<NestedWord> NestedWordRefPtr;
}


// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
