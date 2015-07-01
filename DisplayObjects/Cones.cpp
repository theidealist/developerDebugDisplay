/////////////////////////////////////////////////////////////////
/// @file      Cones.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2015.06.30
/// @brief     Provide a simple interface to draw a cone
///
/// @attention Copyright (C) 2015
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "Cones.h"

#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Geode>

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> get(const ConeVec_t& cones)
{
    osg::ref_ptr<osg::Group> pAddToThisGroup(new osg::Group());
    for ( const auto& cc : cones )
    {
        const osg::Vec3d& center( cc.center );
        const double& radius( cc.radius );
        const double& height( cc.height );

        // Create a cone between the two points with the given radius
        osg::ref_ptr<osg::Cone> cone( new osg::Cone(center, radius, height) );

        // A geode to hold the cone
        osg::ref_ptr<osg::ShapeDrawable> coneDrawable( new osg::ShapeDrawable(cone) );
        coneDrawable->setColor(cc.color);

        osg::ref_ptr<osg::Geode> geode( new osg::Geode() );
        geode->addDrawable(coneDrawable);

        // Set the color of the cone that extends between the two points.
        geode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
        geode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
        pAddToThisGroup->addChild(geode);
    }

    return pAddToThisGroup;
}

} // namespace d3
