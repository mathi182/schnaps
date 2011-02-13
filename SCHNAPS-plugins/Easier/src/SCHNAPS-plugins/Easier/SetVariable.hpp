/*
 * SetVariable.hpp
 *
 *  Created on: 2010-11-20
 *  Author: Audrey Durand
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

#ifndef easier_SetVariable_hpp
#define easier_SetVariable_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace easier {
/*!
 *  \class SetVariable SCHNAPS-plugins/Easier/SetVariable.hpp "SCHNAPS-plugins/Easier/SetVariable.hpp"
 *  \brief Assign a new value to a variable.
 */
class SetVariable: public core::Primitive {
public:
	//! SetVariable allocator type.
	typedef core::AllocatorT<SetVariable, core::Primitive::Alloc> Alloc;
	//! SetVariable handle type.
	typedef core::PointerT<SetVariable, core::Primitive::Handle> Handle;
	//! SetVariable bag type.
	typedef core::ContainerT<SetVariable, core::Primitive::Bag> Bag;

	SetVariable();
	SetVariable(const SetVariable& inOriginal);
	virtual ~SetVariable() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Easier_SetVariable");
			return lName;
		schnaps_StackTraceEndM("const std::string& SetVariable::getName() const");
	}

	virtual	void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	std::string mLabel;			//!< Label of variable to set.
	std::string mValue_Ref; 	//!< New value (reference).
	core::Atom::Handle mValue;	//!< New value.
};
} // end of easier namespace

#endif /* easier_SetVariable_hpp */
