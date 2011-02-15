/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

#ifndef SCHNAPS_Core_IsLessPointerPredicate_hpp
#define SCHNAPS_Core_IsLessPointerPredicate_hpp

#include "SCHNAPS/Core/Object.hpp"

namespace SCHNAPS {
namespace Core {

/*!
 *  \class IsLessPointerPredicate SCHNAPS/Core/IsLessPointerPredicate.hpp
 *    "SCHNAPS/Core/IsLessPointerPredicate.hpp"
 *  \brief Binary predicate functor that test the ordering of two pointed Objects.
 *  \par Note:
 *    The IsLessPointerPredicate is compliant with the STL BinaryPredicate functor.
 *    See http://www.sgi.com/tech/stl/BinaryPredicate.html
 */
class IsLessPointerPredicate {
public:
	IsLessPointerPredicate() {}
	~IsLessPointerPredicate() {}

	/*!
	 *  \brief Test the ordering of two pointed Objects.
	 *  \param inLeftPtr Pointer to the left object to compare.
	 *  \param inRightPtr Pointer to the right object to compare.
	 */
	inline bool operator()(const Pointer& inLeftPtr, const Pointer& inRightPtr) const {
		schnaps_StackTraceBeginM();
			if ((!inLeftPtr) || (!inRightPtr))
				return false;
			return inLeftPtr->isLess(*inRightPtr);
		schnaps_StackTraceEndM("bool operator()(const Pointer& inLeftPtr, const Pointer& inRightPtr) const");
	}
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_IsLessPointerPredicate_hpp
