#ifndef PEVENT_H
#define PEVENT_H

#include "TObject.h"
#include "pHit.h"
#include "TClonesArray.h"
#include "pPoint.h"

using namespace std;


class pEvent : public TObject {

	public:
		pEvent();//default constructor
		pEvent(pPoint *vert, int mult);//standard constructor
		virtual ~pEvent();//destructor
		
		//setter
		void SetMultiplicity(int mult) { fM = mult;};
		void SetVertex(pPoint punto) { fVertex = punto;};

		//getter
		int GetMultiplicity() const { return fM; };
		pPoint GetVertex() const { return fVertex; };
		double GetZVertex() const { return fVertex.GetZ(); };

		//funzione di trasporto
		static pHit Trasporto(pPoint pIniz, double Theta, double Phi);



	private:
		// ASSIGNEMENT OPERATOR E COPY DICHIARATI PRIVATI COSÌ NON LI COSTRUISCE IL COMPILER
		// NON VOGLIO COPIE DEL MIO EVENTO
		void operator=(const pEvent& source){};//assignment operator
		pEvent(const pEvent& source) {};//copy constructor


		// data members
		static int fCounter;//per contare il numero dell'evento
		int fM;// Molteplicità di particelle
		pPoint fVertex;// Puntatore al vertice
		TClonesArray *fHits;// Array di hits per questo evento; 
		





	ClassDef(pEvent,1);
};


#endif
