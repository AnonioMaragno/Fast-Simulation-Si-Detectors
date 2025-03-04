#include "pPoint.h"

ClassImp(pPoint)

// Default constructor
pPoint::pPoint(): TObject(), fX(0.), fY(0.), fZ(0.)/*, fTheta(0.), fPhi(0.), fR(0.)*/ {

}

// Standard constructor
pPoint::pPoint(double x, double y, double z/*, double theta, double phi, double r*/): TObject(), fX(x), fY(y), fZ(z)/*, fTheta(theta), fPhi(phi), fR(r)*/ {

}

// Copy constructor
pPoint::pPoint(const pPoint &source):
  TObject(source),
  fX(source.fX),
  fY(source.fY),
  fZ(source.fZ)/*,
  fTheta(source.fTheta),
  fPhi(source.fPhi),
  fR(source.fR)*/ {

}

void pPoint::operator=(const pPoint &source)
{
  if (this != &source){
    TObject::operator=(source);
    fX = source.fX;
    fY = source.fY;
    fZ = source.fZ;
  }
}

// Destructor
pPoint::~pPoint() {
  std::cout << "Ho distrutto il punto di coordinata x: " << GetX() << std::endl; 
}

