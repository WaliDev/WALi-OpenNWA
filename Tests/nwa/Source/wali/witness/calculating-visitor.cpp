#include "gtest/gtest.h"

#include "wali/Key.hpp"
#include "wali/Common.hpp"
#include "wali/witness/WitnessMerge.hpp"
#include "wali/witness/WitnessCombine.hpp"
#include "wali/witness/WitnessExtend.hpp"
#include "wali/witness/WitnessRule.hpp"
#include "wali/witness/WitnessTrans.hpp"
#include "wali/witness/CalculatingVisitor.hpp"

namespace wali {
    namespace witness {

        struct NumberComputer : public CalculatingVisitor<int>
        {
            //! Overload to calculate the value of an extend
            //! node. Modifications to the AnswerType& parameters will not
            //! persist.
            virtual int calculateExtend( WitnessExtend * w,
                                         int & leftValue,
                                         int & rightValue )
            {
                (void) w;
                return leftValue * rightValue;
            }

            //! Overload to calculate the value of an combine
            //! node. Modifications to the list parameter will not persist.
            virtual int calculateCombine( WitnessCombine * w,
                                          std::list<int> & childrenValues )
            {
                (void) w;
                int i = 0;
                for(std::list<int>::const_iterator iter = childrenValues.begin();
                    iter != childrenValues.end() ; ++iter)
                {
                    i += *iter;
                }
                return i;
            }
      
            //! Overload to calculate the value of an merge
            //! node. Modifications to the int& parameters will not
            //! persist.
            virtual int calculateMerge( WitnessMerge * w,
                                        int & callerValue,
                                        int & ruleValue,
                                        int & calleeValue )
            {
                (void) w;
                return callerValue + ruleValue + calleeValue;
            }

            //! Overload to calculate the value of a rule node.
            virtual int calculateRule( WitnessRule * w)
            {
                (void) w;
                std::string s = key2str(w->getRuleStub().from_stack());
                std::stringstream ss(s);
                int ret = -1;
                ss >> ret;
                return ret;
            }

            //! Overload to calculate the value of a trans node.
            virtual int calculateTrans( WitnessTrans * w)
            {
                (void) w;
                std::string s = key2str(w->getTrans().from_state());
                std::stringstream ss(s);
                int ret = -1;
                ss >> ret;
                return ret;
            }
        };
        
        struct StringComputer : public CalculatingVisitor<std::string>
        {
            //! Overload to calculate the value of an extend
            //! node. Modifications to the AnswerType& parameters will not
            //! persist.
            virtual std::string calculateExtend( WitnessExtend * w,
                                                 std::string & leftValue,
                                                 std::string & rightValue )
            {
                (void) w;
                return "(" + leftValue + " * " + rightValue + ")";
            }

            //! Overload to calculate the value of an combine
            //! node. Modifications to the list parameter will not persist.
            virtual std::string calculateCombine( WitnessCombine * w,
                                                  std::list<std::string> & childrenValues )
            {
                (void) w;
                std::string s = "(";
                for(std::list<std::string>::const_iterator iter = childrenValues.begin();
                    iter != childrenValues.end() ; ++iter)
                {
                    if (iter != childrenValues.begin()) {
                        s += " + ";
                    }
                    s += *iter;
                }
                return s + ")";
            }
      
            //! Overload to calculate the value of an merge
            //! node. Modifications to the std::string& parameters will not
            //! persist.
            virtual std::string calculateMerge( WitnessMerge * w,
                                                std::string & callerValue,
                                                std::string & ruleValue,
                                                std::string & calleeValue )
            {
                (void) w;
                return "merge(" + callerValue + ", " + ruleValue + ", " + calleeValue + ")";
            }

            //! Overload to calculate the value of a rule node.
            virtual std::string calculateRule( WitnessRule * w)
            {
                (void) w;
                return key2str(w->getRuleStub().from_stack());
            }

            //! Overload to calculate the value of a trans node.
            virtual std::string calculateTrans( WitnessTrans * w)
            {
                (void) w;
                return key2str(w->getTrans().from_state());
            }
        };
        

        TEST(wali$witness$CalculatingVisitor, WitnessTrans) {
            WitnessTrans w(wali::wfa::Trans(getKey("1"), 0, 0, NULL));
            
            NumberComputer nc;
            w.accept(nc);
            EXPECT_EQ(1, nc.answer());

            StringComputer sc;
            w.accept(sc);
            EXPECT_EQ("1", sc.answer());
        }
        
    }
}
