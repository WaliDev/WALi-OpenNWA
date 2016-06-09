        // Attach weights to the transitions that represent calling context
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
				EXTERN_TYPES::sem_elem_wrapperRefPtr w = evalRegExpFinNonRec(regExpMap[transReg], aList);
				t->stop();
				//std::cout << "OutWeight: " << tSrc << "," << tTgt << "," << tStack << ":";
				//w.v->print(std::cout) << endl;
				tt->setWeight(w.v);
			}
		}
		//Perform path summary
              t->start();
		outfa.path_summary_tarjan_fwpds(true, true);
              t->stop();
// MISSING: how to associate a tensored regular expression with the context value computed
// via path_summary_tarjan_fwpds

=====================


        // Attach weights to the transitions that represent calling context
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
                                 cout << "Initial-state transition filtered" << endl;
                                 continue;
                             }
				t->start();
				EXTERN_TYPES::sem_elem_wrapperRefPtr w = evalRegExpFinNonRec(regExpMap[transReg], aList);
				t->stop();
				//std::cout << "OutWeight: " << tSrc << "," << tTgt << "," << tStack << ":";
				//w.v->print(std::cout) << endl;
				tt->setWeight(w.v);
			}
		}
		//Perform path summary
              t->start();
		outfa.path_summary_tarjan_fwpds(true, true);
              t->stop();

        // Attach weights to the nodes that do not represent calling context

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
                             
                             if (tSrc != outfa.getInitialState()) {
                                 cout << "Calling-context transition filtered" << endl;
                                 continue;
                             }

                             duetrel_t contextWeight = ((DuetRel*)(outfaNewton.getState(tTgt)->weight().get_ptr()));  // Weight of calling context

                             // MISSING: Perform the evaluation using EvalLC, with contextWeight as the calling context
				t->start();
				EXTERN_TYPES::sem_elem_wrapperRefPtr w = evalRegExpFinNonRec(regExpMap[transReg], aList);
				t->stop();
				//std::cout << "OutWeight: " << tSrc << "," << tTgt << "," << tStack << ":";
				//w.v->print(std::cout) << endl;
				tt->setWeight(w.v);
			}
		}



