//****************************************************************************//
// coreskeleton.h                                                             //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#pragma once

#include <list>
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include "cal3d/global.h"
#include "cal3d/vector.h"

typedef boost::shared_ptr<class CalCoreBone> CalCoreBonePtr;

class CAL3D_API CalCoreSkeleton {
public:
    size_t addCoreBone(const CalCoreBonePtr& pCoreBone);
    CalCoreBone* getCoreBone(const std::string& strName);
    void scale(float factor);

    std::vector<CalCoreBonePtr> coreBones;
    std::vector<size_t> rootBoneIds;
    CalVector sceneAmbientColor;

private:
    size_t getCoreBoneId(const std::string& strName);
    bool mapCoreBoneName(size_t coreBoneId, const std::string& strName);
    std::map<std::string, size_t> m_mapCoreBoneNames;
};

typedef boost::shared_ptr<CalCoreSkeleton> CalCoreSkeletonPtr;
