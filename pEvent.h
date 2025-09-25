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
		pEvent(pPoint *vert, int mult, int eventCounter);//standard constructor
		virtual ~pEvent();//destructor
		
		//setter
		void SetMultiplicity(int mult) { fM = mult;};
		void SetVertex(pPoint* punto) { fVertex = punto;};

		//getter
		TString GetEventID() const { return feventID;}
		int GetMultiplicity() const { return fM; };
		pPoint* GetVertex() const { return fVertex; };
		double GetZVertex() const { return fVertex->GetZ(); };
		
		static TClonesArray* GetPtrHitsBP() { return fHitsBP; }; // Serve per Event Display
		static TClonesArray* GetPtrHitsL1() { return fHitsL1; };
		static TClonesArray* GetPtrHitsL2() { return fHitsL2; };

		//funzione di trasporto
		bool Trasporto(pPoint* pIniz, double* c, Layer lay, int index);

		
		//funzione di cleaning
		static void disallocateMemory(){
			delete fHitsL1;
			fHitsL1 = nullptr;
			delete fHitsL2;
			fHitsL2 = nullptr;
		};




	private:
		// data members
		
		//contatori
		static int fRegisteredL1;//per contare gli eventi registrati su L1
		static int fRegisteredL2;//per contare gli eventi registrati su L2

		int fM;// Molteplicità di particelle
		pPoint* fVertex;// Puntatore al vertice
		TString feventID; //ID dell'evento

		//array che conterranno le hits
		static TClonesArray *fHitsBP; //->
		static TClonesArray *fHitsL1; //-> 
		static TClonesArray *fHitsL2; //-> 
	

		// ASSIGNEMENT OPERATOR E COPY DICHIARATI PRIVATI COSÌ NON LI COSTRUISCE IL COMPILER
		// NON VOGLIO COPIE DEL MIO EVENTO
		void operator=(const pEvent& source){};//assignment operator
		pEvent(const pEvent& source);//copy constructor


		

	ClassDef(pEvent,1);
};


#endif
