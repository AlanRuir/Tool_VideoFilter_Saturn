#ifndef __COMMON_COMMON_TYPE_H__
#define __COMMON_COMMON_TYPE_H__

#include <iostream>

enum FilterType : uint8_t
{
    NONE,
    CROP,
    USPP,
    EDGEDETECT,
    DRAWTEXT,
    SCALE,
    HFLIP,
    VFLIP
};

#endif  // __COMMON_COMMON_TYPE_H__