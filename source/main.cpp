#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <algorithm>
#include <iterator>
#include <omp.h>

#include "extras.h"

#include "Diffur.h"
#include "diffurSolve.h"

int main()
{
    Diffur diffur;

    std::vector<std::vector<double>> exactSolveResult;

    exactSolveResult = diffurSolve::calculateExactSolve(diffur);

    std::vector<std::vector<double>> serialSolveResult;
    int serialSolveExecutionTime = extras::functionTimer (serialSolveResult,
        diffurSolve::calculateSerialSolve, diffur);

    std::vector<std::vector<double>> parallelSolveResult;
    int parallelSolveExecutionTime = extras::functionTimer (parallelSolveResult,
        diffurSolve::calculateParallelSolve, diffur);


    double averageError = diffurSolve::calculateAverageError(diffur, exactSolveResult,
        serialSolveResult);
    double maxError = diffurSolve::calculateMaxError(diffur, exactSolveResult,
        serialSolveResult);
    double relAverageError = diffurSolve::calculateRelativeAverageError(diffur, exactSolveResult,
        serialSolveResult);
    double RelMaxError = diffurSolve::calculateRelativeMaxError(diffur, exactSolveResult,
        serialSolveResult);


    {
        std::cout << "Exact result:" << std::endl;
        extras::printMatrix (exactSolveResult);
        std::cout << std::endl << std::endl;

        std::cout << "Serial solved:" << std::endl;
        extras::printMatrix (serialSolveResult);
        std::cout << std::endl << std::endl;

        std::cout << "Parallel solved:" << std::endl;
        extras::printMatrix (parallelSolveResult);
        std::cout << std::endl << std::endl;

        std::cout << "Serial solving time:\t" << serialSolveExecutionTime << std::endl
            << "Parallel solving time:\t" << parallelSolveExecutionTime << std::endl;
        std::cout << std::endl << std::endl;

        std::cout << "Average error: " << averageError << std::endl;
        std::cout << "Max error: " << maxError << std::endl;

        std::cout << "Relative average error: " << relAverageError << "\%" << std::endl;
        std::cout << "Relative Max error: " << RelMaxError << "\%" << std::endl;
    }

    return 0;
}
