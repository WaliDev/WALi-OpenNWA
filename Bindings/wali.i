%module(directors="1") wali
/* Author: Mads Chr. Olesen <mads@mchro.dk> 
Wraps all of WALi, and provide means for creating SemElem's in Python
*/

%include "wali-common.i"

/* ==================== PySemElem ====================== */
%feature("director") PySemElem;
%feature("nodirector") PySemElem::print;
%feature("nodirector") PySemElem::combine;
%feature("nodirector") PySemElem::extend;
%feature("nodirector") PySemElem::delta;
%feature("nodirector") PySemElem::equal;
%feature("nodirector") PySemElem::one;
%feature("nodirector") PySemElem::zero;
%feature("nodirector") PySemElem::star;
%feature("nodirector") PySemElem::diff;
%feature("nodirector") PySemElem::underApproximates;
%feature("nodirector") PySemElem::extendAndDiff;
%feature("nodirector") PySemElem::getuserdata;

/* crashes stuff */
%feature("nodirector") PySemElem::quasi_one;


/*%feature("director:except") {
    if ($error != NULL) {
        throw Swig::DirectorMethodException();
    }
}*/

/*%exception {
    try { $action }
    catch (Swig::DirectorException &e) { SWIG_fail; }
}*/

/* Additional niceties for PySemElem */
%pythonprepend PySemElem::pycombine(PySemElem* se) const %{
        #Deref smart pointer if needed
        if hasattr(args[1], '__deref__'):
            tmp = list(args)
            tmp[1] = tmp[1].__deref__()
            args = tuple(tmp)
%}

%pythonprepend PySemElem::pyextend(PySemElem* se) const %{
        #Deref smart pointer if needed
        if hasattr(args[1], '__deref__'):
            tmp = list(args)
            tmp[1] = tmp[1].__deref__()
            args = tuple(tmp)
%}



%feature("shadow") PySemElem::PySemElem() %{
def __init__(self, *args): 
    #standard SWIG initializer
    if self.__class__ == PySemElem:
        args = (None,) + args
    else:
        args = (self,) + args
    this = _wali.new_PySemElem(*args)
    try: self.this.append(this)
    except: self.this = this 

    #Default initializer for userdata
    userdata = {}
    self.setuserdata(userdata)

    #SWIG Director refcounting is broken:
    #http://www.nabble.com/Correct-Python-reference-counting-with-directors--td23046686.html
    #Make sure a Python reference to all objects exist, so they are not deleted
    try:
        PySemElem.all_pysemelems[self] = [self]
    except:
        PySemElem.all_pysemelems = {self: self}
def unref(self):
    del PySemElem.all_pysemelems[self]

#decorator for printing errors
@classmethod
def printexc(klass, fn):
    def new(*args):
        try:
            return fn(*args)
        except Exception, e:
            import traceback
            traceback.print_exc()
            raise e
    return new

#decorator for derefing if necessary
@classmethod
def deref_ifneeded(klass, fn):
    def new(*args):
        #Deref smart pointer if needed
        if hasattr(args[1], '__deref__'):
            tmp = list(args)
            tmp[1] = tmp[1].__deref__()
            args = tuple(tmp)
        return fn(*args)
    return new

#Wrap access to userdata in nice fashion
def __getattr__(self, name):
    try:
        return _swig_getattr(self, PySemElem, name)
    except AttributeError, e:
        try:
            return self.getuserdata()[name]
        except:
            raise e
def __setattr__(self, name, value):
    if name in ['this'] or name in self.__dict__:
        self.__dict__[name] = value
    else:
        self.getuserdata()[name] = value
def __delattr__(self, name):
    if name in self.__dict__:
        del self.__dict__[name]
    else:
        del self.getuserdata()[name]

def __eq__(self, other):
    #Deref smart pointer if needed
    if hasattr(other, '__deref__'):
        other = other.__deref__()

    return self.pyequal(other)
def __ne__(self, other):
    #Deref smart pointer if needed
    if hasattr(other, '__deref__'):
        other = other.__deref__()

    return not self.pyequal(other)
%}



%feature("shadow") PySemElem::pydelta(PySemElem * se) %{
def pydelta(self, se):
    "Python implementation of default delta, from SemElem.cpp"
    first = self.pycombine(se)
    if se.equal(first.__deref__()):
        second = self.pyzero()
    else:
        second = first
    toret = PySemElemPtrPair(first, second)
    return toret
%}

