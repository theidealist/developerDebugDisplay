/////////////////////////////////////////////////////////////////
/// @file      ScreenshotCallback.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.28
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "ScreenshotCallback.h"

#include <osgDB/WriteFile>

#include <iostream>
#include <iomanip>
#include <sstream>

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
ScreenshotCallback::ScreenshotCallback(GLenum glBuffer,
                                       const std::string& basename /* = snap_ */ ) :
    m_glBuffer(glBuffer),
    m_basename(basename),
    m_image(),
    m_mutex(),
    m_continuousCapture(false),
    m_singleSnapshot(false)
{
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
ScreenshotCallback::~ScreenshotCallback()
{
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void ScreenshotCallback::setCapture(bool capture)
{
    m_continuousCapture = capture;
    std::cout << "m_captureFrames is set to "
              << (m_continuousCapture?"true":"false")
              << std::endl;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void ScreenshotCallback::grab()
{
    m_singleSnapshot = true;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void ScreenshotCallback::operator()(osg::RenderInfo& renderInfo) const
{
    if ( not m_image ) const_cast<ScreenshotCallback*>(this)->m_image = new osg::Image();

    if ( m_continuousCapture || m_singleSnapshot )
    {
        m_singleSnapshot = false;
        glReadBuffer(m_glBuffer);

        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
        osg::GraphicsContext* graphicsContext =
            renderInfo.getState()->getGraphicsContext();
        if ( graphicsContext && (graphicsContext->getTraits()) )
        {
            GLenum pixelFormat;

            if (graphicsContext->getTraits()->alpha) pixelFormat = GL_RGBA;
            else                                     pixelFormat = GL_RGB;

            int width = graphicsContext->getTraits()->width;
            int height = graphicsContext->getTraits()->height;
            m_image->readPixels(0, 0, width, height, pixelFormat, GL_UNSIGNED_BYTE);

            std::string fname(m_basename);

            static uint frameCount(0);
            std::stringstream ss;
            ss << fname << std::setw(6) << std::setfill('0')
               << frameCount++ << ".ppm";

            std::cout << "Saving frame to file: " << ss.str() << std::endl;
            osgDB::writeImageFile(*m_image, ss.str());
        }
    }
};

} // namespace d3
