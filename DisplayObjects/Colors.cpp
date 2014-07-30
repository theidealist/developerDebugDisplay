/////////////////////////////////////////////////////////////////
/// @file      Colors.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
/// @brief     Declares the Colors
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "Colors.h"

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
Colors::Colors(const double& transparency /* = 1.0 */) :
    m_colors(),
    m_itt()
{
    SetupColorMap(transparency);
    m_itt = m_colors.begin();
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
const osg::Vec4 Colors::next()
{
    osg::Vec4 rv(*m_itt);

    // move to the next color
    ++m_itt;

    // see if we are pointing at the end and reset if we are
    if ( m_colors.end() == m_itt )
        m_itt = m_colors.begin();

    return rv;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
const osg::Vec4& black()
{
    static const osg::Vec4 bb(0.0, 0.0, 0.0, 1.0);
    return bb;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
const osg::Vec4& white()
{
    static const osg::Vec4 ww(1.0, 1.0, 1.0, 1.0);
    return ww;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
const osg::Vec4& red()
{
    static const osg::Vec4 rr(1.0, 0.0, 0.0, 1.0);
    return rr;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
const osg::Vec4& green()
{
    static const osg::Vec4 gg(0.0, 1.0, 0.0, 1.0);
    return gg;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
const osg::Vec4& blue()
{
    static const osg::Vec4 bb(0.0, 0.0, 1.0, 1.0);
    return bb;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
const osg::Vec4& yellow()
{
    static const osg::Vec4 yy(1.0, 1.0, 0.0, 1.0);
    return yy;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
const osg::Vec4& magenta()
{
    static const osg::Vec4 mm(1.0, 0.0, 1.0, 1.0);
    return mm;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
const osg::Vec4& cyan()
{
    static const osg::Vec4 cc(0.0, 1.0, 1.0, 1.0);
    return cc;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void Colors::SetupColorMap(const double& transparency)
{
    m_colors.reserve(64);

    // these colors are derived from the standard colors from Matlab
    m_colors.push_back(osg::Vec4(     0,         0,    1.0000, transparency));
    m_colors.push_back(osg::Vec4(0,    0.5000,         0,      transparency));
    m_colors.push_back(osg::Vec4(1.0000,         0,         0, transparency));
    m_colors.push_back(osg::Vec4(0,    0.7500,    0.7500,      transparency));
    m_colors.push_back(osg::Vec4(0.7500,         0,    0.7500, transparency));
    m_colors.push_back(osg::Vec4(0.7500,    0.7500,         0, transparency));
    m_colors.push_back(osg::Vec4(0.2500,    0.2500,    0.2500, transparency));
    m_colors.push_back(osg::Vec4(0,         0,    1.0000,      transparency));
    m_colors.push_back(osg::Vec4(0,    0.5000,         0,      transparency));
    m_colors.push_back(osg::Vec4(1.0000,         0,         0, transparency));
    m_colors.push_back(osg::Vec4(0,    0.7500,    0.7500,      transparency));
    m_colors.push_back(osg::Vec4(0.7500,         0,    0.7500, transparency));
    m_colors.push_back(osg::Vec4(0.7500,    0.7500,         0, transparency));
    m_colors.push_back(osg::Vec4(0.2500,    0.2500,    0.2500, transparency));
    m_colors.push_back(osg::Vec4(0,         0,    1.0000,      transparency));
    m_colors.push_back(osg::Vec4(0,    0.5000,         0,      transparency));
    m_colors.push_back(osg::Vec4(1.0000,         0,         0, transparency));
    m_colors.push_back(osg::Vec4(0,    0.7500,    0.7500,      transparency));
    m_colors.push_back(osg::Vec4(0.7500,         0,    0.7500, transparency));
    m_colors.push_back(osg::Vec4(0.7500,    0.7500,         0, transparency));
    m_colors.push_back(osg::Vec4(0.2500,    0.2500,    0.2500, transparency));
    m_colors.push_back(osg::Vec4(0,         0,    1.0000,      transparency));
    m_colors.push_back(osg::Vec4(0,    0.5000,         0,      transparency));
    m_colors.push_back(osg::Vec4(1.0000,         0,         0, transparency));
    m_colors.push_back(osg::Vec4(0,    0.7500,    0.7500,      transparency));
    m_colors.push_back(osg::Vec4(0.7500,         0,    0.7500, transparency));
    m_colors.push_back(osg::Vec4(0.7500,    0.7500,         0, transparency));
    m_colors.push_back(osg::Vec4(0.2500,    0.2500,    0.2500, transparency));
    m_colors.push_back(osg::Vec4(0,         0,    1.0000,      transparency));
    m_colors.push_back(osg::Vec4(0,    0.5000,         0,      transparency));
    m_colors.push_back(osg::Vec4(1.0000,         0,         0, transparency));
    m_colors.push_back(osg::Vec4(0,    0.7500,    0.7500,      transparency));
    m_colors.push_back(osg::Vec4(0.7500,         0,    0.7500, transparency));
    m_colors.push_back(osg::Vec4(0.7500,    0.7500,         0, transparency));
    m_colors.push_back(osg::Vec4(0.2500,    0.2500,    0.2500, transparency));
    m_colors.push_back(osg::Vec4(0,         0,    1.0000,      transparency));
    m_colors.push_back(osg::Vec4(0,    0.5000,         0,      transparency));
    m_colors.push_back(osg::Vec4(1.0000,         0,         0, transparency));
    m_colors.push_back(osg::Vec4(0,    0.7500,    0.7500,      transparency));
    m_colors.push_back(osg::Vec4(0.7500,         0,    0.7500, transparency));
    m_colors.push_back(osg::Vec4(0.7500,    0.7500,         0, transparency));
    m_colors.push_back(osg::Vec4(0.2500,    0.2500,    0.2500, transparency));
    m_colors.push_back(osg::Vec4(0,         0,    1.0000,      transparency));
    m_colors.push_back(osg::Vec4(0,    0.5000,         0,      transparency));
    m_colors.push_back(osg::Vec4(1.0000,         0,         0, transparency));
    m_colors.push_back(osg::Vec4(0,    0.7500,    0.7500,      transparency));
    m_colors.push_back(osg::Vec4(0.7500,         0,    0.7500, transparency));
    m_colors.push_back(osg::Vec4(0.7500,    0.7500,         0, transparency));
    m_colors.push_back(osg::Vec4(0.2500,    0.2500,    0.2500, transparency));
    m_colors.push_back(osg::Vec4(0,         0,    1.0000,      transparency));
    m_colors.push_back(osg::Vec4(0,    0.5000,         0,      transparency));
    m_colors.push_back(osg::Vec4(1.0000,         0,         0, transparency));
    m_colors.push_back(osg::Vec4(0,    0.7500,    0.7500,      transparency));
    m_colors.push_back(osg::Vec4(0.7500,         0,    0.7500, transparency));
    m_colors.push_back(osg::Vec4(0.7500,    0.7500,         0, transparency));
    m_colors.push_back(osg::Vec4(0.2500,    0.2500,    0.2500, transparency));
    m_colors.push_back(osg::Vec4(0,         0,    1.0000,      transparency));
    m_colors.push_back(osg::Vec4(0,    0.5000,         0,      transparency));
    m_colors.push_back(osg::Vec4(1.0000,         0,         0, transparency));
    m_colors.push_back(osg::Vec4(0,    0.7500,    0.7500,      transparency));
    m_colors.push_back(osg::Vec4(0.7500,         0,    0.7500, transparency));
    m_colors.push_back(osg::Vec4(0.7500,    0.7500,         0, transparency));
    m_colors.push_back(osg::Vec4(0.2500,    0.2500,    0.2500, transparency));
    m_colors.push_back(osg::Vec4(0,         0,    1.0000,      transparency));
};

/// @brief    change an rgb color to an index
unsigned int toIndex(const double rr,
                     const double gg,
                     const double bb)
{
    return (static_cast<unsigned int>(rr*255.0)
            + (static_cast<unsigned int>(gg*255.0)*256
               + static_cast<unsigned int>(bb*255.0)*256)*256);
};

} // namespace d3