class PySemElem : public wali::SemElem {
    protected:
      PyObject* userdata;
    public:
      PySemElem() : wali::SemElem() {
        /*this->data = new std::map<std::string, PyObject*>();*/
        userdata = NULL;
      }
      virtual std::ostream& print( std::ostream & o ) const;

      /*std::map<std::string, PyObject*> data;*/
      void setuserdata(PyObject* data);
      PyObject* getuserdata();

      virtual std::string __str__ () const;

      virtual wali::ref_ptr<PySemElem> pycombine (PySemElem* se) const = 0;
      sem_elem_t combine( SemElem* se );

      virtual wali::ref_ptr<PySemElem> pyextend (PySemElem* se) const = 0;
      sem_elem_t extend( SemElem* se );

      virtual std::pair< wali::ref_ptr<PySemElem> , wali::ref_ptr<PySemElem> > pydelta( PySemElem * se ) = 0;
      std::pair< sem_elem_t , sem_elem_t > delta( SemElem * se );

      virtual bool pyequal( PySemElem * se ) const = 0;
      bool equal( SemElem * se ) const;

      virtual bool pyunderApproximates (PySemElem* se) const  = 0;
      bool underApproximates( SemElem * se );

      virtual wali::ref_ptr<PySemElem> pystar () = 0;
      sem_elem_t star();

      virtual wali::ref_ptr<PySemElem> pyextendAndDiff(
          PySemElem * next,
          PySemElem * subtrahend) = 0;
      sem_elem_t extendAndDiff(sem_elem_t next,
                               sem_elem_t subtrahend);

      virtual wali::ref_ptr<PySemElem> pydiff(PySemElem * se) = 0;
      sem_elem_t diff(SemElem * se);

      virtual wali::ref_ptr<PySemElem> pyone() const = 0;
      virtual wali::ref_ptr<PySemElem> pyzero() const = 0;
      sem_elem_t one() const;
      sem_elem_t zero() const;
};


%{
class PySemElem : public wali::SemElem {
    protected:
      PyObject* userdata;
    public:
      PySemElem() : wali::SemElem() {
        userdata = NULL;
      }
      virtual std::string __str__ () const {
        std::stringstream out;
        out << "<PySemElem at ";
        out << this;
        out << ": ";
        
        /* get representation of userdata*/
        std::string repr = "None";
        PyObject* py_repr = PyObject_Repr (userdata);
        if ( py_repr && PyString_Check(py_repr) ) {
          repr = PyString_AsString(py_repr);
        }
        Py_XDECREF( py_repr );
        out << repr;

        out << " >";
        return out.str();
      }
      virtual std::ostream& print( std::ostream & o ) const {
        return o << __str__();
      }

      void setuserdata(PyObject* data) {
        Py_XDECREF(userdata);
        Py_XINCREF(data);
        userdata = data;
      }
      PyObject* getuserdata() {
        /* Python will assume the object is its own (without ref'ing it):
        teach it that we still have a ref */
        Py_XINCREF(userdata);
        return userdata;
      }

      virtual wali::ref_ptr<PySemElem> pycombine (PySemElem* se) const = 0;
      virtual wali::ref_ptr<PySemElem> pyextend (PySemElem* se) const = 0;

      sem_elem_t combine( SemElem* se )
      {
        PySemElem* rhs = dynamic_cast< PySemElem* >(se);
        return pycombine(rhs);
      }

      sem_elem_t extend( SemElem* se )
      {
        PySemElem* rhs = dynamic_cast< PySemElem* >(se);
        return pyextend(rhs);
      }

      virtual std::pair< wali::ref_ptr<PySemElem> , wali::ref_ptr<PySemElem> > pydelta( PySemElem * se ) = 0;
      std::pair< sem_elem_t , sem_elem_t > delta( SemElem * se ) {
        PySemElem* rhs = dynamic_cast< PySemElem* >(se);
        return pydelta(rhs);
      }

      virtual bool pyequal( PySemElem * se ) const = 0;
      bool equal( SemElem * se ) const {
        PySemElem* rhs = dynamic_cast< PySemElem* >(se);
        bool toret = pyequal(rhs);
        return toret;
      }

      virtual bool pyunderApproximates (PySemElem* se) const  = 0;
      bool underApproximates( SemElem * se ) {
          PySemElem * rhs = dynamic_cast< PySemElem* >(se);
          bool toret = pyunderApproximates(rhs);
          return toret;
      }

      virtual wali::ref_ptr<PySemElem> pystar () = 0;
      sem_elem_t star() {
          return pystar();
      }

      virtual wali::ref_ptr<PySemElem> pyextendAndDiff(
          PySemElem * next,
          PySemElem * subtrahend) = 0;
      sem_elem_t extendAndDiff(sem_elem_t next,
                               sem_elem_t subtrahend)
      {
          PySemElem * n = dynamic_cast< PySemElem* >(next.get_ptr());
          PySemElem * s = dynamic_cast< PySemElem* >(subtrahend.get_ptr());
          return pyextendAndDiff(n, s);
      }

      virtual wali::ref_ptr<PySemElem> pydiff(PySemElem * se) = 0;
      sem_elem_t diff(SemElem * se) {
          PySemElem * rhs = dynamic_cast< PySemElem* >(se);
          return pydiff(rhs);
      }

      virtual wali::ref_ptr<PySemElem> pyone() const = 0;
      virtual wali::ref_ptr<PySemElem> pyzero() const = 0;
      sem_elem_t one() const {
        return pyone();
      }
      sem_elem_t zero() const {
        return pyzero();
      }
};
%}


