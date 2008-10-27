/***********************************************************************
 Amethyst-GL
  - Module Management implementations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "module.h"
#include "global.h"

#include <boost/bind.hpp>

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif


namespace amethyst {
namespace client {

static const std::string blank("NULL");

/// Module Class Functions
Module::Module(const std::string &module_name, const std::string &module_path)
    : name_(module_name),
      path_(module_path)
{
    std::cout << "Loading Module: " <<  module_path << std::endl;

    /// Loading Module
    #ifdef WIN32
    LoadLibrary (module_path.c_str());
    #else
    dl_handle_ = dlopen(path_.c_str(), RTLD_NOW | RTLD_LOCAL);
    std::cout << dlerror() << std::endl;

    /// if dl_handle == NULL, Throw()
    #endif

    /// Getting Entry Points
    #ifdef WIN32

    #else
    mod_start_ = reinterpret_cast<Mod_Start_Func>(dlsym(dl_handle_, "mod_start"));
    mod_stop_  = reinterpret_cast<Mod_Stop_Func> (dlsym(dl_handle_, "mod_stop"));
    mod_get_name_    = reinterpret_cast<Mod_Get_Name_Func>(dlsym(dl_handle_, "mod_get_name"));
    mod_get_version_ = reinterpret_cast<Mod_Get_Version_Func>(dlsym(dl_handle_, "mod_get_version"));
    mod_is_active_   = reinterpret_cast<Mod_Is_Active_Func>  (dlsym(dl_handle_, "mod_is_active"));
    #endif
}

Module::~Module()
{
    if (mod_stop_)
        mod_stop_();

    #ifdef WIN32
    LoadLibrary (module_path.c_str());
    #else
    if (dl_handle_)
        dlclose(dl_handle_);
    #endif
}

bool Module::start(Amethyst_GL &agl)
{
    std::cout << "Starting Module: " << name() << std::endl;

    if(mod_start_)
        return mod_start_(agl);

    return false;
}

bool Module::stop()
{
    if(mod_stop_)
        return mod_stop_();

    return false;
}

const std::string& Module::mod_name()
{
    if(mod_get_name_)
        return mod_get_name_();

    return blank;
}

const std::string& Module::name()
{
    return name_;
}

bool Module::is_active()
{
    if(mod_is_active_)
        return mod_is_active_();

    return true;
}

/// Module Manager Class Functions
void Module_Manager::reg(Module_ptr module)
{
    modules_.insert(module);
    std::cout << "Module: " << module->name() << " Registered." << std::endl;
}

void Module_Manager::unreg(Module_ptr module)
{
    modules_.erase(module);
    std::cout << "Module: " << module->name() << " Unregistered." << std::endl;
}

void Module_Manager::start(const std::string &name,Amethyst_GL &agl)
{
    /// Find Module name "name"
    std::set<Module_ptr>::iterator mod =
            std::find_if(modules_.begin(), modules_.end(),  (bind(&Module::name, _1) == name) );

    /// Call start on it.
    if(mod != modules_.end())
        (*mod)->start(agl);
    else
        std::cout << "Starting Module Failed: Module, " << name << ", not found!" << std::endl;

}

void Module_Manager::stop(const std::string &name)
{
    /// Find Module name "name"
    std::set<Module_ptr>::iterator mod =
            std::find_if(modules_.begin(), modules_.end(),  (bind(&Module::name, _1) == name) );

    /// Call start on it.
    if(mod != modules_.end())
        (*mod)->stop();
    else
        std::cout << "Stopping Module Failed: Module, " << name << ", not found!" << std::endl;

}

void Module_Manager::stop_all(void)
{
    /// Stop Each Module
    std::for_each(modules_.begin(), modules_.end(),  bind(&Module::stop, _1) );

}

bool Module_Manager::load(const std::string &module_name)
{
    /// Resolve path to module
    #ifdef WIN32
    std::string soext = ".dll";
    #else
    std::string soext = ".so";
    #endif

    std::string module_path = Global.dir_modules + "lib" + module_name + soext;

    /// Verify Module

    /// Load module
    Module_ptr module;
    try
    {
        module = Module_ptr(new Module(module_name, module_path));
    }
    catch (...)
    {
    }

    modules_.insert(module);

    return true;
}


void Module_Manager::unload_all(void)
{
    stop_all();

    modules_.clear();

}

/// Module Manager Instantiated
Module_Manager module_manager;

}
}
