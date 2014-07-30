/////////////////////////////////////////////////////////////////
/// @file      HeightGrid.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.06.12
/// @brief     Provide a simple interface to draw a height grid
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "HeightGrid.h"

#include <osg/Geometry>
#include <osg/Geode>
#include <osg/ShapeDrawable>

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> get(const HeightGrid& heightGrid)
{
    // get the height from the points - is there a better way?
    uint height( heightGrid.points.size()/heightGrid.width );

    // create the heightfield
    osg::ref_ptr<osg::HeightField> heightField(new osg::HeightField());
    heightField->allocate(height, heightGrid.width);
    heightField->setXInterval(heightGrid.x_interval);
    heightField->setYInterval(heightGrid.y_interval);
    heightField->setOrigin(heightGrid.origin);

    // set all the heights from the point cloud
    auto iter(heightGrid.points.begin());
    for ( uint ii(0) ; ii<height ; ++ii )
        for ( uint jj(0) ; jj<heightGrid.width ; ++jj, ++iter )
            heightField->setHeight(ii,jj,iter->location.z());

    // create the shape
    osg::ref_ptr<osg::ShapeDrawable> heightDrawable( new osg::ShapeDrawable(heightField) );

    // set the color
    heightDrawable->setColor(heightGrid.color);

    // embed this in a geode
    osg::ref_ptr<osg::Geode> geode( new osg::Geode() );
    geode->addDrawable(heightDrawable);

    // Let the thing be transparent
    geode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
    geode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

    // return the geode
    return geode;
};

} // namespace d3
