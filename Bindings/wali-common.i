/* Author: Mads Chr. Olesen <mads@mchro.dk> 
Wraps enough of WALi to be able to create subclassed SemElem's in C++,
and access them from Python.
*/

%include "typemaps.i"
%include "std_string.i"
%include "std_pair.i"
%define SWIG_EXPORT_ITERATOR_METHODS 1
%enddef
%include "std_set.i"
%include "std_map.i"
%include "std_vector.i"

%{
/* Util stuff */
#include "../Source/wali/ref_ptr.hpp"
#include "../Source/wali/Countable.hpp"
#include "../Source/wali/IMarkable.hpp"
#include "../Source/wali/Printable.hpp"
#include "../Source/wali/Key.hpp"
#include "../Source/wali/regex/Regex.hpp"

/* WFA Util stuff */
#include "../Source/wali/wfa/TransFunctor.hpp"
/* WFA */
#include "../Source/wali/wfa/WFA.hpp"

/* WPDS util stuff */
#include "../Source/wali/wpds/Rule.hpp"
#include "../Source/wali/wpds/GenKeySource.hpp"

/* WPDS's */
#include "../Source/wali/wpds/WPDS.hpp"
#include "../Source/wali/wpds/ewpds/EWPDS.hpp"
#include "../Source/wali/wpds/fwpds/FWPDS.hpp"

/* Example Weight Domains */
#include "../Source/wali/Reach.cpp"

#include <sstream>
#include <utility>

using namespace wali;
using namespace wali::wpds;
using namespace wali::wfa;
typedef wali::wfa::ITrans::WorklistData WorklistData; // (!)
%}

%feature("ref")   SemElem "$this->count++;"
%feature("unref") SemElem "$this->count--;"

/* Ignore ostream users */
%ignore wali::SemElem::print;
%ignore wali::SemElem::marshall;
%ignore wali::Printable::print;

/* Guards */
%ignore wali_wpds_WPDS_GUARD;
%ignore wali_KEY_GUARD;
%ignore wali_REF_PTR_GUARD;
%ignore wali_SEM_ELEM_GUARD;
%ignore wali_HASH_MAP_GUARD;
%ignore wali_HM_HASH_GUARD;
%ignore wali_KEY_SOURCE_GUARD;
%ignore wali_PRINTABLE_GUARD;
%ignore wali_wfa_TRANSSET_GUARD;
%ignore wali_wfa_TRANS_GUARD;
%ignore wali_wfa_WFA_GUARD;
%ignore wali_wpds_GENKEY_SOURCE_GUARD;
%ignore wali_wpds_ewpds_EWPDS_GUARD;
%ignore wali_wpds_fwpds_FWPDS_GUARD;
%ignore REACH_GUARD;

%ignore HASHMAP_GROWTH_FRACTION;
%ignore HASHMAP_SHRINK_FRACTION;
%ignore IMPL_LIST;


/* Let's just grab the original header file here */
%include "../Source/wali/ref_ptr.hpp"
%include "../Source/wali/Printable.hpp"
%rename(__str__) wali::Printable::to_string;
%ignore wali::Printable::toString;

%rename("%(undercase)s", %$isfunction) "";
%rename("%(undercase)s", %$isvariable) "";

%include "../Source/wali/Key.hpp"

/* Helper function for getting epsilon char */
wali::Key getEpsilonKey();
%{
wali::Key getEpsilonKey() {
    return wali::getKey("*");
}
%}

%include "../Source/wali/KeySource.hpp"

%include "../Source/wali/wpds/GenKeySource.hpp"

%include "../Source/wali/SemElem.hpp"

%template(SemElemPtr) wali::ref_ptr<SemElem>;

%extend wali::SemElem {
    PyObject* __str__() {
        std::ostringstream outs;
        $self->print(outs);

        return PyString_FromString(outs.str().c_str());
    }
}

%pythonprepend wali::SemElem::combine(SemElem* se) %{
        #Deref smart pointer if needed
        if hasattr(args[1], '__deref__'):
            tmp = list(args)
            tmp[1] = tmp[1].__deref__()
            args = tuple(tmp)
%}

%pythonprepend wali::SemElem::extend(SemElem* se) %{
        #Deref smart pointer if needed
        if hasattr(args[1], '__deref__'):
            tmp = list(args)
            tmp[1] = tmp[1].__deref__()
            args = tuple(tmp)
%}

%template(KeyPair) std::pair< Key,Key >;

%include "../Source/wali/hm_hash.hpp"
%include "../Source/wali/HashMap.hpp"
%template(KeyPairMap) wali::HashMap< KeyPair, TransSet, hm_hash< KeyPair >, hm_equal< KeyPair > >;
