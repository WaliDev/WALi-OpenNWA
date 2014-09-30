#ifndef RELATION_INTERFACE_GUARD
#define RELATION_INTERFACE_GUARD

class RelationInterface {
	public:
		virtual RelationInterface* Transpose() = 0;
		virtual RelationInterface* Detensor() = 0;
		virtual RelationInterface* DetensorTranspose() = 0;
};

#endif //RELATION_INTERFACE_GUARD
