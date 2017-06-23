#include "ForwardProjectionOperator.h"


std::vector<float> ForwardProjectionOperator::forwardProject(std::size_t row)
{
    std::vector<float> ret;
    ret.reserve(_pose->getPixelHorizontal());

    for(int x = 0; x < _pose->getPixelHorizontal(); ++x)
    {
        ret.push_back(forwardProject(row, x));
    }

    return ret;
}

float ForwardProjectionOperator::forwardProject(std::size_t row, std::size_t col)
{
    return RayTracing::forwardProject(_volume, _pose->getRay(row, col));
}

std::vector<float> ForwardProjectionOperator::forwardProject ()
{
    std::vector<float> ret;
    ret.reserve(_pose->getPixelVertical() * _pose->getPixelHorizontal());
    for(int x = 0; x < _pose->getPixelHorizontal(); ++x){
        for(int y = 0; y < _pose->getPixelVertical(); ++y){
            ret.push_back(RayTracing::forwardProject(_volume, _pose->getRay(x, y)));
        }
    }
    return ret;
}


std::vector<std::vector<float>> ForwardProjectionOperator::forwardProjectFull()
{
    int ANGLES = 1;

    float ry = _pose->getRotationLocalY();
    float rz = _pose->getRotationGlobalZ();

    float rotation = rz;

    std::vector<std::vector<float>> ret;
    ret.reserve(ANGLES);
    for(int i = 0; i<ANGLES; i++)
    {
        _pose->setRotation(rotation+(i/ANGLES)*2*M_PI, ry);
        ret.push_back(forwardProject());
    }

    _pose->setRotation(rz, ry);

    return ret;
}
