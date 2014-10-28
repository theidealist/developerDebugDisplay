/////////////////////////////////////////////////////////////////
/// @file      MeshGrid.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
/// @brief     Provide a simple interface to draw a mesh grid
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "MeshGrid.h"

#include <osg/Geometry>
#include <osg/Geode>
#include <osg/PolygonMode>
#include <osg/Version>

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> get(const MeshGrid& meshGrid)
{
    // build the vertex array
    osg::ref_ptr<osg::Vec3Array> vertices( new osg::Vec3Array() );
    vertices->reserve(meshGrid.points.size());

    // Construct the color array
    osg::ref_ptr<osg::Vec4Array> colors( new osg::Vec4Array() );
    colors->reserve(meshGrid.points.size());

    // add the points to the vetext and color array
    for ( const auto& pt : meshGrid.points )
    {
        vertices->push_back( pt.location );
        colors->push_back( pt.color );
    }

    // The normal array
    osg::ref_ptr<osg::Vec3Array> normals( new osg::Vec3Array() );
    for ( const auto& nn : meshGrid.normals )
        normals->push_back(nn);

    // Construct the polygon geometry
    osg::ref_ptr<osg::Geometry> polygon( new osg::Geometry() );
    polygon->setVertexArray( vertices.get() );
    polygon->setNormalArray( normals.get() );
#if      OSG_MIN_VERSION_REQUIRED(3,2,0)
    polygon->setColorArray( colors.get(), osg::Array::Binding::BIND_PER_VERTEX );
#else    // OSG_MIN_VERSION_REQUIRED(3,2,0)
    polygon->setColorArray(colors.get());
    polygon->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
#endif   // OSG_MIN_VERSION_REQUIRED(3,2,0)
    polygon->setNormalBinding( osg::Geometry::BIND_OVERALL );
    if ( not meshGrid.fill )
    {
        polygon->getOrCreateStateSet()->
            setAttribute(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,
                                              osg::PolygonMode::LINE));
    }
    polygon->getOrCreateStateSet()->setMode(GL_LIGHTING, 0);
    polygon->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
    polygon->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

    // build the thing
    for ( uint ii=0 ; ii<meshGrid.points.size()-meshGrid.width-1 ; ++ii )
    {
        if ( not ((ii+1) % meshGrid.width) ) ++ii;
        osg::ref_ptr<osg::DrawElementsUInt>
            pyramidBase(new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0));
        pyramidBase->push_back(ii);
        pyramidBase->push_back(ii+1);
        pyramidBase->push_back(ii+meshGrid.width+1);
        pyramidBase->push_back(ii+meshGrid.width);
        polygon->addPrimitiveSet(pyramidBase);
    }

    // create a geode for this
    osg::ref_ptr<osg::Geode> geode( new osg::Geode() );
    geode->addDrawable( polygon.get() );

    return geode;
};

} // namespace d3
