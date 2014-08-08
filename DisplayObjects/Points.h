/////////////////////////////////////////////////////////////////
/// @file      Points.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
/// @brief     Provide a simple interface to draw a point
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#ifndef  _DDD__DISPLAY_OBJECTS__POINTS_H_
#define  _DDD__DISPLAY_OBJECTS__POINTS_H_

#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Node>

namespace d3
{

/// @brief   Define a simple point
struct Point
{
    /// The location of the point
    osg::Vec3d location;

    /// The color of the point
    osg::Vec4 color;
};

/// typedef a vector of points
typedef std::vector<Point> PointVec_t;

/// @brief   get an osg node from a vector of points
/// @param   points The points to add
/// @param   size The size of all the points
osg::ref_ptr<osg::Node> get(const PointVec_t& points,
                            const float size = 3.0);

/// @brief   get an osg node
/// @param   point The point to add
/// @param   size The size of the point
inline osg::ref_ptr<osg::Node> get(const Point& point,
                                   const float size = 3.0)
{
    return get(PointVec_t(1, point), size);
};

} // namespace d3

#endif   // _DDD__DISPLAY_OBJECTS__POINTS_H_
