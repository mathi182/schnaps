/*
 * PopulationManager.hpp
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

#ifndef SCHNAPS_Simulation_PopulationManager_hpp
#define SCHNAPS_Simulation_PopulationManager_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Simulation/Generator.hpp"
#include "SCHNAPS/Simulation/Individual.hpp"

#include <map>

namespace SCHNAPS {
namespace Simulation {

/*!
 * \struct Source SCHNAPS/Simulation/PopulationManager.hpp "SCHNAPS/Simulation/PopulationManager.hpp"
 * \brief Source for generating individuals.
 */
struct Source {
	std::string mProfile;	//!< Name of the source profile.
	unsigned int mSize;		//!< Quantity of individuals to generate.

	Source() {}
	explicit Source(std::string inProfile, unsigned int inSize) :
		mProfile(inProfile),
		mSize(inSize)
	{}
};

/*!
 * \class PopulationManager SCHNAPS/Simulation/PopulationManager.hpp "SCHNAPS/Simulation/PopulationManager.hpp"
 * \brief In charge of creating new individuals when needed. Maps a clock time to a source (profile and size).
 */
class PopulationManager: public SCHNAPS::Core::Object, public std::multimap<unsigned long, Source> {
public:
	//! PopulationManager allocator type.
	typedef SCHNAPS::Core::AllocatorT<PopulationManager, SCHNAPS::Core::Object::Alloc> Alloc;
	//! PopulationManager handle type.
	typedef SCHNAPS::Core::PointerT<PopulationManager, SCHNAPS::Core::Object::Handle> Handle;
	//! PopulationManager bag type.
	typedef SCHNAPS::Core::ContainerT<PopulationManager, SCHNAPS::Core::Object::Bag> Bag;

	PopulationManager();
	explicit PopulationManager(SCHNAPS::Core::System::Handle inSystem, Clock::Handle inClock, Environment::Handle inEnvironment);
	virtual ~PopulationManager() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("PopulationManager");
		return lName;
		schnaps_StackTraceEndM("const std::string& PopulationManager::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Read the population manager from string.
	void readStr(const std::string& inStr) {
		std::stringstream lISS(inStr);
		PACC::Tokenizer lTokenizer(lISS);
		lTokenizer.setDelimiters(",", "");

		std::string lInput;

		while (lTokenizer.getNextToken(lInput)) {
			readEntryStr(lInput);
		}
	}

	//! Return the content of the population manager as a string.
	std::string writeStr() const {
		std::stringstream lSS;
		std::string lInput;

		for (PopulationManager::const_iterator lIt = this->begin(); lIt != this->end(); lIt++) {
			// write time
			lSS << "time" << "=" << lIt->first << ";";

			// write profile name
			lSS << "profile" << "=" << lIt->second.mProfile.c_str() << ";";

			// write size
			lSS << "size" << "=" << lIt->second.mSize << ",";
		}

		lInput = lSS.str();
		lInput.erase(lInput.end()-1);
		return lInput;
	}

	//! Return a pointer to a vector of newly created individuals.
	Individual::Bag::Handle getIndividuals();

	//! Return a const reference to the map of available prefixes.
	const std::map<std::string, Source>& getPrefixes() const {
		return mPrefixes;
	}

	//! Return a reference to the map of available prefixes.
	std::map<std::string, Source>& getPrefixes() {
		return mPrefixes;
	}

	//! Return a const reference to the individual generator.
	const Generator& getGenerator() const {
		schnaps_NonNullPointerAssertM(mGenerator);
		return *mGenerator;
	}

	//! Return a reference to the individual generator.
	Generator& getGenerator() {
		schnaps_NonNullPointerAssertM(mGenerator);
		return *mGenerator;
	}

private:
	//! Read one population manager entry from string.
	void readEntryStr(const std::string& inStr) {
		std::stringstream lISS(inStr);
		PACC::Tokenizer lTokenizer(lISS);
		lTokenizer.setDelimiters(";", "");

		std::string lParam;
		std::string::size_type lPos;

		unsigned long lTime;
		std::string lProfile;
		unsigned int lSize;

		// read time
		if (lTokenizer.getNextToken(lParam)) {
			lPos = lParam.find("=");

			if (lPos == std::string::npos) {
				printf("Expected value of input parameter %s!\n", lParam.c_str());
			} else {
				lTime = SCHNAPS::str2uint(lParam.substr(lPos+1));

				// read profile name
				if (lTokenizer.getNextToken(lParam)) {
					lPos = lParam.find("=");

					if (lPos == std::string::npos) {
						printf("Expected value of input parameter %s!\n", lParam.c_str());
					} else {
						lProfile = lParam.substr(lPos+1);

						// read size
						if (lTokenizer.getNextToken(lParam)) {
							lPos = lParam.find("=");

							if (lPos == std::string::npos) {
								printf("Expected value of input parameter %s!\n", lParam.c_str());
							} else {
								lSize = SCHNAPS::str2uint(lParam.substr(lPos+1));

								this->insert(std::pair<unsigned long, Source>(lTime, Source(lProfile, lSize)));
							}
						}
					}
				}
			}
		}
	}

private:
	std::map<std::string, Source> mPrefixes;	//!< Individual IDs prefixes to sources.
	Generator::Handle mGenerator;				//!< Individual generator.
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_PopulationManager_hpp */
