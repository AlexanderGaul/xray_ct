#ifndef DEBUGSTUFF_H
#define DEBUGSTUFF_H
#include <iostream>
#include <Eigen/Dense>

class Debug{
public:
    static void printVec222(Eigen::VectorXf vec){
        for(int i = 0; i < vec.size(); ++i){
            if(vec[i] >= 0.001 || vec[i] <= -0.001){
                std::cout << i << ": " << vec[i] << " ";
            }
        }
        std::cout << "\n\n";
    }
};

#endif // DEBUGSTUFF_H
