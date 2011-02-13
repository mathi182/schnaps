/*
 * ChoiceIsBetween.cpp
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

#include "SCHNAPS-plugins/Easier/Easier.hpp"

using namespace easier;

/*!
 *  \brief Construct a new primitive that returns the result of a ChoiceIsBetween.
 */
ChoiceIsBetween::ChoiceIsBetween() :
		Primitive(),	// Unknown number of children
		mChoiceVariableLabel(""),
		mChoices_Ref(""),
		mChoices(new core::Vector())
{}

ChoiceIsBetween::ChoiceIsBetween(const ChoiceIsBetween& inOriginal) :
		Primitive(inOriginal.getNumberArguments()),
		mChoiceVariableLabel(inOriginal.mChoiceVariableLabel.c_str()),
		mChoices_Ref(inOriginal.mChoices_Ref.c_str()),
		mChoices(new core::Vector())
{
	if (mChoices_Ref.empty()) {
		mChoices->clear();
		for (unsigned int i = 0; i < inOriginal.mChoices->size(); i++) {
			mChoices->push_back((*inOriginal.mChoices)[i]->clone());
		}
	} else {
		mChoices = inOriginal.mChoices;
	}
}

void ChoiceIsBetween::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
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
		// Retrieve choice variable label
		if (inIter->getAttribute("choiceVariableLabel").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "choice variable label expected!");
		}
		mChoiceVariableLabel = inIter->getAttribute("choiceVariableLabel");

		// Retrieve choice boundaries
		if (inIter->getAttribute("choices").empty()) {
			if (inIter->getAttribute("choices.ref").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "possible choices expected!");
			} else { // Use referenced choices
				mChoices_Ref = inIter->getAttribute("choices.ref");
				std::stringstream lSS;
				lSS << "ref." << mChoices_Ref;

				mChoices = core::castHandleT<core::Vector>(ioSystem.getParameters()[lSS.str().c_str()]);
			}
		} else { // Use defined choice boundaries
			// Retrieve choice type
			if (inIter->getAttribute("choiceType").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "type of choice expected!");
			}
			core::Atom::Alloc::Handle lAlloc = core::castHandleT<core::Atom::Alloc>(ioSystem.getFactory().getAllocator(inIter->getAttribute("choiceType")));

			std::stringstream lSS(inIter->getAttribute("choices"));
			PACC::Tokenizer lTokenizer(lSS);
			lTokenizer.setDelimiters("|", "");

			std::string lBoundary;
			core::Number::Handle lNumber;

			mChoices->clear();
			while (lTokenizer.getNextToken(lBoundary)) {
				// Add to SwitchBins
				lNumber = core::castHandleT<core::Number>(lAlloc->allocate());
				lNumber->readStr(lBoundary);
				if (!mChoices->empty()) {
					if ((lNumber->isLess(*mChoices->back())) || (lNumber->isEqual(*mChoices->back()))) {
						throw schnaps_IOExceptionNodeM(*inIter, "boundaries of choices expected to be in crescent order!");
					}
				}
				mChoices->push_back(lNumber);
			}
		}

#ifndef SIMULATOR_NDEBUG
		if (mChoices->size() == 0) {
			throw schnaps_IOExceptionNodeM(*inIter, "at least one choice expected!");
		}
#else
		schnaps_AssertM(mChoices->size() > 0);
#endif

		setNumberArguments(mChoices->size());
	schnaps_StackTraceEndM("void core::ChoiceIsBetween::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void ChoiceIsBetween::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.insertAttribute("choiceVariableLabel", mChoiceVariableLabel);

		if (mChoices_Ref.empty()) {
			std::stringstream lSS;
			for (unsigned int i = 0; i < mChoices->size(); i++) {
				lSS << (*mChoices)[i]->writeStr() << "|";
			}
			ioStreamer.insertAttribute("choiceType", (*mChoices)[0]->getName());
			ioStreamer.insertAttribute("choices", lSS.str().c_str());
		}

		ioStreamer.insertAttribute("choices.ref", mChoices_Ref);
	schnaps_StackTraceEndM("void core::ChoiceIsBetween::writeContent(PACC::XML::Streamer&, bool) const");
}

ChoiceIsBetween& ChoiceIsBetween::operator=(const ChoiceIsBetween& inOriginal) {
	schnaps_StackTraceBeginM();
		this->setNumberArguments(inOriginal.getNumberArguments());
		mChoiceVariableLabel = inOriginal.mChoiceVariableLabel.c_str();
		mChoices_Ref = inOriginal.mChoices_Ref.c_str();
		if (mChoices_Ref.empty()) {
			mChoices->clear();
			for (unsigned int i = 0; i < inOriginal.mChoices->size(); i++) {
				mChoices->push_back((*inOriginal.mChoices)[i]->clone());
			}
		} else {
			mChoices = inOriginal.mChoices;
		}
		return *this;
	schnaps_StackTraceEndM("core::ChoiceIsBetween& core::ChoiceIsBetween::operator=(const core::ChoiceIsBetween&)");
}

core::AnyType::Handle ChoiceIsBetween::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		simulator::ExecutionContext& lContext = core::castObjectT<simulator::ExecutionContext&>(ioContext);

		// Retrieve individual variable
#ifndef SIMULATOR_NDEBUG
		if (lContext.getIndividual().getState().find(mChoiceVariableLabel) == lContext.getIndividual().getState().end()) {
			throw schnaps_InternalExceptionM("Could not find variable '" + mChoiceVariableLabel + "' in current individual state!");
		}
		if (lContext.getIndividual().getState().find(mChoiceVariableLabel)->second == NULL) {
			throw schnaps_InternalExceptionM("Variable '" + mChoiceVariableLabel + "' is empty for current individual!");
		}
#else
		schnaps_AssertM(lContext.getIndividual().getState().find(mChoiceVariableLabel) != lContext.getIndividual().getState().end());
		schnaps_NonNullPointerAssertM(lContext.getIndividual().getState().find(mChoiceVariableLabel)->second);
#endif
		core::Number::Handle lVariable = core::castHandleT<core::Number>(lContext.getIndividual().getState()[mChoiceVariableLabel]);

#ifndef SIMULATOR_NDEBUG
		if (lVariable->isLess(*(*mChoices)[0])) {
			printf("Variable value: %s\n", lVariable->writeStr().c_str());
			printf("Lower bound: %s\n", (*mChoices)[0]->writeStr().c_str());
			throw schnaps_InternalExceptionM("Variable '" + mChoiceVariableLabel + "' not in any range of choices!");
		}
#else
		schnaps_AssertM(!lVariable->isLess(*(*mChoices)[0]));
#endif
		for (unsigned int i = 0; i < mChoices->size(); i++) {
			if (lVariable->isLess(*(*mChoices)[i])) {
				return getArgument(inIndex, i-1, ioContext);
			}
		}
		return getArgument(inIndex, mChoices->size()-1, ioContext);
	schnaps_StackTraceEndM("core::AnyType::Handle core::ChoiceIsBetween::execute(unsigned int, core::ExecutionContext&) const");
}

const std::string& ChoiceIsBetween::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<getNumberArguments());
		unsigned int lNodeIndex = getArgumentIndex(inIndex, inN, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& core::ChoiceIsBetween::getArgType(unsigned int, unsigned int, core::ExecutionContext&) const");
}

const std::string& ChoiceIsBetween::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		if (getNumberArguments() < 2) {
			unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
			return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
		}

		std::string lCommonType = ioContext.getSystem().getTypingManager().commonType(getArgType(inIndex, 0, ioContext), getArgType(inIndex, 1, ioContext));

		for (unsigned int i = 2; i < getNumberArguments(); i++) {
			lCommonType = ioContext.getSystem().getTypingManager().commonType(lCommonType, getArgType(inIndex, i, ioContext));
		}

		const static std::string lCommonType_Final(lCommonType);
		return lCommonType_Final;
	schnaps_StackTraceEndM("const std::string& core::ChoiceIsBetween::getReturnType(unsigned int, core::ExecutionContext&) const");
}
