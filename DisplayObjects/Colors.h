/////////////////////////////////////////////////////////////////
/// @file      Colors.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
/// @brief     Declares the Colors
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#pragma once

#include <osg/Vec4>
#include <vector>

namespace d3
{

/////////////////////////////////////////////////////////////////
/// @brief   Class to manage a simplified color list
/////////////////////////////////////////////////////////////////
class Colors
{
  public:
    /// @brief   Constructor
    /// @param   transparency The transparency of the color
    Colors(const double& transparency = 1.0);

    /// @brief   Destructor
    ~Colors() {};

    /// @brief   Shorthand for getting the next color
    /// @return  osg::Vec4 const reference to the next color
    const osg::Vec4 next();

    /// @brief   To get the same last color again
    /// @return  osg::Vec4 const The reference to the current color
    const osg::Vec4 get() { return *m_itt; };

  private:

    /// @brief   Method to setup the color list initially
    void SetupColorMap(const double& transparency);

    /// The vector of colors
    std::vector<osg::Vec4> m_colors;

    /// The iterator pointing to the current entry in the color map
    std::vector<osg::Vec4>::const_iterator m_itt;
};

/// @brief   Provide a method to get the next color in a static sense
inline osg::Vec4 nextColor()
{
    static Colors colors;
    return colors.next();
};

/// @{
/// @name    Some standard colors by name
const osg::Vec4& black();
const osg::Vec4& white();

const osg::Vec4& red();
const osg::Vec4& green();
const osg::Vec4& blue();

const osg::Vec4& yellow();
const osg::Vec4& magenta();
const osg::Vec4& cyan();
/// @}

/// @brief    change an rgb color to an index
unsigned int toIndex(const double rr,
                     const double gg,
                     const double bb);

} // namespace d3

