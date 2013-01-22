/*  ArduixPL - xPL for arduino  Copyright (c) 2012/2013 Mathieu GRENET.  All right reserved.  This file is part of ArduixPL.    ArduixPL is free software: you can redistribute it and/or modify    it under the terms of the GNU General Public License as published by    the Free Software Foundation, either version 3 of the License, or    (at your option) any later version.    ArduixPL is distributed in the hope that it will be useful,    but WITHOUT ANY WARRANTY; without even the implied warranty of    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    GNU General Public License for more details.    You should have received a copy of the GNU General Public License    along with ArduixPL.  If not, see <http://www.gnu.org/licenses/>.	  Modified 2013-1-22 by Mathieu GRENET 	  mailto:mathieu@mgth.fr	  http://www.mgth.fr*/#include "xPL_SensorDHT.h"

xPL_SensorDHT::xPL_SensorDHT(uint8_t pin,byte type, const VString & tName, const VString & hName){	_pin=pin;	_type = type;	_temp.setId(tName);	_temp.setBuffer(50);	_humidity.setId(hName);	_humidity.setBuffer(50);	_humidity.setThreshold(0.25);	addChild(&_temp);	addChild(&_humidity);	_time = millis()-2000;}void xPL_SensorDHT::loop(){		if (_type==22)		{			if (millis()-_time<2000) return;		} else if (millis()-_time<1000) return;		_time = millis();		int res = (_type==22)?_dht.read22(_pin):_dht.read11(_pin);		if (res==DHTLIB_OK)		{			_temp.setValue(_dht.temperature);			_humidity.setValue(_dht.humidity);		}}