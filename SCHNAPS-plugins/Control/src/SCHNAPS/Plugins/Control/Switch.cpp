/*
 * Switch.cpp
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

#include "SCHNAPS/Plugins/Control/Control.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Control;

/*!
 * \brief Default constructor.
 */
Switch::Switch() :
	Primitive(), // unknown number of children
	mKeys_Ref(""),
	mKeyType("")
{}

/*!
 * \brief Construct a primitive for switching over a value as a copy of an original.
 * \param inOriginal A const reference to the original primitive for switching over a value.
 */
Switch::Switch(const Switch& inOriginal) :
	Primitive(inOriginal.getNumberArguments()),
	mKeys_Ref(inOriginal.mKeys_Ref.c_str()),
	mKeyType(inOriginal.mKeyType.c_str())
{
	mSwitchMap.clear();
	for (SwitchMap::const_iterator lIt = inOriginal.mSwitchMap.begin(); lIt != inOriginal.mSwitchMap.end(); lIt++) {
		mSwitchMap.insert(std::pair<Core::Atom::Handle, unsigned int>(Core::castHandleT<Core::Atom>(lIt->first->clone()), lIt->second));
	}
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
Switch& Switch::operator=(const Switch& inOriginal) {
	schnaps_StackTraceBeginM();
	this->setNumberArguments(inOriginal.getNumberArguments());
	mKeys_Ref = inOriginal.mKeys_Ref.c_str();
	mKeyType = inOriginal.mKeyType.c_str();
	mSwitchMap.clear();
	for (SwitchMap::const_iterator lIt = inOriginal.mSwitchMap.begin(); lIt != inOriginal.mSwitchMap.end(); lIt++) {
		mSwitchMap.insert(std::pair<Core::Atom::Handle, unsigned int>(Core::castHandleT<Core::Atom>(lIt->first->clone()), lIt->second));
	}
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Switch& SCHNAPS::Plugins::Control::Switch::operator=(const SCHNAPS::Core::Switch&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if keys attribute and keys.ref attribute are missing.
 * \throw SCHNAPS::Core::IOException if keys attribute is used and keyType attribute is missing.
 */
void Switch::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
	schnaps_StackTraceBeginM();
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != getName()) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}

	unsigned int lChoice;

	// retrieve keys
	if (inIter->getAttribute("keys").empty()) {
		if (inIter->getAttribute("keys.ref").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "keys of the children expected!");
		} else { // use referenced keys
			mKeys_Ref = inIter->getAttribute("keys.ref");
			std::stringstream lSS;
			lSS << "ref." << mKeys_Ref;

			Core::Vector::Handle lKeys = Core::castHandleT<Core::Vector>(ioSystem.getParameters().getParameterHandle(lSS.str().c_str()));
			for (unsigned int i = 0; i < lKeys->size(); i++) {
				mSwitchMap.insert(std::pair<Core::Atom::Handle, unsigned int>(Core::castHandleT<Core::Atom>((*lKeys)[i]), i));
			}
			lChoice = lKeys->size();
		}
	} else { // use defined keys
		// retrieve key type
		if (inIter->getAttribute("keyType").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "type of the keys expected!");
		}
		mKeyType = inIter->getAttribute("keyType");
		Core::Atom::Alloc::Handle lAlloc = Core::castHandleT<Core::Atom::Alloc>(ioSystem.getFactory().getAllocator(mKeyType));

		std::stringstream lSS(inIter->getAttribute("keys"));
		PACC::Tokenizer lTokenizer(lSS);
		lTokenizer.setDelimiters("|", "");

		std::string lKey;
		Core::Atom::Handle lAtom;

		lChoice = 0;
		while (lTokenizer.getNextToken(lKey)) {
			// add to map
			lAtom = Core::castHandleT<Core::Atom>(lAlloc->allocate());
			lAtom->readStr(lKey);
			mSwitchMap.insert(std::pair<Core::Atom::Handle, unsigned int>(lAtom, lChoice));
			lChoice++;
		}
	}

	setNumberArguments(lChoice + 2); // + 1 for the value switched on, +1 for default case
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::Switch::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Switch::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	if (mKeys_Ref.empty()) {
		std::string lKeys;
		for (std::map<Core::Atom::Handle, unsigned int, Core::IsLessPointerPredicate>::const_iterator lIt = mSwitchMap.begin(); lIt != mSwitchMap.end(); lIt++) {
			lKeys += (*lIt).first->writeStr();
			lKeys += " ";
		}
		ioStreamer.insertAttribute("keyType", mKeyType);
		ioStreamer.insertAttribute("keys", lKeys);
	}

	ioStreamer.insertAttribute("keys.ref", mKeys_Ref);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::Switch::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle Switch::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Core::Atom::Handle lArg = Core::castHandleT<Core::Atom>(getArgument(inIndex, 0, ioContext));
	SwitchMap::const_iterator lIt = mSwitchMap.find(lArg);
	if (lIt != mSwitchMap.end()) {
		return getArgument(inIndex, (*lIt).second+2, ioContext); // +2 to skip switch value and default case
	}
	// Default
	return getArgument(inIndex, 1, ioContext);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Control::Switch::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssterException if the argument index is out of bounds.
 */
const std::string& Switch::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, getNumberArguments()-1);
	if (inN == 0) {
		// switch value
		if (mSwitchMap.begin() == mSwitchMap.end()) {
			// if no switch cases, can take any
			const static std::string lType("Any");
			return lType;
		} else {
			// if switch cases, the switch value must correspond to the switch cases type
			return mSwitchMap.begin()->first->getType();
		}
	}
	// default case and regular cases execution
	const static std::string lType("Any");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::Switch::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& Switch::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	if (getNumberArguments() < 3) {
		unsigned int lNodeIndex = getArgumentIndex(inIndex, 1, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	}

	std::string lCommonType = ioContext.getSystem().getTypingManager().commonType(getArgType(inIndex, 1, ioContext), getArgType(inIndex, 2, ioContext));

	for (unsigned int i = 2; i < getNumberArguments(); i++) {
		lCommonType = ioContext.getSystem().getTypingManager().commonType(lCommonType, getArgType(inIndex, i, ioContext));
	}

	const static std::string lCommonType_Final(lCommonType);
	return lCommonType_Final;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::Switch::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
