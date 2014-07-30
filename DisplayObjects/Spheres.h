/////////////////////////////////////////////////////////////////
/// @file      Spheres.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.06.10
/// @brief     Provide a simple interface to draw a sphere
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#ifndef  _DDD__DISPLAY_OBJECTS__SPHERES_H_
#define  _DDD__DISPLAY_OBJECTS__SPHERES_H_

#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Node>

#include <vector>

namespace d3
{

/// @brief   Define a single sphere
struct Sphere
{
    /// The start of the sphere
    osg::Vec3 center;

    /// The radius of the sphere
    double radius;

    /// The color of the sphere
    osg::Vec4 color;
};

/// typedef a vector of spheres
typedef std::vector<Sphere> SphereVec_t;

/// @brief   Create an osg::Node from a vector of spheres
/// @param   spheres The vector of spheres to create the node from
/// @return  osg::ref_ptr<osg::Node> The displayable node
osg::ref_ptr<osg::Node> get(const SphereVec_t& spheres);

/// @brief   Create an osg::Node from a single sphere
/// @param   sphere The sphere to create the node from
/// @return  osg::ref_ptr<osg::Node> The displayable node
inline osg::ref_ptr<osg::Node> get(const Sphere& sphere)
{
    return get(SphereVec_t(1, sphere));
};

} // namespace d3

#endif   // _DDD__DISPLAY_OBJECTS__SPHERES_H_
