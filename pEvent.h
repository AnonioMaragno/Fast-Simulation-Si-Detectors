#ifndef PEVENT_H
#define PEVENT_H

#include "TObject.h"


class pEvent : public TObject {

	public:
		pEvent();
		pEvent(double x, double y, double z, double theta, double phi);

		virtual ~pEvent();
		
		double SetMultiplicity();



	private:
    
		double fX;           // Coordinata cartesiana x del vertice
		double fY;           //    ''          ''     y     ''
		double fZ;           //    ''          ''     z     ''
//		double fTheta;       // Angolo polare theta nel SRL
//		double fPhi;         // Angolo azimutale nel SRL
		double fId;          // Numero dell'evento
		double fM;           // Molteplicità di particelle
		double *fVertex;     // Array contente le informazioni riguardanti il vertice
		





	ClassDef(pEvent,1);
};


#endif
