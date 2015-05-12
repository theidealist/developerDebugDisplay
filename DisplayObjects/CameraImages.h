/////////////////////////////////////////////////////////////////
/// @file      CameraImages.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2015.02.10
/// @brief     Display an images in 3D
///
/// @attention Copyright (C) 2015
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#ifndef  _DDD__DISPLAY_OBJECTS__CAMERA_IMAGE_H_
#define  _DDD__DISPLAY_OBJECTS__CAMERA_IMAGE_H_

#include <osg/MatrixTransform>
#include <osg/Image>

namespace d3
{

/// @brief   Define a simple image display - in 3D, this implies that there will
///          be a simulated camera viewing this image, so we have a camera pose
///          as part of it
///
/// The image type supported are really just
struct CameraImage
{
    /// Store the camera pose
    osg::Matrix cameraPose;

    /// Store the image to display
    osg::ref_ptr<osg::Image> image;

    /// Store some camera parameters indicating how to display the image
    double focalLengthX_pix;
    double focalLengthY_pix;
    double scale;
};

/// The standard "lots of these things"
typedef std::vector<CameraImage> CameraImageVec_t;

/// The get for the vector
osg::ref_ptr<osg::Node> get(const CameraImageVec_t& images);

/// The get for a single image
inline osg::ref_ptr<osg::Node> get(const CameraImage& image)
{
    return get(CameraImageVec_t(1, image));
};

} // namespace d3

#endif   // _DDD__DISPLAY_OBJECTS__CAMERA_IMAGE_H_
