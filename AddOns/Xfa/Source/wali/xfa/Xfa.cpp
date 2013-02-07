#include "Xfa.hpp"

#include <buddy/fdd.h>

namespace {

    bool include_all_vocabularies(wali::domains::binrel::VectorVocabulary const & UNUSED_PARAMETER(v))
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

        
        sem_elem_t
        IntroduceStateToRelationWeightGen::liftAcceptWeight(wali::wfa::WFA const & UNUSED_PARAMETER(original_wfa),
                                                            Key state,
                                                            sem_elem_t original_accept_weight) const
        {
            //std::cout << "Lifting weight.\n";
            using wali::domains::binrel::BddContext;
            using wali::domains::binrel::BinRel;
            using wali::domains::binrel::bddinfo_t;

            BinaryRelation orig_rel
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
            BinaryRelation orig_rel
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
        transformer_accepts(bdd exists_relation,
                            bdd accept_exists_prime,
                            bdd forall_relation,
                            bdd accept_forall_prime)
        {
            // forall (l, l', r, r') in R.  ~accept_L(l')     [uncomplemented]
            // and exists (l, l', r, r') in R. accept_R(r')
            bdd
                forall_check = bdd_imp(forall_relation, accept_forall_prime),
                exists_check = exists_relation & accept_exists_prime;

            return (forall_check == bddtrue) && (exists_check != bddfalse);
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


        std::pair<binrel_t, binrel_t>
	getNonsubsumedElements(BinRel * left_rel,
                               BinRel * right_rel,
                               bdd left,
                               bdd right,
                               BddContext const * context,
			       std::vector<std::string> const & left_vars_to_ignore,
                               std::vector<std::string> const & right_vars_to_ignore)
	{
	    bdd
                left_ignore = to_bdd_variable_set(left_vars_to_ignore, *context),
                right_ignore = to_bdd_variable_set(right_vars_to_ignore, *context),
		left_keep = bdd_exist(left, left_ignore),
		right_keep = bdd_exist(right, right_ignore);

	    if (wali::domains::binrel::details::bddImplies(left_keep, right_keep)
		|| wali::domains::binrel::details::bddImplies(right_keep, left_keep))
	    {
		std::vector<bdd> ans;
		ans.push_back(left | right);
		return std::make_pair(new BinRel(context, left | right),
                                      static_cast<BinRel*>(NULL));
	    }
	    else
	    {
		return std::make_pair(left_rel, right_rel);
	    }
        }

        
        struct
        XfaContainSetSubsume
        {
            std::vector<std::string> const & left_vocab_;
            std::vector<std::string> const & right_vocab_;

            XfaContainSetSubsume(std::vector<std::string> const & left_vocab,
                                 std::vector<std::string> const & right_vocab)
                : left_vocab_(left_vocab)
                , right_vocab_(right_vocab)
            {}
            
            
            std::pair<sem_elem_t, sem_elem_t>
            operator() (sem_elem_t left, sem_elem_t right) const
            {
                BinRel
                    * left_down = dynamic_cast<BinRel*>(left.get_ptr()),
                    * right_down = dynamic_cast<BinRel*>(right.get_ptr());

                bdd
                    left_bdd = left_down->getBdd(),
                    right_bdd = right_down->getBdd();

                BddContext const * context = &(left_down->getVocabulary());
                assert(context = &(right_down->getVocabulary()));
                
                std::vector<std::string> a, b;

                return getNonsubsumedElements(left_down, right_down,
                                              left_bdd, right_bdd,
                                              context,
                                              right_vocab_, left_vocab_); // switched because this is what is going to be IGNORED
            }
        };


        bool
        language_contains(Xfa const & left, std::vector<std::string> const & left_vocab,
                          Xfa const & right, std::vector<std::string> const & right_vocab)
        {
            XfaContainSetSubsume subsumption(left_vocab, right_vocab);

            Xfa const & intersected = left.intersect(right);

            wali::wfa::WFA::AccessibleStateSetMap reaching_weights =
                intersected.wfa().computeAllReachingWeights(SemElemSet::KeepMinimalElements);

            
            bdd_accepts(reaching
        }

    }
}


        
