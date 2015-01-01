#include <iostream>
#include <stdio.h>
#include "bfs.h"

using namespace std;

static void usage()
{
    cout <<
        "Usage: bfs [width] [height] [output_file_name] \n"
        "    Example: bfs 3 2 RightSolutions.rs\n"
        << endl;
}

int main(int argc, char *argv[])
{
    // Parse arguments
    if (argc != 4)
    {
        usage();
        return EXIT_FAILURE;
    }
    uint32_t width;
    sscanf(argv[1], "%d", &width);
    uint32_t height;
    sscanf(argv[2], "%d", &height);
    // Solve cases
    Puzzle puz(width,height);
    if (puz.SolveAll() == EXIT_FAILURE)
    {
        cout << "Error: " << "failed to solve cases!" << endl;
        return EXIT_FAILURE;
    }
    cout << "Cases successfully solved!" << endl;

    // Dump cases
    if (puz.DumpAllCases(argv[3]) == EXIT_FAILURE)
    {
        cout << "Error: " << "failed to dump the solution!" << endl;
        return EXIT_FAILURE;
    }
    cout << "Successfully dumped!" << endl;

    return EXIT_SUCCESS;
}