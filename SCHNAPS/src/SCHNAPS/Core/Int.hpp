/*
 * Int.hpp
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

#ifndef SCHNAPS_Core_Int_hpp
#define SCHNAPS_Core_Int_hpp

#include "SCHNAPS/Core/Number.hpp"
#include "SCHNAPS/Core/ArrayT.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"

namespace SCHNAPS {
namespace Core {

// forward declarations
class Double;
class Float;
class Long;
class UInt;
class ULong;

//! Array of the atomic integer type.
typedef ArrayT<int> IntArray;

/*!
 *  \class Int SCHNAPS/Core/Int.hpp "SCHNAPS/Core/Int.hpp"
 *  \brief The integer implementation as Object.
 */
class Int: public Number {
public:
	//! Int allocator type.
	typedef AllocatorT<Int, Number::Alloc> Alloc;
	//! Int handle type.
	typedef PointerT<Int, Number::Handle> Handle;
	//! Int bag type.
	typedef ContainerT<Int, Number::Bag> Bag;

	Int() {}
	Int(int inValue);
	Int(const Int& inOriginal);
	virtual ~Int() {}

	//! Copy operator.
	Int& operator=(const Int& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Int");
		return lName;
		schnaps_StackTraceEndM("const std::string& Int::getName() const");
	}

	/*!
	 * \brief  Return a const reference to the type of object.
	 * \return A const reference to the type of object.
	 */
	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
		const static std::string lType("Int");
		return lType;
		schnaps_StackTraceEndM("const std::string& Int::getType() const");
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
	//! Subtract a number.
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
	 * \brief Return a const reference to the value of integer.
	 * \return A const reference to the value of integer.
	 */
	const int& getValue() const {
		return mValue;
	}

	/*!
	 * \brief Set the value of integer.
	 * \param inValue A const reference to the new value of integer.
	 */
	void setValue(const int& inValue) {
		mValue = inValue;
	}

private:
	int mValue; //! Value of integer.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Int_hpp
