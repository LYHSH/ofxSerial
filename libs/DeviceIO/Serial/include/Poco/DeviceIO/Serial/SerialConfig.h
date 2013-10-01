//
// SerialConfig.h
//
// $Id: //poco/Main/DeviceIO/include/Poco/DeviceIO/Serial/SerialConfig.h#1 $
//
// Library: Serial
// Package: Serial
// Module:  SerialConfig
//
// Definition of the SerialConfig class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Serial_SerialConfig_INCLUDED
#define Serial_SerialConfig_INCLUDED


#include "Poco/DeviceIO/Serial/Serial.h"
#include "Poco/DeviceIO/ChannelConfig.h"
#include <iostream>

#if defined(POCO_OS_FAMILY_WINDOWS)
	#include "Poco/DeviceIO/Serial/SerialConfig_WIN32.h"
#elif defined(POCO_OS_FAMILY_UNIX)
	#include "Poco/DeviceIO/Serial/SerialConfig_POSIX.h"
#endif


namespace Poco {
namespace DeviceIO {
namespace Serial {


class Serial_API SerialConfig: public ChannelConfig, public SerialConfigImpl
{
public:
	enum DataBits
	{
		DATA_BITS_FIVE = SerialConfigImpl::DATA_BITS_FIVE_IMPL,
		DATA_BITS_SIX = SerialConfigImpl::DATA_BITS_SIX_IMPL,
		DATA_BITS_SEVEN = SerialConfigImpl::DATA_BITS_SEVEN_IMPL,
		DATA_BITS_EIGHT = SerialConfigImpl::DATA_BITS_EIGHT_IMPL
	};

	enum Parity
	{
		//on Windows, PARITY_* conflicts with
		//system macros, hence PAR_*
		PAR_NONE = SerialConfigImpl::PARITY_NONE_IMPL,
		PAR_ODD = SerialConfigImpl::PARITY_ODD_IMPL,
		PAR_EVEN = SerialConfigImpl::PARITY_EVEN_IMPL,
		PAR_MARK = SerialConfigImpl::PARITY_MARK_IMPL,
		PAR_SPACE = SerialConfigImpl::PARITY_SPACE_IMPL
	};

	enum StartBits
	{
		START_ONE = SerialConfigImpl::START_ONE_IMPL,
		START_ONE5 = SerialConfigImpl::START_ONE5_IMPL,
		START_TWO = SerialConfigImpl::START_TWO_IMPL
	};

	enum StopBits
	{
		STOP_ONE = SerialConfigImpl::STOP_ONE_IMPL,
		STOP_ONE5 = SerialConfigImpl::STOP_ONE5_IMPL,
		STOP_TWO = SerialConfigImpl::STOP_TWO_IMPL
	};

	enum FlowControl
	{
		FLOW_CTRL_HARDWARE = SerialConfigImpl::FLOW_CTRL_HARDWARE_IMPL,
		FLOW_CTRL_SOFTWARE = SerialConfigImpl::FLOW_CTRL_SOFTWARE_IMPL
	};

	enum BPSRate
	{
		BPS_110 = SerialConfigImpl::BPS_110_IMPL,
		BPS_300 = SerialConfigImpl::BPS_300_IMPL,
		BPS_600 = SerialConfigImpl::BPS_600_IMPL,
		BPS_1200 = SerialConfigImpl::BPS_1200_IMPL,
		BPS_2400 = SerialConfigImpl::BPS_2400_IMPL,
		BPS_4800 = SerialConfigImpl::BPS_4800_IMPL,
		BPS_9600 = SerialConfigImpl::BPS_9600_IMPL,
		BPS_14400 = SerialConfigImpl::BPS_14400_IMPL,
		BPS_19200 = SerialConfigImpl::BPS_19200_IMPL,
		BPS_38400 = SerialConfigImpl::BPS_38400_IMPL,
		BPS_57600 = SerialConfigImpl::BPS_57600_IMPL,
		BPS_115200 = SerialConfigImpl::BPS_115200_IMPL,
		BPS_128000 = SerialConfigImpl::BPS_128000_IMPL,
		BPS_230400 = SerialConfigImpl::BPS_230400_IMPL,
		BPS_256000 = SerialConfigImpl::BPS_256000_IMPL,
		//BPS_460800 = SerialConfigImpl::BPS_460800_IMPL
	};

	SerialConfig(const std::string& name);

	SerialConfig(const std::string& name,
		BPSRate bpsRate,
		DataBits dataBits = DATA_BITS_EIGHT,
		char parity = 'N',
		StartBits startBits = START_ONE,
		StopBits stopBits = STOP_ONE,
		FlowControl flowControl = FLOW_CTRL_HARDWARE,
		unsigned char xOnChar = 0,
		unsigned char xOffChar = 0,
		bool useEOF = true,
		unsigned char eof = DEFAULT_EOF,
		int bufferSize = 1,
		int timeout = INFINITE_TIMEOUT);

