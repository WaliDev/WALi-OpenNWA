#ifndef WALI_TSLWEIGHT_GUARD
#define WALI_TSLWEIGHT_GUARD 1
#include "gtr/src/lang/gtr_config.h"
#include "wali/ref_ptr.hpp"
#include "wali/SemElemTensor.hpp"
#include "tsl/analysis_components/src/reinterps/emul/conc_externs.hpp"
typedef tsl_regexp::Conc1LevelRTG RTG;

namespace wali
{
	namespace domains
	{
		namespace tsl_weight
		{
			class TSLWeight : public wali::SemElemTensor
			{
				typedef wali::ref_ptr<TSLWeight> tsl_weight_t;
			public:
				/** @see TSLWeight::Compose */
				friend tsl_weight_t operator*(tsl_weight_t a, tsl_weight_t b);
				/** @see TSLWeight::Union */
				friend tsl_weight_t operator|(tsl_weight_t a, tsl_weight_t b);
				/** @see TSLWeight::Intersect */
				friend tsl_weight_t operator&(tsl_weight_t a, tsl_weight_t b);
			public:
				TSLWeight(const TSLWeight& that);
				TSLWeight(RTG::regExpRefPtr tslW);
				virtual ~TSLWeight();
			public:
				tsl_weight_t Compose(tsl_weight_t that) const;
				tsl_weight_t Union(tsl_weight_t that) const;
				tsl_weight_t Intersect(tsl_weight_t that) const;
				bool Equal(tsl_weight_t se) const;
				tsl_weight_t Transpose() const;
			public:
				// ////////////////////////////////
				// SemElem methods
				sem_elem_t one() const;
				sem_elem_t zero() const;

				bool isOne() const {
					return false;
				}

				bool isZero() const {
					return false;
				}

				/** @return [this]->Union( cast<TSLWeight*>(se) ) */
				sem_elem_t combine(SemElem* se);

				/** @return [this]->Compose( cast<TSLWeight*>(se) ) */
				sem_elem_t extend(SemElem* se);

				sem_elem_t star();


				/** @return [this]->Equal( cast<TSLWeight*>(se) ) */
				bool equal(SemElem* se) const;

				std::ostream& print(std::ostream& o) const;
				// ////////////////////////////////
				// SemElemTensor methods

				/** @return [this]->Transpose() */
				sem_elem_tensor_t transpose();

				sem_elem_tensor_t tensor(SemElemTensor* se);

				sem_elem_tensor_t detensor();

				sem_elem_tensor_t detensorTranspose();

				RTG::regExpRefPtr getWeight() const {
					return tslW;
				}

				// ////////////////////////////////
				// Printing functions
				//static void printHandler(FILE *o, int var);
			protected:
				RTG::regExpRefPtr tslW;
			};
		}
	}
}
#endif
