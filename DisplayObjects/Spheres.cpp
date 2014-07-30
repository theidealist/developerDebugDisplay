/////////////////////////////////////////////////////////////////
/// @file      Spheres.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.06.10
/// @brief     Provide a simple interface to draw a sphere
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "Spheres.h"

#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Geode>

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> get(const SphereVec_t& spheres)
{
    osg::ref_ptr<osg::Group> pAddToThisGroup(new osg::Group());
    for ( const auto& cc : spheres )
    {
        // Create a sphere between the two points with the given radius
        osg::ref_ptr<osg::Sphere> sphere( new osg::Sphere(cc.center, cc.radius) );

        // A geode to hold the sphere
        osg::ref_ptr<osg::ShapeDrawable> sphereDrawable( new osg::ShapeDrawable(sphere) );
        sphereDrawable->setColor(cc.color);
        osg::ref_ptr<osg::Geode> geode( new osg::Geode() );
        geode->addDrawable(sphereDrawable);

        // Set the color of the sphere that extends between the two points.
//         osg::ref_ptr<osg::Material> pMaterial( new osg::Material() );
//         pMaterial->setDiffuse( osg::Material::FRONT, cc.color);
//         geode->getOrCreateStateSet()->setAttribute( pMaterial, osg::StateAttribute::OVERRIDE );
        geode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
        geode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
        pAddToThisGroup->addChild(geode);
    }

    return pAddToThisGroup;
}

} // namespace d3
