/////////////////////////////////////////////////////////////////
/// @file      Capsules.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.06.10
/// @brief     Provide a simple interface to draw a capsule
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#pragma once

#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Node>

#include <vector>

namespace d3
{

/// @brief   Define a single capsule
struct Capsule
{
    /// The start of the capsule
    osg::Vec3d begin;

    /// The end of the capsule
    osg::Vec3d end;

    /// The radius of the capsule
    double radius;

    /// The color of the capsule
    osg::Vec4 color;
};

/// typedef a vector of capsules
typedef std::vector<Capsule> CapsuleVec_t;

/// @brief   Create an osg::Node from a vector of capsules
/// @param   capsules The vector of capsules to create the node from
/// @return  osg::ref_ptr<osg::Node> The displayable node
osg::ref_ptr<osg::Node> get(const CapsuleVec_t& capsules);

/// @brief   Create an osg::Node from a single capsule
/// @param   capsule The capsule to create the node from
/// @return  osg::ref_ptr<osg::Node> The displayable node
inline osg::ref_ptr<osg::Node> get(const Capsule& capsule)
{
    return get(CapsuleVec_t(1, capsule));
};

} // namespace d3

