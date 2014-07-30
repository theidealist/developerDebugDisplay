/////////////////////////////////////////////////////////////////
/// @file      MeshGrid.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
/// @brief     Provide a simple interface to draw a mesh grid
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#ifndef  _DDD__DISPLAY_OBJECTS__MESH_GRID_H_
#define  _DDD__DISPLAY_OBJECTS__MESH_GRID_H_

#include "Points.h"

#include <osg/Node>
#include <vector>

namespace d3
{

/// @brief   Define the things that make up a MeshGrid
/// @note    This is expected to be a well-formed rectangular grid of points
/// Form a MeshGrid set of points with something like this:
///
///   ddd::PointVec_t points;
///   osg::Vec4 color(1,0,1,0.3);
///   double step(0.01);
///   for ( double xx(-3.0) ; xx<=3.0 ; xx+=step )
///       for ( double yy(-3.0) ; yy<=3.0 ; yy+=step )
///           points.push_back(ddd::Point{osg::Vec3(xx,yy,perlinNoise.get(xx,yy)), color});
///
/// And so the width here is sqrt(points.size()) (since it's square)
struct MeshGrid
{
    /// The points for the grid
    PointVec_t points;

    /// The normals for each of the points
    std::vector<osg::Vec3> normals;

    /// The width of the grid of points
    uint       width;

    /// Flag to indicate if we should fill the grid (or just draw the
    bool       fill;
};

/// @brief   get an osg node that is a mesh built from a set of points on a
///          regularly spaced grid
/// @param   meshGrid The mesh grid created that we should draw
osg::ref_ptr<osg::Node> get(const MeshGrid& meshGrid);

} // namespace d3

#endif   // _DDD__DISPLAY_OBJECTS__GRIDS_H_
