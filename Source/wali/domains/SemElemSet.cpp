#include "wali/domains/SemElemSet.hpp"

namespace
{
  using namespace wali::domains;
  using namespace wali;
  
  bool
  is_present(SemElemSet::ElementSet const & set, sem_elem_t add_this)
  {
    return (set.find(add_this) != set.end());
  }


  void
  push_if_not_present(SemElemSet::ElementSet & set, sem_elem_t add_this)
  {
    if (!is_present(set, add_this)) {
      set.insert(add_this);
      //set.push_back(add_this);
    }
  }
  
}


namespace wali
{
  namespace domains
  {

    SemElemSet::SemElemSet(sem_elem_t base_element)
      : base_one(base_element->one())
    {}


    SemElemSet::SemElemSet(sem_elem_t base_element, ElementSet const & es)
      : base_one(base_element->one())
    {
      for (ElementSet::const_iterator element = es.begin();
           element != es.end(); ++element)
      {
        push_if_not_present(this->elements, *element);
      }
    }

    
    sem_elem_t
    SemElemSet::one() const
    {
      ElementSet es;
      //es.push_back(this->base_one);
      es.insert(this->base_one);
      return new SemElemSet(this->base_one, es);
    }

    
    sem_elem_t
    SemElemSet::zero() const
    {
      return new SemElemSet(this->base_one);
    }
    

    sem_elem_t
    SemElemSet::extend( SemElem * se )
    {
      SemElemSet const * other = dynamic_cast<SemElemSet const *>(se);
      assert(other);
      assert(this->base_one->equal(other->base_one));

      Ptr result = new SemElemSet(this->base_one);

      for (ElementSet::const_iterator this_element = this->elements.begin();
           this_element != this->elements.end(); ++this_element)
      {
        for (ElementSet::const_iterator other_element = other->elements.begin();
             other_element != other->elements.end(); ++other_element)
        {
          push_if_not_present(result->elements,
                              (*this_element)->extend(*other_element));
        }
      }
      
      return result;
    }

    
    sem_elem_t
    SemElemSet::combine( SemElem * se )
    {
      SemElemSet const * other = dynamic_cast<SemElemSet const *>(se);
      assert(other);
      assert(this->base_one->equal(other->base_one));

      Ptr result = new SemElemSet(this->base_one, this->elements);
      for (ElementSet::const_iterator other_element = other->elements.begin();
           other_element != other->elements.end(); ++other_element)
      {
        push_if_not_present(result->elements, *other_element);
      }
      return result;
    }
    

    bool
    SemElemSet::equal( SemElem * se ) const
    {
      SemElemSet const * other = dynamic_cast<SemElemSet const *>(se);
      assert(other);
      assert(this->base_one->equal(other->base_one)); // or return false? or what?

      if (this->elements.size() != other->elements.size()) {
        return false;
      }

      for (ElementSet::const_iterator this_element = this->elements.begin();
           this_element != this->elements.end(); ++this_element)
      {
        if (!is_present(other->elements, *this_element)) {
          return false;
        }
      }

      return true;
    }

    
    std::ostream&
    SemElemSet::print( std::ostream & o ) const
    {
      o << "{ ";
      bool first = true;
      for (ElementSet::const_iterator element = this->elements.begin();
           element != this->elements.end(); ++element)
      {
        if (!first) {
          o << ", ";
        }
        (*element)->print(o);
      }
      o << "}";
      return o;
    }


    SemElemSet::ElementSet const &
    SemElemSet::getElements() const
    {
      return elements;
    }

  }
}


// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:
