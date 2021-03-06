/*
 * ConcatVariable.cpp
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

#include "SCHNAPS/Plugins/Operators/Operators.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Operators;

/*!
 * \brief Default constructor.
 */
ConcatVariable::ConcatVariable() :
	Core::Primitive(0),
	mResult_Ref(""),
	mArgLeft_Ref(""),
	mArgLeft(NULL),
	mArgRight_Ref(""),
	mArgRight(NULL)
{}

/*!
 * \brief Construct a primitive for string concatenation of right argument (as string) to left argument (as string) and store result in a variable as a copy of an original.
 * \param inOriginal A const reference to the original primitive for string concatenation of right argument (as string) to left argument (as string) and store result in a variable.
 */
ConcatVariable::ConcatVariable(const ConcatVariable& inOriginal) :
	Core::Primitive(0),
	mResult_Ref(inOriginal.mResult_Ref.c_str()),
	mArgLeft_Ref(inOriginal.mArgLeft_Ref.c_str()),
	mArgRight_Ref(inOriginal.mArgRight_Ref.c_str())
{
	switch (mArgLeft_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mArgLeft = NULL;
			break;
		case '$':
			// parameter value
			mArgLeft = inOriginal.mArgLeft;
			break;
		default:
			// direct value
			mArgLeft = Core::castHandleT<Core::Atom>(inOriginal.mArgLeft->clone());
	}
	
	switch (mArgRight_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mArgRight = NULL;
			break;
		case '$':
			// parameter value
			mArgRight = inOriginal.mArgRight;
			break;
		default:
			// direct value
			mArgRight = Core::castHandleT<Core::Atom>(inOriginal.mArgRight->clone());
	}
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
ConcatVariable& ConcatVariable::operator=(const ConcatVariable& inOriginal) {
	schnaps_StackTraceBeginM();
	mResult_Ref.assign(inOriginal.mResult_Ref.c_str());
	mArgLeft_Ref.assign(inOriginal.mArgLeft_Ref.c_str());
	mArgRight_Ref.assign(inOriginal.mArgRight_Ref.c_str());
	
	switch (mArgLeft_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mArgLeft = NULL;
			break;
		case '$':
			// parameter value
			mArgLeft = inOriginal.mArgLeft;
			break;
		default:
			// direct value
			mArgLeft = Core::castHandleT<Core::Atom>(inOriginal.mArgLeft->clone());
	}
	
	switch (mArgRight_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mArgRight = NULL;
			break;
		case '$':
			// parameter value
			mArgRight = inOriginal.mArgRight;
			break;
		default:
			// direct value
			mArgRight = Core::castHandleT<Core::Atom>(inOriginal.mArgRight->clone());
	}

	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Operators::ConcatVariable& SCHNAPS::Plugins::Operators::ConcatVariable::operator=(const SCHNAPS::Plugins::Operators::ConcatVariable&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if outResult or inArgLeft or inArgRight attributes are missing.
 * \throw SCHNAPS::Core::RunTimeException if outResult attribute is not a reference to a variable.
 */
void ConcatVariable::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	// retrieve output variable
	if (inIter->getAttribute("outResult").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "label of variable where to store result expected!");
	}
	mResult_Ref.assign(inIter->getAttribute("outResult"));
	
	if (mResult_Ref[0] != '@') {
		throw schnaps_RunTimeExceptionM("The primitive is undefined for the specific output result source!");
	}

	// retrieve left argument
	if (inIter->getAttribute("inArgLeft").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "left argument expected!");
	}
	mArgLeft_Ref.assign(inIter->getAttribute("inArgLeft"));
	
	switch (mArgLeft_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mArgLeft = NULL;
			break;
		case '$':
			// parameter value
			mArgLeft = Core::castHandleT<Core::Atom>(ioSystem.getParameters().getParameterHandle(mArgLeft_Ref.substr(1)));
			break;
		default:
			// direct value
			mArgLeft = new Core::String(mArgLeft_Ref);
			break;
	}
	
	// retrieve right argument
	if (inIter->getAttribute("inArgRight").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "right argument expected!");
	}
	mArgRight_Ref.assign(inIter->getAttribute("inArgRight"));
	
	switch (mArgRight_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mArgRight = NULL;
			break;
		case '$':
			// parameter value
			mArgRight = Core::castHandleT<Core::Atom>(ioSystem.getParameters().getParameterHandle(mArgRight_Ref.substr(1)));
			break;
		default:
			// direct value
			mArgRight = new Core::String(mArgRight_Ref);
			break;
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::ConcatVariable::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void ConcatVariable::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("outResult", mResult_Ref);
	ioStreamer.insertAttribute("inArgLeft", mArgLeft_Ref);
	ioStreamer.insertAttribute("inArgRight", mArgRight_Ref);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::ConcatVariable::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle ConcatVariable::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);
	std::string lArgRight, lArgLeft;
	
	switch (mArgLeft_Ref[0]) {
		case '@':
			// individual variable value
			lArgLeft = lContext.getIndividual().getState().getVariable(mArgLeft_Ref.substr(1)).writeStr();
			break;
		case '#':
			// environment variable value
			lArgLeft = lContext.getEnvironment().getState().getVariable(mArgLeft_Ref.substr(1)).writeStr();
			break;
		case '%':
			// local variable value
			lArgLeft = lContext.getLocalVariable(mArgLeft_Ref.substr(1)).writeStr();
			break;
		default:
			// parameter value or direct value
			lArgLeft = mArgLeft->writeStr();
			break;
	}
	
	switch (mArgRight_Ref[0]) {
		case '@':
			// individual variable value
			lArgRight = lContext.getIndividual().getState().getVariable(mArgRight_Ref.substr(1)).writeStr();
			break;
		case '#':
			// environment variable value
			lArgRight = lContext.getEnvironment().getState().getVariable(mArgRight_Ref.substr(1)).writeStr();
			break;
		case '%':
			// local variable value
			lArgRight = lContext.getLocalVariable(mArgRight_Ref.substr(1)).writeStr();
			break;
		default:
			// parameter value or direct value
			lArgRight = mArgRight->writeStr();
			break;
	}

	std::stringstream lSS;
	lSS << lArgLeft << lArgRight;
	
	if (mResult_Ref[0] == '@') {
		// individual variable
		Simulation::SimulationContext& lSimulationContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
		lSimulationContext.getIndividual().getState().setVariable(mResult_Ref.substr(1), new Core::String(lSS.str()));
	} else { // mResult_Ref[0] == '%'
		// local variable
		lContext.setLocalVariable(mResult_Ref.substr(1), new Core::String(lSS.str()));
	}
	
	return NULL;
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Operators::ConcatVariable::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& ConcatVariable::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Void");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::ConcatVariable::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
