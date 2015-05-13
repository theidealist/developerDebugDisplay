/////////////////////////////////////////////////////////////////
/// @file      Images.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2015.05.15
/// @brief     Display an images in 3D
///
/// @attention Copyright (C) 2015
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#pragma once

#include <osg/MatrixTransform>
#include <osg/Image>

#include <array>

namespace d3
{

/// @brief   Define a simple image display
struct Image
{
    /// Store the image to display
    osg::ref_ptr<osg::Image> image;

    /// The 4 corners of the image in a clockwise rotation around the image with
    /// the first corner being the position of the upper left corner
    std::array<osg::Vec3,4> corners;
};

/// The standard "lots of these things"
typedef std::vector<Image> ImageVec_t;

/// The get for the vector
osg::ref_ptr<osg::Node> get(const ImageVec_t& images);

/// The get for a single image
inline osg::ref_ptr<osg::Node> get(const Image& image)
{
    return get(ImageVec_t(1, image));
};

} // namespace d3
