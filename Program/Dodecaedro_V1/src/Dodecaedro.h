#ifndef _DODECAEDRO_H_
#define _DODECAEDRO_H_

#include <SinricProDevice.h>
#include <Capabilities/PowerStateController.h>
#include <Capabilities/ModeController.h>

class Dodecaedro 
: public SinricProDevice
, public PowerStateController<Dodecaedro>
, public ModeController<Dodecaedro> {
  friend class PowerStateController<Dodecaedro>;
  friend class ModeController<Dodecaedro>;
public:
  Dodecaedro(const String &deviceId) : SinricProDevice(deviceId, "Dodecaedro") {};
};

#endif
