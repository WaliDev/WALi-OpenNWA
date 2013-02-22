#include "Xfa.hpp"

#include <wali/SemElemPair.hpp>
#include <wali/domains/SemElemSet.hpp>
#include <wali/domains/KeyedSemElemSet.hpp>
#include <wali/wfa/State.hpp>

#include <buddy/fdd.h>

using wali::wfa::CreateKeyedSet;
using namespace wali::domains;

namespace {

    bool include_all_vocabularies(wali::domains::binrel::VectorVocabulary const & UNUSED_PARAMETER(v))
    {
        return true;
    }

    bool
    is_any_transition(wali::wfa::ITrans const * trans)
    {
        return true;
    }

    
}

namespace wali {
    namespace xfa {

        using namespace wali::domains::binrel;

        namespace details {
            sem_elem_t
            IntersectionWeightMaker::make_weight( sem_elem_t lhs, sem_elem_t rhs )
            {
                return lhs->extend(rhs);
                BinaryRelation left = dynamic_cast<BinRel*>(lhs.get_ptr());
                BinaryRelation right = dynamic_cast<BinRel*>(rhs.get_ptr());

                assert(left.get_ptr());
                assert(right.get_ptr());

                bdd left_bdd = left->getBdd();
                bdd right_bdd = right->getBdd();
                
                BddContext const & voc = left->getVocabulary();
                assert(&voc == &(right->getVocabulary()));

                BinaryRelation result = new BinRel(&voc, left_bdd & right_bdd);

                std::cerr << "== IntersectionWeightMaker:\n"
                          << "==        " << lhs->toString() << "\n"
                          << "==    and " << rhs->toString() << "\n"
                          << "==    ->  " << result->toString() << "\n";

                return result;
            }
        }
        

        std::set<State>
        Xfa::getFinalStates() const
        {
            std::set<State> result;
            std::set<wali::Key> const & finals = getFinalStateKeys();
            for (std::set<wali::Key>::const_iterator final_key = finals.begin();
                 final_key != finals.end(); ++final_key)
            {
                result.insert(State(*final_key));
            }
            return result;
        }



        Xfa
        from_internal_only_nwa(opennwa::Nwa const & nwa,
                               BinaryRelation rel)
        {
            assert(rel.get_ptr());

            using opennwa::Nwa;
            using wali::domains::binrel::BinRel;
            Xfa xfa;

            BinaryRelation one = dynamic_cast<BinRel*>(rel->one().get_ptr());
            BinaryRelation zero = dynamic_cast<BinRel*>(rel->zero().get_ptr());

            assert(one.get_ptr());
            assert(zero.get_ptr());

            for (Nwa::StateIterator state = nwa.beginStates();
                 state != nwa.endStates() ; ++state)
            {
                xfa.addState(State(*state), zero);
            }

            assert(nwa.sizeInitialStates() == 1);
            xfa.setInitialState(State(*nwa.beginInitialStates()));

            for (Nwa::StateIterator state = nwa.beginFinalStates();
                 state != nwa.endFinalStates() ; ++state)
            {
                xfa.addFinalState(State(*state));
            }

            for (Nwa::InternalIterator trans = nwa.beginInternalTrans();
                 trans != nwa.endInternalTrans() ; ++trans)
            {
                xfa.addTrans(State(trans->first),
                             Symbol(trans->second),
                             State(trans->third),
                             one);
            }

            assert(nwa.sizeCallTrans() == 0);
            assert(nwa.sizeReturnTrans() == 0);

            return xfa;
        }

        
        void
        HoverWeightPrinter::print_extra_attributes(const wali::wfa::ITrans* t,
                                                   std::ostream & os)
        {
            wali::sem_elem_t w_se = t->weight();
            BinaryRelation w = dynamic_cast<wali::domains::binrel::BinRel*>(w_se.get_ptr());

            if (w == NULL) {
                SemElemPair * p = dynamic_cast<SemElemPair*>(w_se.get_ptr());
                assert(p != NULL);
                sem_elem_t r = p->get_first()->extend(p->get_second());
                w = dynamic_cast<wali::domains::binrel::BinRel*>(r.get_ptr());
            }
            
            assert(w != NULL);

            std::stringstream command;

            printImagemagickInstructions(w->getBdd(),
                                         w->getVocabulary(),
                                         command,
                                         "png:-",
                                         include_all_vocabularies);

            FILE* image_data_stream = popen(command.str().c_str(), "r");

            if (image_data_stream == NULL) {
                std::cerr << "Error opening pipe to " << command << ": ";
                perror(NULL);
            }

            std::vector<char> image_data = util::read_file(image_data_stream);
            pclose(image_data_stream);
                    
            std::vector<unsigned char> image_data_u(image_data.begin(), image_data.end());

            std::string image_data_base64 = util::base64_encode(&image_data_u[0], image_data_u.size());

            os << ",onhover=\"<img src='data:image/png;base64," << image_data_base64 << "'>\"";
        }


