/*
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
 
#ifndef SCHNAPS_Core_Plugin_hpp
#define SCHNAPS_Core_Plugin_hpp

#include <map>
#include <vector>

#ifdef SCHNAPS_IS_WINDOWS
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/Allocator.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"
#include "SCHNAPS/Core/PointerT.hpp"

/*
 *  Use this macro before C functions declaration to enable the function symbols to be exported
 *  in the dynamic loadable libraries.
 */
#ifdef SCHNAPS_IS_WINDOWS
#define SCHNAPS_Plugin_DynLibExport __declspec( dllexport )
#else
#define SCHNAPS_Plugin_DynLibExport
#endif

/*!
 *  \def   SCHNAPS_Plugin_BeginDefinitionM(NAME,VERSION)
 *  \brief Begin declaration of a LSD dynamically loadable plugin.
 *  \param NAME Name of the plugin (a string).
 *  \param VERSION Version of the plugin (a string).
 *  \ingroup Util
 */
#ifdef SCHNAPS_IS_WINDOWS
#define SCHNAPS_Plugin_BeginDefinitionM(NAME,VERSION) \
extern "C" { \
  __declspec( dllexport ) void __SCHNAPS_Plugin_getPluginSpecs( \
         std::string& outName, \
         std::string& outVersion, \
         std::map<std::string, SCHNAPS::Core::Allocator::Handle>& outAllocators) \
  { \
    outName = (NAME); \
    outVersion = (VERSION); \
    outAllocators.clear();
#else
#define SCHNAPS_Plugin_BeginDefinitionM(NAME,VERSION) \
extern "C" { \
  void __SCHNAPS_Plugin_getPluginSpecs( \
         std::string& outName, \
         std::string& outVersion, \
         std::map<std::string, SCHNAPS::Core::Allocator::Handle>& outAllocators) \
  { \
    outName = (NAME); \
    outVersion = (VERSION); \
    outAllocators.clear();
#endif

/*!
 *  \def   SCHNAPS_Plugin_AddAllocM(ID,ALLOC)
 *  \brief Add a name-factory pair to register a new type of objects available in the plugin.
 *  \param ID Name of the object that can be generated by the factory (a string).
 *  \param FACTORY Factory type that can generate the object (a plugins::Factory object).
 *  \ingroup Util
 */
#define SCHNAPS_Plugin_AddAllocM(ID,ALLOC) \
    outAllocators[(ID)] = new (ALLOC);

/*!
 *  \def   SCHNAPS_Plugin_EndDefinitionM()
 *  \brief End of declaration of a PACC dynamically loadable plugin.
 *  \ingroup Util
 */
#define SCHNAPS_Plugin_EndDefinitionM() \
  } \
}

namespace SCHNAPS {

namespace Core {

/*!
 *  A plugin object is used to load a dynamic library into memory. It provides method to generate
 *  objects from the dynamic library through calls to object factories (i.e. plugins::Factory objects).
 *
 *  A dynamic library can be defined as a plugin with the SCHNAPS_Plugin_BeginDefinitionM,
 *  SCHNAPS_Plugin_AddFactoryM, and SCHNAPS_Plugin_EndDefinitionM macros. The SCHNAPS_Plugin_BeginDefinitionM
 *  macro begins the plugin definition and receives the plugin name and version as arguments, the
 *  macro SCHNAPS_Plugin_AddFactoryM is called to enumerate the objects available in the plugin, with
 *  name-factory pairs provided for each of them, while the SCHNAPS_Plugin_EndDefinitionM is used, of
 *  course, to terminate the plugin definition. The following example shows a definition of a
 *  plugin named "MyPlugin", with two objects named "MyObject1" and "MyObject2", derived from a
 *  known "BasicObject" type, that can be obtained from the plugin.
 *
 \code
 SCHNAPS_Plugin_BeginDefinitionM("MyPlugin", "0.1");
 SCHNAPS_Plugin_AddFactoryM("MyObject1", plugins::FactoryT<MyObject1>);
 SCHNAPS_Plugin_AddFactoryM("MyObject2", plugins::FactoryT<MyObject2>);
 SCHNAPS_Plugin_EndDefinitionM();
 \endcode
 *
 *  The following lines of code allow loading the previous plugin compiled as file "MyPlugin.so":
 *
 \code
 try {
 plugins::Plugin lPlugin("MyPlugin.so");
 BasicObject* lMyObj1 = (BasicObject*)lPlugin.allocate("MyObject1");
 lMyObj1->someVirtualMethod();
 delete lMyObj1;
 lPlugin.unload();
 }
 catch(std::runtime_error& inError) {
 // Handle plugin exceptions here...
 }
 \endcode
 *
 *  The plugins::Plugin class also provides low-level static methods loadDynLib, unloadDynLib, and
 *  bindDynLibFunctionand, to respectively load or unload manually dynamic libraries, and bind
 *  on functions of loaded libraries. To define an exported, bindable functions in a dynamic library,
 *  be sure to add the SCHNAPS_Plugin_DynLibExport macro before its definition, as in the following
 *  example:
 *
 \code
 export "C" {
 SCHNAPS_Plugin_DynLibExport void myDynLibExportedFunction(int inAnArgument)
 {
 ...
 }
 }
 \endcode
 *
 *  Compiling of programs that can load dynamic libraries:
 *
 *  On Unices: programs that are loading dynamic libraries must be linked against the dl library
 *  (i.e. add the flag "-ldl" to the linker). Furthermore, it is a good idea to add the
 *  "-rdynamic" flag when linking with gcc in order to allow the loaded dynamic libraries to get
 *  access to the symbols of the running executable.
 *
 *  On Windows: it is important to link the program on the same kind of system libraries (i.e.
 *  Single Threaded Debug, Single Threaded DLL, Multi-threaded Debug, Multi-threaded DLL, etc.)
 *  that was used to generate the loaded dynamic libraries, as well as any others libraries on
 *  which the executable is linked against.
 *
 *  Compiling dynamic loadable libraries:
 *
 *  On Unices with gcc: all object files (.o files) that are making the shared libraries must
 *  be compiled with the "-fPIC" option (PIC = Position Independent Code). For more details, see
 *  http://www.tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html.
 *  It is also possible to set up an autoconf/automake project to appropriately generate and
 *  dynamically loadable library with little effort, for more details see
 *  http://www.gnu.org/software/automake/manual/html_mono/automake.html#A%20Shared%20Library.
 *
 *  On Windows: Microsoft Visual C++ allow the generation of DLL files quite easily, just follow
 *  the graphical wizard and set up your project to generate a DLL. Be cautious to link against the
 *  same type of system libraries (Single Threaded Debug, Multi-threaded DLL, etc.) that will be
 *  used to generate in the binaries that will load the DLL. For more details on DLL in
 *  Microsoft Visual C++, see
 *  http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vccore98/html/_SCHNAPS_Core_dlls.3a_.overview.asp.
 *
 */
class Plugin : public Object {
public:
	typedef std::map<std::string, Allocator::Handle> AllocatorMap;

	//! Event allocator type.
	typedef AllocatorT<Plugin, Object::Alloc> Alloc;
	//! Event handle type.
	typedef PointerT<Plugin, Object::Handle> Handle;
	//! Event bag type.
	typedef ContainerT<Plugin, Object::Bag> Bag;

	Plugin();
	~Plugin(){};

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Plugin");
			return lName;
		schnaps_StackTraceEndM("const std::string& Plugin::getName() const");
	}

	virtual void read(PACC::XML::ConstIterator inIter);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	/*!
	 *  \brief Get the name of the loaded plugin.
	 *  \return Name of the loaded plugin.
	 */
	inline const std::string& getLibName() const {
		return mLibName;
	}

	/*!
	 *  \brief Get the version of the loaded plugin.
	 *  \return Version of the loaded plugin.
	 */
	inline const std::string& getVersion() const {
		return mVersion;
	}

	/*!
	 *  \brief Get the source of the loaded plugin.
	 *  \return Source of the loaded plugin.
	 */
	inline const std::string& getSource() const {
		return mSource;
	}

	//! Lists all allocators contained in plugin.
	void listFactories(std::vector<std::string>& outAllocators) const;
	//! Get allocator of specified name.
	Allocator::Handle getAllocator(std::string inAllocName) const;

private:
	//! Load the dynamic library given by the filename in memory.
	void load(std::string inSource);
	//! Bind to a function of a dynamically loaded library.
	static void* bindDynLibFunction(void* inDynLib, std::string inFunctionName);
	//! Load a dynamic library into memory.
	static void* loadDynLib(std::string inFilename);

protected:
	Plugin(const Plugin&) {}			//!< Copy constructor forbiden.
	Plugin& operator=(const Plugin&) {} //!< Copy operator forbiden.

	AllocatorMap mAllocatorMap;			//!< Map of allocators contained by plugin.

	std::string mLibName;				//!< Name of the plugin.
	std::string mVersion;				//!< Version of the plugin.
	std::string mSource;				//!< Source of the plugin.
	void* mDynLib;						//!< Opaque handler of the dynamic lib.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Plugin_hpp
