/////////////////////////////////////////////////////////////////
/// @file      Voxels.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2015.06.05
/// @brief     Provide a simple interface to draw a bunch of voxels
///
/// @attention Copyright (C) 2015
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#pragma once

#include <osg/Vec3d>
#include <osg/Vec4>
#include <osg/Node>

#include <vector>

namespace d3
{

/// @brief   Define a single voxel
/// @note    The min/max corners don't really need to be the min/max, but they
///          should be oposite corners
struct Voxel
{
    /// The corner closest to (0,0,0)
    osg::Vec3d minCorner;

    /// The corner furthest from (0,0,0)
    osg::Vec3d maxCorner;

    /// The color of the voxel
    osg::Vec4 color;
};

/// alias a vector of voxels
typedef std::vector<Voxel> VoxelVec_t;

/// @brief   get an osg node from a vector of voxels
/// @param   voxels The voxels we should draw
/// @return  The constructed node
osg::ref_ptr<osg::Node> get(const VoxelVec_t& voxels);

/// @brief   get an osg node from a single voxel
/// @param   voxel the voxel that we should draw
/// @return  The constructed node
inline osg::ref_ptr<osg::Node> get(const Voxel& voxel)
{
    return get(VoxelVec_t(1, voxel));
};

} // namespace d3

