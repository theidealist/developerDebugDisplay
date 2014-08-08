/////////////////////////////////////////////////////////////////
/// @file      Cylinders.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.06.10
/// @brief     Provide a simple interface to draw a cylinder
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#ifndef  _DDD__DISPLAY_OBJECTS__CYLINDERS_H_
#define  _DDD__DISPLAY_OBJECTS__CYLINDERS_H_

#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Node>

#include <vector>

namespace d3
{

/// @brief   Define a single cylinder
struct Cylinder
{
    /// The start of the cylinder
    osg::Vec3d begin;

    /// The end of the cylinder
    osg::Vec3d end;

    /// The radius of the cylinder
    double radius;

    /// The color of the cylinder
    osg::Vec4 color;
};

/// typedef a vector of cylinders
typedef std::vector<Cylinder> CylinderVec_t;

/// @brief   Create an osg::Node from a vector of cylinders
/// @param   cylinders The vector of Cylinders to create the node from
/// @return  osg::ref_ptr<osg::Node> The displayable node
osg::ref_ptr<osg::Node> get(const CylinderVec_t& cylinders);

/// @brief   Create an osg::Node from a single cylinder
/// @param   cylinder The Cylinder to create the node from
/// @return  osg::ref_ptr<osg::Node> The displayable node
inline osg::ref_ptr<osg::Node> get(const Cylinder& cylinder)
{
    return get(CylinderVec_t(1, cylinder));
};

} // namespace d3

#endif   // _DDD__DISPLAY_OBJECTS__CYLINDERS_H_
