#include "gtest/gtest.h"

#include "fixtures.hpp"
#include <boost/tuple/tuple.hpp>

using namespace wali::xfa;

#define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))


static Xfa::Word const words[] = {
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

static const unsigned num_words = NUM_ELEMENTS(words);

// WARNING: the order of the rows and columns in this table must be
//          consistent with the order of 'words' and 'nwas' below.
#if 0
static bool inclusion_answers_by_language[][num_words] = {
    /*             eps    a      aa     aaa    aaaa   a^5    a^6    a^7    a^8    b      bb     ab     ba */
    /* {}     */ { false, false, false, false, false, false, false, false, false, false, false, false, false },
    /* {eps}  */ { true,  false, false, false, false, false, false, false, false, false, false, false, false },
    /* {a}    */ { false, true,  false, false, false, false, false, false, false, false, false, false, false },
    /* {a^n}  */ { false, true,  true,  true,  true,  true,  true,  true,  true,  false, false, false, false },
    /* {a^2n} */ { false, false, true,  false, true,  false, true,  false, true,  false, false, false, false },
    /* {ab}   */ { false, false, false, false, false, false, false, false, false, false, false, true,  false },
    /* {a,b}  */ { false, true,  false, false, false, false, false, false, false, true,  false, false, false }
};
#endif

static std::string expected_intersection[][7] = {
    /*             {}     {eps}    {a}    {a^n}     {a^2n}    {ab}    {a,b} */
    /* {}     */ { "{}",  "{}",    "{}",  "{}",     "{}",     "{}",   "{}"    },
    /* {eps}  */ { "{}",  "{eps}", "{}",  "{}",     "{}",     "{}",   "{}"    },
    /* {a}    */ { "{}",  "{}",    "{a}", "{a}",    "{}",     "{}",   "{a}"   },
    /* {a^n}  */ { "{}",  "{}",    "{a}", "{a^n}",  "{a^2n}", "{}",   "{a}"   },
    /* {a^2n} */ { "{}",  "{}",    "{}",  "{a^2n}", "{a^2n}", "{}",   "{}"    },
    /* {ab}   */ { "{}",  "{}",    "{}",  "{}",     "{}",     "{ab}", "{}"    },
    /* {a,b}  */ { "{}",  "{}",    "{a}", "{a}",    "{}",     "{}",   "{a,b}" }
};

static std::string languages[] = {
    "{}", "{eps}", "{a}", "{a^n}", "{a^2n}", "{ab}", "{a,b}"
};

static std::string get_expected_intersection_language(std::string const & left, std::string const & right) {
    std::string const
        * start = &languages[0],
        * end = start + NUM_ELEMENTS(languages),
        * left_ptr = std::find(start, end, left),
        * right_ptr = std::find(start, end, right);

    assert(left_ptr != end);
    assert(right_ptr != end);

    return expected_intersection[left_ptr - start][right_ptr - start];
}


static bool xfas_same_modulo_words(Xfa const & left, Xfa const & right) {
    for (unsigned word = 0; word < num_words; ++word) {
        bool in_left = left.isAcceptedWithNonzeroWeight(words[word]);
        bool in_right = right.isAcceptedWithNonzeroWeight(words[word]);
        if (in_left != in_right) {
            std::cerr << "***** XFAs differ at word #" << word << "\n"
                      << "*****     which is " << (in_left ? "" : "not ") << "in the expected answer\n"
                      << "*****     and is " << (in_right ? "" : "not ") << "in the actual answer\n";
            return false;
        }
    }
    return true;
}


namespace wali {
    namespace xfa {

        TEST(wali$xfa$Xfa$helper, epsilonClose)
        {
            XfaContext context;
            Relations lrelations(context.voc, "left");
            Relations rrelations(context.voc, "left");
            BinaryRelation zero = lrelations.false_;

            Xfa lxfa = EmptyByImpossibleDataTransition(zero, lrelations).xfa;
            Xfa rxfa = EmptyByImpossibleDataTransition(zero, rrelations).xfa;

            wali::wfa::WFA::EpsilonCloseCache lcache,rcache;

            lxfa.wfa().epsilonCloseCached(getKey("p"), lcache);
            lxfa.wfa().epsilonCloseCached(getKey("q"), lcache);
            rxfa.wfa().epsilonCloseCached(getKey("p"), rcache);
            rxfa.wfa().epsilonCloseCached(getKey("q"), rcache);
        }


        std::pair<std::map<std::string, std::vector<Xfa> >,
                  std::map<std::string, std::vector<Xfa> > >
        produce_xfas(BinaryRelation zero,
                     Relations const & left_relations,
                     Relations const & right_relations)
        {
            std::map<std::string, std::vector<Xfa>> left_xfas, right_xfas;

            left_xfas["{}"].push_back(TrivialEmptyXfa(zero).xfa);
            right_xfas["{}"].push_back(TrivialEmptyXfa(zero).xfa);
            left_xfas["{eps}"].push_back(AcceptEmptyStringOnly(zero).xfa);
            right_xfas["{eps}"].push_back(AcceptEmptyStringOnly(zero).xfa);
            left_xfas["{}"].push_back(EmptyByMissingTransition(zero).xfa);
            right_xfas["{}"].push_back(EmptyByMissingTransition(zero).xfa);

#define REGISTER_XFA(language, name) \
            left_xfas[language].push_back(name(zero, left_relations).xfa);   \
            right_xfas[language].push_back(name(zero, right_relations).xfa);

            REGISTER_XFA("{}",     EmptyByImpossibleDataTransition);
            REGISTER_XFA("{a}",    SingleSimpleTrans);
            REGISTER_XFA("{a^n}",  BoringCycle);
            REGISTER_XFA("{a^2n}", DataRestrictedCycle);
            REGISTER_XFA("{ab}",   SimpleAB);
            REGISTER_XFA("{ab}",   AEpsilonB);
            REGISTER_XFA("{}",     ImpossibleSequence);
            REGISTER_XFA("{a,b}",  AOrB);
            REGISTER_XFA("{a}",    AlternativePathImpossible);
            REGISTER_XFA("{}",     EmptyByImpossibleDataTransitionEpsilon);
            REGISTER_XFA("{eps}",  SingleSimpleTransEpsilon);
#undef REGISTER_XFA

            return std::make_pair(left_xfas, right_xfas);
        }
        

        TEST(wali$xfa$Xfa$intersect, Battery)
        {
            XfaContext context;
            Relations left_relations(context.voc, "left");
            Relations right_relations(context.voc, "right");

            BinaryRelation zero = left_relations.false_;
            ASSERT_TRUE(zero->equal(right_relations.false_.get_ptr()));

            std::map<std::string, std::vector<Xfa>> left_xfas, right_xfas;
            boost::tie(left_xfas, right_xfas) = produce_xfas(zero, left_relations, right_relations);

            // Sanity check: every language should be the same.
            for (auto left_map_iter = left_xfas.begin(); left_map_iter != left_xfas.end(); ++left_map_iter) {
                std::string const & left_language = left_map_iter->first;

                auto const & left_some_xfas = left_map_iter->second;
                auto const & right_some_xfas = right_xfas[left_language];

                // Now make sure all XFAs with the given left and right
                // languages, when intersected, give the
                // expected_representative.
                for (auto left_xfa = left_some_xfas.begin(); left_xfa != left_some_xfas.end(); ++left_xfa) {
                    for (auto right_xfa = right_some_xfas.begin(); right_xfa != right_some_xfas.end(); ++right_xfa) {
                        std::stringstream ss;
                        ss << "Current left language: " << left_language << "\n"
                           << "Current XFA in that left language: " << (left_xfa - left_some_xfas.begin()) << "\n"
                           << "Current XFA in that right language: " << (right_xfa - right_some_xfas.begin()) << "\n";
                        SCOPED_TRACE(ss.str());
                            
                        ASSERT_TRUE(xfas_same_modulo_words(*left_xfa, *right_xfa));
                    }
                }
            }
            

            // Now intersect every XFA in left_xfas with every XFA in
            // right_xfas. Each "every" gets TWO loops. We'll interleave them
            // though.
            for (auto left_map_iter = left_xfas.begin(); left_map_iter != left_xfas.end(); ++left_map_iter) {
                for (auto right_map_iter = right_xfas.begin(); right_map_iter != right_xfas.end(); ++right_map_iter) {
                    std::string const & left_language = left_map_iter->first;
                    std::string const & right_language = right_map_iter->first;

                    auto const & left_some_xfas = left_map_iter->second;
                    auto const & right_some_xfas = right_map_iter->second;

                    std::string const & expected_language = get_expected_intersection_language(left_language, right_language);
                    Xfa const & expected_representative = *(left_xfas[expected_language].begin());

                    // Now make sure all XFAs with the given left and right
                    // languages, when intersected, give the
                    // expected_representative.
                    for (auto left_xfa = left_some_xfas.begin(); left_xfa != left_some_xfas.end(); ++left_xfa) {
                        for (auto right_xfa = right_some_xfas.begin(); right_xfa != right_some_xfas.end(); ++right_xfa) {
                            std::stringstream ss;
                            ss << "Current left language: " << left_language << "\n"
                               << "Current right language: " << right_language << "\n"
                               << "Current XFA in that left language: " << (left_xfa - left_some_xfas.begin()) << "\n"
                               << "Current XFA in that right language: " << (right_xfa - right_some_xfas.begin()) << "\n";
                            SCOPED_TRACE(ss.str());

                            Xfa const & intersection = left_xfa->intersect_conjoin(*right_xfa);
                            EXPECT_TRUE(xfas_same_modulo_words(expected_representative, intersection));
                        }
                    }
                }
            }
            
        }

    }
}

