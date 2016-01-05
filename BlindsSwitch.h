/*
  BlindsSwitch - Arduino libary for remote control outlet switches
  Copyright (c) 2011 Suat Özgür.  All right reserved.

  Contributors:
  - Andre Koehler / info(at)tomate-online(dot)de
  - Gordeev Andrey Vladimirovich / gordeev(at)openpyro(dot)com
  - Skineffect / http://forum.ardumote.com/viewtopic.php?f=2&t=46
  - Dominik Fischer / dom_fischer(at)web(dot)de
  - Frank Oltmanns / <first name>.<last name>(at)gmail(dot)com
  
  Project home: http://code.google.com/p/rc-switch/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef _BlindsSwitch_h
#define _BlindsSwitch_h

#include "Arduino.h"

#define BS_DOWN    0x1
#define BS_LIMIT   0x2
#define BS_CONFIRM 0x4
#define BS_STOP    0x5
#define BS_UP      0xC

class BlindsSwitch {

  public:
    BlindsSwitch(int transmitter_pin = -1);

    void Down   (unsigned int remote_id, unsigned int blinds_id)
    {
       send(remote_id, blinds_id, BS_DOWN);
    }
    void Up     (unsigned int remote_id, unsigned int blinds_id)
    {
       send(remote_id, blinds_id, BS_UP);
    }
    void Stop   (unsigned int remote_id, unsigned int blinds_id)
    {
       send(remote_id, blinds_id, BS_STOP);
    }
    void Confirm(unsigned int remote_id, unsigned int blinds_id)
    {
       send(remote_id, blinds_id, BS_CONFIRM);
    }
    void Limit  (unsigned int remote_id, unsigned int blinds_id)
    {
       send(remote_id, blinds_id, BS_LIMIT);
    }

    void AllDown   (unsigned int remote_id) { Down   (remote_id, 0xF); }
    void AllUp     (unsigned int remote_id) { Up     (remote_id, 0xF); }
    void AllStop   (unsigned int remote_id) { Stop   (remote_id, 0xF); }
    void AllConfirm(unsigned int remote_id) { Confirm(remote_id, 0xF); }
    void AllLimit  (unsigned int remote_id) { Limit  (remote_id, 0xF); }

    void send(unsigned int remote_id, unsigned int blinds_id, unsigned int command);
    
    void enableTransmit(int nTransmitterPin);
    void disableTransmit();
    void setPulseLength(int nPulseLength);
    void setRepeatTransmit(int nRepeatTransmit);
  
  private:
    void send0();
    void send1();
    void sendSync();
    void transmit(int nHighPulses, int nLowPulses);

    int nTransmitterPin;
    int nPulseLength;
    int nRepeatTransmit;

};

#endif