%template(SemElemPtrPair) std::pair< sem_elem_t , sem_elem_t >;
%template(PySemElemPtr) wali::ref_ptr<PySemElem>;
%template(PySemElemPtrPair) std::pair< wali::ref_ptr<PySemElem>, wali::ref_ptr<PySemElem> >;



/* ==================== WFA ====================== */

/*%import "../Source/wali/wfa/TransFunctor.hpp"*/ /* This messes stuff up :-/ */

/* Normally defined in wali/Common.hpp by compiler type,
but swig is not cased for - therefore do it here */
#define ATTR_UNUSED
%import "../Source/wali/IMarkable.hpp"

%rename(fromState) wali::wfa::Trans::from;
%rename(toState) wali::wfa::Trans::to;
%nodefaultctor wali::TaggedWeight;
%include "../Source/wali/wfa/Trans.hpp"

%traits_swigtype(ITrans);
%fragment(SWIG_Traits_frag(ITrans));

%template(TransSetImpl) std::set< ITrans*,ITransLT >;
/*%template(TransSetIterator) std::set< ITrans*,ITransLT >::iterator;*/
%template(ConstTransPair) std::pair<ITrans* const, bool>;

%include "../Source/wali/wfa/TransSet.hpp"
%template(TransVector) std::vector<wali::wfa::Trans*>;
%extend wali::wfa::TransSet {
    std::vector< wali::wfa::Trans* > asList() {
        std::vector< Trans* > toRet;

        TransSet::iterator it = $self->begin();
        TransSet::iterator itend = $self->end();
        for( ; it != itend ; it++ ) {
          toRet.push_back( static_cast<wali::wfa::Trans*>(*it) );
        }

        return toRet;
    }
}

%include "../Source/wali/wfa/State.hpp"

%include "../Source/wali/wfa/WFA.hpp"
%extend wali::wfa::WFA {
    PyObject* __str__() {
        std::ostringstream outs;
        $self->print(outs);

        return PyString_FromString(outs.str().c_str());
    }

    PyObject* print_dot() {
        std::ostringstream outs;
        $self->print_dot(outs);

        return PyString_FromString(outs.str().c_str());
    }
}

%template(KeySet) std::set<size_t>;

/* ==================== WPDS ====================== */

%include "../Source/wali/wpds/WPDS.hpp"

%extend wali::wpds::WPDS {
    PyObject* __str__() {
        std::ostringstream outs;
        $self->print(outs);

        return PyString_FromString(outs.str().c_str());
    }
}

%include "../Source/wali/wpds/ewpds/EWPDS.hpp"
%include "../Source/wali/wpds/fwpds/FWPDS.hpp"

%extend wali::wpds::ewpds::EWPDS {
    PyObject* __str__() {
        std::ostringstream outs;
        $self->print(outs);

        return PyString_FromString(outs.str().c_str());
    }
}

/* ==================== Examples ====================== */
%include "../Source/wali/Reach.hpp"
