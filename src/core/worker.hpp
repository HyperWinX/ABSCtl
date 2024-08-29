#pragma once

#include <vector>
#include <fstream>

#include <parsers/argparse.hpp>
#include <util/util.hpp>


namespace absctl {
  struct configuration {
    std::string repo_directory;
    std::string makepkg_args;
    bool exit_after_error;
  };

  class worker {
  private:
    std::vector<std::string> packages;
    std::vector<std::string> all_packages;
    std::fstream config_fd;
    std::string filename;
    configuration config;
    
    std::fstream open_config_file() noexcept;
    void get_all_packages() noexcept;
    void create_config() const noexcept;
    void save_config() noexcept;
    void parse_tracked_packages() noexcept;
    void track_packages() noexcept;
    void untrack_packages(std::string& pkg) noexcept;
  public:
    worker() = default;
    worker(configuration conf) : config(conf) {}
    worker(worker&&) = default;
    ~worker() = default;

    void add_packages(std::vector<argument>& args) noexcept;
    void set_configuration(configuration& conf) noexcept;
    int process(arg_type work_type) noexcept;
  };
}