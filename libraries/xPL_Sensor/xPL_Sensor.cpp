/*
  ArduixPL - xPL for arduino
  Copyright (c) 2012 Mathieu GRENET.  All right reserved.

  This file is part of ArduixPL.

    ArduixPL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ArduixPL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ArduixPL.  If not, see <http://www.gnu.org/licenses/>.

	  Modified Dec 23, 2012 by Mathieu GRENET
*/
#include "xPL_Sensor.h"

xPL_SchemaSensor xplSensor;


xPL_Sensor_Message::xPL_Sensor_Message(xPL_Sensor& sensor,const VString& request):xPL_Message(sensor)
	}
	}
bool xPL_Sensor::targeted(xPL_MessageIn& msg)
{
//	VString s = msg.key_device();
//	if (!s) return false;
//	if (!id() || s!=*id()) return false;

	if (msg.key_device() != id()) return false;

	VString s = msg.key_type();
	if (!s) return true;
	if (s==className()) return true;
	return false;
}

bool xPL_Sensor::parseMessage(xPL_MessageIn& msg) {

	}