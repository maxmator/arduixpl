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

	  Modified Apr 17, 2012 by Mathieu GRENET
*/

#ifndef XPLSCHEMA_H
#define XPLSCHEMA_H

#include <xPL.h>

class xPL_Schema : public xPL_Node
{
protected:
	virtual bool parseMessage(xPL_Message& msg);
	virtual bool configure(xPL_Key& key);
	virtual bool targeted(xPL_Message& msg);
	
public:

	xPL_Schema();

	virtual void reg();
	virtual xPL_Schema* schema();
};
#endif
