#pragma once

#include "core/exceptions/exception_handler.hpp"
#include <unordered_map>
#include <vector>
#include <fstream>

#include <core/logging/logging.hpp>
#include <parsers/argparse.hpp>
#include <util/util.hpp>


namespace absctl {
  struct configuration {
    std::string repo_directory;
    std::string makepkg_args;
    bool exit_after_error;
  };

  struct package {
    std::string name;
    std::string version;

    bool operator==(const package& pkg) const {
      return this->name == pkg.name;
    }
  };

  class worker {
  private:
    std::vector<package> packages;
    std::vector<package> all_packages;
    std::fstream config_fd;
    std::string filename;
    configuration config;
    logger& log;
    exception_handler& exc_handler;
    
    std::fstream open_config_file() noexcept;
    void get_all_packages() noexcept;
    void create_config() const noexcept;
    void save_config() noexcept;
    void parse_tracked_packages() noexcept;
    void track_packages() noexcept;
    void untrack_packages() noexcept;
    std::string get_package_version(const std::string& name) noexcept;
    std::unordered_map<std::string, std::string> get_all_versions() noexcept;
  public:
    worker(logger& log, exception_handler& exc_handler) : log(log), exc_handler(exc_handler) {}
    worker(configuration conf, logger& log, exception_handler& exc_handler) : config(conf), log(log), exc_handler(exc_handler) {}
    worker(worker&&) = default;
    ~worker() = default;

    void add_packages(std::vector<argument>& args) noexcept;
    void set_configuration(configuration& conf) noexcept;
    void set_logger(logger& log) noexcept;
    int process(arg_type work_type) noexcept;
  };
}
