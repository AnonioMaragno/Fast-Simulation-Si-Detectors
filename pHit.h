#ifndef PHIT_H
#define PHIT_H

#include "pPoint.h"
#include "Riostream.h"
#include "TString.h"

class pHit : public pPoint {

    public:

        // Costruttori e distruttore
        pHit();
        pHit(pPoint* pto, Layer layer, int numPart, TString evID);
        pHit(double z, double phi, Layer layer, int numPart, TString evID);
        pHit(const pHit& source);
        void operator=(const pHit& source);//assignment operator
        virtual ~pHit();

        // Setter
        void SetLayer(Layer layer) {fLayer = layer;}
        void SetPhi(double phi);

        // Getter
        double GetR();
        Layer GetLayer() const {return fLayer;}
        double GetPhi() const { return fPhi;}
        TString GetId() const { return fId; }  // Serve per Event Display

    private:

        // Funzioni dichiarate private così che non si possa settare direttamente la X e la Y quando si ha un oggetto hit
        using pPoint::SetX;
        using pPoint::SetY;

        // Data members
        TString fId;    // Id della particella
        double fPhi;    // Info aggiuntiva per avere coordinate cilindriche
        Layer fLayer;   // Layer a cui appartiene la hit

    ClassDef(pHit,1);
};

#endif