        bool
        transformer_accepts(sem_elem_t weight, sem_elem_t left_accept, sem_elem_t right_accept)
        {
            SemElemPair
                * weight_down = dynamic_cast<SemElemPair*>(weight.get_ptr());

            sem_elem_t
                weight_exists = weight_down->get_first()->extend(left_accept),
                weight_forall = weight_down->get_second()->extend(right_accept);

            // The exist half just needs to be non-zero. This means that
            // there is some path to any data configuration. The forall half
            // must be zero: this means that there is NO path to any data
            // configuration.
            //
            // This assumes that the weight on the final state is 1;
            // otherwise we would have to do an intersection or something.

            bool
                exists_is_zero = weight_exists->equal(weight_exists->zero()),
                forall_is_zero = weight_forall->equal(weight_forall->zero());

            return !exists_is_zero && forall_is_zero;
        }


        bdd
        to_bdd_variable_set(std::vector<std::string> const & vars,
                            BddContext const & context)
        {
            bdd result = bddtrue;
            for (size_t i=0; i<vars.size(); ++i) {
                assert(context.find(vars[i]) != context.end());
                bddinfo_t info = context.find(vars[i])->second;
                result = result
                    & fdd_ithset(info->baseLhs)
                    & fdd_ithset(info->baseRhs)
                    & fdd_ithset(info->baseExtra)
                    & fdd_ithset(info->tensor1Lhs)
                    & fdd_ithset(info->tensor1Rhs)
                    & fdd_ithset(info->tensor1Extra)
                    & fdd_ithset(info->tensor2Lhs)
                    & fdd_ithset(info->tensor2Rhs)
                    & fdd_ithset(info->tensor2Extra);
            }
            return result;
        }


        std::pair<sem_elem_t, sem_elem_t>
        asymmetric_pair_keep_minimal(sem_elem_t left, sem_elem_t right)
        {
            SemElemPair
                * left_down = dynamic_cast<SemElemPair*>(left.get_ptr()),
                * right_down = dynamic_cast<SemElemPair*>(right.get_ptr());

            sem_elem_t
                left_exists = left_down->get_first(),
                left_forall = left_down->get_second(),
                right_exists = right_down->get_first(),
                right_forall = right_down->get_second(),
                null;

            // left <= right if
            //  * left exists >= right exists
            //  * left forall <= right forall
            if (right_exists->underApproximates(left_exists)
                && left_forall->underApproximates(right_forall))
            {
                // Keeping minimal, so we keep left.
                return std::pair<sem_elem_t, sem_elem_t>(left, null);
            }

            // right <= left if
            //  * right exists >= left exists
            //  * right forall <= left forall
            if (left_exists->underApproximates(right_exists)
                && right_forall->underApproximates(left_forall))
            {
                // Keeping minimal, so we keep right.
                return std::pair<sem_elem_t, sem_elem_t>(right, null);
            }

            // Neither subsumes the other, so return both
            return std::make_pair(left, right);
        }


