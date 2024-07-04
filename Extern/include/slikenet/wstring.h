/*
 *  Original work: Copyright (c) 2014, Oculus VR, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  RakNet License.txt file in the licenses directory of this source tree. An additional grant
 *  of patent rights can be found in the RakNet Patents.txt file in the same directory.
 *
 *
 *  Modified work: Copyright (c) 2016-2018, SLikeSoft UG (haftungsbeschränkt)
 *
 *  This source code was modified by SLikeSoft. Modifications are licensed under the MIT-style
 *  license found in the license.txt file in the root directory of this source tree.
 */

#ifndef __RAK_W_STRING_H
#define __RAK_W_STRING_H 

#include "Export.h"
#include "types.h" // int64_t
#include "string.h"

#ifdef _WIN32
#include "WindowsIncludes.h"
#endif

namespace SLNet
{
/// \brief String class for Unicode
class RAK_DLL_EXPORT RakWString
{
public:
	// Constructors
	constexpr RakWString() = default;
	RakWString(const RakString& right)
	{
		*this = right;
	}
	RakWString(const wchar_t *input)
	{
		*this = input;
	}
	RakWString(const RakWString& right)
	{
		*this = right;
	}
	RakWString(RakWString&& right) noexcept;
	RakWString(const char *input)
	{
		*this = input;
	}
	~RakWString();

#if USE_IMPLICIT_CONVERSIONS
	/// Implicit conversion to const wchar_t*
	operator const wchar_t*() const
	{
		return data;
	}
#endif

	/// Same as std::string::c_str()
	const wchar_t *C_String() const
	{
		return data;
	}

	/// Assignment operators
	RakWString& operator=(const RakWString& right);
	RakWString& operator=(RakWString&& right) noexcept;
	RakWString& operator=(const RakString& right);
	RakWString& operator=(const wchar_t *const str);
	RakWString& operator=(const char *const str);

	/// Concatenation
	RakWString& operator+=(const RakWString& right);
	RakWString& operator+=(const wchar_t *const right);

	/// Equality
	bool operator==(const RakWString &right) const;

	// Comparison
	bool operator<(const RakWString& right) const;
	bool operator<=(const RakWString& right) const;
	bool operator>(const RakWString& right) const;
	bool operator>=(const RakWString& right) const;

	/// Inequality
	bool operator!=(const RakWString& right) const;

	/// Set the value of the string
	void Set(const wchar_t *str)
	{
		*this = str;
	}

	/// Returns if the string is empty. Also, C_String() would return ""
	bool IsEmpty() const
	{
		return length == 0;
	}

	/// Returns the length of the string
	size_t GetLength() const
	{
		return length;
	}

	/// Has the string into an unsigned int
	static unsigned long ToInteger(const RakWString& rs);

	/// Compare strings (case sensitive)
	int StrCmp(const RakWString& right) const;

	/// Compare strings (not case sensitive)
	int StrICmp(const RakWString& right) const;

	/// Clear the string
	void Clear();

	/// Print the string to the screen
	void Printf();

	/// Print the string to a file
	void FPrintf(FILE *fp);

	/// Serialize to a bitstream, uncompressed (slightly faster)
	/// \param[out] bs Bitstream to serialize to
	void Serialize(BitStream *bs) const;

	/// Static version of the Serialize function
	static void Serialize(const wchar_t *const str, BitStream *bs);

	/// Deserialize what was written by Serialize
	/// \param[in] bs Bitstream to serialize from
	/// \return true if the deserialization was successful
	bool Deserialize(BitStream *bs);

	/// Static version of the Deserialize() function
	static bool Deserialize(wchar_t *str, BitStream *bs);
	static bool Deserialize(wchar_t *str, size_t strLength, BitStream *bs);


protected:
	wchar_t *data = nullptr;
	size_t length = 0;
};
}

const SLNet::RakWString RAK_DLL_EXPORT operator+(const SLNet::RakWString& lhs, const SLNet::RakWString& rhs);

#endif
