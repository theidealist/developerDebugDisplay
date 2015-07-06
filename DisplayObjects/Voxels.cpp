/////////////////////////////////////////////////////////////////
/// @file      Voxels.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2015.06.05
/// @brief     Provide a simple interface to draw a bunch of voxels
///
/// @attention Copyright (C) 2015
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "Voxels.h"
#include "Lines.h"

#include <osg/Geometry>
#include <osg/Version>

#include <algorithm>

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> get(const VoxelVec_t& voxels)
{
    // here are all the edges of all the cells
    LineVec_t cellEdges;
    cellEdges.reserve( cellEdges.size() + voxels.size() );
    
    for ( const auto& vv : voxels )
    {
        // make the 8 corners of the cell
        osg::Vec3d corner0{vv.minCorner.x(), vv.minCorner.y(), vv.minCorner.z()};
        osg::Vec3d corner1{vv.maxCorner.x(), vv.minCorner.y(), vv.minCorner.z()};
        osg::Vec3d corner2{vv.maxCorner.x(), vv.maxCorner.y(), vv.minCorner.z()};
        osg::Vec3d corner3{vv.minCorner.x(), vv.maxCorner.y(), vv.minCorner.z()};
        osg::Vec3d corner4{vv.minCorner.x(), vv.minCorner.y(), vv.maxCorner.z()};
        osg::Vec3d corner5{vv.maxCorner.x(), vv.minCorner.y(), vv.maxCorner.z()};
        osg::Vec3d corner6{vv.maxCorner.x(), vv.maxCorner.y(), vv.maxCorner.z()};
        osg::Vec3d corner7{vv.minCorner.x(), vv.maxCorner.y(), vv.maxCorner.z()};

        // top of this cell
        cellEdges.emplace_back(Line{corner4, corner5, vv.color});
        cellEdges.emplace_back(Line{corner5, corner6, vv.color});
        cellEdges.emplace_back(Line{corner6, corner7, vv.color});
        cellEdges.emplace_back(Line{corner7, corner4, vv.color});

        // bottom-to-top for this cell
        cellEdges.emplace_back(Line{corner0, corner4, vv.color});
        cellEdges.emplace_back(Line{corner1, corner5, vv.color});
        cellEdges.emplace_back(Line{corner2, corner6, vv.color});
        cellEdges.emplace_back(Line{corner3, corner7, vv.color});
        
        // bottom of this cell
        cellEdges.emplace_back(Line{corner0, corner1, vv.color});
        cellEdges.emplace_back(Line{corner1, corner2, vv.color});
        cellEdges.emplace_back(Line{corner2, corner3, vv.color});
        cellEdges.emplace_back(Line{corner3, corner0, vv.color});
    }

    // sort the edges so we can remove duplicates
    std::sort(cellEdges.begin(), cellEdges.end(),
              [](const Line& edge0, const Line& edge1)
              {
                  osg::Vec3d mid0{ (edge0.begin + edge0.end)/2.0 };
                  double dist0{ mid0.x()*mid0.x() + mid0.y()*mid0.y() + mid0.z()*mid0.z() };

                  osg::Vec3d mid1{ (edge1.begin + edge1.end)/2.0 };
                  double dist1{ mid1.x()*mid1.x() + mid1.y()*mid1.y() + mid1.z()*mid1.z() };
                  return (dist0 < dist1);
              });

    // now uniquify the edges
    auto newEndIter =
        std::unique(cellEdges.begin(), cellEdges.end(),
                    [](const Line& edge0, const Line& edge1)
                    {
                        // the beginning and end of the two edges
                        const auto& b0( edge0.begin );
                        const auto& e0( edge0.end );
                        const auto& b1( edge1.begin );
                        const auto& e1( edge1.end );

                        // an arbitrary scale for comparison
                        static const double scale(1000.0);

                        // The "int" version of of the beginning/end * scale
                        const int b0x( static_cast<int>(b0.x() * scale) );
                        const int b0y( static_cast<int>(b0.y() * scale) );
                        const int b0z( static_cast<int>(b0.z() * scale) );
                        const int e0x( static_cast<int>(e0.x() * scale) );
                        const int e0y( static_cast<int>(e0.y() * scale) );
                        const int e0z( static_cast<int>(e0.z() * scale) );

                        const int b1x( static_cast<int>(b1.x() * scale) );
                        const int b1y( static_cast<int>(b1.y() * scale) );
                        const int b1z( static_cast<int>(b1.z() * scale) );
                        const int e1x( static_cast<int>(e1.x() * scale) );
                        const int e1y( static_cast<int>(e1.y() * scale) );
                        const int e1z( static_cast<int>(e1.z() * scale) );

                        // see if the beginning of the edge and end of the
                        // edge are the same (accurate to 1/scale
                        if ( (b0x == b1x) && (b0y == b1y) && (b0z == b1z) &&
                             (e0x == e1x) && (e0y == e1y) && (e0z == e1z) )
                        {
                            // these are the same edge
                            return true;
                        }

                        // check if the edge are the same, but just reversed
                        if ( (b0x == e1x) && (b0y == e1y) && (b0z == e1z) &&
                             (e0x == b1x) && (e0y == b1y) && (e0z == b1z) )
                        {
                            return true;
                        }

                        // these are not the same edge
                        return false;
                    });
    
     // remove the duplicates
    cellEdges.resize( std::distance(cellEdges.begin(), newEndIter) );

    // return the created edges as voxels
    return get(cellEdges);
};

} // namespace d3
