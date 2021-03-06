/*
 * Plugins.hpp
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

#ifndef SCHNAPS_Core_Plugins_hpp
#define SCHNAPS_Core_Plugins_hpp

#include "SCHNAPS/Core/AllocatorT.hpp"
#include "SCHNAPS/Core/PointerT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"
#include "SCHNAPS/Core/Component.hpp"
#include "SCHNAPS/Core/Plugin.hpp"
#include "SCHNAPS/Core/HashString.hpp"

#if defined(SCHNAPS_HAVE_STD_HASHMAP) | defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
#include <hash_map>
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
#include <tr1/unordered_map>
#else
#include <map>
#endif

namespace SCHNAPS {
namespace Core {

/*!
 *  \class Plugins SCHNAPS/Core/Plugins.hpp "SCHNAPS/Core/Plugins.hpp"
 *  \brief Object factory allowing dynamic types allocation.
 *
 *  A factory is used to generate and copy data types dynamically. It contains a database of
 *  object allocators associated to a name. String-based lookup are thus possible to obtain
 *  the allocators, to allocate, clone or copy given dynamically-binded data type. For example,
 *  asking for the type given in a string "FitnessSimple" would provide a instanciation
 *  of the allocator for that type exact type, FitnessSimple.
 */
class Plugins: public Component {

protected:
#if defined(SCHNAPS_HAVE_STD_HASHMAP)
	typedef std::hash_map<std::string, Plugin::Handle, HashString> PluginMap;
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
	typedef std::tr1::unordered_map<std::string, Plugin::Handle, HashString> PluginMap;
#elif defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
	typedef stdext::hash_map<std::string, Plugin::Handle, HashString> PluginMap;
#else // no hash_map found
	typedef std::map<std::string, Plugin::Handle> PluginMap;
#endif

public:
	//! Plugins allocator type.
	typedef AllocatorT<Plugins, Component::Alloc> Alloc;
	//! Plugins handle type.
	typedef PointerT<Plugins, Component::Handle> Handle;
	//! Plugins bag type.
	typedef ContainerT<Plugins, Component::Bag> Bag;

	Plugins();
	virtual ~Plugins() {}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Insert new plugin.
	void insertPlugin(const std::string& inLabel, Plugin::Handle inPlugin);
	
	/*!
	 * \brief Return a handle to the plugin with specific label.
	 * \param inLabel A const reference to the label of the plugin.
	 * \return A handle to the plugin.
	 * \throw SCHNAPS::Core::RunTimeException if the plugin does not exist.
	 */
	Plugin::Handle getPlugin(const std::string& inLabel) const {
		schnaps_StackTraceBeginM();
		Plugins::PluginMap::const_iterator lIterPlugins = mPluginMap.find(inLabel);
		if(lIterPlugins == mPluginMap.end()) {
			std::ostringstream lOSS;
			lOSS << "The plugin '" << inLabel << "' does not exist; ";
			lOSS << "could not get it.";
			throw schnaps_RunTimeExceptionM(lOSS.str());
		}
		return lIterPlugins->second;
		schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Core::Parameters::getPlugin(const std::string&) const");
	}

protected:
	PluginMap mPluginMap; //!< Map of stored plugins.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Plugins_hpp
