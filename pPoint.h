#ifndef PPOINT_H
#define PPOINT_H

#include "TObject.h"


enum class Layer {BP, L1, L2};

class pPoint : public TObject {

	public:

		pPoint();
		pPoint(double x, double y, double z/*, double theta, double phi, double r*/);
        pPoint(const pPoint& source);
		virtual ~pPoint();
		
        double GetX() const {return fX;} 
        double GetY() const {return fY;}
        double GetZ() const {return fZ;}
        /*double GetTheta() const {return fTheta;}
        double GetPhi() const {return fPhi;}
        double GetR() const {return fR;}*/
        void SetX(double x) {fX = x;}
        void SetY(double y) {fY = y;}
        void SetZ(double z) {fZ = z;}
        /*void SetTheta(double theta) {fTheta = theta;}
        void SetPhi(double phi) {fPhi = phi;}
        void SetR(double r) {fR = r;}*/

	private:
    
		double fX;           // Coordinata cartesiana x del vertice
		double fY;           //    ''          ''     y     ''
		double fZ;           //    ''          ''     z     ''
		/*double fR;           // Raggio
        double fTheta;       // Angolo polare 
		double fPhi;         // Angolo azimutale*/

	ClassDef(pPoint,1);
};


#endif
