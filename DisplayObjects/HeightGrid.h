/////////////////////////////////////////////////////////////////
/// @file      HeightGrid.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.06.12
/// @brief     Provide a simple interface to draw a height grid
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#ifndef  _DDD__DISPLAY_OBJECTS__HEIGHT_FIELD_H_
#define  _DDD__DISPLAY_OBJECTS__HEIGHT_FIELD_H_

#include "Points.h"

#include <osg/Node>

namespace d3
{

/// @brief   Define the things that make up a HeightGrid
/// @note    This is expected to be a well-formed rectangular grid of points
/// Form a HeightGrid set of points with something like this:
///
///   ddd::PointVec_t points;
///   osg::Vec4 color(1,0,1,0.3);
///   double step(0.01);
///   for ( double xx(-3.0) ; xx<=3.0 ; xx+=step )
///       for ( double yy(-3.0) ; yy<=3.0 ; yy+=step )
///           points.push_back(ddd::Point{osg::Vec3(xx,yy,perlinNoise.get(xx,yy)), color});
///
/// And so the width here is sqrt(points.size()) (since it's square)
struct HeightGrid
{
    /// The points for the grid
    PointVec_t points;

    /// The x-interval
    double     x_interval;

    /// The y-interval
    double     y_interval;

    /// The origin
    osg::Vec3  origin;

    /// The width of the grid of points
    uint       width;

    /// The color
    osg::Vec4  color;
};

/// @brief   get an osg node that is a height field built from a set of points
///          on a regularly spaced grid
/// @param   heightGrid The height field created that we should draw
osg::ref_ptr<osg::Node> get(const HeightGrid& heightGrid);

} // namespace d3

#endif   // _DDD__DISPLAY_OBJECTS__HEIGHT_FIELD_H_
