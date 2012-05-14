#ifndef wali_genereate_Random_WPDS_guard
#define wali_genereate_Random_WPDS_guard 1

// ::wali::wpds::fwpds
#include "wali/wpds/WPDS.hpp"

// ::wali
#include "wali/Key.hpp"
#include "wali/SemElem.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/Countable.hpp"

// ::std
#include <string>
#include <vector>
#include <fstream>

namespace wali 
{
  namespace wpds
  {

    class RandomPdsGen;
    typedef wali::ref_ptr< RandomPdsGen > random_pdsgen_t;

    class RandomPdsGen : public Countable
    {
      public:
        class WtGen : public Countable
        {
          public:
            /**
             * Extend this class and implement this function so I can get random weights.
             **/
            WtGen() : Countable() {}
            virtual sem_elem_t operator () () = 0;
            virtual ~WtGen() {}
        };
        typedef wali::ref_ptr< WtGen > wtgen_t;

        /**
         * Used to return the important names to the function  calling randfwpds(...)
         */
        struct Names
        {
          typedef std::vector<wali::Key> KeyVector;
          wali::Key pdsState;
          KeyVector entries;
          KeyVector exits;
          KeyVector errs;
        };

        /**
         * Generate a random FWPDS with the configuration given by conf
         * @detail ...
         * @see struct Conf
         **/
        RandomPdsGen(
            wtgen_t randomWt,
            int numprocs = 100,
            int numcalls = 100,
            int numnodes = 400,
            int numsplits = 100,
            int numerrs = 0,
            double pCall = 0.45, 
            double pSplit = 0.45,
            unsigned seed = 0
            );
        ~RandomPdsGen();
        void get(wali::wpds::WPDS& pds, Names& names, std::ostream * o = NULL);

      private:
        // /Forward declaration of functions.
        void genproc(
            wali::wpds::WPDS& pds, 
            int procnum, 
            unsigned& curKey,
            int remNodes, 
            int remSplits, 
            int remCalls, 
            std::ostream *o, 
            int tabstop
            );
        wali::Key genblock(
            wali::wpds::WPDS& pds, 
            wali::Key curhead, 
            unsigned& curKey,
            int remNodes, 
            int remSplits, 
            int remCalls, 
            std::ostream *o, 
            int tabstop
            );
        std::string getName(unsigned curKey);

      private:
        //Configuration of the Generator as set by the constructor
        //! pointer to a function that generates random weights
        wtgen_t randomWt;
        //! The number of procedures to be generated
        int numprocs;
        //! The number of callsites to be generated
        int numcalls;
        //! The number of cfg nodes to be generated [loosely followed]
        int numnodes;
        //! The number of two-way splits to be generated.
        int numsplits;
        //! Number of error points to be generated
        int numerrs;
        //! Probability with wich a call is generated
        double pCall;
        //! Probability with wich a split is generated
        double pSplit;
        //! If Non-zero, this seed is used for the random number generator
        unsigned seed;

        // Data Used during the generation of a new randomd PDS
        // Array that stores keys for all procedure entries.
        std::vector<wali::Key> entries;
        // Array that stores keys for all procedure exits.
        std::vector<wali::Key> exits;
        // Name of the pds state to use
        wali::Key pdsState;
    };
  } // namespace wpds
} // namespace wali
#endif //wali_genereate_Random_WPDS_guard
