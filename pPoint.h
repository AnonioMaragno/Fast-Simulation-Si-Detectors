#ifndef PPOINT_H
#define PPOINT_H

#include "TObject.h"
#include <Riostream.h>

enum class Layer {BP, L1, L2}; // contiene i 3 strati attraversati dalle particelle

class pPoint : public TObject {

	public:

        // Costruttori e distruttore
		pPoint();
		pPoint(double x, double y, double z);
        pPoint(const pPoint& source);
        void operator=(const pPoint& source);
		virtual ~pPoint();
		
        // Getter
        double GetX() const {return fX;} 
        double GetY() const {return fY;}
        double GetZ() const {return fZ;}

        // Setter
        void SetX(double x) {fX = x;}
        void SetY(double y) {fY = y;}
        void SetZ(double z) {fZ = z;}
        void SetCoord(double x, double y, double z);
        void SetEqualTo(const pPoint &source);

	private:
    
        // Data members
		double fX;           // Coordinata cartesiana x del vertice
		double fY;           //    ''          ''     y     ''
		double fZ;           //    ''          ''     z     ''

	ClassDef(pPoint,1);
};


#endif
