/*
 * ProcessPushed.hpp
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

#ifndef SCHNAPS_Simulation_ProcessPushed_hpp
#define SCHNAPS_Simulation_ProcessPushed_hpp

#include "SCHNAPS/Simulation/Process.hpp"

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class ProcessPushed SCHNAPS/Simulation/ProcessPushed.hpp "SCHNAPS/Simulation/ProcessPushed.hpp"
 *  \brief Describe a pushed process applied to individuals.
 */
class ProcessPushed: public Process {
public:
	//! ProcessPushed allocator type.
	typedef Core::AllocatorT<ProcessPushed, Process::Alloc> Alloc;
	//! ProcessPushed handle type.
	typedef Core::PointerT<ProcessPushed, Process::Handle> Handle;
	//! ProcessPushed bag type.
	typedef Core::ContainerT<ProcessPushed, Process::Bag> Bag;

	ProcessPushed();
	ProcessPushed(const ProcessPushed& inOriginal);
	explicit ProcessPushed(const std::string inLabel);
	virtual ~ProcessPushed() {}

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("ProcessPushed");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Simulation::ProcessPushed::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Return a handle to the result of process execution.
	virtual Core::AnyType::Handle execute(Core::ExecutionContext& ioContext) const;
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_ProcessPushed_hpp */
