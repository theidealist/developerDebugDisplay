/////////////////////////////////////////////////////////////////
/// @file      Cones.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2015.06.30
/// @brief     Provide a simple interface to draw a cone
///
/// @attention Copyright (C) 2015
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#pragma once

#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Node>

#include <vector>

namespace d3
{

/// @brief   Define a single cone
struct Cone
{
    /// The Center of the bottom of the cone
    osg::Vec3d center;

    /// The radius of the bottom of the cone
    double radius;

    /// The height of the cone
    double height;

    /// The color of the cone
    osg::Vec4 color;
};

/// typedef a vector of cones
typedef std::vector<Cone> ConeVec_t;

/// @brief   Create an osg::Node from a vector of cones
/// @param   cones The vector of cones to create the node from
/// @return  osg::ref_ptr<osg::Node> The displayable node
osg::ref_ptr<osg::Node> get(const ConeVec_t& cones);

/// @brief   Create an osg::Node from a single cone
/// @param   cone The cone to create the node from
/// @return  osg::ref_ptr<osg::Node> The displayable node
inline osg::ref_ptr<osg::Node> get(const Cone& cone)
{
    return get(ConeVec_t(1, cone));
};

} // namespace d3

