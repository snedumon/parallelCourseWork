#ifndef DIFFURSOLVE_H
#define DIFFURSOLVE_H

#include <cmath>
#include <vector>

#include "Diffur.h"

class diffurSolve
{   
    public:
        static std::vector<std::vector<double>> calculateSerialSolve (Diffur& diffur)
        {
            int HPointsQuantity = diffur.getHPointsQuantity();
            int TPointsQuantity = diffur.getTPointsQuantity();
            double h = diffur.getH();
            double tau = diffur.getTau();

            double t = diffur.getT0() + diffur.getTau();
            double x = diffur.getX0();

            int HPointsQuantityWithoutRightBorder = HPointsQuantity-1;

            std::vector<std::vector<double>> result (TPointsQuantity,
                std::vector<double> (HPointsQuantity));

            for (size_t i = 0; i < HPointsQuantity; i++, x += h)
            {
                result[0][i] = diffur.calculateBottomBorder(x);
            }


            for (size_t i = 1; i < TPointsQuantity; i++, t += tau)
            {
                result[i][0] = diffur.calculateLeftBorder(t);

                for (size_t j = 1;
                    j < HPointsQuantityWithoutRightBorder; j++)
                {
                    result[i][j] = diffur.calculateApproximateSolution(
                        result[i-1][j-1], result[i-1][j], result[i-1][j+1]
                    );
                }
            result[i][HPointsQuantity-1] = diffur.calculateRightBorder(t);
            }

            return result;
        }

        static std::vector<std::vector<double>> calculateParallelSolve (Diffur& diffur)
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


            for (size_t i = 0; i < HPointsQuantity; i++, x += h)
            {
                result[0][i] = diffur.calculateBottomBorder(x);
            }

            for (size_t i = 1; i < TPointsQuantity; i++, t += tau)
            {
                result[i][0] = diffur.calculateLeftBorder(t);
                #pragma omp parallel for
                for (size_t j = 1;
                    j < HPointsQuantityWithoutRightBorder; j++)
                {
                    
                    // std::cout << omp_get_thread_num() << std::endl;

                    result[i][j] = diffur.calculateApproximateSolution(
                        result[i-1][j-1], result[i-1][j], result[i-1][j+1]
                    );
                }
                result[i][HPointsQuantity-1] = diffur.calculateRightBorder(t);
            }

            return result;
        }

        static std::vector<std::vector<double>> calculateExactSolve (Diffur& diffur)
        {
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

                for (int j = 0; j < diffur.getHPointsQuantity(); j++)
                {
                        result[i][j] = diffur.calculateTrueSolution(x, t);
                        x += diffur.getH();
                }

                t += diffur.getTau();
            }

            return result;
        }

        static double calculateAverageError (Diffur diffur, std::vector<std::vector<double>>& exactSolveResult,
            std::vector<std::vector<double>>& anotherResult)
        {
            int TPointsQuantity = diffur.getTPointsQuantity();
            int HPointsQuantity = diffur.getHPointsQuantity();

            auto errorMatrix = diffurSolve::getErrorMatrix(diffur, exactSolveResult, anotherResult);
                
            double error = matrixAbsoluteAverageValue(errorMatrix);
            return error;
        }

        static double calculateMaxError (Diffur diffur, std::vector<std::vector<double>>& exactSolveResult,
            std::vector<std::vector<double>>& anotherResult)
        {
            int TPointsQuantity = diffur.getTPointsQuantity();
            int HPointsQuantity = diffur.getHPointsQuantity();

            auto errorMatrix = diffurSolve::getErrorMatrix(diffur, exactSolveResult, anotherResult);
            
            double error = matrixMaxValue(errorMatrix);

            return error;
        }

        static double calculateRelativeAverageError (Diffur diffur, std::vector<std::vector<double>>& exactSolveResult,
            std::vector<std::vector<double>>& anotherResult)
        {
            int TPointsQuantity = diffur.getTPointsQuantity();
            int HPointsQuantity = diffur.getHPointsQuantity();

            auto errorMatrix = diffurSolve::getRelativeErrorMatrix(diffur, exactSolveResult, anotherResult);
                
            double error = matrixAbsoluteAverageValue(errorMatrix);

            return error;
        }

        static double calculateRelativeMaxError (Diffur diffur, std::vector<std::vector<double>>& exactSolveResult,
            std::vector<std::vector<double>>& anotherResult)
        {

            auto errorMatrix = diffurSolve::getRelativeErrorMatrix(diffur, exactSolveResult, anotherResult);

            double error = matrixMaxValue(errorMatrix);
            
            return error;
        }

    private:
        static double matrixMaxValue (std::vector<std::vector<double>>& matrix)
        {
            double max =  matrix[0][0];;

            double matrixRowSize = matrix.size();
            double matrixColumnSize = matrix[0].size();

            for (int i = 0; i < matrixRowSize; i++)
            {
                for (int j = 0; j < matrixColumnSize; j++)
                {
                    if (matrix[i][j] > max)
                    {
                        max = matrix[i][j];
                    }
                }
            }

            return max;
        }

        static double matrixAbsoluteAverageValue (std::vector<std::vector<double>>& matrix)
        {
            double matrixRowSize = matrix.size();
            double matrixColumnSize = matrix[0].size();

            double sum = 0.0;

            for (int i = 0; i < matrixRowSize; i++)
            {
                for (int j = 0; j < matrixColumnSize; j++)
                {
                    sum += matrix[i][j];
                }
            }
            
            return sum / (matrixRowSize * matrixColumnSize);
        }

        static std::vector<std::vector<double>> getErrorMatrix (Diffur diffur,
            std::vector<std::vector<double>>& exactSolveResult, std::vector<std::vector<double>>& anotherSolveResult)
        {
            int TPointsQuantity = diffur.getTPointsQuantity();
            int HPointsQuantity = diffur.getHPointsQuantity();

            std::vector<std::vector<double>> errorMatrix (TPointsQuantity, 
                std::vector<double> (HPointsQuantity));

            if (exactSolveResult.size() == anotherSolveResult.size() &&
                exactSolveResult[0].size() == anotherSolveResult[0].size())
            {
                for (int i = 0; i < TPointsQuantity; i++) 
                {
                    for (int j = 0; j < HPointsQuantity; j++)
                    {
                        errorMatrix[i][j] = fabs(exactSolveResult[i][j] - anotherSolveResult[i][j]);
                    }
                }

            } else
            {
                std::cout << "Results have different sizes!!!" << std::endl;
            }

            return errorMatrix;
        }

        static std::vector<std::vector<double>> getRelativeErrorMatrix (Diffur diffur,
            std::vector<std::vector<double>>& exactSolveResult, std::vector<std::vector<double>>& anotherSolveResult)
        {
            int TPointsQuantity = diffur.getTPointsQuantity();
            int HPointsQuantity = diffur.getHPointsQuantity();

            std::vector<std::vector<double>> errorMatrix (TPointsQuantity, 
                std::vector<double> (HPointsQuantity));

            if (exactSolveResult.size() == anotherSolveResult.size() &&
                exactSolveResult[0].size() == anotherSolveResult[0].size())
            {
                for (int i = 0; i < TPointsQuantity; i++) 
                {
                    for (int j = 0; j < HPointsQuantity; j++)
                    {
                        errorMatrix[i][j] = fabs(exactSolveResult[i][j] - anotherSolveResult[i][j]) * 100;
                    }
                }

            } else
            {
                std::cout << "Results have different sizes!!!" << std::endl;
            }

            return errorMatrix;
        }

};

#endif // DIFFURSOLVE_H