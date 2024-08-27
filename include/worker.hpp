#pragma once

#include <vector>
#include <fstream>
#include <argparse.hpp>
#include <util.hpp>

namespace absctl {
  class worker {
  private:
    std::vector<std::string> packages;
    std::vector<std::string> all_packages;
    std::fstream config_fd;
    std::string filename;
    
    std::fstream open_config_file()  noexcept;
    void try_create_config() const noexcept;
    void save_config() noexcept;
    void parse_config_file() noexcept;
    void track_packages() noexcept;
    void untrack_packages(std::string& pkg) noexcept;
  public:
    worker() = default;
    worker(worker&&) = default;
    ~worker() = default;

    void add_packages(std::vector<argument>& args) noexcept;

    int process(arg_type work_type) noexcept;
  };
}