        class XfaContainWeightMaker
            : public wali::wfa::WeightMaker
        {
        public:
            virtual sem_elem_t make_weight( sem_elem_t lhs, sem_elem_t rhs ) {
                // left and right are both KeyedSemElemSets. I want to make a
                // new weight which is a normal semelemset, where the
                // elements do the pairing. (So this is like a version of
                // keepboth, but lifted to operate on sets instead.)
                
                // The pairs are are just pairs of (guard,weight) like I was
                // thinking about using originally but decided not to for
                // performance reasons. We'll see if those reasons are
                // important.
                KeyedSemElemSet
                    * lhs_down = dynamic_cast<KeyedSemElemSet*>(lhs.get_ptr()),
                    * rhs_down = dynamic_cast<KeyedSemElemSet*>(rhs.get_ptr());

                sem_elem_t pair;

                SemElemSet::ElementSet es;
                for (KeyedSemElemSet::const_iterator left_element = lhs_down->begin();
                     left_element != lhs_down->end(); ++left_element)
                {
                    for (KeyedSemElemSet::const_iterator right_element = rhs_down->begin();
                         right_element != rhs_down->end(); ++right_element)
                    {
                        pair = new SemElemPair(new SemElemPair(left_element->first, left_element->second),
                                               new SemElemPair(right_element->first, right_element->second));
                        es.insert(pair);
                    }
                }

                if (es.empty()) {
                    return sem_elem_t();
                }
                return new SemElemSet(SemElemSet::KeepAllNonduplicates,
                                      pair,
                                      es);
            }
        };



        bool
        language_contains(Xfa const & left, Xfa const & right,
                          wali::domains::binrel::ProgramBddContext const & voc)
        {
            typedef domains::SemElemSet::ElementSet ElementSet;

            std::set<Key>
                left_alpha = left.wfa().alphabet(),
                right_alpha = right.wfa().alphabet(),
                both_alpha;
            
            both_alpha.insert(left_alpha.begin(), left_alpha.end());
            both_alpha.insert(right_alpha.begin(), right_alpha.end());

            CreateKeyedSet r_det_weight_gen;
            wali::wfa::WFA right_det = right.semideterminize(r_det_weight_gen);
            right_det.complete(both_alpha);
            right_det.complementStates();
            
            CreateKeyedSet l_det_weight_gen;
            wali::wfa::WFA left_det = left.semideterminize(l_det_weight_gen);
            left_det.complete(both_alpha);

            //XfaContainWeightMaker maker;
            //wali::wfa::WFA intersected = left_det.intersect(maker, right_det);
            wali::wfa::WFA intersected = left_det.intersect(right_det);

            wali::wfa::WFA::AccessibleStateSetMap reached_states =
                intersected.computeAllReachingWeights(domains::SemElemSet::KeepAllNonduplicates);
                //intersected.computeAllReachingWeights(asymmetric_pair_keep_minimal);

            std::set<Key> const
                & left_states = left_det.getStates(),
                & right_states = right_det.getStates();
            
            for(std::set<Key>::const_iterator left_state = left_states.begin();
                left_state != left_states.end(); ++left_state)
            {
                for(std::set<Key>::const_iterator right_state = right_states.begin();
                    right_state != right_states.end(); ++right_state)
                {
                    Key pairkey = getKey(*left_state, *right_state);
                    ElementSet const & reached_weights = reached_states[pairkey];

                    if (reached_weights.size() == 0u) {
                        // This state doesn't even exist, so of course it's fine
                        continue;
                    }

                    sem_elem_t
                        left_accept = left_det.getState(*left_state)->acceptWeight(),
                        right_accept = right_det.getState(*right_state)->acceptWeight();

                    // There must be a reaching weight which is accepting --
                    // that is, has a zero forall and non-zero exists weight.
                    for (ElementSet::const_iterator final_weight = reached_weights.begin();
                         final_weight != reached_weights.end(); ++final_weight)
                    {
                        if (transformer_accepts(*final_weight, left_accept, right_accept)) {
                            // there is a word present in L-R
                            return false;
                        }
                    }

                }
            }

            return true;
        }

    }
}


        
// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 4
//     indent-tabs-mode: nil
// End:
