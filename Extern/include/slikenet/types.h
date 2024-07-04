/*
 *  Original work: Copyright (c) 2014, Oculus VR, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  RakNet License.txt file in the licenses directory of this source tree. An additional grant
 *  of patent rights can be found in the RakNet Patents.txt file in the same directory.
 *
 *
 *  Modified work: Copyright (c) 2016-2019, SLikeSoft UG (haftungsbeschränkt)
 *
 *  This source code was modified by SLikeSoft. Modifications are licensed under the MIT-style
 *  license found in the license.txt file in the root directory of this source tree.
 */

 /// \file
 /// \brief Types used by RakNet, most of which involve user code.
 ///
#ifndef __NETWORK_TYPES_H
#define __NETWORK_TYPES_H

#include <slikenet/NativeTypes.h>
#include "defines.h"
#include "time.h"
#include "Export.h"
#include "WindowsIncludes.h"
#include "SocketIncludes.h"

namespace SLNet
{
/// Forward declarations
class RakPeerInterface;
class BitStream;
struct Packet;

enum StartupResult
{
	RAKNET_STARTED,
	RAKNET_ALREADY_STARTED,
	INVALID_SOCKET_DESCRIPTORS,
	INVALID_MAX_CONNECTIONS,
	SOCKET_FAMILY_NOT_SUPPORTED,
	SOCKET_PORT_ALREADY_IN_USE,
	SOCKET_FAILED_TO_BIND,
	SOCKET_FAILED_TEST_SEND,
	PORT_CANNOT_BE_ZERO,
	FAILED_TO_CREATE_NETWORK_THREAD,
	COULD_NOT_GENERATE_GUID,
	STARTUP_OTHER_FAILURE
};


enum ConnectionAttemptResult
{
	CONNECTION_ATTEMPT_STARTED,
	INVALID_PARAMETER,
	CANNOT_RESOLVE_DOMAIN_NAME,
	ALREADY_CONNECTED_TO_ENDPOINT,
	CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS,
	SECURITY_INITIALIZATION_FAILED
};

/// Returned from RakPeerInterface::GetConnectionState()
enum ConnectionState
{
	/// Connect() was called, but the process hasn't started yet
	IS_PENDING,
	/// Processing the connection attempt
	IS_CONNECTING,
	/// Is connected and able to communicate
	IS_CONNECTED,
	/// Was connected, but will disconnect as soon as the remaining messages are delivered
	IS_DISCONNECTING,
	/// A connection attempt failed and will be aborted
	IS_SILENTLY_DISCONNECTING,
	/// No longer connected
	IS_DISCONNECTED,
	/// Was never connected, or else was disconnected long enough ago that the entry has been discarded
	IS_NOT_CONNECTED
};

/// Given a number of bits, return how many bytes are needed to represent that.
#define BITS_TO_BYTES(x) (((x) + 7) >> 3)
#define BYTES_TO_BITS(x) ((x) << 3)

/// \sa NetworkIDObject.h
typedef unsigned char UniqueIDType;
typedef unsigned short SystemIndex;
typedef unsigned char RPCIndex;
const int MAX_RPC_MAP_SIZE = ((RPCIndex)-1) - 1;
const int UNDEFINED_RPC_INDEX = ((RPCIndex)-1);

/// First byte of a network message
typedef unsigned char MessageID;

typedef uint32_t BitSize_t;

/// Used with the PublicKey structure
enum PublicKeyMode
{
	/// The connection is insecure. You can also just pass 0 for the pointer to PublicKey in RakPeerInterface::Connect()
	PKM_INSECURE_CONNECTION,

	/// Accept whatever public key the server gives us. This is vulnerable to man in the middle, but does not require
	/// distribution of the public key in advance of connecting.
	PKM_ACCEPT_ANY_PUBLIC_KEY,

	/// Use a known remote server public key. PublicKey::remoteServerPublicKey must be non-zero.
	/// This is the recommended mode for secure connections.
	PKM_USE_KNOWN_PUBLIC_KEY,

	/// Use a known remote server public key AND provide a public key for the connecting client.
	/// PublicKey::remoteServerPublicKey, myPublicKey and myPrivateKey must be all be non-zero.
	/// The server must cooperate for this mode to work.
	/// I recommend not using this mode except for server-to-server communication as it significantly increases the CPU requirements during connections for both sides.
	/// Furthermore, when it is used, a connection password should be used as well to avoid DoS attacks.
	PKM_USE_TWO_WAY_AUTHENTICATION
};

/// Passed to RakPeerInterface::Connect()
struct RAK_DLL_EXPORT PublicKey
{
	/// How to interpret the public key, see above
	PublicKeyMode publicKeyMode;

	/// Pointer to a public key of length cat::EasyHandshake::PUBLIC_KEY_BYTES. See the Encryption sample.
	char *remoteServerPublicKey;

	/// (Optional) Pointer to a public key of length cat::EasyHandshake::PUBLIC_KEY_BYTES
	char *myPublicKey;

	/// (Optional) Pointer to a private key of length cat::EasyHandshake::PRIVATE_KEY_BYTES
	char *myPrivateKey;
};

/// Describes the local socket to use for RakPeer::Startup
struct RAK_DLL_EXPORT SocketDescriptor
{
	constexpr SocketDescriptor()
#ifdef __native_client__
		: blockingSocket(false)
#endif
	{
	}
	SocketDescriptor(unsigned short port, const char *_hostAddress = nullptr);

	/// The local port to bind to.  Pass 0 to have the OS autoassign a port.
	unsigned short port = 0;

	/// The local network card address to bind to, such as "127.0.0.1".  Pass an empty string to use INADDR_ANY.
	char hostAddress[INET6_ADDRSTRLEN] = {0};

	/// IP version: For IPv4, use AF_INET (default). For IPv6, use AF_INET6. For dual stack, use AF_UNSPEC and set \a hostAddress to an empty string.
	/// IPv6 is the newer internet protocol. Instead of addresses such as 192.0.2.1, you may have an address such as 2001:db8:63b3:1::3490.
	/// Encoding takes 16 bytes instead of 4, so IPv6 is less efficient for bandwidth.
	/// On the positive side, NAT Punchthrough is not needed and should not be used with IPv6 because there are enough addresses that routers do not need to create address mappings.
	/// RakPeer::Startup() will fail if this IP version is not supported.
	/// \pre RAKNET_SUPPORT_IPV6 must be set to 1 in RakNetDefines.h for AF_INET6
	short socketFamily = AF_INET;

	unsigned short remotePortRakNetWasStartedOn_PS3_PSP2 = 0;

	// Required for Google chrome
	_PP_Instance_ chromeInstance = {};

	// Set to true to use a blocking socket (default, do not change unless you have a reason to)
	bool blockingSocket = true;

	/// XBOX only: set IPPROTO_VDP if you want to use VDP. If enabled, this socket does not support broadcast to 255.255.255.255
	unsigned int extraSocketOptions = 0;
};

/// \brief Network address for a system
/// \details Corresponds to a network address<BR>
/// This is not necessarily a unique identifier. For example, if a system has both LAN and internet connections, the system may be identified by either one, depending on who is communicating<BR>
/// Therefore, you should not transmit the SystemAddress over the network and expect it to identify a system, or use it to connect to that system, except in the case where that system is not behind a NAT (such as with a dedciated server)
/// Use RakNetGUID for a unique per-instance of RakPeer to identify systems
struct RAK_DLL_EXPORT SystemAddress
{
	/// Constructors
	constexpr SystemAddress()
	{
		address.addr.sa_family = AF_INET;
	}
	constexpr SystemAddress(const SystemAddress& other)
		: address(other.address),
		systemIndex(other.systemIndex)
	{
	}
	SystemAddress(const char *str, char delim = '|')
	{
		address.addr.sa_family = AF_INET;
		FromString(str, delim);
	}
	SystemAddress(const char *str, unsigned short port)
	{
		address.addr.sa_family = AF_INET;
		FromStringExplicitPort(str, port);
	}

	/// \internal Return the size of an IPv4 address to write to a bitStream
	static constexpr size_t size4()
	{
		return sizeof(unsigned char) + sizeof(in_addr) + sizeof(unsigned short);
	}
	/// \internal Return the size of an IPv6 address to write to a bitStream
	static constexpr size_t size6()
	{
		return sizeof(unsigned char) + sizeof(sockaddr_in6);
	}

	/// Hash the system address
	static unsigned long ToInteger(const SystemAddress &sa);

	/// Return the IP version, either IPV4 or IPV6
	/// \return Either 4 or 6
	unsigned char GetIPVersion(void) const
	{
		return (address.addr.sa_family == AF_INET) ? 4 : 6;
	}

	/// \internal Returns either IPPROTO_IP or IPPROTO_IPV6
	/// \sa GetIPVersion
	unsigned int GetIPPROTO() const
	{
		return (GetIPVersion() == 4) ? IPPROTO_IP : IPPROTO_IPV6;
	}

	/// Call SetToLoopback(), with whatever IP version is currently held. Defaults to IPV4
	void SetToLoopback()
	{
		SetToLoopback(GetIPVersion());
	}

	/// Call SetToLoopback() with a specific IP version
	/// \param[in] ipVersion Either 4 for IPV4 or 6 for IPV6
	void SetToLoopback(unsigned char ipVersion);

	/// \return If was set to 127.0.0.1 or ::1
	bool IsLoopback(void) const;

	// Return the systemAddress as a string in the format <IP>|<Port>
	// Returns a static string
	// NOT THREADSAFE
	// portDelineator should not be '.', ':', '%', '-', '/', a number, or a-f
	const char *ToString(bool writePort = true, char portDelineator = '|') const;

	// Return the systemAddress as a string in the format <IP>|<Port>
	// dest must be large enough to hold the output (INET_ADDRSTRLEN + 6 for IPv4 and INET6_ADDRSTRLEN + 6 for IPv6)
	// portDelineator should not be '.', ':', '%', '-', '/', a number, or a-f
	// THREADSAFE
	void ToString(bool writePort, char *dest, char portDelineator = '|') const;
	void ToString(bool writePort, char *dest, size_t destLength, char portDelineator = '|') const;

	/// Set the system address from a printable IP string, for example "192.0.2.1" or "2001:db8:63b3:1::3490"
	/// You can write the port as well, using the portDelineator, for example "192.0.2.1|1234"
	/// IPv6 addresses can also be specified like in a URL, for example "[::1]:80"
	/// \param[in] str A printable IP string, for example "192.0.2.1" or "2001:db8:63b3:1::3490". Pass nullptr for \a str to set to UNASSIGNED_SYSTEM_ADDRESS
	/// \param[in] portDelineator if \a str contains a port, delineate the port with this character. portDelineator should not be '.', '%', '-', '/', a number, or a-f
	/// \param[in] ipVersion if you pass a domain name, it will be resolved to an IP address of this version. Otherwise this parameter should be 0
	/// \note The current port is unchanged if a port is not specified in \a str
	/// \return True on success, false if ipVersion does not match type of passed string
	bool FromString(const char *str, char portDelineator = '|', int ipVersion = 0);

	/// Same as FromString(), but you explicitly set a port at the same time
	bool FromStringExplicitPort(const char *str, unsigned short port, int ipVersion = 0);

	/// Copy the port from another SystemAddress structure
	void CopyPort(const SystemAddress& right);

	/// Returns if two system addresses have the same IP (port is not checked)
	bool EqualsExcludingPort(const SystemAddress& right) const;

	/// Returns the port in host order (this is what you normally use)
	unsigned short GetPort() const;

	/// \internal Returns the port in network order
	unsigned short GetPortNetworkOrder() const
	{
		return address.addr4.sin_port;
	}

	/// Sets the port. The port value should be in host order (this is what you normally use)
	/// Renamed from SetPort because of winspool.h http://edn.embarcadero.com/article/21494
	void SetPortHostOrder(unsigned short s);

	/// \internal Sets the port. The port value should already be in network order.
	void SetPortNetworkOrder(unsigned short s);

	/// \internal sockaddr_in6 requires extra data beyond just the IP and port. Copy that extra data from an existing SystemAddress that already has it
	void FixForIPVersion(const SystemAddress &boundAddressToSocket);

	bool IsLANAddress();

	SystemAddress& operator=(const SystemAddress& input);
	bool operator==(const SystemAddress& right) const;
	bool operator!=(const SystemAddress& right) const;
	bool operator>(const SystemAddress& right) const;
	bool operator<(const SystemAddress& right) const;


	/// SystemAddress holds both an sockaddr_in6 and a sockaddr_in
	union In6OrIn4
	{
		sockaddr addr;
		sockaddr_in6 addr6;
		sockaddr_in addr4;
	} address = {};
	/// \internal Used internally for fast lookup. Optional (use -1 to do regular lookup). Don't transmit this.
	SystemIndex systemIndex = (SystemIndex)-1;
};

/// Uniquely identifies an instance of RakPeer. Use RakPeer::GetGuidFromSystemAddress() and RakPeer::GetSystemAddressFromGuid() to go between SystemAddress and RakNetGUID
/// Use RakPeer::GetGuidFromSystemAddress(UNASSIGNED_SYSTEM_ADDRESS) to get your own GUID
struct RAK_DLL_EXPORT RakNetGUID
{
	constexpr RakNetGUID() = default;
	explicit constexpr RakNetGUID(uint64_t _g)
		: g(_g)
	{
	}
	constexpr RakNetGUID(const RakNetGUID&) = default;
	constexpr RakNetGUID& operator=(const RakNetGUID& input) = default;

	// Return the GUID as a string
	// Returns a static string
	// NOT THREADSAFE
	const char *ToString(void) const;

	// Return the GUID as a string
	// dest must be large enough to hold the output (>= 23)
	// THREADSAFE
	void ToString(char *dest) const;
	void ToString(char *dest, size_t destSize) const;

	bool FromString(const char *source);

	static unsigned long ToUint32(const RakNetGUID &g);

	static constexpr size_t size()
	{
		return sizeof(uint64_t);
	}

	bool operator==(const RakNetGUID& rhs) const
	{
		return g == rhs.g;
	}
	bool operator!=(const RakNetGUID& rhs) const
	{
		return g != rhs.g;
	}
	bool operator>(const RakNetGUID& rhs) const
	{
		return g > rhs.g;
	}
	bool operator<(const RakNetGUID& rhs) const
	{
		return g < rhs.g;
	}


	//	uint32_t g[6];
	uint64_t g = (uint64_t)-1;
	// Used internally for fast lookup. Optional (use -1 to do regular lookup). Don't transmit this.
	SystemIndex systemIndex = (SystemIndex)-1;
};

/// Index of an invalid SystemAddress
//const SystemAddress UNASSIGNED_SYSTEM_ADDRESS =
//{
//	0xFFFFFFFF, 0xFFFF
//};
#ifndef SWIG
inline constexpr SystemAddress UNASSIGNED_SYSTEM_ADDRESS{};
inline constexpr RakNetGUID UNASSIGNED_RAKNET_GUID{};
#endif
//{
//	{0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF}
//	0xFFFFFFFFFFFFFFFF
//};


struct RAK_DLL_EXPORT AddressOrGUID
{
	RakNetGUID rakNetGuid;
	SystemAddress systemAddress;

	constexpr AddressOrGUID() = default;
	constexpr AddressOrGUID(const AddressOrGUID& input)
		: rakNetGuid(input.rakNetGuid),
		systemAddress(input.systemAddress)
	{
	}
	constexpr AddressOrGUID(const SystemAddress& input)
		: systemAddress(input)
	{
	}
	// must be a valid pointer!
	AddressOrGUID(const Packet *packet);
	constexpr AddressOrGUID(const RakNetGUID& input)
		: rakNetGuid(input)
	{
	}
	AddressOrGUID& operator=(const AddressOrGUID& input)
	{
		rakNetGuid = input.rakNetGuid;
		systemAddress = input.systemAddress;
		return *this;
	}

	AddressOrGUID& operator=(const SystemAddress& input)
	{
		rakNetGuid = UNASSIGNED_RAKNET_GUID;
		systemAddress = input;
		return *this;
	}

	AddressOrGUID& operator=(const RakNetGUID& input)
	{
		rakNetGuid = input;
		systemAddress = UNASSIGNED_SYSTEM_ADDRESS;
		return *this;
	}

	inline bool operator==(const AddressOrGUID& right) const
	{
		return (rakNetGuid != UNASSIGNED_RAKNET_GUID && rakNetGuid == right.rakNetGuid) || (systemAddress != UNASSIGNED_SYSTEM_ADDRESS && systemAddress == right.systemAddress);
	}

	SystemIndex GetSystemIndex() const
	{
		if(rakNetGuid != UNASSIGNED_RAKNET_GUID)
			return rakNetGuid.systemIndex;
		return systemAddress.systemIndex;
	}
	bool IsUndefined() const
	{
		return rakNetGuid == UNASSIGNED_RAKNET_GUID && systemAddress == UNASSIGNED_SYSTEM_ADDRESS;
	}
	void SetUndefined()
	{
		rakNetGuid = UNASSIGNED_RAKNET_GUID; systemAddress = UNASSIGNED_SYSTEM_ADDRESS;
	}
	static unsigned long ToInteger(const AddressOrGUID &aog);
	const char *ToString(bool writePort = true) const;
	void ToString(bool writePort, char *dest) const;
	void ToString(bool writePort, char *dest, size_t destLength) const;
};

typedef uint64_t NetworkID;

/// This represents a user message from another system.
struct Packet
{
	/// The system that send this packet.
	SystemAddress systemAddress;

	/// A unique identifier for the system that sent this packet, regardless of IP address (internal / external / remote system)
	/// Only valid once a connection has been established (ID_CONNECTION_REQUEST_ACCEPTED, or ID_NEW_INCOMING_CONNECTION)
	/// Until that time, will be UNASSIGNED_RAKNET_GUID
	RakNetGUID guid;

	/// The length of the data in bytes
	unsigned int length;

	/// The length of the data in bits
	BitSize_t bitSize;

	/// The data from the sender
	unsigned char *data;

	/// @internal
	/// Indicates whether to delete the data, or to simply delete the packet.
	bool deleteData;

	/// @internal
	/// If true, this message is meant for the user, not for the plugins, so do not process it through plugins
	bool wasGeneratedLocally;
};

///  Index of an unassigned player
inline constexpr SystemIndex UNASSIGNED_PLAYER_INDEX = (SystemIndex)-1;

/// Unassigned object ID
inline constexpr NetworkID UNASSIGNED_NETWORK_ID = (NetworkID)-1;

inline constexpr int PING_TIMES_ARRAY_SIZE = 5;

struct RAK_DLL_EXPORT uint24_t
{
	uint32_t val = 0;

	constexpr uint24_t() = default;
	constexpr uint24_t(const uint24_t& other)
		: val(other.val)
	{
	}
	inline operator uint32_t()
	{
		return val;
	}
	inline operator uint32_t() const
	{
		return val;
	}
	inline uint24_t operator++()
	{
		++val; val &= 0x00FFFFFF; return *this;
	}
	inline uint24_t operator--()
	{
		--val; val &= 0x00FFFFFF; return *this;
	}
	inline uint24_t operator++(int)
	{
		uint24_t temp(val); ++val; val &= 0x00FFFFFF; return temp;
	}
	inline uint24_t operator--(int)
	{
		uint24_t temp(val); --val; val &= 0x00FFFFFF; return temp;
	}
	inline uint24_t operator&(const uint24_t& a)
	{
		return uint24_t(val & a.val);
	}
	inline uint24_t& operator=(const uint24_t& a)
	{
		val = a.val; return *this;
	}
	inline uint24_t& operator+=(const uint24_t& a)
	{
		val += a.val; val &= 0x00FFFFFF; return *this;
	}
	inline uint24_t& operator-=(const uint24_t& a)
	{
		val -= a.val; val &= 0x00FFFFFF; return *this;
	}
	inline bool operator==(const uint24_t& right) const
	{
		return val == right.val;
	}
	inline bool operator!=(const uint24_t& right) const
	{
		return val != right.val;
	}
	inline bool operator > (const uint24_t& right) const
	{
		return val > right.val;
	}
	inline bool operator < (const uint24_t& right) const
	{
		return val < right.val;
	}
	inline const uint24_t operator+(const uint24_t &other) const
	{
		return uint24_t(val + other.val);
	}
	inline const uint24_t operator-(const uint24_t &other) const
	{
		return uint24_t(val - other.val);
	}
	inline const uint24_t operator/(const uint24_t &other) const
	{
		return uint24_t(val / other.val);
	}
	inline const uint24_t operator*(const uint24_t &other) const
	{
		return uint24_t(val * other.val);
	}

	inline uint24_t(const uint32_t& a)
	{
		val = a; val &= 0x00FFFFFF;
	}
	inline uint24_t operator&(const uint32_t& a)
	{
		return uint24_t(val & a);
	}
	inline uint24_t& operator=(const uint32_t& a)
	{
		val = a; val &= 0x00FFFFFF; return *this;
	}
	inline uint24_t& operator+=(const uint32_t& a)
	{
		val += a; val &= 0x00FFFFFF; return *this;
	}
	inline uint24_t& operator-=(const uint32_t& a)
	{
		val -= a; val &= 0x00FFFFFF; return *this;
	}
	inline bool operator==(const uint32_t& right) const
	{
		return val == (right & 0x00FFFFFF);
	}
	inline bool operator!=(const uint32_t& right) const
	{
		return val != (right & 0x00FFFFFF);
	}
	inline bool operator > (const uint32_t& right) const
	{
		return val > (right & 0x00FFFFFF);
	}
	inline bool operator < (const uint32_t& right) const
	{
		return val < (right & 0x00FFFFFF);
	}
	inline const uint24_t operator+(const uint32_t &other) const
	{
		return uint24_t(val + other);
	}
	inline const uint24_t operator-(const uint32_t &other) const
	{
		return uint24_t(val - other);
	}
	inline const uint24_t operator/(const uint32_t &other) const
	{
		return uint24_t(val / other);
	}
	inline const uint24_t operator*(const uint32_t &other) const
	{
		return uint24_t(val * other);
	}
};

} // namespace SLNet

#endif
