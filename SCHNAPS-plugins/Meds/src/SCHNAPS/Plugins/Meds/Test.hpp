/*
 * Test.hpp
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

#ifndef SCHNAPS_Plugins_Meds_Test_hpp
#define SCHNAPS_Plugins_Meds_Test_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Meds {

/*!
 *  \class Test SCHNAPS/Plugins/Meds/Test.hpp "SCHNAPS/Plugins/Meds/Test.hpp"
 *  \brief Medical test.
 */
class Test: public Core::Primitive {
public:
	//! Test allocator type.
	typedef Core::AllocatorT<Test, Core::Primitive::Alloc> Alloc;
	//! Test handle type.
	typedef Core::PointerT<Test, Core::Primitive::Handle> Handle;
	//! Test bag type.
	typedef Core::ContainerT<Test, Core::Primitive::Bag> Bag;

	Test();
	Test(const Test& inOriginal);
	virtual ~Test() {}

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Meds_Test");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::Test::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Execute the primitive.
	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	//! Return the primitive return type.
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mLabel;					//!< Label of test.
	Core::Double::Handle mCompliance;	//!< A handle to the compliance value.
	Core::Double::Handle mCost;			//!< A handle to the cost value.
	Core::Double::Handle mDiscountRate;	//!< A handle to the discount rate value.
	std::string mCompliance_Ref;		//!< Label of compliance parameter.
	std::string mCost_Ref;				//!< Label of cost parameter.
	std::string mDiscountRate_Ref;		//!< Label of discount rate parameter.
	std::string mCostVariableLabel;		//!< Label of individual variable for cumulating cost.
};
} // end of Meds namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Meds_Test_hpp */