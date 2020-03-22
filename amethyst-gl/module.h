#ifndef AMETHYST_CLIENT_MODULE_H
#define AMETHYST_CLIENT_MODULE_H

/***********************************************************************
 Amethyst-GL
  - Module Management declarations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/


#ifdef WIN32
 #ifndef _WIN32_WINNT
  #define _WIN32_WINNT 0x0501
 #endif
#endif

#include "amethyst-gl.h"

#include <string>
#include <set>

#include <memory>

namespace amethyst {
namespace client {


class Module
{
   public:
    typedef std::shared_ptr<Module> sptr;
    typedef std::weak_ptr<Module> wptr;

    Module(const std::string &module_name, const std::string &module_path);
    ~Module();

    bool start(Amethyst_GL &);
    bool stop();

    const std::string& mod_name();
    const std::string& name();

    bool is_active();

    typedef bool               (*Mod_Start_Func)    (Amethyst_GL &);
    typedef bool               (*Mod_Stop_Func)     ();
    typedef const std::string& (*Mod_Get_Name_Func)   ();
    typedef const std::string& (*Mod_Get_Version_Func)();
    typedef bool               (*Mod_Is_Active_Func)  ();

   private:
    std::string name_;
    std::string path_;

    /// Module Entry Pointers
    Mod_Start_Func  mod_start_;
    Mod_Stop_Func   mod_stop_;
    Mod_Get_Name_Func    mod_get_name_;
    Mod_Get_Version_Func mod_get_version_;
    Mod_Is_Active_Func   mod_is_active_;

    #ifdef WIN32
    HMODULE dl_handle_;
    #else 
    void *dl_handle_;
    #endif
};

class Module_Manager
{
   public:
    void reg(Module::sptr);
    void unreg(Module::sptr);

    void start(const std::string &module_name, Amethyst_GL &);
    void stop(const std::string &module_name);
    void stop_all();

    bool load(const std::string &module_name);
    bool unload(const std::string &module_name);
    void unload_all();

   private:
    std::set<Module::sptr> modules_;
};

/// Module Manager
/// All modules are required to register with the module manager
extern Module_Manager module_manager;

} //namespace client
} //namespace amethyst

#endif // AMETHYST_CLIENT_MODULE_H
