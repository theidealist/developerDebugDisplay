/////////////////////////////////////////////////////////////////
/// @file      Triads.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
/// @brief     Provide a simple interface to draw a triad
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#pragma once

#include <osg/Node>
#include <vector>

#include <osg/Matrix>

namespace d3
{

/// @brief   Define a single triad
///
/// Since a triad is really just a coordinate axis and we can define that in
/// Identity and then make the user actually provide the matrix to put the triad
/// into, this becomes pretty useless. So, we just encapsulate the Triad as an
/// osg::Matrix and the mount the actual triad in that transform
struct Triad
{
    /// The pose of the triad
    osg::Matrix pose;
};

/// typedef a vector of grids
typedef std::vector<Triad> TriadVec_t;

/// @brief   get an osg node from a vector of triads
/// @param   triads The triads we should draw
osg::ref_ptr<osg::Node> get(const TriadVec_t& grids, const double& scale = 1.0);

/// @brief   get an osg node from a single triad
/// @param   triad The triad that we should draw
inline osg::ref_ptr<osg::Node> get(const Triad& triad, const double& scale = 1.0)
{
    return get(TriadVec_t(1, triad), scale);
};

/// @brief   Convenience for drawing the origin
inline osg::ref_ptr<osg::Node> origin()
{
    return get(Triad{osg::Matrix::identity()}, 1.0);
};

} // namespace d3

