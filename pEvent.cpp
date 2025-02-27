#include "pEvent.h"

// Default Constructor
pEvent::pEvent(): TObject(), fX(), fY(), fZ(), fTheta(), fVertex(NULL)    {

}

// Standard Constructor
pEvent::pEvent(double x, double y, double z): TObject(),
fX(x),
fY(y),
fZ(z),
fTheta(theta),
fPhi(phi),
fId(),
fVertex(NULL)   {

}

void pEvent::SetMultiplicity()
