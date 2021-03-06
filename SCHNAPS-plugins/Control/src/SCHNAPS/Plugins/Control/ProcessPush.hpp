/*
 * ProcessPush.hpp
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

#ifndef SCHNAPS_Plugins_Control_ProcessPush_hpp
#define SCHNAPS_Plugins_Control_ProcessPush_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Control {

/*!
 *  \class ProcessPush SCHNAPS/Plugins/Control/ProcessPush.hpp "SCHNAPS/Plugins/Control/ProcessPush.hpp"
 *  \brief Primitive for pushing process.
 * 		   If the push is for the current time (delay = 0), it will be done at the next substep using the blackboard.
 */
class ProcessPush: public Core::Primitive {
public:
	//! ProcessPush allocator type.
	typedef Core::AllocatorT<ProcessPush, Core::Primitive::Alloc> Alloc;
	//! ProcessPush handle type.
	typedef Core::PointerT<ProcessPush, Core::Primitive::Handle> Handle;
	//! ProcessPush bag type.
	typedef Core::ContainerT<ProcessPush, Core::Primitive::Bag> Bag;

	ProcessPush();
	ProcessPush(const ProcessPush& inOriginal);
	virtual ~ProcessPush() {}

	//! Copy operator.
	ProcessPush& operator=(const ProcessPush& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Control_ProcessPush");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::ProcessPush::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Execute the primitive.
	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	//! Return the nth argument requested return type.
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mLabel;						//!< Label of process to push.
	Simulation::Process::Target mTarget;	//!< Target of the push.
	Core::ULong::Handle mDelay;				//!< A handle to the delay before execution.
	std::string mDelay_Ref;					//!< Reference to the delay value.
	Core::ULong::Handle mRepeat;			//!< A handle to the number of repetition.
	std::string mRepeat_Ref;				//!< Reference to the number of repetition value.
	Core::ULong::Handle mDelta;				//!< A handle to the delta between repetitions.
	std::string mDelta_Ref;					//!< Reference to the delta value between repetitions.
	Simulation::Clock::Units mUnits;		//!< The units in which the delay, repeat and delta attributes are expressed.
};
} // end of Control namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Control_ProcessPush_hpp */
