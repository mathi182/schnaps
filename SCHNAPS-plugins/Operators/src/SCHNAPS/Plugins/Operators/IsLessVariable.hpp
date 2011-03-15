/*
 * IsLessVariable.hpp
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

#ifndef SCHNAPS_Plugins_Operators_IsLessVariable_hpp
#define SCHNAPS_Plugins_Operators_IsLessVariable_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class IsLessVariable SCHNAPS/Plugins/Operators/IsLessVariable.hpp "SCHNAPS/Plugins/Operators/IsLessVariable.hpp"
 *  \brief Comparison operator "is less" between a current individual variable and a value.
 */
class IsLessVariable: public Core::Primitive {
public:
	//! IsLessVariable allocator type.
	typedef Core::AllocatorT<IsLessVariable, Core::Primitive::Alloc> Alloc;
	//! IsLessVariable handle type.
	typedef Core::PointerT<IsLessVariable, Core::Primitive::Handle> Handle;
	//! IsLessVariable bag type.
	typedef Core::ContainerT<IsLessVariable, Core::Primitive::Bag> Bag;

	IsLessVariable();
	IsLessVariable(const IsLessVariable& inOriginal);
	virtual ~IsLessVariable() {}

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Operators_IsLessVariable");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::IsLessVariable::getName() const");
	}

	//! Read object from XML using system.
	virtual	void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Execute the primitive.
	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	//! Return the primitive return type.
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mLabel;				//!< Label of concerned variable.
	std::string mValue_Ref;			//!< Value for comparison (reference).
	Core::Number::Handle mValue;	//!< A handle to the value for comparison.
};
} // end of Operators namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Operators_IsLessVariable_hpp */