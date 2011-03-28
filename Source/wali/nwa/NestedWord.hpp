#ifndef wali_nwa_NESTED_WORD_HPP
#define wali_nwa_NESTED_WORD_HPP

namespace wali
{
  namespace nwa
  {
    class NestedWord
    {
    public:
      // Each position in the nested word has a symbol and a type.
      // (Think of this more of a visibly-pushdown word.)
      struct Position {
        enum Type {
          CallType, InternalType, ReturnType
        };
        Symbol symbol;
        Type type;
        
        Position(Symbol sym, Type ty) : symbol(sym), type(ty) {}
      };
      
    private:
      std::vector<Position> word;
      
    public:
      typedef  std::vector<Position>::const_iterator const_iterator;
      
      void append(Position p) {
        word.push_back(p);
      }
      
      void appendCall(Symbol sym)     { append(Position(sym, Position::CallType)); }
      void appendInternal(Symbol sym) { append(Position(sym, Position::InternalType)); }
      void appendReturn(Symbol sym)   { append(Position(sym, Position::ReturnType)); }
      
      const_iterator begin() const {
        return word.begin();
      }
      
      const_iterator end() const {
        return word.end();
      }
    };
  }
}

#endif
