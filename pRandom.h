#ifndef PRANDOM_H
#define PRANDOM_H

#include "TObject.h"
#include "TRandom3.h"

class pRandom : public TObject() {

	public:
		pRandom();
		virtual ~pRandom();
		
		double GaussRndm(double mean, double std);
		double UnifRndm(double alpha);
		double AssignRndm();
		



	private:
    	

	ClassDef(pRandom,1);
};


#endif
