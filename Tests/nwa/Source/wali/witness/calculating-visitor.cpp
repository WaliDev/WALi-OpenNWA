#include "gtest/gtest.h"

#include "wali/Key.hpp"
#include "wali/Common.hpp"
#include "wali/wpds/Config.hpp"
#include "wali/wpds/Rule.hpp"
#include "wali/witness/WitnessMerge.hpp"
#include "wali/witness/WitnessCombine.hpp"
#include "wali/witness/WitnessExtend.hpp"
#include "wali/witness/WitnessRule.hpp"
#include "wali/witness/WitnessTrans.hpp"
#include "wali/witness/CalculatingVisitor.hpp"

using wali::wpds::Config;
using wali::wpds::Rule;
using wali::wfa::Trans;

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
                std::string s = key2str(w->getRuleStub().from_state());
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
                return key2str(w->getRuleStub().from_state());
            }

            //! Overload to calculate the value of a trans node.
            virtual std::string calculateTrans( WitnessTrans * w)
            {
                (void) w;
                return key2str(w->getTrans().from_state());
            }
        };

        
        witness_t make_example_trans(std::string val)
        {
            Key k = getKey("dummy");
            witness_t w = new WitnessTrans(Trans(getKey(val), k, k, NULL));
            return w;
        }

        witness_t make_example_rule(std::string val)
        {
            Key k = getKey("dummy");
            Config* from = new Config(getKey(val), k);
            Config* to   = new Config(k, k);
            Rule rule(from, to, 0, NULL);
            witness_t w = new WitnessRule(rule);
            return w;
        }

        witness_t make_combine(witness_t left, witness_t right)
        {
            WitnessCombine * w = new WitnessCombine(NULL);
            w->addChild(left);
            w->addChild(right);
            return w;
        }
        

        TEST(wali$witness$CalculatingVisitor, WitnessTrans) {
            witness_t w = make_example_trans("1");
            
            NumberComputer nc;
            w->accept(nc);
            EXPECT_EQ(1, nc.answer());

            StringComputer sc;
            w->accept(sc);
            EXPECT_EQ("1", sc.answer());
        }

        TEST(wali$witness$CalculatingVisitor, WitnessRule) {
            witness_t w = make_example_rule("1");
            
            NumberComputer nc;
            w->accept(nc);
            EXPECT_EQ(1, nc.answer());

            StringComputer sc;
            w->accept(sc);
            EXPECT_EQ("1", sc.answer());
        }
        
        TEST(wali$witness$CalculatingVisitor, WitnessCombine) {
            witness_t w1 = make_example_rule("2");
            witness_t w2 = make_example_rule("3");

            witness_t w = make_combine(w1, w2);
            
            NumberComputer nc;
            w->accept(nc);
            EXPECT_EQ(5, nc.answer());

            StringComputer sc;
            w->accept(sc);
            EXPECT_EQ("(2 + 3)", sc.answer());
        }

        TEST(wali$witness$CalculatingVisitor, WitnessExtend$2arg) {
            witness_t w1 = make_example_rule("2");
            witness_t w2 = make_example_rule("3");

            witness_t w = new WitnessExtend(NULL, w1, w2);
            
            NumberComputer nc;
            w->accept(nc);
            EXPECT_EQ(6, nc.answer());

            StringComputer sc;
            w->accept(sc);
            EXPECT_EQ("(2 * 3)", sc.answer());
        }

        TEST(wali$witness$CalculatingVisitor, WitnessExtend$1arg$impossible) {
            witness_t w1 = make_example_rule("1");
            EXPECT_DEATH({
                    WitnessExtend e(NULL, NULL, w1);
                },
                "Assertion `the_left != __null\' failed");
            EXPECT_DEATH({
                    WitnessExtend e(NULL, w1, NULL);
                },
                "Assertion `the_right != __null\' failed");
            EXPECT_DEATH({
                    WitnessExtend e(NULL, NULL, NULL);
                },
                "Assertion `the_left != __null\' failed");
        }


        TEST(wali$witness$CalculatingVisitor, largeExamples$Richs) {
            witness_t stem =
                new WitnessExtend(NULL,
                                  new WitnessExtend(NULL,
                                                    make_example_trans("1 root"),
                                                    make_example_rule("2->3")),
                                  make_example_rule("3->6"));

            witness_t loop_body =
                new WitnessExtend(NULL,
                                  new WitnessExtend(NULL,
                                                    new WitnessExtend(NULL,
                                                                      new WitnessExtend(NULL,
                                                                                        stem,
                                                                                        make_example_rule("6->7")),
                                                                      make_example_rule("7->11")),
                                                    make_example_rule("11->13")),
                                  make_example_rule("13->6"));

            witness_t loop_head = make_combine(loop_body, stem);

            witness_t tree =
                new WitnessExtend(NULL,
                                  new WitnessExtend(NULL,
                                                    new WitnessExtend(NULL,
                                                                      new WitnessExtend(NULL,
                                                                                        loop_head,
                                                                                        make_example_rule("6->7")),
                                                                      make_example_rule("7->11")),
                                                    make_example_rule("11->13")),
                                  make_example_rule("13->15"));
                                                                      
            NumberComputer nc;
            tree->accept(nc);
            EXPECT_EQ(216468252, nc.answer());


            std::string str_stem = "((1 root * 2->3) * 3->6)";
            std::string str_body = "((((" + str_stem + " * 6->7) * 7->11) * 11->13) * 13->6)";
            std::string str_loop = "(" + str_body + " + " + str_stem + ")";
            std::string str_tree = "((((" + str_loop + " * 6->7) * 7->11) * 11->13) * 13->15)";

            StringComputer sc;
            tree->accept(sc);
            EXPECT_EQ(str_tree, sc.answer());
        }

    }
}
