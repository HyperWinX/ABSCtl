#include <alpm.h>
#include <alpm_list.h>

#include <core/exceptions/exception_handler.hpp>
#include <util/constants.hpp>

namespace absctl {
  class alpm_api {
  private:
    alpm_handle_t* handle;
    alpm_list_t* syncdbs;
    alpm_db_t* localdb;
    exception_handler& exc_handler;

  public:
    alpm_api(exception_handler& hndlr) : exc_handler(hndlr) {
      alpm_errno_t errno_;
      handle = alpm_initialize(ALPM_ROOT, ALPM_DB_PATH, &errno_);
      [[unlikely]] if (!handle)
        exc_handler._throw(exception_type::ALPM_INIT_FAIL);
        
      syncdbs = alpm_get_syncdbs(handle);
      auto _errno = alpm_errno(handle);
      auto str = alpm_strerror(_errno); 
      [[unlikely]] if (!syncdbs)
        exc_handler._throw(exception_type::ALPM_SYNCDB_INIT_FAIL);

      localdb = alpm_get_localdb(handle);
      [[unlikely]] if (!localdb)
        exc_handler._throw(exception_type::ALPM_LOCALDB_INIT_FAIL);
    }
    std::optional<std::string_view> get_pkg_version(std::string_view name);
  };
}