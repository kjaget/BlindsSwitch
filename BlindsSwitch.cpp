/*
  BlindsSwitch - control window blinds using RF protocol from Bofu LCD-106 remote
  Based heavily on RCSwitch Arduino libary for remote control outlet switches
  Copyright (c) 2011 Suat Özgür.  All right reserved.
  
  Contributors:
  - Andre Koehler / info(at)tomate-online(dot)de
  - Gordeev Andrey Vladimirovich / gordeev(at)openpyro(dot)com
  - Skineffect / http://forum.ardumote.com/viewtopic.php?f=2&t=46
  - Dominik Fischer / dom_fischer(at)web(dot)de
  - Frank Oltmanns / <first name>.<last name>(at)gmail(dot)com
  - Andreas Steinel / A.<lastname>(at)gmail(dot)com
  
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

//TODO : see how sensitive the timings are
//TODO : check repeat counts
#include "BlindsSwitch.h"

BlindsSwitch::BlindsSwitch(int transmitter_pin) {
  enableTransmit(transmitter_pin);
  setPulseLength(350);
  setRepeatTransmit(15);
}

/**
  * Sets pulse length in microseconds
  */
void BlindsSwitch::setPulseLength(int nPulseLength) {
  this->nPulseLength = nPulseLength;
}

/**
 * Sets Repeat Transmits
 */
void BlindsSwitch::setRepeatTransmit(int nRepeatTransmit) {
  this->nRepeatTransmit = nRepeatTransmit;
}

/**
 * Enable transmissions
 *
 * @param nTransmitterPin    Arduino Pin to which the sender is connected to
 */
void BlindsSwitch::enableTransmit(int nTransmitterPin) {
  this->nTransmitterPin = nTransmitterPin;
  pinMode(nTransmitterPin, OUTPUT);
}

/**
  * Disable transmissions
  */
void BlindsSwitch::disableTransmit() {
  this->nTransmitterPin = -1;
}

void BlindsSwitch::transmit(int nHighPulses, int nLowPulses) {

    if (this->nTransmitterPin != -1) {
        digitalWrite(this->nTransmitterPin, HIGH);
        delayMicroseconds( this->nPulseLength * nHighPulses);
        digitalWrite(this->nTransmitterPin, LOW);
	// Subtract overhead from returning to next call from
	// timing delay here.
        delayMicroseconds( (this->nPulseLength - 55) * nLowPulses);
    }
}

/*
 * Everything is encoded bit-reversed to make checksum calculations easier
 */
void BlindsSwitch::send(unsigned int remote_id, unsigned int blinds_id, unsigned int command) {
   if (this->nTransmitterPin == -1)
      return;

   for (int repeat = 0; repeat < this->nRepeatTransmit; repeat++)
   {
   sendSync();
   /* first bit high pulse duration is combined with start of sync */
   send1();
   send1();
   /* 8-bit remote ID */
   for (unsigned int i = 0x1; i != 0x100; i <<= 1)
   {
      if (remote_id & i)
	 send1();
      else
	 send0();
   }
   /* Hard-coded 7-bit 0b1100001 */
   for (unsigned int i = 0x1; i != 0x80; i <<= 1)
   {
      if (0x43 & i)
	 send1();
      else
	 send0();
   }
   /* 4-bit blind ID */
   for (unsigned int i = 0x1; i != 0x10; i <<= 1)
   {
      if (blinds_id & i)
	 send1();
      else
	 send0();
   }
   /* 4-bit command */
   for (unsigned int i = 0x1; i != 0x10; i <<= 1)
   {
      if (command & i)
	 send1();
      else
	 send0();
   }
   /* Hard-coded 8-bit 0b10000000 */
   for (unsigned int i = 0x1; i != 0x100; i <<= 1)
   {
      if (0x01 & i)
	 send1();
      else
	 send0();
   }
   /* 8-bit checksum  : 121 - (remote_id rotated left 1 bit) - (command | blinds_ID) */
   unsigned char checksum =  121 - (remote_id << 1 | ((remote_id >> 7) & 1)) - ((command << 4) | blinds_id); 
   for (unsigned int i = 0x1; i != 0x100; i <<= 1)
   {
      if (checksum & i)
	 send1();
      else
	 send0();
   }
   /* Trailing 1 */
   send1();
   }
}

/**
 * Sends a "0" Bit
 *                    _    
 * Waveform Protocol | |__
 */
void BlindsSwitch::send0() {
   //Serial.println("send0");
   transmit(1,2);
}

/**
 * Sends a "1" Bit
 *                    __  
 * Waveform Protocol |  |_
 */
void BlindsSwitch::send1() {
   //Serial.println("send1");
   transmit(2,1);
}

/**
 * Sends a "Sync" Bit
 *                     ______________
 * Waveform Protocol  |              |_______
 */
void BlindsSwitch::sendSync() {
    if (this->nTransmitterPin != -1) {
        digitalWrite(this->nTransmitterPin, HIGH);
        delayMicroseconds( 4875 );
        digitalWrite(this->nTransmitterPin, LOW);
        delayMicroseconds( 2425 );
        digitalWrite(this->nTransmitterPin, HIGH);
        delayMicroseconds( 1255 - this->nPulseLength);
    }
}
