/////////////////////////////////////////////////////////////////
/// @file      Capsules.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.06.10
/// @brief     Provide a simple interface to draw a capsule
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "Capsules.h"

#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Geode>

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> get(const CapsuleVec_t& capsules)
{
    osg::ref_ptr<osg::Group> pAddToThisGroup(new osg::Group());
    for ( const auto& cc : capsules )
    {
        osg::Vec3 pp( cc.begin - cc.end );
        double height( pp.length() );
        if ( height < 0.000001 ) continue;

        osg::Vec3 center( osg::Vec3((cc.begin.x() + cc.end.x())/2.0,
                                    (cc.begin.y() + cc.end.y())/2.0,
                                    (cc.begin.z() + cc.end.z())/2.0) );

        // This is the default direction for the capsules to face in OpenGL
        static const osg::Vec3 ZZ( osg::Vec3(0,0,1) );

        // Get CROSS product (the axis of rotation)
        osg::Vec3 tt( ZZ^pp );

        // Get angle. length is magnitude of the vector
        double angle( acos( (ZZ * pp) / height) );

        // Create a capsule between the two points with the given radius
        osg::ref_ptr<osg::Capsule> capsule( new osg::Capsule(center, cc.radius, height) );
        capsule->setRotation(osg::Quat(angle, osg::Vec3(tt.x(), tt.y(), tt.z())));

        // A geode to hold the capsule
        osg::ref_ptr<osg::ShapeDrawable> capsuleDrawable( new osg::ShapeDrawable(capsule) );
        capsuleDrawable->setColor(cc.color);

        osg::ref_ptr<osg::Geode> geode( new osg::Geode() );
        geode->addDrawable(capsuleDrawable);

        // Set the color of the capsule that extends between the two points.
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
