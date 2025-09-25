#ifndef PHIT_H
#define PHIT_H

#include "pPoint.h"
#include "Riostream.h"
#include "TString.h"

class pHit : public pPoint {

    public:

        pHit();
        pHit(pPoint* pto, Layer layer, int numPart, TString evID);
        pHit(double z, double phi, Layer layer, int numPart, TString evID);
        pHit(const pHit& source);
        void operator=(const pHit& source);//assignment operator
        virtual ~pHit();

        void SetLayer(Layer layer) {fLayer = layer;}
        void SetPhi(double phi);
        double GetR();
        Layer GetLayer() const {return fLayer;}
        double GetPhi() const { return fPhi;}
        TString GetId() const { return fId; }  // Serve per Event Display

    private:

        //dichiarati private così che non si possa settare direttamente la X e la Y quando si ha un oggetto hit
        using pPoint::SetX;
        using pPoint::SetY;

        TString fId;
        double fPhi; //info aggiuntiva per avere coordinate cilindriche
        Layer fLayer;

    ClassDef(pHit,1);
};

#endif
