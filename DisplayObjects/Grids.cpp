/////////////////////////////////////////////////////////////////
/// @file      Grids.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
/// @brief     Provide a simple interface to draw a grid
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "Grids.h"
#include "Lines.h"

#include <osg/Geometry>

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> get(const GridVec_t& grids)
{
    LineVec_t lines;

    for ( const auto& grid : grids )
    {
        for ( double xx(-grid.halfSpan.x());
              xx<=grid.halfSpan.x();
              xx+=grid.spacing.x() )
        {
            lines.push_back
                ({
                    osg::Vec3(xx, -grid.halfSpan.y(), 0.0),
                    osg::Vec3(xx,  grid.halfSpan.y(), 0.0),
                    grid.color
                });
        }

        for ( double yy(-grid.halfSpan.y());
              yy<=grid.halfSpan.y();
              yy+=grid.spacing.y() )
        {
            lines.push_back
                ({
                    osg::Vec3(-grid.halfSpan.x(), yy, 0.0),
                    osg::Vec3( grid.halfSpan.x(), yy, 0.0),
                    grid.color
                });
        }
    }

    return get(lines);
};

} // namespace d3
