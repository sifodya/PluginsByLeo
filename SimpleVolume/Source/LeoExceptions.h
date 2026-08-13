//
// Created by vboxuser on 06.08.2026.
//

#pragma once
#include <stdexcept>

namespace LeoExceptions
{
    enum LEO_RETURN
    {
        LEO_SUCCESS = 0,
        LEO_ERROR = 1,
        LEO_FAILURE = 2,
        LEO_INVALID_ARGUMENT = 3,
        LEO_NOT_FOUND = 4,
        LEO_NOT_IMPLEMENTED = 5,
        LEO_OUT_OF_RANGE = 6,
        LEO_OUT_OF_MEMORY = 7,
        LEO_OUT_OF_BOUNDS = 8
    };

    class LeoThrow : public std::exception
    {

    };
}
