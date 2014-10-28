/////////////////////////////////////////////////////////////////
/// @file      Points.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
/// @brief     Provide a simple interface to draw a point
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "Colors.h"
#include "Points.h"

#include <osg/Geometry>
#include <osg/Point>
#include <osg/Geode>
#include <osg/Version>

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> get(const PointVec_t& points,
                            const float size)
{
    // the color array
    osg::ref_ptr<osg::Vec4Array> osgColors( new osg::Vec4Array() );
    osgColors->reserve(points.size());

    // the vertex array
    osg::ref_ptr<osg::Vec3Array> verts( new osg::Vec3Array() );
    verts->reserve(points.size());

    // the actual cloud indices
    osg::ref_ptr<osg::DrawElementsUInt>
        theCloud( new osg::DrawElementsUInt(osg::PrimitiveSet::POINTS, 0) );
    theCloud->reserveElements(points.size());

    // add the points and colors - only iterate the list once so we know we have
    // the right number of points and colors
    unsigned int index(0);
    for ( const Point& pt : points )
    {
        verts->push_back(pt.location);
        osgColors->push_back(pt.color);
        theCloud->push_back(index++);
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
    cloudGeometry->addPrimitiveSet(theCloud);

    // set the state - point size and lighting
    osg::ref_ptr<osg::StateSet> cloudStateSet( cloudGeometry->getOrCreateStateSet() );
    cloudStateSet->setAttribute(new osg::Point(size), osg::StateAttribute::ON);
    cloudStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    // build the geode to return
    osg::ref_ptr<osg::Geode> geode(new osg::Geode());
    geode->addDrawable(cloudGeometry);
    return geode;
};

} // namespace d3
