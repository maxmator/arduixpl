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


#include "xPL_AdapterENC28J60.h"
#include <EtherCard.h>
#include "utility/xPL_Debug.h"

xPL_AdapterENC28J60 xplAdapter;

byte Ethernet::buffer[XPL_BUFFER_SIZE+UDP_DATA_P];

bool xPL_AdapterENC28J60::begin()
{
	DBG(F("<begin ENC28J60>"),);

	_mac.toArray(ether.mymac);
	ether.begin(sizeof Ethernet::buffer, EtherCard::mymac, ENC28J60_PIN);

#ifdef XPL_DHCP
	if (_dhcp && ether.dhcpSetup())
	{
		_ip.bin=ether.myip;
		_mask.bin=ether.mymask;
	}
	else
#endif */
	{
		
		_ip.toArray(ether.myip);
		_mask.toArray(ether.mymask);
		
/*
		ether.myip[0]=0xFF;
		ether.myip[1]=0xFF;
		ether.myip[2]=0xFF;
		ether.myip[3]=0xFF;
		ether.mymask[0]=0x00;
		ether.mymask[1]=0x00;
		ether.mymask[2]=0x00;
		ether.mymask[3]=0x00;

		ether.staticSetup();

		_ip.bin=ether.myip;
		_mask.bin=ether.mymask;*/
	}

	ether.enableBroadcast();

	DBG(F("IP: "),_ip);

	return true;
}

bool xPL_AdapterENC28J60::connection()
{
#ifdef XPL_DHCP
  if (!ether.using_dhcp)
  {
	  if (!ether.dhcpSetup()) return false;
  }
#endif 
  return true;
}


bool xPL_AdapterENC28J60::loop()
{
	if (!connection()) { return false; }

	word receiveLen = ether.packetReceive();
	if (!receiveLen) { return false; }

	DBG(F("received:"),receiveLen);


#ifdef XPL_HTML_SUPPORT
  word receiveTCP = ether.packetLoop(receiveLen);

  if(receiveTCP &&
	 ether.bufferSize &&
     ether.buffer[IP_PROTO_P] == IP_PROTO_TCP_V && 
     ether.buffer[UDP_DST_PORT_H_P] == 0x00 && 
     ether.buffer[UDP_DST_PORT_L_P] == 0x50) 
	{
		char* data = (char *) Ethernet::buffer + receiveTCP;

		if (strncmp("GET / ", data, 6) == 0) 
		{
#ifdef XPL_DEBUG
			S(html).printDebug();
#endif
			sendHomePage();
		}
		
		return true;
	}
#endif

  // UDP port 3865 -> xPL
  if(ether.bufferSize &&
     ether.buffer[IP_PROTO_P] == IP_PROTO_UDP_V && 
     ether.buffer[UDP_DST_PORT_H_P] == highByte(XPL_PORT) && 
     ether.buffer[UDP_DST_PORT_L_P] == lowByte(XPL_PORT) ) 
    {
		DBG(F("<xPL>"),);
		xPL.receivedMessage((char*)ether.buffer+UDP_DATA_P);
    }

	return true;
}



bool xPL_AdapterENC28J60::sendMessage(xPL_Message& msg) {


	DBG(F("<send_ENC28J60>"),VString(msg).len());
	if (connection()) 
	{
		ether.udpPrepare(XPL_PORT, ether.myip, XPL_PORT);
		ether.copyMac(ether.buffer + ETH_SRC_MAC, ether.mymac);
		for (int i=0;i<6;i++) ether.buffer[ETH_DST_MAC + i] = 0x0FF;

		ether.buffer[IP_TOTLEN_L_P]=0x82;
		ether.buffer[IP_PROTO_P]=IP_PROTO_UDP_V;
		for (int i=0;i<4;i++) ether.buffer[IP_DST_P+i] = 0x0FF;
	
		xPL_BufferFiller buffer(ether.buffer + UDP_DATA_P);

		buffer.print(msg);
	
		ether.udpTransmit(buffer.position());

		DBG(F("sent:"),buffer.position());

		buffer.unlink();

		return true;
	}
	return false;
}


#ifdef XPL_HTML_SUPPORT

bool xPL_AdapterEtherCard::sendHomePage() {

  xPL_BufferFiller buf = ether.tcpOffset();

  buf.print(S(html_header));

   buf.printTagContent_P(S(title),S(xPL));
    buf.printTag(S(h1));
    S(xPL).printlnTo(buf,':');
	source().printlnTo(buf);
	buf.printTagClose(S(h1));
//    "<p>..TODO: some state here..</p>"
  xPL_Schema* sch = (xPL_Schema*)child();
  while (sch)
  {
	  buf.printTagContent(S(li),sch->className());
	  sch=(xPL_Schema*)sch->next();
  }
  ether.httpServerReply(buf.position()); // send web page data
  
  return true;
}
#endif
 
 