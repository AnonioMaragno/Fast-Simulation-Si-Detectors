#ifndef PHIT_H
#define PHIT_H

#include "pPoint.h"
#include "Riostream.h"
using std::string;

class pHit : public pPoint {

    public:

        pHit();
        pHit(double x, double y, double z, Layer layer, int index, int count);
        pHit(const pHit& source);
        void operator=(const pHit& source);//assignment operator
        virtual ~pHit();

        void SetLayer(Layer layer) {fLayer = layer;}
        Layer GetLayer() const {return fLayer;}
        double GetPhi() const;

    private:

        string fId;
        Layer fLayer;

    ClassDef(pHit,1);
};

#endif
