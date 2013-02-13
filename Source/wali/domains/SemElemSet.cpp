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
  push_if_not_present(SemElemSet::ElementSet & set, size_t & the_hash, sem_elem_t add_this)
  {
    if (!is_present(set, add_this)) {
      the_hash ^= add_this->hash();
      set.insert(add_this);
      //set.push_back(add_this);
    }
  }

  std::pair<sem_elem_t, sem_elem_t>
  dummy_keep_nonduplicates(sem_elem_t new_, sem_elem_t existing)
  {
    std::abort();
  }

  std::pair<sem_elem_t, sem_elem_t>
  keep_smaller(sem_elem_t new_, sem_elem_t existing)
  {
    if (new_->underApproximates(existing)) {
      // new_ < existing; keeping smaller; keeping new_
      return std::pair<sem_elem_t, sem_elem_t>(new_, NULL);
    }
    else if (existing->underApproximates(new_)) {
      // existing < new; keeping smaller; keeping existing
      return std::pair<sem_elem_t, sem_elem_t>(existing, NULL);
    }
    else {
      // no subsumption; keep both
      return std::pair<sem_elem_t, sem_elem_t>(new_, existing);
    }
  }

  std::pair<sem_elem_t, sem_elem_t>
  keep_larger(sem_elem_t new_, sem_elem_t existing)
  {
    if (existing->underApproximates(new_)) {
      // existing < new_; keeping larger; keeping new_
      return std::pair<sem_elem_t, sem_elem_t>(new_, NULL);
    }
    else if (new_->underApproximates(existing)) {
      // new_ < existing; keeping larger; keeping existing
      return std::pair<sem_elem_t, sem_elem_t>(existing, NULL);
    }
    else {
      // no subsumption; keep both
      return std::pair<sem_elem_t, sem_elem_t>(new_, existing);
    }
  }


  sem_elem_t
  remove_subsumed_elements(SemElemSet::ElementSet & set,
                           size_t & the_hash,
                           sem_elem_t element,
                           SemElemSet::SemElemSubsumptionComputer keep_what)
  {
    SemElemSet::ElementSet::const_iterator iter = set.begin();
    while (iter != set.end()) {
      std::pair<sem_elem_t, sem_elem_t> keep_these = keep_what(element, *iter);
      if (keep_these.second == NULL) {
        // set.erase(iter++) for std::map
        the_hash ^= (*iter)->hash();
        element = keep_these.first;
        iter = set.erase(iter);
      }
      else {
        // These two assertions are to check that 'keep_what' returns either
        // a single element which means that the existing one should be
        // removed and the returned single element is carried forward and
        // eventually inserted (this is the true case of this if statement),
        // or (this case) it returns both the new and existing elements
        // unchanged.
        assert(keep_these.first == element || keep_these.second == element);
        assert(keep_these.first == *iter || keep_these.second == *iter);
        ++iter;
      }
    }
    return element;
  }


  void
  add_element_if_not_subsumed(SemElemSet::ElementSet & set,
                              size_t & the_hash,
                              sem_elem_t element,
                              SemElemSet::SemElemSubsumptionComputer keep_what)
  {
    for(SemElemSet::ElementSet::const_iterator iter = set.begin();
        iter != set.end(); ++iter)
    {
      std::pair<sem_elem_t, sem_elem_t> keep_these = keep_what(element, *iter);
      if (keep_these.second == NULL) {
        // The existing element subsumes the new one. However, the existing
        // element may need to incorporate information from the new element
        // into it. The following is like *iter = keep_these.first, except
        // that we can do it. :-)
        if (*iter != keep_these.first) {
          // 'tis OK to invalidate iter because we're just returning anyway :-)
          the_hash ^= (*iter)->hash();
          the_hash ^= keep_these.first->hash();
          set.erase(iter);
          set.insert(keep_these.first);
        }
        return;
      }
      // These two assertions are to check that 'keep_what' returns either a
      // single element which means that the existing one should be removed
      // and the returned single element is carried forward and eventually
      // inserted (this is the true case of this if statement), or (this
      // case) it returns both the new and existing elements unchanged.
      assert(keep_these.first == element || keep_these.second == element);
      assert(keep_these.first == *iter || keep_these.second == *iter);
    }
    set.insert(element);
  }
                              
    
  void
  add_element_if_appropriate(SemElemSet::SemElemSubsumptionComputer keep_what,
                             SemElemSet::ElementSet & set,
                             size_t & the_hash,
                             sem_elem_t add_this)
  {
    if (keep_what == dummy_keep_nonduplicates) {
      push_if_not_present(set, the_hash, add_this);
    }
    else {
      // Should remove an element if new >= existing
      add_this = remove_subsumed_elements(set, the_hash, add_this, keep_what);
      add_element_if_not_subsumed(set, the_hash, add_this, keep_what);
    }
  }
}


namespace wali
{
  namespace domains
  {
    SemElemSet::SemElemSubsumptionComputer const SemElemSet::KeepAllNonduplicates(dummy_keep_nonduplicates);
    SemElemSet::SemElemSubsumptionComputer const SemElemSet::KeepMaximalElements(keep_larger);
    SemElemSet::SemElemSubsumptionComputer const SemElemSet::KeepMinimalElements(keep_smaller);

    

    SemElemSet::SemElemSet(SemElemSubsumptionComputer keep, sem_elem_t base_element)
      : base_one(base_element->one())
      , keep_what(keep)
      , the_hash(0)
    {}


    SemElemSet::SemElemSet(SemElemSubsumptionComputer keep, sem_elem_t base_element, ElementSet const & es)
      : base_one(base_element->one())
      , keep_what(keep)
      , the_hash(0)
    {
      for (ElementSet::const_iterator element = es.begin();
           element != es.end(); ++element)
      {
        size_t pre_size = this->elements.size();
        add_element_if_appropriate(this->keep_what, this->elements, this->the_hash, *element);
        if (pre_size != this->elements.size()) {
          // Added
          assert(this->elements.size() == pre_size + 1u);
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
      //assert(this->keep_what == other->keep_what);
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
                                     result->the_hash,
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
      //assert(this->keep_what == other->keep_what);
      assert(this->base_one->equal(other->base_one));

      Ptr result = new SemElemSet(this->keep_what, this->base_one, this->elements);
      for (ElementSet::const_iterator other_element = other->elements.begin();
           other_element != other->elements.end(); ++other_element)
      {
        add_element_if_appropriate(this->keep_what, result->elements, result->the_hash, *other_element);
      }

      return result;
    }
    

    bool
    SemElemSet::equal( SemElem * se ) const
    {
      SemElemSet const * other = dynamic_cast<SemElemSet const *>(se);
      assert(other);
      //assert(this->keep_what == other->keep_what);
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
        first = false;
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
