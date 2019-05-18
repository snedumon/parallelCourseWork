#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <string>
#include <algorithm>
#include <iterator>
#include <omp.h>

#include "extras.h"
#include "diffur.h"

std::vector<std::vector<double>> serialSolve (Diffur diffur);

std::vector<std::vector<double>> parallelSolve (Diffur diffur);

std::vector<std::vector<double>> exactSolve (Diffur diffur);


int main() {
    Diffur diffur;

    std::vector<std::vector<double>> exactSolveResult;

    exactSolveResult = exactSolve(diffur);

    std::vector<std::vector<double>> serialSolveResult;

    int serialSolveExecutionTime = extras::functionTimer (serialSolveResult, serialSolve, diffur);

    std::vector<std::vector<double>> parallelSolveResult;

    std::cout << "Serial solved:" << std::endl;
    extras::printMatrix (serialSolveResult);

    int parallelSolveExecutionTime = extras::functionTimer (parallelSolveResult, parallelSolve, diffur);

    std::cout << "Parallel solved:" << std::endl;
    extras::printMatrix (parallelSolveResult);

    std::cout << "Serial solving time:\t" << serialSolveExecutionTime << std::endlfor (size_t i = 1; i < TPointsQuantity; i++, t += tau) {
            result[i][0] = diffur.calculateLeftBorder(t);

            #pragma omp for
            for (size_t j = 1;
                j < HPointsQuantityWithoutRightBorder; j++) {

                result[i][j] = diffur.calculateApproximateSolution(
                    result[i-1][j-1], result[i-1][j], result[i-1][j+1]
                );
            }

            result[i][HPointsQuantity-1] = diffur.calculateRightBorder(t);
        }
        << "Parallel solving time:\t" << parallelSolveExecutionTime << std::endl;

    return 0;
}


std::vector<std::vector<double>> serialSolve (Diffur diffur) {
    int HPointsQuantity = diffur.getHPointsQuantity();
    int TPointsQuantity = diffur.getTPointsQuantity();
    double h = diffur.getH();
    double tau = diffur.getTau();

    double t = diffur.getT0() + diffur.getTau();
    double x = diffur.getX0();

    int HPointsQuantityWithoutRightBorder = HPointsQuantity-1;

    std::vector<std::vector<double>> result (TPointsQuantity,
        std::vector<double> (HPointsQuantity));
 
    for (size_t i = 0; i < HPointsQuantity; i++, x += h) {
        result[0][i] = diffur.calculateBottomBorder(x);
    }

    for (size_t i = 1; i < TPointsQuantity; i++, t += tau) {
        result[i][0] = diffur.calculateLeftBorder(t);
    
        for (size_t j = 1;
            j < HPointsQuantityWithoutRightBorder; j++) {
            result[i][j] = diffur.calculateApproximateSolution(
                result[i-1][j-1], result[i-1][j], result[i-1][j+1]
            );
        }
    result[i][HPointsQuantity-1] = diffur.calculateRightBorder(t);
    }

    return result;
}

std::vector<std::vector<double>> parallelSolve (Diffur diffur)
{
    int HPointsQuantity = diffur.getHPointsQuantity();
    int TPointsQuantity = diffur.getTPointsQuantity();
    double h = diffur.getH();
    double tau = diffur.getTau();
    double t = diffur.getT0() + diffur.getTau();
    double x = diffur.getX0();

    std::vector<std::vector<double>> result (TPointsQuantity,
        std::vector<double> (HPointsQuantity));
    
    int HPointsQuantityWithoutRightBorder = HPointsQuantity-1;

    
    for (size_t i = 0; i < HPointsQuantity; i++, x += h) {
        result[0][i] = diffur.calculateBottomBorder(x);
    }
    
    #pragma omp parallel
    {
        for (size_t i = 1; i < TPointsQuantity; i++, t += tau) {
            result[i][0] = diffur.calculateLeftBorder(t);

            #pragma omp for
            for (size_t j = 1;
                j < HPointsQuantityWithoutRightBorder; j++) {

                result[i][j] = diffur.calculateApproximateSolution(
                    result[i-1][j-1], result[i-1][j], result[i-1][j+1]
                );
            }

            result[i][HPointsQuantity-1] = diffur.calculateRightBorder(t);
        }
    }

    return result;
}

std::vector<std::vector<double>> exactSolve (Diffur diffur) {

    double HPointsQuantity = diffur.getHPointsQuantity();
    double TPointsQuantity = diffur.getTPointsQuantity();
    double h = diffur.getH();
    double tau = diffur.getTau();
    double t = diffur.getT0();
    double x = diffur.getX0();

    std::vector<std::vector<double>> result (TPointsQuantity,
        std::vector<double> (HPointsQuantity));

    for (int i = 0; i < diffur.getTPointsQuantity(); i++)
    {
        double x = diffur.getX0();

        for (int j = 0; j < diffur.getHPointsQuantity(); j++) {
                result[i][j] = diffur.calculateTrueSolution(x, t);
                x += diffur.getH();
        }

        t += diffur.getTau();
    }

    return result;
} 
