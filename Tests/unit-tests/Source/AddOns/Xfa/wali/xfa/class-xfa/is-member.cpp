#include "gtest/gtest.h"

#include "fixtures.hpp"

using namespace wali::xfa;

#define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))



// WARNING: the order of the rows and columns in this table must be
//          consistent with the order of 'words' and 'nwas' below.
static bool expected_answers[][13] = {
    /*                     eps    a      aa     aaa    aaaa   a^5    a^6    a^7    a^8    b      bb     ab     ba */
    /* trivial {}     */ { false, false, false, false, false, false, false, false, false, false, false, false, false },
    /* accept empty   */ { true,  false, false, false, false, false, false, false, false, false, false, false, false },
    /* no trans {}    */ { false, false, false, false, false, false, false, false, false, false, false, false, false },
    /* impossible data*/ { false, false, false, false, false, false, false, false, false, false, false, false, false },
    /* simple {a}     */ { false, true,  false, false, false, false, false, false, false, false, false, false, false },
    /* simple cycle   */ { false, true,  true,  true,  true,  true,  true,  true,  true,  false, false, false, false },
    /* even cycle     */ { false, false, true,  false, true,  false, true,  false, true,  false, false, false, false },
    /* simple {ab}    */ { false, false, false, false, false, false, false, false, false, false, false, true,  false },
    /* impossible seq */ { false, false, false, false, false, false, false, false, false, false, false, false, false },
    /* simple {a, b}  */ { false, true,  false, false, false, false, false, false, false, true,  false, false, false },
    /* only {a} poss. */ { false, true,  false, false, false, false, false, false, false, false, false, false, false },
    /* immpossible eps*/ { false, false, false, false, false, false, false, false, false, false, false, false, false },
    /* simple eps     */ { true,  false, false, false, false, false, false, false, false, false, false, false, false }
};


namespace wali {
    namespace xfa {

        TEST(wali$xfa$Xfa$isAcceptedWithNonzeroWeight, Battery)
        {
            XfaContext context;
            Relations relations(context.voc, "left");
            sem_elem_t zero = relations.false_;

            Xfa const xfas[] = {
                TrivialEmptyXfa(zero).xfa,
                AcceptEmptyStringOnly(zero).xfa,
                EmptyByMissingTransition(zero).xfa,
                EmptyByImpossibleDataTransition(zero, relations).xfa,
                SingleSimpleTrans(zero, relations).xfa,
                BoringCycle(zero, relations).xfa,
                DataRestrictedCycle(zero, relations).xfa,
                SimpleAB(zero, relations).xfa,
                ImpossibleSequence(zero, relations).xfa,
                AOrB(zero, relations).xfa,
                AlternativePathImpossible(zero, relations).xfa,
                EmptyByImpossibleDataTransitionEpsilon(zero, relations).xfa,
                SingleSimpleTransEpsilon(zero, relations).xfa
            };

            Xfa::Word const words[] = {
                Words().epsilon,
                Words().a,
                Words().aa,
                Words().aaa,
                Words().aaaa,
                Words().a5,
                Words().a6,
                Words().a7,
                Words().a8,
                Words().b,
                Words().bb,
                Words().ab,
                Words().ba
            };
            

            const unsigned num_xfas = NUM_ELEMENTS(xfas);
            ASSERT_EQ(num_xfas, NUM_ELEMENTS(expected_answers));

            const unsigned num_words = NUM_ELEMENTS(words);
            ASSERT_EQ(num_words, NUM_ELEMENTS(expected_answers[0]));
            
            for (unsigned word = 0 ; word < num_words ; ++word) {
                for (unsigned xfa = 0 ; xfa < num_xfas ; ++xfa) {
                    std::stringstream ss;
                    ss << "Current XFA number " << xfa << " and word number " << word;
                    SCOPED_TRACE(ss.str());
                    
                    if (expected_answers[xfa][word]) {
                        EXPECT_TRUE(xfas[xfa].isAcceptedWithNonzeroWeight(words[word]));
                    }
                    else {
                        EXPECT_FALSE(xfas[xfa].isAcceptedWithNonzeroWeight(words[word]));
                    }
                }
            }
        }

    }
}