	void setBPSRate(BPSRate bpsRate);
	void setDataBits(DataBits dataBits);
	void setParity(Parity parity);
	void setParityChar(char parityChar);
	void setStartBits(StartBits startBits);
	void setStopBits(StopBits stopBits);
	void setFlowControl(FlowControl flowControl,
		unsigned char xOnChar,
		unsigned char xOffChar);
	void setUseXonXoff(unsigned char xOnChar,
		unsigned char xOffChar);
	void setXonChar(unsigned char xOn);
	void setXoffChar(unsigned char xOff);
	void setUseEOF(bool useEOF);
	void setEOFChar(unsigned char eof);
	void setBufferSize(int size);

	void setTimeout(int timeoutMS);
	void setBlocking();
	void setNonblocking(int timeoutMS);

	BPSRate getBPSRate() const;
	DataBits getDataBits() const;
	Parity getParity() const;
	char getParityChar() const;
	StartBits getStartBits() const;
	StopBits getStopBits() const;
	FlowControl getFlowControl() const;
	bool getUseXonXoff() const;
	unsigned char getXonChar() const;
	unsigned char getXoffChar() const;
	bool getUseEOF() const;
	unsigned char getEOFChar() const;
	int getBufferSize() const;

	const std::string& name() const;

	static const unsigned char DEFAULT_XON;
	static const unsigned char DEFAULT_XOFF;
	static const unsigned char DEFAULT_EOF;
};


//
// inlines
//


inline void SerialConfig::setDataBits(SerialConfig::DataBits dataBits)
{
	setDataBitsImpl((SerialConfigImpl::DataBitsImpl) dataBits);
}


inline void SerialConfig::setParity(Parity parity)
{
	setParityImpl(static_cast<SerialConfigImpl::ParityImpl>(parity));
}


inline void SerialConfig::setParityChar(char parityChar)
{
	setParityCharImpl(parityChar);
}


inline void SerialConfig::setStartBits(SerialConfig::StartBits startBits)
{
	setStartBitsImpl((SerialConfigImpl::StartBitsImpl) startBits);
}


inline void SerialConfig::setStopBits(SerialConfig::StopBits stopBits)
{
	setStopBitsImpl((SerialConfigImpl::StopBitsImpl) stopBits);
}


inline void SerialConfig::setFlowControl(SerialConfig::FlowControl flowControl,
	unsigned char xOnChar,
	unsigned char xOffChar)
{
	setFlowControlImpl((SerialConfigImpl::FlowControlImpl) flowControl, 
		xOnChar, 
		xOffChar);
}


inline void SerialConfig::setUseXonXoff(unsigned char xOnChar,
		unsigned char xOffChar)
{
	setUseXonXoffImpl(xOnChar, xOffChar);
}


inline void SerialConfig::setUseEOF(bool useEOF)
{
	setUseEOFImpl(useEOF);
}


inline void SerialConfig::setEOFChar(unsigned char eof)
{
	setEOFCharImpl(eof);
}


inline void SerialConfig::setBufferSize(int size)
{
	setBufferSizeImpl(size);
}


inline void SerialConfig::setTimeout(int timeoutMS)
{
	Poco::DeviceIO::ChannelConfig::setTimeout(timeoutMS);
	setTimeoutImpl(getTimeout());
}


inline void SerialConfig::setBlocking()
{
	setBlockingImpl();
}


inline void SerialConfig::setNonblocking(int timeoutMS)
{
	setTimeoutImpl(timeoutMS);
}

inline SerialConfig::BPSRate SerialConfig::getBPSRate() const
{
	return (SerialConfig::BPSRate) getBPSRateImpl();
}


inline SerialConfig::DataBits SerialConfig::getDataBits() const
{
	return (SerialConfig::DataBits) getDataBitsImpl();
}


inline SerialConfig::Parity SerialConfig::getParity() const
{
	return static_cast<SerialConfig::Parity>(getParityImpl());
}


inline char SerialConfig::getParityChar() const
{
	return getParityCharImpl();
}

inline SerialConfig::StartBits SerialConfig::getStartBits() const
{
	return (SerialConfig::StartBits) getStartBitsImpl();
}


inline SerialConfig::StopBits SerialConfig::getStopBits() const
{
	return (SerialConfig::StopBits) getStopBitsImpl();
}


inline SerialConfig::FlowControl SerialConfig::getFlowControl() const
{
	return (SerialConfig::FlowControl) getFlowControlImpl();
}


inline bool SerialConfig::getUseXonXoff() const
{
	return getUseXonXoffImpl();
}


inline unsigned char SerialConfig::getXonChar() const
{
	return getXonCharImpl();
}


inline unsigned char SerialConfig::getXoffChar() const
{
	return getXoffCharImpl();
}


inline bool SerialConfig::getUseEOF() const
{
	return getUseEOFImpl();
}


inline unsigned char SerialConfig::getEOFChar() const
{
	return getEOFCharImpl();
}


inline int SerialConfig::getBufferSize() const
{
	return getBufferSizeImpl();
}


inline const std::string& SerialConfig::name() const
{
	return getName();
}


} } } // namespace Poco::DeviceIO::Serial


#endif // Serial_SerialConfig_INCLUDED
