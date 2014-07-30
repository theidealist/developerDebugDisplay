/////////////////////////////////////////////////////////////////
/// @file      Grids.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
/// @brief     Provide a simple interface to draw a grid
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#ifndef  _DDD__DISPLAY_OBJECTS__GRIDS_H_
#define  _DDD__DISPLAY_OBJECTS__GRIDS_H_

#include <osg/Vec2>
#include <osg/Vec4>
#include <osg/Node>

#include <vector>

namespace d3
{

/// @brief   Define a single grid
/// The grid is always defined, centered at 0,0,0 in the x/y plane. Simply place
/// in a MatrixTransform to locate/orient it differently
struct Grid
{
    /// The x/y spacing
    osg::Vec2 spacing;

    /// The half-span (i.e. cover from -hs to hs
    osg::Vec2 halfSpan;

    /// The color of the grid
    osg::Vec4 color;
};

/// typedef a vector of grids
typedef std::vector<Grid> GridVec_t;

/// @brief   get an osg node from a vector of grids
/// @param   grids The grids we should draw
osg::ref_ptr<osg::Node> get(const GridVec_t& grids);

/// @brief   get an osg node from a single grid
/// @param   grid The grid that we should draw
inline osg::ref_ptr<osg::Node> get(const Grid& grid)
{
    return get(GridVec_t(1, grid));
};

/// @brief   Convenience function to get a ground plane
inline osg::ref_ptr<osg::Node> ground(const double& spacing = 0.5,
                                      const double& halfSpan = 5.0,
                                      const osg::Vec4& color = osg::Vec4(0.0, 0.2, 0.0, 1.0))
{
    return get(Grid{osg::Vec2(spacing, spacing),
                    osg::Vec2(halfSpan, halfSpan),
                    color});
};

} // namespace d3

#endif   // _DDD__DISPLAY_OBJECTS__GRIDS_H_
