#ifndef _LAMPARA_H_
#define _LAMPARA_H_

#include <SinricProDevice.h>
#include <Capabilities/PowerStateController.h>
#include <Capabilities/ModeController.h>

class  Lampara
    : public SinricProDevice,
      public PowerStateController< Lampara>,
      public ModeController< Lampara>
{
  friend class PowerStateController< Lampara>;
  friend class ModeController< Lampara>;

public:
   Lampara(const String &deviceId) : SinricProDevice(deviceId, " Lampara"){};
};

#endif
