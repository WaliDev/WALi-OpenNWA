#include "BinRel_Interface.hpp"
#include "BinRel.hpp"

using namespace wali::domains::binrel;

BinRelInterface::BinRelInterface(){
	this->n = NULL;
}

BinRelInterface::BinRelInterface(BinRel * n){
	this->n = n;
}

void BinRelInterface::setTensored(bool b){
	this->tensored = b;
}
      
BinRelInterface * BinRelInterface::Transpose(){
	this->n = static_cast<BinRel*>(this->n->transpose().get_ptr());
	return this;
}
                
BinRelInterface* BinRelInterface::Detensor(){
	this->n = static_cast<BinRel*>(this->n->detensor().get_ptr());
	return this;
}
                
BinRelInterface* BinRelInterface::DetensorTranspose(){
	this->n = static_cast<BinRel*>(this->n->detensorTranspose().get_ptr());
	return this;
}
                
BinRelInterface* BinRelInterface::Tensor(BinRelInterface * RI){
	this->n = static_cast<BinRel*>(this->n->Kronecker(RI->n).get_ptr());
	return this;
}

BinRelInterface* BinRelInterface::Star()
{
	this->n = static_cast<BinRel*>(this->n->star().get_ptr());
	return this;
}

BinRelInterface* BinRelInterface::Extend(BinRelInterface * RI)
{
	this->n = static_cast<BinRel*>(this->n->extend(RI->n).get_ptr());
	return this;
}
                
BinRelInterface* BinRelInterface::Combine(BinRelInterface* RI)
{
	this->n = static_cast<BinRel*>(this->n->combine(RI->n).get_ptr());
	return this;
}
                
BinRelInterface* BinRelInterface::One()
{
	this->n = static_cast<BinRel*>(this->n->one().get_ptr());
	return this;
}

BinRelInterface* BinRelInterface::Zero()
{
	this->n = static_cast<BinRel*>(this->n->zero().get_ptr());
	return this;
}
