/*
 * Primitive.hpp
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

#ifndef SCHNAPS_Core_Primitive_hpp
#define SCHNAPS_Core_Primitive_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/AbstractAllocT.hpp"
#include "SCHNAPS/Core/PointerT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"
#include "SCHNAPS/Core/AnyType.hpp"

#include <limits.h>

namespace SCHNAPS {
namespace Core {

// forward declaration
class ExecutionContext;

/*!
 * \class Primitive SCHNAPS/Core/Primitive.hpp "SCHNAPS/Core/Primitive.hpp"
 * \brief Primitive base class.
 */
class Primitive: public Object {
public:
	//! Specify the constants for specific kind of primitive arguments.
	enum ArgumentKind {
		eTerminal = 0, eAny = UINT_MAX
	};

	//! Primitive allocator type.
	typedef AbstractAllocT<Primitive, Object::Alloc> Alloc;
	//! Primitive handle type.
	typedef PointerT<Primitive, Object::Handle> Handle;
	//! Primitive bag type.
	typedef ContainerT<Primitive, Object::Bag> Bag;

	Primitive(const Primitive& inOriginal);
	explicit Primitive(unsigned int inNumberArguments = Primitive::eAny);
	virtual ~Primitive() {}

	//! Copy operator.
	Primitive& operator=(const Primitive& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Primitive");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::Primitive::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Test if object is equal to another.
	virtual bool isEqual(const Object& inRightObj) const;

	//! Execute the primitive.
	virtual AnyType::Handle execute(unsigned int inIndex, ExecutionContext& ioContext) const;
	//! Return the nth argument requested return type.
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, ExecutionContext& ioContext) const;
	//! Return the primitive return type.
	virtual const std::string& getReturnType(unsigned int inIndex, ExecutionContext& ioContext) const;
	//! Validate primitive and children recursively.
	bool isValid(unsigned int inIndex, ExecutionContext& ioContext) const;

	/*!
	 * \brief  Return the number of arguments of primitive.
	 * \return Number of arguments.
	 */
	unsigned int getNumberArguments() const {
		return mNumberArguments;
	}

	/*!
	 * \brief Set number of arguments of primitive.
	 * \param inNumberArguments Number of arguments.
	 */
	void setNumberArguments(unsigned int inNumberArguments) {
		mNumberArguments = inNumberArguments;
	}

protected:
	//! Get index in the primitive tree of the Nth argument to actual primitive.
	unsigned int getArgumentIndex(unsigned int inIndex, unsigned int inN, ExecutionContext& ioContext) const;
	//! Get the value of the nth argument.
	AnyType::Handle getArgument(unsigned int inIndex, unsigned int inN, ExecutionContext& ioContext) const;

private:
	unsigned int mNumberArguments; //!< Number of arguments of the primitive.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Core_Primitive_hpp */
