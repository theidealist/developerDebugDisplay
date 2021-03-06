/////////////////////////////////////////////////////////////////
/// @file      Lines.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
/// @brief     Provide a simple interface to draw a line
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "Lines.h"

#include <osg/Geometry>
#include <osg/Version>

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> get(const LineVec_t& lines)
{
    // define the color array - these are all the colors we need!
    osg::ref_ptr<osg::Vec4Array> osgColors( new osg::Vec4Array() );

    // here are the lines lto add
    osg::ref_ptr<osg::Vec3Array> verts( new osg::Vec3Array() );

    // color mapping
    osg::ref_ptr<osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 4, 4> >
        colorIndexArray(new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 4, 4>());

    // the actual line indices
    osg::ref_ptr<osg::DrawElementsUInt> theLines( new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0) );

    // add the lines and colors - only iterate the list once so we know we have
    // the right number of lines and colors
    unsigned int index(0);
    for ( const Line& line : lines )
    {
        verts->push_back(line.begin);
        osgColors->push_back(line.color);
        colorIndexArray->push_back(index);
        theLines->push_back(index++);

        verts->push_back(line.end);
        osgColors->push_back(line.color);
        colorIndexArray->push_back(index);
        theLines->push_back(index++);
    }

    // now add all this stuff to the geometry object
    osg::ref_ptr<osg::Geometry> cloudGeometry( new osg::Geometry() );
    cloudGeometry->setVertexArray(verts);
#if      OSG_MIN_VERSION_REQUIRED(3,2,0)
    cloudGeometry->setColorArray(osgColors, osg::Array::Binding::BIND_PER_VERTEX);
#else    // OSG_MIN_VERSION_REQUIRED(3,2,0)
    cloudGeometry->setColorArray(osgColors);
    cloudGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
#endif   // OSG_MIN_VERSION_REQUIRED(3,2,0)
    cloudGeometry->addPrimitiveSet(theLines);

    // set the state - line size and lighting
    cloudGeometry->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    // create and return the geode
    osg::ref_ptr<osg::Geode> geode(new osg::Geode());
    geode->addDrawable(cloudGeometry);
    return geode;
};

} // namespace d3
