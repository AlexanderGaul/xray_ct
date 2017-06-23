#include "ForwardProjectionOperator.h"


std::vector<float> ForwardProjectionOperator::forwardProject(std::size_t row)
{
    std::vector<float> ret;
    ret.reserve(_pose->getPixelHorizontal());

    for(int x = 0; x < _pose->getPixelHorizontal(); ++x)
    {
        ret.push_back(RayTracing::forwardProject(_volume, _pose->getRay(x, row)));
    }

    return ret;
}

float ForwardProjectionOperator::forwardProject(std::size_t row, std::size_t col)
{
    return RayTracing::forwardProject(_volume, _pose->getRay(col, row));
}

std::vector<std::vector<float>> ForwardProjectionOperator::forwardProject (){
    std::vector<std::vector<float>> ret;
    ret.reserve(_pose->getPixelVertical());
    for(int y = 0; y < _pose->getPixelVertical(); ++y){
        ret.push_back(forwardProject(y));
    }
    return ret;
}
