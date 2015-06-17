/////////////////////////////////////////////////////////////////
/// @file      ScreenshotCallback.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.28
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#pragma once

#include <osgViewer/Viewer>

namespace d3
{

/////////////////////////////////////////////////////////////////
/// @brief   Capture a frame of the viewer and save to disk
/////////////////////////////////////////////////////////////////
class ScreenshotCallback : public osg::Camera::DrawCallback
{
  public:

    /// @brief   Constructor
    /// @param   glBuffer The gl buffer to grab
    /// @param   basename The base name to save to disk
    ScreenshotCallback(GLenum glBuffer,
                       const std::string& basename = "snap_");

    /// @brief   Destructor
    virtual ~ScreenshotCallback();

    /// @brief   Method to toggle the frame capture on and off
    void setCapture(bool capture);

    /// @brief   Do a single frame snapshot grab
    void grab();

    /// @brief   Do the work...
    void operator() (osg::RenderInfo& renderInfo) const;

  protected:

    ///
    GLenum                      m_glBuffer;

    ///
    std::string                 m_basename;

    ///
    osg::ref_ptr<osg::Image>    m_image;

    ///
    mutable OpenThreads::Mutex  m_mutex;

    ///
    bool                        m_continuousCapture;

    ///
    mutable bool                m_singleSnapshot;
};

} // namespace d3

