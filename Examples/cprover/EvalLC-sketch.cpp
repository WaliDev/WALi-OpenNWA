void computeAndAttachContextWeights(
        WFA &outfa,
        tslRegExpMap &regExpMap,  //The map of all tsl regular expression
        map<pair<pair<int, int>, int>, int> &transMap,
        RTG::assignmentRefPtr aList
       )
{

        // Attach weights to the transitions that represent calling context
	std::set<Key> faStates = outfa.getStates();
	std::set<Key>::iterator stateIter;
	for (stateIter = faStates.begin(); stateIter != faStates.end(); stateIter++)
	{
		TransSet & transSet = outfa.getState(*stateIter)->getTransSet();
		TransSet::iterator transIt;
		for (transIt = transSet.begin(); transIt != transSet.end(); transIt++)
		{
			ITrans * tt = *transIt;
			int tSrc = tt->from();
			int tTgt = tt->to();
			int tStack = tt->stack();
			int transReg = transMap[std::make_pair(std::make_pair(tSrc, tTgt), tStack)];
                            
                            if (tSrc == outfa.getInitialState()) {
                                cout << "Transition filtered" << endl;
                                continue;
                            }
			t->start();
			EXTERN_TYPES::sem_elem_wrapperRefPtr w = Eval_LC(regExpMap[transReg], EXTERNS::getOneWt(), aList);
			t->stop();
			//std::cout << "OutWeight: " << tSrc << "," << tTgt << "," << tStack << ":";
			//w.v->print(std::cout) << endl;
			tt->setWeight(w.v);
		}
	}
	// Perform path summary
       t->start();
       // TO DO: Use Eval_LC(XXX, EXTERNS::getOneWt(), aList) inside path_summary_tarjan_fwpds
	outfa.path_summary_tarjan_fwpds(true, true);
       t->stop();

       return;
}
--------------------------------

              // Attach weights to the transitions that represent calling context
              computeAndAttachContextWeights(outfa, regExpMap, transMap, aList);

		// For each variable in the equation system, perform right-context
               // evaluation of its tensored regular expression
		for (assignIt = tensoredRegExpMap.begin(); assignIt != tensoredRegExpMap.end(); assignIt++)
		{
                      int var = assignIt->first;
                      int varContextState = mapBack[var].first.second;  // Target of epsilon transition that holds the summary function for var
                      duetrel_t contextWeight = ((DuetRel*)(outfa.getState(varContextState)->weight().get_ptr()));  // Weight of calling context
                      EvalT_RC(RT, EXTERNS::evalTensorTranspose(contextWeight, EXTERNS::getOneWt()), aList);
                }

-----------------------------------

EXTERN_TYPES::sem_elem_wrapperRefPtr EvalT_RC(RTG::regExpTRefPtr exp, EXTERN_TYPES::sem_elem_wrapperRefPtr, RTG::assignmentRefPtr);

=====================

Replacement for Step 7
        // Attach weights to the transitions that represent calling context
        computeAndAttachContextWeights(outfa, regExpMap, transMap, aList);

------------------------------------------
Replacement for assertion checking

    std::cout << "================================================" << std::endl;
    std::cout << "Assertion Checkings at Error Points" << std::endl << std::endl;
    
    // Check the assertion at each error point
    for (std::vector<caml_error_rule>::iterator it = errorRuleHolder.begin(); it != errorRuleHolder.end(); it++)
    {
        wali::wfa::TransSet error_transitions;
        error_transitions = outfaNewton.match(st1(), it->first);
        for(wali::wfa::TransSet::iterator tsit = error_transitions.begin(); tsit != error_transitions.end(); tsit++)
        {
            std::cout << "Checking assertion at vertex " << it->first
		      << ", line " << it->second.second << std::endl
		      << std::endl;

            // Check is_sat(contextWeight extend intraprocWeight extend negatedAssertionWeight)

            // Obtain the weight for the negated assertion condition
            duetrel_t negatedAssertionWeight = ((DuetRel*)(it->second.first.get_ptr()));
            negatedAssertionWeight->print(std::cout);
            std::cout << std::endl << std::endl;

            // Obtain the weight for the calling context
            duetrel_t contextWeight = ((DuetRel*)(outfaNewton.getState((*tsit)->to())->weight().get_ptr()));

            // Obtain the weight from the containing procedure's entry to the assertion pt
            int transReg = transMap[std::make_pair(std::make_pair(st1(), (*tsit)->to()), it->first)];
            t->start();
            EXTERN_TYPES::sem_elem_wrapperRefPtr w = Eval_LC(regExpMap[transReg], contextWeight, aList);
            t->stop();
            duetrel_t intraprocWeight = w.v;

            duetrel_t composedWeight = contextWeight->Compose(intraprocWeight).get_ptr();    // FIXME: Compose badly named: Compose should be Extend
            duetrel_t finalWeight = composedWeight->Compose(negatedAssertionWeight).get_ptr();    // FIXME: Compose badly named: Compose should be Extend

            bool isSat = finalWeight->IsSat();

			if (isSat) {
				std::cout << "Is SAT! (Assertion Failed)" << std::endl ;	
			
				// This print statement causes seg fault (not sure why) -Ashkan		
				//extval->print(std::cout);
				//std::cout << std::endl;
			}
			else {
				std::cout << "Is not SAT! (Assertion Passed)" << std::endl;
			}

            std::cout << std::endl << "contextWeight extend intraproceduralWeight = " << std::endl;
		    composedWeight->print(std::cout);
		    std::cout << std::endl << std::endl;
	
			if (testMode) {
				std::fstream testFile(testFileName.c_str(), std::fstream::out | std::fstream::app);
				testFile << "__ASSERTION " << (isSat ? "FAIL" : "PASS") << std::endl;
				testFile.close();
			}

			std::cout << "---------------------------------------------" << std::endl << std::endl;
        }
    }
 



