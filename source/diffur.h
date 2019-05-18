#ifndef DIFFUR_H
#define DIFFUR_H

#include <string>
#include <cmath>

class Diffur
{
    private: 
        double x0 = 0;
        double x1 = 1;
        double h = 1.0/30;
        double t0 = 0;
        double t1 = 1;
        double tau = 1.0/900;
        double a = 5;
        double A = 2;

        double getSigma () {
            return tau / pow(h, 2);
        }

    public:
        double getX0 () { return this->x0; }
        double getX1 () { return this->x1; }
        double getH () { return this->h; }
        double getT0 () { return this->t0; }
        double getT1 () { return this->t1; }
        double getTau () { return this->tau; }

        int getTPointsQuantity () {
            return ceil((this->t1 - this->t0) / this->tau) + 1;
        }

        int getHPointsQuantity () {
            return ceil((this->t1 - this->t0) / this->h) + 1;
        }

        double calculateTrueSolution (double x, double t) {
            return 1 / (1 + this->A * exp(0.5 * pow(2, 0.5) * x + 
                0.5 * (2 * this->a) * t));
        }

        double calculateBottomBorder (double x) {
            return 1 / (1 + this->A * exp(0.5 * pow(2, 0.5) * x));
        }

        double calculateLeftBorder (double t) {
            return 1 / (1 + this->A * exp( 0.5 * (2 * this->a) * t));
        }

        double calculateRightBorder (double t) {
            return  1 / (1 + this->A * exp(0.5 * pow(2, 0.5) + 0.5 * (2 * this->a) * t));
        }

        double calculateApproximateSolution(double wLeft, double wCurrent , double wRight) {
            return wCurrent + this->tau * ((wLeft - 2 * wCurrent + wRight) / (this->h * this->h) 
                - wCurrent * this->a + pow(wCurrent, 2.0) + a * pow(wCurrent, 2.0) - pow(wCurrent, 3.0));
        }

};

#endif // DIFFUR_H