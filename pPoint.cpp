#include "pPoint.h"

ClassImp(pPoint)

// Default constructor
pPoint::pPoint(): TObject(), fX(0.), fY(0.), fZ(0.){

}

// Standard constructor
pPoint::pPoint(double x, double y, double z): TObject(), fX(x), fY(y), fZ(z) {
}

// Copy constructor
pPoint::pPoint(const pPoint &source):
  TObject(source),
  fX(source.fX),
  fY(source.fY),
  fZ(source.fZ) {

}

//assignment operator
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
}

//per settare tutte e tre le coordinate insieme
void pPoint::SetCoord(double x, double y, double z) {
  SetX(x);
  SetY(y);
  SetZ(z);
}

// per copiare le coordinate di un punto in un altro senza creare un nuovo oggetto
void pPoint::SetEqualTo(const pPoint &source)
{
  SetCoord(source.fX, source.fY, source.fZ);
}
