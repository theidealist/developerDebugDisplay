/////////////////////////////////////////////////////////////////
/// @file      Lines.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
/// @brief     Provide a simple interface to draw a line
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#ifndef  _DDD__DISPLAY_OBJECTS__LINES_H_
#define  _DDD__DISPLAY_OBJECTS__LINES_H_

#include <osg/Geode>
#include <osg/Vec3>
#include <osg/Vec4>

#include <vector>

namespace d3
{

/// @brief   Define a single line
struct Line
{
    /// The beginning of the line
    osg::Vec3 begin;

    /// The end of the line
    osg::Vec3 end;

    /// The color of the line
    osg::Vec4 color;
};

/// typedef a vector of lines
typedef std::vector<Line> LineVec_t;

/// @brief   get an osg node from a vector of lines
/// @param   lines The lines we should draw
/// @return  The built node
osg::ref_ptr<osg::Node> get(const LineVec_t& lines);

/// @brief   get an osg node from a single line
/// @param   line the line that we should draw
/// @return  The built node
inline osg::ref_ptr<osg::Node> get(const Line& line)
{
    return get(LineVec_t(1, line));
};

} // namespace d3

#endif   // _DDD__DISPLAY_OBJECTS__LINES_H_
