#ifndef PEVENT_H
#define PEVENT_H

#include "TObject.h"
#include "pHit.h"
#include <Riostream.h>
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

		
		//funzione di cleaning
		static void disallocateMemory(){
			delete fHitsL1;
			fHitsL1 = nullptr;
			delete fHitsL2;
			fHitsL2 = nullptr;
		};




	private:
		// ASSIGNEMENT OPERATOR E COPY DICHIARATI PRIVATI COSÌ NON LI COSTRUISCE IL COMPILER
		// NON VOGLIO COPIE DEL MIO EVENTO
		void operator=(const pEvent& source){};//assignment operator
		pEvent(const pEvent& source) {};//copy constructor


		// data members
		static int fCounter;//per contare il numero dell'evento
		int fM;// Molteplicità di particelle
		pPoint fVertex;// Puntatore al vertice
		static TClonesArray *fHitsL1;// Array di hits per questo evento su layer 1;
		static TClonesArray *fHitsL2;// Array di hits per questo evento su layer 2; 
		

		//funzioni ausiliarie
		void findCosDirection(double *cosDir, double th, double phi);//trova coseni direttori		



	ClassDef(pEvent,1);
};


#endif
