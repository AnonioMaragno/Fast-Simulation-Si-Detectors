#ifndef PPOINT_H
#define PPOINT_H

#include "TObject.h"
#include <Riostream.h>

enum class Layer {BP, L1, L2};

class pPoint : public TObject {

	public:

		pPoint();
		pPoint(double x, double y, double z);
        pPoint(const pPoint& source);
        void operator=(const pPoint& source);
		virtual ~pPoint();
		
        double GetX() const {return fX;} 
        double GetY() const {return fY;}
        double GetZ() const {return fZ;}

        void SetX(double x) {fX = x;}
        void SetY(double y) {fY = y;}
        void SetZ(double z) {fZ = z;}
        void SetCoord(double x, double y, double z);
        void SetEqualTo(const pPoint &source);

	private:
    
		double fX;           // Coordinata cartesiana x del vertice
		double fY;           //    ''          ''     y     ''
		double fZ;           //    ''          ''     z     ''

	ClassDef(pPoint,1);
};


#endif
