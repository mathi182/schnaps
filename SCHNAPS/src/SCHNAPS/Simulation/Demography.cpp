/*
 * Demography.cpp
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

#include "SCHNAPS/Core.hpp"
#include "SCHNAPS/Simulation.hpp"

using namespace SCHNAPS;
using namespace Simulation;

/*!
 * \brief Construct a demography as a copy of an original.
 * \param inOriginal A const reference to the original demography.
 */
Demography::Demography(const Demography& inOriginal) {
	this->mVariablesMap.clear();
	for (VariablesMap::const_iterator lIt = inOriginal.mVariablesMap.begin(); lIt != inOriginal.mVariablesMap.end(); lIt++) {
		this->mVariablesMap.insert(std::pair<std::string, Core::PrimitiveTree::Handle>(
			lIt->first,
			lIt->second));
	}
}

/*!
 * \brief  Return a handle to a deep copy of the object.
 * \return A handle to a deep copy of the object.
 */
Core::Object::Handle Demography::deepCopy(const Core::System& inSystem) const {
	schnaps_StackTraceBeginM();
	Demography::Handle lCopy = new Demography();
	
	for (VariablesMap::const_iterator lIt = this->mVariablesMap.begin(); lIt != this->mVariablesMap.end(); lIt++) {
		lCopy->mVariablesMap.insert(std::pair<std::string, Core::PrimitiveTree::Handle>(
			lIt->first.c_str(),
			Core::castHandleT<Core::PrimitiveTree>(lIt->second->deepCopy(inSystem))));
	}
	return lCopy;
	schnaps_StackTraceEndM("SCHNAPS::Core::Object::Handle SCHNAPS::Simulation::Demography::deepCopy(const SCHNAPS::Core::System&) const");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if variable label is missing.
 */
void Demography::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
	schnaps_StackTraceBeginM();
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != getName()) {
		std::ostringstream lOSS;
		lOSS << "tag <" + getName() + "> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}

	PACC::XML::Document lDocument;
	std::string lFile, lLabel;

	lFile = inIter->getAttribute("file");
	if (lFile.empty()) {
#ifdef SCHNAPS_FULL_DEBUG
		printf("Reading %s\n", getName().c_str());
#endif
		mVariablesMap.clear();
		for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
			if (lChild->getType() == PACC::XML::eData) {
				if (lChild->getValue() != "Variable") {
					std::ostringstream lOSS;
					lOSS << "tag <Variable> expected, but ";
					lOSS << "got tag <" << lChild->getValue() << "> instead!";
					throw schnaps_IOExceptionNodeM(*lChild, lOSS.str());
				}
				if (lChild->getAttribute("label").empty()) {
					throw schnaps_IOExceptionNodeM(*lChild, "label attribute expected!");
				}

				lLabel = lChild->getAttribute("label");
#ifdef SCHNAPS_FULL_DEBUG
				printf("Reading variable: %s\n", lLabel.c_str());
#endif

				mVariablesMap.insert(std::pair<std::string, Core::PrimitiveTree::Handle>(lLabel, new Core::PrimitiveTree()));
				mVariablesMap[lLabel]->readWithSystem(lChild->getFirstChild(), ioSystem);
			}
		}
	} else {
#ifdef SCHNAPS_FULL_DEBUG
	printf("Opening file: %s\n", lFile.c_str());
#endif
		lDocument.parse(lFile);
		this->readWithSystem(lDocument.getFirstDataTag(), ioSystem);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Demography::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Demography::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	for (VariablesMap::const_iterator lIt = mVariablesMap.begin(); lIt != mVariablesMap.end(); lIt++) {
		ioStreamer.openTag("Variable");
		ioStreamer.insertAttribute("label", lIt->first);
		lIt->second->write(ioStreamer, inIndent);
		ioStreamer.closeTag();
	}
}

/*!
 * \brief  Return the list of variables contained in demography.
 * \return The list of variables contained in demography.
 */
std::vector<std::string> Demography::getVariableList() const {
	schnaps_StackTraceBeginM();
	std::vector<std::string> lVariables;
	for (VariablesMap::const_iterator lIt = mVariablesMap.begin(); lIt != mVariablesMap.end(); lIt++) {
		lVariables.push_back(lIt->first.c_str());
	}
	return lVariables;
	schnaps_StackTraceEndM("std::vector<std::string> SCHNAPS::Simulation::Demography::getVariableList() const");
}

/*!
 * \brief  Return a const reference to a variable initialization tree function.
 * \param  inLabel A const reference to the label of variable.
 * \return A const reference to a variable initialization tree function or NULL if the variable is not in demography.
 * \thorw  SCHNAPS::Core::RunTimeException if the variable does not exist.
 * \thorw  SCHNAPS::Core::AssertException if the initialization tree of the variable is NULL.
 */
const Core::PrimitiveTree& Demography::getVariableInitTree(const std::string& inLabel) const {
	schnaps_StackTraceBeginM();
	VariablesMap::const_iterator lIterVariable = mVariablesMap.find(inLabel);
	if (lIterVariable == mVariablesMap.end()) {
		std::ostringstream lOSS;
		lOSS << "The variable '" << inLabel << "' does not exist; ";
		lOSS << "could not get its initialization tree.";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}
	schnaps_NonNullPointerAssertM(lIterVariable->second);
	return *lIterVariable->second;
	schnaps_StackTraceEndM("const SCHNAPS::Core::PrimitiveTree& SCHNAPS::Simulation::Demography::getVariableInitTree(const std::string&) const");
}

/*!
 * \brief  Check if a variable is contained in demography.
 * \param  inLabel A const reference to the label of variable.
 * \return True is the variable is contained in demography, false if not.
 */
bool Demography::hasVariable(const std::string& inLabel) const {
	schnaps_StackTraceBeginM();
	VariablesMap::const_iterator lIterVariable = mVariablesMap.find(inLabel);
	if (lIterVariable == mVariablesMap.end()) {
		return false;
	}
	return true;
	schnaps_StackTraceEndM("bool SCHNAPS::Simulation::Demography::hasVariable(const std::string&) const");
}