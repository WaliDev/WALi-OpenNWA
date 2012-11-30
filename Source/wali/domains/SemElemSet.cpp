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


  bool
  new_less_than_existing(sem_elem_t new_, sem_elem_t existing)
  {
    return new_->underApproximates(existing);
  }

  bool
  existing_less_than_new(sem_elem_t new_, sem_elem_t existing)
  {
    return existing->underApproximates(new_);
  }

  typedef bool(*SemElemCompare)(sem_elem_t, sem_elem_t);

  void
  remove_subsumed_elements(SemElemSet::ElementSet & set,
                           sem_elem_t element,
                           SemElemCompare should_remove_existing)
  {
    SemElemSet::ElementSet::const_iterator iter = set.begin();
    while (iter != set.end()) {
      if (should_remove_existing(element, *iter)) {
        // set.erase(iter++) for std::map
        iter = set.erase(iter);
      }
      else {
        ++iter;
      }
    }
  }
  

  void
  add_element_if_appropriate(SemElemSet::KeepWhat keep_what,
                             SemElemSet::ElementSet & set,
                             sem_elem_t add_this)
  {
    switch(keep_what) {
      case SemElemSet::KeepAllNonduplicates:
        push_if_not_present(set, add_this);
        break;

      case SemElemSet::KeepMaximalElements:
        // Should remove the existing element if new >= existing
        remove_subsumed_elements(set, add_this, existing_less_than_new);
        set.insert(add_this);
        break;

      case SemElemSet::KeepMinimalElements:
        // Opposite of the previous case
        remove_subsumed_elements(set, add_this, new_less_than_existing);
        set.insert(add_this);
        break;
    }
  }
}


namespace wali
{
  namespace domains
  {

    SemElemSet::SemElemSet(KeepWhat keep, sem_elem_t base_element)
      : base_one(base_element->one())
      , keep_what(keep)
      , the_hash(0)
    {}


    SemElemSet::SemElemSet(KeepWhat keep, sem_elem_t base_element, ElementSet const & es)
      : base_one(base_element->one())
      , keep_what(keep)
      , the_hash(0)
    {
      for (ElementSet::const_iterator element = es.begin();
           element != es.end(); ++element)
      {
        size_t pre_size = this->elements.size();
        add_element_if_appropriate(this->keep_what, this->elements, *element);
        if (pre_size != this->elements.size()) {
          // Added
          assert(this->elements.size() == pre_size + 1u);
          the_hash ^= (*element)->hash();
        }
      }
    }

    
    sem_elem_t
    SemElemSet::one() const
    {
      ElementSet es;
      //es.push_back(this->base_one);
      es.insert(this->base_one);
      return new SemElemSet(this->keep_what, this->base_one, es);
    }

    
    sem_elem_t
    SemElemSet::zero() const
    {
      return new SemElemSet(this->keep_what, this->base_one);
    }
    

    sem_elem_t
    SemElemSet::extend( SemElem * se )
    {
      SemElemSet const * other = dynamic_cast<SemElemSet const *>(se);
      assert(other);
      assert(this->keep_what == other->keep_what);
      assert(this->base_one->equal(other->base_one));

      Ptr result = new SemElemSet(this->keep_what, this->base_one);

      for (ElementSet::const_iterator this_element = this->elements.begin();
           this_element != this->elements.end(); ++this_element)
      {
        for (ElementSet::const_iterator other_element = other->elements.begin();
             other_element != other->elements.end(); ++other_element)
        {
          add_element_if_appropriate(this->keep_what,
                                     result->elements,
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
      assert(this->keep_what == other->keep_what);
      assert(this->base_one->equal(other->base_one));

      Ptr result = new SemElemSet(this->keep_what, this->base_one, this->elements);
      for (ElementSet::const_iterator other_element = other->elements.begin();
           other_element != other->elements.end(); ++other_element)
      {
        add_element_if_appropriate(this->keep_what, result->elements, *other_element);
      }
      return result;
    }
    

    bool
    SemElemSet::equal( SemElem * se ) const
    {
      SemElemSet const * other = dynamic_cast<SemElemSet const *>(se);
      assert(other);
      assert(this->keep_what == other->keep_what);
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


    size_t
    SemElemSet::hash() const
    {
      return the_hash;
    }

  }
}


// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:
