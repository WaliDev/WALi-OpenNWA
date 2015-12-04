#ifndef WALI_DOMAINS_DETAILS_SEM_ELEM_WRAPPER_HPP
#define WALI_DOMAINS_DETAILS_SEM_ELEM_WRAPPER_HPP

#include <algorithm>
#include <limits>

#include "wali/Common.hpp"
#include "wali/SemElem.hpp"

namespace wali {
  namespace domains {
    namespace details {

      ///////////////////////////////////////
      struct Expr : Countable {
        virtual ~Expr() {}
        virtual sem_elem_t eval(sem_elem_t example) const = 0;

        virtual bool is_concrete_one() const { return false; }
        virtual bool is_concrete_zero() const { return false; }

        virtual std::string to_string() const = 0;

        virtual bool same_tree(ref_ptr<Expr> that) const = 0;
      };

      struct ExprZero : Expr {
        virtual sem_elem_t eval(sem_elem_t example) const 
        {
          return example->zero();
        }

        virtual bool is_concrete_zero() const { return true; }

        virtual bool same_tree(ref_ptr<Expr> that) const
        {
          return that->is_concrete_zero();
        }

        virtual std::string to_string() const { return "0"; }
      };

      struct ExprOne : Expr {
        virtual sem_elem_t eval(sem_elem_t example) const 
        {
          return example->one();
        }

        virtual bool is_concrete_one() const { return true; }

        virtual bool same_tree(ref_ptr<Expr> that) const
        {
          return that->is_concrete_one();
        }

        virtual std::string to_string() const { return "1"; }
      };

      struct ExprCombine : Expr {
        ref_ptr<Expr> m_left, m_right;
        ExprCombine(ref_ptr<Expr> l, ref_ptr<Expr> r)
          : m_left(l), m_right(r)
        {
          fast_assert(!l->is_concrete_zero());
          fast_assert(!r->is_concrete_zero());
        }

        virtual sem_elem_t eval(sem_elem_t example) const 
        {
          return m_left->eval(example)->combine(m_right->eval(example));
        }

        virtual std::string to_string() const
        {
          return "(" + m_left->to_string() + " + " + m_right->to_string() + ")";
        }

        virtual bool same_tree(ref_ptr<Expr> that) const
        {
          ExprCombine* that_down = dynamic_cast<ExprCombine*>(that.get_ptr());
          return
            that_down != NULL
            && m_left->same_tree(that_down->m_left)
            && m_right->same_tree(that_down->m_right);
        }
      };

      struct ExprExtend : Expr {
        ref_ptr<Expr> m_left, m_right;
        ExprExtend(ref_ptr<Expr> l, ref_ptr<Expr> r)
          : m_left(l), m_right(r)
        {
          fast_assert(!l->is_concrete_zero());
          fast_assert(!r->is_concrete_zero());
          fast_assert(!l->is_concrete_one());
          fast_assert(!r->is_concrete_one());
        }

        virtual sem_elem_t eval(sem_elem_t example) const 
        {
          return m_left->eval(example)->extend(m_right->eval(example));
        }

        virtual std::string to_string() const
        {
          return "(" + m_left->to_string() + " * " + m_right->to_string() + ")";
        }

        virtual bool same_tree(ref_ptr<Expr> that) const
        {
          ExprExtend* that_down = dynamic_cast<ExprExtend*>(that.get_ptr());
          return
            that_down != NULL
            && m_left->same_tree(that_down->m_left)
            && m_right->same_tree(that_down->m_right);
        }
      };


      ///////////////////////////////////////

      class SemElemWrapper
      {
      public:
        // This is not really necessary here, but it will unify
        // the interface with MinPlusInt, in case that ever seems
        // beneficial...
        typedef sem_elem_t value_type;

      private:
        // We need to be able to make 0 and 1 elements without knowing
        // what kind of semiring we want. So we need special indications.
        //
        // But, we can also take an unknown zero and combine with an
        // unknown one for example, so we really need a way to be able
        // to express expressions of 0s and 1s.
        //
        // IMPORTANT NOTE: It is possible that 0 can be represented by
        // both a ZERO kind as well as a NORMAL kind with a 0 m_value,
        // and similar for 1.
        //
        // I think this is "necessary." If we replaced a concrete 0
        // with the unknown 0, then if the user created a wrapper with
        // concrete 0 and asked for the value, we wouldn't know what
        // to give it.
        ref_ptr<Expr> m_expr;
        value_type m_value;

        void assert_sane() const {
          fast_assert(m_expr.is_valid() || m_value.is_valid());
          fast_assert(!m_expr.is_valid() || !m_value.is_valid());
        }

        explicit SemElemWrapper (ref_ptr<Expr> expr)
          : m_expr(expr)
          , m_value()
        {
          fast_assert(expr.is_valid());
        }


      public:
        SemElemWrapper (sem_elem_t v)
          : m_expr()
          , m_value(v)
        {
          fast_assert(v.is_valid());
        }

        // This is needed by boost::matrix:
        //    template<class T, class ALLOC>
        //    const typename identity_matrix<T, ALLOC>::value_type
        //        identity_matrix<T, ALLOC>::one_ (1);
        //    // ISSUE: need 'one'-traits here
        //
        // The zero element for identity_matrix and zero_matrix
        // are constructed by default parameter, so this should be
        // the only call to this function.
        //
        // Once the boost thing is resolved, this should become
        // private. (Or really I should figure out how to make it
        // a friend.)
        explicit SemElemWrapper(int t)
          : m_expr(t == 0
                   ? static_cast<Expr*>(new ExprZero())
                   : static_cast<Expr*>(new ExprOne()))
          , m_value()
        {
          fast_assert(t == 0 || t == 1);
          assert_sane();
        }

        SemElemWrapper()
          : m_expr(new ExprZero())
          , m_value()
        {}

        SemElemWrapper(SemElemWrapper const & other)
          : m_expr(other.m_expr)
          , m_value(other.m_value)
        {
          assert_sane();
        }

        SemElemWrapper &
        operator= (SemElemWrapper const & other) {
          other.assert_sane();
          m_expr = other.m_expr;
          m_value = other.m_value;
          return *this;
        }

        SemElemWrapper &
        operator= (value_type v) {
          assert_sane();
          fast_assert(v.is_valid());
          m_expr = NULL;
          m_value = v;
          return *this;
        }

        bool
        operator== (SemElemWrapper const & that) const {
          this->assert_sane();
          that.assert_sane();

          // Both are known values
          if (this->m_value.is_valid() && that.m_value.is_valid()) {
            return this->m_value->equal(that.m_value);
          }

          // Exactly one is a known value
          if (this->m_value.is_valid()) {
            sem_elem_t that_val = that.m_expr->eval(this->m_value);
            return this->m_value->equal(that_val);
          }
          if (that.m_value.is_valid()) {
            sem_elem_t this_val = this->m_expr->eval(that.m_value);
            return this_val->equal(that.m_value);
          }

          // Both are unknown values

          // Are they definitely the same?
          if (this->m_expr->is_concrete_one() && that.m_expr->is_concrete_one()) {
            return true;
          }
          if (this->m_expr->is_concrete_zero() && that.m_expr->is_concrete_zero()) {
            return true;
          }

          // Are they definitely different?
          if (this->m_expr->is_concrete_one() && that.m_expr->is_concrete_zero()) {
            return false;
          }
          if (this->m_expr->is_concrete_zero() && that.m_expr->is_concrete_one()) {
            return false;
          }

          // A single value is "definitely" different than a tree
          if (this->m_expr->is_concrete_zero()
              || this->m_expr->is_concrete_one()
              || that.m_expr->is_concrete_zero()
              || that.m_expr->is_concrete_one())
          {
            return false;
          }

          fast_assert(false);
          return false;
        }

        bool
        operator!= (SemElemWrapper const & that) const {
          return !(*this == that);
        }

        value_type
        get_value() const {
          assert_sane();
          fast_assert(m_value.is_valid());
          return m_value;
        }

        std::ostream&
        print(std::ostream & os) const {
          assert_sane();
          if (m_value.is_valid()) {
            return m_value->print(os);
          }
          else {
            return os << "[unknown: " << m_expr->to_string() << "]";
          }
        }

        SemElemWrapper
        operator+ (SemElemWrapper const & that) const {
          this->assert_sane();
          that.assert_sane();
          
          sem_elem_t this_val, that_val;

          // Figure out the SemElem from this
          if (this->m_value.is_valid()) {
            this_val = this->m_value;
          }
          else if (that.m_value.is_valid()) {
            this_val = this->m_expr->eval(that.m_value);
          }

          // Figure out the SemElem from that
          if (that.m_value.is_valid()) {
            that_val = that.m_value;
          }
          else if (this->m_value.is_valid()) {
            that_val = that.m_expr->eval(this->m_value);
          }

          // If we have known values, then we can return
          if (this_val.is_valid() && that_val.is_valid()) {
            return SemElemWrapper(this_val->combine(that_val));
          }

          // Otherwise we build a new expression:
          if (this->m_expr->is_concrete_zero()) {
            return SemElemWrapper(that.m_expr);
          }
          else if (that.m_expr->is_concrete_zero()) {
            return SemElemWrapper(this->m_expr);
          }
          else if (this->m_expr->same_tree(that.m_expr)) {
            return SemElemWrapper(this->m_expr);
          }

          else {
            // This case will sometimes be wrong...
            return SemElemWrapper(
              new ExprCombine(
                this->m_expr,
                that.m_expr));
          }
        }

        void
        operator+= (SemElemWrapper const & that) {
          SemElemWrapper sum = *this + that;
          *this = sum;
        }

        SemElemWrapper
        operator* (SemElemWrapper const & that) const {
          this->assert_sane();
          that.assert_sane();
          
          sem_elem_t this_val, that_val;

          // Figure out the SemElem from this
          if (this->m_value.is_valid()) {
            this_val = this->m_value;
          }
          else if (that.m_value.is_valid()) {
            this_val = this->m_expr->eval(that.m_value);
          }

          // Figure out the SemElem from that
          if (that.m_value.is_valid()) {
            that_val = that.m_value;
          }
          else if (this->m_value.is_valid()) {
            that_val = that.m_expr->eval(this->m_value);
          }

          // If we have known values, then we can return
          if (this_val.is_valid() && that_val.is_valid()) {
            return SemElemWrapper(this_val->extend(that_val));
          }

          // Otherwise we build a new expression:

          // 0*v or v*0 => 0
          if (this->m_expr->is_concrete_zero()) {
            return SemElemWrapper(this->m_expr);
          }
          else if (that.m_expr->is_concrete_zero()) {
            return SemElemWrapper(that.m_expr);
          }
          // 1*v or v*1 => v
          else if (this->m_expr->is_concrete_one()) {
            return SemElemWrapper(that.m_expr);
          }
          else if (that.m_expr->is_concrete_one()) {
            return SemElemWrapper(this->m_expr);
          }
          else {
            return SemElemWrapper(
              new ExprExtend(
                this->m_expr,
                that.m_expr));
          }
        }

        void
        operator*= (SemElemWrapper const & that) {
          SemElemWrapper product = *this * that;
          *this = product;
        }

      };

      inline
      std::ostream&
      operator<< (std::ostream& os, SemElemWrapper const & that)
      {
        return that.print(os);
      }

    }
  }
}

#endif /* WALI_DOMAINS_DETAILS_SEM_ELEM_WRAPPER_HPP */

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:
