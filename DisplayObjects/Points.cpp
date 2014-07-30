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

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> get(const PointVec_t& points,
                            const float size)
{
    // the color array
    osg::ref_ptr<osg::Vec4Array> osgColors( new osg::Vec4Array() );

    // make the color map
    typedef std::map<unsigned int, unsigned int> colorMap_t;
    colorMap_t colorMap;
    unsigned int inx(0);
    std::pair<colorMap_t::iterator, bool> insertResult;
    for ( const Point& pt : points )
    {
        insertResult = colorMap.insert(std::make_pair(toIndex(pt.color[0],
                                                              pt.color[1],
                                                              pt.color[2]), inx));
         // if we successfully inserted a new color
        if ( insertResult.second )
        {
            osgColors->push_back(pt.color);
            ++inx;
        }
    }

    // here are the points to add
    osg::ref_ptr<osg::Vec3Array> verts( new osg::Vec3Array() );

    // color mapping
    osg::ref_ptr<osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 4, 4> >
        colorIndexArray( new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 4, 4>() );

    // the actual cloud index
    osg::ref_ptr<osg::DrawElementsUInt> theCloud( new osg::DrawElementsUInt(osg::PrimitiveSet::POINTS, 0) );

    // add the points and colors - only iterate the list once so we know we have
    // the right number of points and colors
    unsigned int index(0);
    for ( const Point& pt : points )
    {
        verts->push_back(pt.location);
        colorIndexArray->push_back(colorMap.at(toIndex(pt.color[0],
                                                       pt.color[1],
                                                       pt.color[2])));
        theCloud->push_back(index++);
    }

    // now add all this stuff to the geometry object
    osg::ref_ptr<osg::Geometry> cloudGeometry( new osg::Geometry() );
    cloudGeometry->setVertexArray(verts);
    cloudGeometry->setColorArray(osgColors);
    cloudGeometry->setColorIndices(colorIndexArray);
    cloudGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
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
