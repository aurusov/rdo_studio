#include <iostream>
#include <stdio.h>
#include "boost/program_options.hpp"
#include "bfs.h"

int main(int argc, char *argv[])
{
    namespace po = boost::program_options;
    boost::program_options::options_description desc("Допустимые опции");
    desc.add_options()
        ("help,h", "отобразить справку")
        ("width,W", po::value<std::uint32_t>(), "задать ширину игрового поля")
        ("height,H", po::value<std::uint32_t>(), "задать высоту игрового поля")
        ("filename,f", po::value<std::string>(), "задать имя выходного файла с решениями (RightSolutions.rs по умолчанию)")
    ;

    po::variables_map vm;
    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    }
    catch (const std::exception&)
    {
        return EXIT_FAILURE;
    }

    if (vm.count("help"))
    {
        std::cout << "RAO-studio game5 plugin unility" << std::endl;
        std::cout << "Использование: game5_gen [опции]" << std::endl;
        std::cout << desc << std::endl;
        return EXIT_SUCCESS;
    }

    std::uint32_t width;
    std::uint32_t height;
    if (vm.count("width") && vm.count("height"))
    {
        width = vm["width"].as<std::uint32_t>();
        height = vm["height"].as<std::uint32_t>();
    }
    else
    {
        std::cout << "Ширина и высота поля должны быть указаны!" << std::endl;
        std::cout << desc << std::endl;
        return EXIT_FAILURE;
    }

    const char* file_path = nullptr;
    if (vm.count("filename"))
    {
        file_path = vm["filename"].as<std::string>().c_str();
    }
    else
    {
        file_path = "RightSolutions.rs";
    }

    //Solve cases
    Puzzle puzzle(width,height);
    if (puzzle.solveAll() == EXIT_FAILURE)
    {
        std::cout << "Error: " << "failed to solve cases!" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Cases successfully solved!" << std::endl;

    // Dump cases
    if (puzzle.dumpAllCases(file_path) == EXIT_FAILURE)
    {
        std::cout << "Error: " << "failed to dump the solution!" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Successfully dumped!" << std::endl;

    return EXIT_SUCCESS;
}
