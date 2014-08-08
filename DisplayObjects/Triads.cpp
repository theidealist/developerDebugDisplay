/////////////////////////////////////////////////////////////////
/// @file      Triads.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
/// @brief     Provide a simple interface to draw a triad
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "Triads.h"
#include "Lines.h"

#include <osg/Geometry>

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> get(const TriadVec_t& triads,
                            const double& scale /* = 1.0 */)
{
    LineVec_t lines;

    for ( const auto& triad : triads )
    {
        // x-axis
        lines.push_back
            ({
                osg::Vec3d(-0.1*scale,  0.0,  0.0) * triad.pose,
                osg::Vec3d( 1.0*scale,  0.0,  0.0) * triad.pose,
                osg::Vec4( 1.0,  0.0,  0.0, 1.0)
            });

        // y-axis
        lines.push_back
            ({
                osg::Vec3d( 0.0, -0.1*scale,  0.0) * triad.pose,
                osg::Vec3d( 0.0,  1.0*scale,  0.0) * triad.pose,
                osg::Vec4( 0.0,  1.0,  0.0, 1.0)
            });

        // z-axis
        lines.push_back
            ({
                osg::Vec3d( 0.0, 0.0,-0.1*scale) * triad.pose,
                osg::Vec3d( 0.0, 0.0, 1.0*scale) * triad.pose,
                osg::Vec4( 0.0, 0.0, 1.0, 1.0)
            });
    }

    return get(lines);
};

} // namespace d3
