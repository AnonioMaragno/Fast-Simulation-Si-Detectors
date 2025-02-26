#include "pRandom.h"

// Default Constructor
pRandom::pRandom(): TObject()    {

}

// Destructor
pRandom::~pRandom(){
  delete this;
}

// Random number from a gaussian distribution
pRandom::GaussRndm(double mean, double std) {
	return this->Gauss(mean,std);
}

// Random number from a uniform distribution
pEvent::UnifRndm(double alpha) {
	return this->Rndm()*alpha;
}

// Random number from an assigned distribution
pEvent::AssignRndm() {

}
