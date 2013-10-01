//
// SocketChannel.cpp
//
// $Id: //poco/Main/DeviceIO/Socket/src/SocketChannel.cpp#1 $
//
// Library: Socket
// Package: Socket
// Module:  SocketChannel
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
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


#include "Poco/DeviceIO/Socket/SocketChannel.h"
#include "Poco/Exception.h"
#include "Poco/Net/SocketImpl.h"


using Poco::InvalidArgumentException;
using Poco::Net::SocketImpl;
using Poco::Net::SocketAddress;


namespace Poco {
namespace DeviceIO {
namespace Socket {


SocketChannel::SocketChannel(ChannelConfig* pConfig): 
	Poco::DeviceIO::Channel(pConfig),
	_pSocket(0)
{
	init();
}


SocketChannel::~SocketChannel()
{
	delete _pSocket;
}


void SocketChannel::init()
{
	delete _pSocket;

	if (isStream())
		_pSocket = new Poco::Net::StreamSocket();
	else if (isDatagram())
		_pSocket = new Poco::Net::DatagramSocket();
	else 
		throw IllegalStateException();

	poco_check_ptr (_pSocket);

	open();
	if (!_pSocket) throw NullPointerException();
}


void SocketChannel::open()
{
	Poco::Timespan timeout(config().getTimeout() * Timespan::MILLISECONDS);
	SocketAddress address(config().getName());
	try
	{
		if (isStream() && timeout != ChannelConfig::INFINITE_TIMEOUT)
			socketImpl()->connect(address, timeout);
		else
			socketImpl()->connect(address);

		socketImpl()->setBlocking(false);
	}
	catch (Exception&) 
	{ 
		close(); 
	}
}


int SocketChannel::readData(char*& pReadBuf)
{
	int bufSize = socketImpl()->getReceiveBufferSize();
	char* pBuffer = new char[bufSize];
	int size = 0;

	try
	{
		int read = 0;
		do
		{
			read = readData(pBuffer, bufSize);

			if (read > 0) 
			{
				pReadBuf = static_cast<char*>(std::realloc(pReadBuf, size + read));
				std::memcpy(pReadBuf + size, pBuffer, read);
				size += read;
			}
			else break;
		} while (true);
	}
	catch (TimeoutException&) 
	{
	}
	catch (Exception&)
	{
		delete [] pBuffer;
		throw;
	}
	
	delete [] pBuffer;
	return size;
}


int SocketChannel::readData(char* pBuffer, int length)
{
	int received = 0;

	try
	{
		if (!socketImpl()->poll(config().getTimeout() * Timespan::MILLISECONDS, SocketImpl::SELECT_READ)) 
			throw TimeoutException("read timed out", socketImpl()->address().toString());
		received = socketImpl()->receiveBytes(pBuffer, (int) length);
	}
	catch (Poco::Exception&)
	{
		throw;
	}

	return received;
}


int SocketChannel::writeData(const char* pBuffer, int length)
{
	int sent = 0;

	try
	{
		if (!socketImpl()->poll(config().getTimeout() * Timespan::MILLISECONDS, SocketImpl::SELECT_WRITE)) 
			throw Poco::TimeoutException("write timed out", socketImpl()->address().toString());
		sent = socketImpl()->sendBytes(pBuffer, (int) length);
	}
	catch (Poco::Exception&)
	{
		throw;
	}
	
	return sent;
}


} } } // namespace Poco::DeviceIO::Socket
