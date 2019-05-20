#ifndef EXTRAS_H
#define EXTRAS_H

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <mutex>

class extras
{
    public:
        template<typename R, typename F, typename... Args>
        static int functionTimer (R& funcRetun, F &func, Args&&... args)
        {
            auto clkstart = std::chrono::steady_clock::now();
            funcRetun = func (std::forward<Args>(args)...);
            auto clkfinish = std::chrono::steady_clock::now();

            return (std::chrono::duration_cast<std::chrono::nanoseconds>(clkfinish - clkstart)).count();
        }

        template<typename F, typename... Args>
        static int void_functionTimer (F &func, Args&&... args)
        {
            auto clkstart = std::chrono::steady_clock::now();
            func (std::forward<Args>(args)...);
            auto clkfinish = std::chrono::steady_clock::now();

            return (std::chrono::duration_cast<std::chrono::nanoseconds>(clkfinish - clkstart)).count();
        }

        template<class T>
        static void printMatrix (T matrix)
        {
            std::cout.setf(std::ios::fixed);
            std::cout.precision(8);

            std::for_each (matrix.begin(), matrix.end(), 
                [] (auto& i)
                {
                    std::for_each (i.begin(), i.end(), 
                        [] (auto& n)
                        {
                            std::cout << n << " ";
                        }
                    );
                    std::cout << std::endl;
                }
            );
        }

    private:
        static std::mutex mtx;
};

std::mutex extras::mtx;

#endif // EXTRAS_H