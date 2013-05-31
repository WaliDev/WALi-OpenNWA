#include "Xfa.hpp"

#include <wali/SemElemPair.hpp>
#include <wali/domains/SemElemSet.hpp>
#include <wali/wfa/State.hpp>

#include <buddy/fdd.h>

namespace {

    bool include_all_vocabularies(wali::domains::binrel::VectorVocabulary const & UNUSED_PARAMETER(v))
    {
        return true;
    }

    bool
    is_any_transition(wali::wfa::ITrans const * UNUSED_PARAMETER(trans))
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
                BinRel * left = dynamic_cast<BinRel*>(lhs.get_ptr());
                BinRel * right = dynamic_cast<BinRel*>(rhs.get_ptr());

                assert(left);
                assert(right);

                bdd left_bdd = left->getBdd();
                bdd right_bdd = right->getBdd();
                
                BddContext const & voc = left->getVocabulary();
                assert(&voc == &(right->getVocabulary()));

                BinRel * result = new BinRel(&voc, left_bdd & right_bdd);

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
                               sem_elem_t rel)
        {
            assert(rel.get_ptr());

            using opennwa::Nwa;
            using wali::domains::binrel::BinRel;
            Xfa xfa;

            BinRel * one = dynamic_cast<BinRel*>(rel->one().get_ptr());
            BinRel * zero = dynamic_cast<BinRel*>(rel->zero().get_ptr());

            assert(one);
            assert(zero);

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
            BinRel * w = dynamic_cast<wali::domains::binrel::BinRel*>(w_se.get_ptr());

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

        
        sem_elem_t
        IntroduceStateToRelationWeightGen::liftAcceptWeight(wali::wfa::WFA const & UNUSED_PARAMETER(original_wfa),
                                                            Key state,
                                                            sem_elem_t original_accept_weight) const
        {
            //std::cout << "Lifting weight.\n";
            using wali::domains::binrel::BddContext;
            using wali::domains::binrel::BinRel;
            using wali::domains::binrel::bddinfo_t;

            BinRel * orig_rel
                = dynamic_cast<BinRel*>(original_accept_weight.get_ptr());

            // Now pull out the BDD and vocabulary
            bdd orig_bdd = orig_rel->getBdd();
            BddContext const & orig_voc = orig_rel->getVocabulary();

            // First step: rename variables to new domain
            std::vector<int> orig_names, new_names;

            for (BddContext::const_iterator var=orig_voc.begin(); var!=orig_voc.end(); ++var) {
                bddinfo_t
                    orig_info = var->second,
                    new_info = util::map_at(new_voc, var->first);

                assert(new_info.get_ptr());

                //std::cout << "Will rename: " << orig_info->baseLhs << "->" << new_info->baseLhs << "\n";
                //std::cout << "Will rename: " << orig_info->baseRhs << "->" << new_info->baseRhs << "\n";

                orig_names.push_back(orig_info->baseLhs);
                orig_names.push_back(orig_info->baseRhs);
                new_names.push_back(new_info->baseLhs);
                new_names.push_back(new_info->baseRhs);

            }

            bddPair* rename_pairs = bdd_newpair();
            assert(orig_names.size() == new_names.size());
            fdd_setpairs(rename_pairs,
                         &orig_names[0],
                         &new_names[0],
                         static_cast<int>(orig_names.size()));

            bdd renamed_bdd = bdd_replace(orig_bdd, rename_pairs);

            bdd_freepair(rename_pairs);

            // Second step: create a BDD that enforces that the current state
            // is "state"
            SequentialFromZeroState sfz_state = from_state(State(state));

            bdd state_change = new_voc.Assume(new_voc.From(current_state), new_voc.Const(sfz_state.index));

            // Third step: combine them together
            return new BinRel(&new_voc, renamed_bdd & state_change);
        }


        sem_elem_t
        IntroduceStateToRelationWeightGen::getOne(wali::wfa::WFA const & original_wfa) const
        {
            return original_wfa.getSomeWeight()->one();
        }


        sem_elem_t
        IntroduceStateToRelationWeightGen::liftWeight(wali::wfa::WFA const & UNUSED_PARAMETER(original_wfa),
                                                      wali::Key source,
                                                      wali::Key UNUSED_PARAMETER(symbol),
                                                      wali::Key target,
                                                      sem_elem_t orig_weight) const
        {
            //std::cout << "Lifting weight.\n";
            using wali::domains::binrel::BddContext;
            using wali::domains::binrel::BinRel;
            using wali::domains::binrel::bddinfo_t;

            // Zeroth step: havoc the current state
            sem_elem_t orig_rel_then_havoc =
                orig_weight->extend(havoc_current_state);

            // not really the *original* relation, but close enough
            BinRel * orig_rel
                = dynamic_cast<BinRel*>(orig_rel_then_havoc.get_ptr());

            // Now pull out the BDD and vocabulary
            bdd orig_bdd = orig_rel->getBdd();
            BddContext const & orig_voc = orig_rel->getVocabulary();

            // First step: rename variables to new domain
            std::vector<int> orig_names, new_names;

            for (BddContext::const_iterator var=orig_voc.begin(); var!=orig_voc.end(); ++var) {
                bddinfo_t
                    orig_info = var->second,
                    new_info = util::map_at(new_voc, var->first);

                assert(new_info.get_ptr());

                //std::cout << "Will rename: " << orig_info->baseLhs << "->" << new_info->baseLhs << "\n";
                //std::cout << "Will rename: " << orig_info->baseRhs << "->" << new_info->baseRhs << "\n";

                orig_names.push_back(orig_info->baseLhs);
                orig_names.push_back(orig_info->baseRhs);
                new_names.push_back(new_info->baseLhs);
                new_names.push_back(new_info->baseRhs);

            }

            bddPair* rename_pairs = bdd_newpair();
            assert(orig_names.size() == new_names.size());
            fdd_setpairs(rename_pairs,
                         &orig_names[0],
                         &new_names[0],
                         static_cast<int>(orig_names.size()));

            bdd renamed_bdd = bdd_replace(orig_bdd, rename_pairs);

            bdd_freepair(rename_pairs);

            // Second step: create a BDD that enforces that the state
            // changes in the right way.
            int source_fdd = util::map_at(new_voc, current_state)->baseLhs;
            int dest_fdd = util::map_at(new_voc, current_state)->baseRhs;

            SequentialFromZeroState
                sfz_source = from_state(State(source)),
                sfz_dest = from_state(State(target));

            //std::cout << "Creating state change BDD, setting\n"
            //          << "    FDD " << source_fdd << " to " << source << " (really " << sfz_source.index << ")\n"
            //          << "    FDD " << dest_fdd << " to " << dest << " (really " << sfz_dest.index << ")\n";

            bdd state_change = fdd_ithvar(source_fdd, sfz_source.index) & fdd_ithvar(dest_fdd, sfz_dest.index);

            //std::cout << "Created. Returning answer.\n";

            // Third step: combine them together
            return new BinRel(&new_voc, renamed_bdd & state_change);
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

            IntroduceStateToRelationWeightGen r_det_weight_gen(voc, right, "right_");
            wali::wfa::WFA right_det = right.semideterminize(r_det_weight_gen);
            right_det.complete(both_alpha);
            right_det.complementStates();
            
            IntroduceStateToRelationWeightGen l_det_weight_gen(voc, left, "left_");
            wali::wfa::WFA left_det = left.semideterminize(l_det_weight_gen);
            left_det.complete(both_alpha);

            wali::wfa::WFA intersected = left_det.intersect(right_det);

            wali::wfa::WFA::AccessibleStateSetMap reached_states =
                intersected.computeAllReachingWeights(domains::SemElemSet::KeepAllNonduplicates, false);
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
