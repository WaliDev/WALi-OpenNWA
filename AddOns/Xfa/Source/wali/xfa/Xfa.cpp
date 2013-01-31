#include "Xfa.hpp"

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
        
    }
}


        
