/*
 * UInt.hpp
 *
 * SCHNAPS
 * Copyright (C) 2009-2011 by Audrey Durand
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SCHNAPS_Core_UInt_hpp
#define SCHNAPS_Core_UInt_hpp

#include "SCHNAPS/Core/Number.hpp"
#include "SCHNAPS/Core/ArrayT.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"

namespace SCHNAPS {
namespace Core {

// forward declaration
class Double;
class Float;
class Int;
class Long;
class ULong;

//! Array of the atomic unsigned integer type.
typedef ArrayT<unsigned int> UIntArray;

/*!
 *  \class UInt SCHNAPS/Core/UInt.hpp "SCHNAPS/Core/UInt.hpp"
 *  \brief The unsigned integer implementation as Object.
 */
class UInt: public Number {
public:
	//! UInt allocator type.
	typedef AllocatorT<UInt, Number::Alloc> Alloc;
	//! UInt handle type.
	typedef PointerT<UInt, Number::Handle> Handle;
	//! UInt bag type.
	typedef ContainerT<UInt, Number::Bag> Bag;

	UInt() {}
	UInt(unsigned int inValue);
	UInt(const UInt& inOriginal);
	virtual ~UInt() {}

	//! Copy operator.
	UInt& operator=(const UInt& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("UInt");
		return lName;
		schnaps_StackTraceEndM("const std::string& UInt::getName() const");
	}

	/*!
	 * \brief  Return a const reference to the type of object.
	 * \return A const reference to the type of object.
	 */
	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
		const static std::string lType("UInt");
		return lType;
		schnaps_StackTraceEndM("const std::string& UInt::getType() const");
	}

	//! Write the content of the object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Test if an object is equal to another.
	virtual bool isEqual(const Object& inRightObj) const;
	//! Test if an object is less than another.
	virtual bool isLess(const Object& inRightObj) const;

	//! Read the object from string.
	virtual void readStr(const std::string& inStr);
	//! Write the object to string.
	virtual std::string writeStr() const;

	//! Return a handle to a clone of the current object.
	virtual AnyType::Handle clone() const;

	//! Compute absolute value.
	virtual Number& abs();
	//! Add a number.
	virtual Number& add(Number& inRightNumber);
	//! Divide by a number.
	virtual Number& div(Number& inRightNumber);
	//! Compute the modulo by a number.
	virtual Number& mod(Number& inRightNumber);
	//! Multiply by a number.
	virtual Number& mult(Number& inRightNumber);
	//! Subtract by a number.
	virtual Number& sub(Number& inRightNumber);

	//! Casting operator to double.
	operator Double() const;
	//! Casting operator to float.
	operator Float() const;
	//! Casting operator to integer.
	operator Int() const;
	//! Casting operator to long.
	operator Long() const;
	//! Casting operator to unsigned integer.
	operator UInt() const;
	//! Casting operator to unsigned long.
	operator ULong() const;

	/*!
	 * \brief Return a const refernce to the value of unsigned integer.
	 * \return A const reference to the value of unsigned integer.
	 */
	const unsigned int& getValue() const {
		return mValue;
	}

	/*!
	 * \brief Set the value of unsigned integer.
	 * \param inValue A const reference to the new value of unsigned integer.
	 */
	void setValue(const unsigned int& inValue) {
		mValue = inValue;
	}


private:
	unsigned int mValue; //! Value of unsigned integer.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_UInt_hpp