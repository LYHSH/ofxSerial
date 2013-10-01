//
// Protocol.h
//
// $Id: //poco/1.2/DeviceIO/include/Poco/DeviceIO/Protocol.h#1 $
//
// Library: DeviceIO
// Package: Protocol
// Module:  Protocol
//
// Definition of the Protocol class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
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


#ifndef DeviceIO_Protocol_INCLUDED
#define DeviceIO_Protocol_INCLUDED


#include "Poco/DeviceIO/DeviceIO.h"
#include "Poco/DeviceIO/Channel.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include "Poco/SharedPtr.h"
#include "Poco/Mutex.h"
#include <vector>


namespace Poco {
namespace DeviceIO {


class DeviceIO_API Protocol: public Poco::RefCountedObject
	/// Protocol is the common base class for protocol classes.
	/// A protocol can be standalone or it can be part of a chain.
	/// When part of a chain, protocol can be either root protocol or just ordinary member of the chain.
	/// Root protocol is the lowest-level protocol in the chain owning the chain's one-and-only channel 
	/// and internal buffer storage. 
	/// Both root channel and buffer are used by all subsequent (higher-level) chain members.
	///
	/// A protocol is attached to another protocol by invoking the parent's add(Protocol*) method.
	/// An attached protocol can be detached from its parent through either its own detach() method
	/// or parents remove(name) method call. Detached protocols are automatically deleted.
	///
{
public:
	typedef std::vector<Protocol*> ProtocolVec;
	typedef std::string ProtocolData;

	Protocol(const std::string& name, Channel* pChannel);
		/// Creates the Protocol using the given channel.

	Protocol(const std::string& name/*, Protocol* pParent*/);
		/// Creates the Protocol and attaches it to the parent.
		
	virtual ~Protocol();
		/// Destroys the Protocol.

	virtual int establish();
		/// Establishes the protocol conection.
		/// Must be overridden to establish the protocol connection.

	void writeRaw(const char* buffer, std::size_t length);
		/// Fills the internal buffer with string exactly as supplied.

	void writeRaw(const std::string& buffer);
		/// Fills the internal buffer with string exactly as supplied.
		
	int write(const char* buffer, int length, bool send = false);
		/// Wraps the given buffer into protocol data. 
		/// If send is true, internal buffer is sent through the channel and cleared.
		/// Returns the number of bytes sent.

	int write(const std::string& buffer);
		/// Wraps the given buffer into protocol data and sends it through the channel.
		/// Returns the number of bytes sent.
	
	int read(char* pBuffer, int length);
		/// Reads the data from channel, unwraps it from protocol data and 
		/// stores it into the supplied buffer.

	const std::string& readRaw();
		/// Returns the internal buffer contents without unwrapping it.

	virtual void terminate();
		/// Terminates the protocol connection.
		/// Must be overridden to do the appropriate protocol termination.

	int send();
		/// Sends the data over the wire and clears the internal buffer.

	std::string& receive(std::string& buffer);
		/// Receives the data, places the unwrapped data into the supplied buffer
		/// and return the reference to the supplied buffer.

	void clear();
		/// Clears the internal buffer.

	void add(Protocol* pProtocol);
		/// Attaches a protocol to this protocol. Root protocol takes ownership of it.

	void remove(const std::string& name);
		/// Removes the protocol with specified name from the chain.

	void detach();
		/// Detaches this protocol from its parent and turns it into a standalone root protocol. 
		/// The detached protocol keeps the previous parent's channel.
		/// Does nothing if protocol is already a root protocol.

	const std::string& name() const;
		/// Returns this protocol name.

	void setChannel(Channel* pChannel);
		/// Sets the channel for this protocol.

	bool isEstablished() const;
		/// Returns the boolean value indicating whether the
		/// connection is established or not.

protected:

	Protocol& root();
		/// Returns the root protocol.

	ProtocolVec& protocols();
		/// Returns the reference to the protocols vector.

	Channel& channel();
		/// Returns the reference to the underlying communication channel.

	std::string& buffer();
		/// Returns the reference to the internal buffer.

	std::string& data();
		/// Returns the reference to bare data.

	bool isRoot();
		/// Returns true if this protocol is the root of the hierarchy.

	void setEstablished(bool established);
		/// Sets the established state flag for all chained protocols.

	virtual std::string& wrap() = 0;
	virtual std::string& unwrap() = 0;

private:
	Protocol();
	Protocol(const Protocol& other);
	Protocol& operator = (const Protocol& other);

	void makeRoot();

	void detachImpl(bool destroy = true);
		/// Detaches the protocol from its parent and, if destroy is true,
		/// deletes it. 
		/// Does nothing if protocol is root.

	std::string   _name;
	Channel*      _pChannel;
	ProtocolVec*  _pProtocols;
	ProtocolData* _pBuffer;
	Protocol*     _pParent;
	bool          _established;
	Poco::Mutex   _mutex;
};


///
/// inlines
///


inline std::string& Protocol::buffer()
{
	if (_pBuffer) return *_pBuffer;
	else if (_pParent) return _pParent->buffer();
	else throw NullPointerException();
}


inline void Protocol::writeRaw(const char* buf, std::size_t length)
{
	buffer().assign(buf, length);
}


inline void Protocol::writeRaw(const std::string& buf)
{
	buffer().assign(buf.begin(), buf.end());
}


inline int Protocol::write(const std::string& buffer)
{
	return write(buffer.c_str(), static_cast<int>(buffer.size()), true);
}


inline const std::string& Protocol::readRaw()
{
	return buffer();
}


inline Protocol& Protocol::root()
{
	if (!_pParent) return *this;
	else return _pParent->root();
}


inline Protocol::ProtocolVec& Protocol::protocols()
{
	if (_pProtocols) return *_pProtocols;
	else if (_pParent) return _pParent->protocols();
	
	throw NullPointerException();
}


inline Channel& Protocol::channel()
{
	if (_pChannel) return *_pChannel;
	else if (_pParent) return _pParent->channel();
	
	throw NullPointerException();
}


inline void Protocol::clear()
{
	buffer().clear();
}


inline bool Protocol::isRoot()
{
	return 0 == _pParent;
}


inline const std::string& Protocol::name() const
{
	return _name;
}


inline void Protocol::detach()
{
	detachImpl(true);
}


inline bool Protocol::isEstablished() const
{
	return _established;
}


} } // namespace Poco::DeviceIO


#endif // DeviceIO_Protocol_INCLUDED
