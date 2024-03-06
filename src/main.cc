#include <boost/program_options.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

#include "entrypoint.h"

namespace po = boost::program_options;

constexpr auto GPU_STR = "GPU";
constexpr auto CPU_STR = "GPU";
constexpr auto ITER = 100'000'000;

po::options_description define_options()
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "show usage");
    desc.add_options()
        ("exec,e", po::value<std::string>(),
         "Either CPU or GPU");
    desc.add_options()
        ("iter,i", po::value<unsigned long long>()->default_value(ITER),
         "Number of iterations for pi computation");
    desc.add_options()
        ("test,t", "Compare CPU and GPU implementations for safety checks");
    return desc;
}

po::variables_map parse_options(const po::options_description& desc, int argc,
        char** argv)
{
    po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    return vm;
}

int run(const po::options_description& desc, const po::variables_map& vm)
{
    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }
    if (vm["iter"].empty()) {
        std::cerr << "[ERROR] Missing input !" << std::endl;
        return 1;
    }

    if (!vm["iter"].empty()) {
        if (!vm["exec"].empty() ) {
            if (vm["exec"].as<std::string>() == CPU_STR) {
                executeCpu(vm["iter"].as<unsigned long long>());
            } else if (vm["exec"].as<std::string>() == GPU_STR) {
                executeGpu(vm["iter"].as<unsigned long long>());
            }
        }
        if (!vm["test"].empty()) {
            executeTest(vm["iter"].as<unsigned long long>());
        }
    }
    return 0;
}

int main(int argc, char** argv)
{
    try {
        auto const& desc = define_options();
        auto const& vm = parse_options(desc, argc, argv);
        return run(desc, vm);
    } catch (po::error &e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return 2;
    }
}