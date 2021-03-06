/*
 * PortHandler.h
 *
 *  Created on: 2016. 1. 26.
 *      Author: zerom, leon
 */

#ifndef DYNAMIXEL_SDK_INCLUDE_DYNAMIXEL_SDK_PORTHANDLER_H_
#define DYNAMIXEL_SDK_INCLUDE_DYNAMIXEL_SDK_PORTHANDLER_H_

#ifdef __linux__
#define WINDECLSPEC
#elif defined(_WIN32) || defined(_WIN64)
#ifdef WINDLLEXPORT
#define WINDECLSPEC __declspec(dllexport)
#else
#define WINDECLSPEC __declspec(dllimport)
#endif
#endif

#include "RobotisDef.h"

namespace ROBOTIS
{

class WINDECLSPEC PortHandler
{
public:
    static const int DEFAULT_BAUDRATE = 1000000;

    static PortHandler *GetPortHandler(const char *port_name);

    bool    is_using;

    virtual ~PortHandler() { }

    virtual bool    OpenPort() = 0;
    virtual void    ClosePort() = 0;
    virtual void    ClearPort() = 0;

    virtual void    SetPortName(const char* port_name) = 0;
    virtual char   *GetPortName() = 0;

    virtual bool    SetBaudRate(const int baudrate) = 0;
    virtual int     GetBaudRate() = 0;

    virtual int     GetBytesAvailable() = 0;

    virtual int     ReadPort(UINT8_T *packet, int length) = 0;
    virtual int     WritePort(UINT8_T *packet, int length) = 0;

    virtual void    SetPacketTimeout(UINT16_T packet_length) = 0;
    virtual void    SetPacketTimeout(double msec) = 0;
    virtual bool    IsPacketTimeout() = 0;
};

}


#endif /* DYNAMIXEL_SDK_INCLUDE_DYNAMIXEL_SDK_PORTHANDLER_H_ */
