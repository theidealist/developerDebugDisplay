/////////////////////////////////////////////////////////////////
/// @file      Cylinders.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.06.10
/// @brief     Provide a simple interface to draw a cylinder
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "Cylinders.h"

#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Geode>

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> get(const CylinderVec_t& cylinders)
{
    osg::ref_ptr<osg::Geode> geode( new osg::Geode() );
    for ( const auto& cc : cylinders )
    {
        osg::Vec3d pp( cc.begin - cc.end );
        double height( pp.length() );
        if ( height < 0.000001 ) continue;

        osg::Vec3d center( (cc.begin.x() + cc.end.x())/2.0,
                           (cc.begin.y() + cc.end.y())/2.0,
                           (cc.begin.z() + cc.end.z())/2.0 );

        // This is the default direction for the cylinders to face in OpenGL
        static const osg::Vec3d ZZ( 0,0,1 );

        // Get CROSS product (the axis of rotation)
        osg::Vec3d tt( ZZ^pp );

        // Get angle. length is magnitude of the vector
        double angle( acos( (ZZ * pp) / height) );

        // Create a cylinder between the two points with the given radius
        osg::ref_ptr<osg::Cylinder> cylinder( new osg::Cylinder(center, cc.radius, height) );
        cylinder->setRotation(osg::Quat(angle, osg::Vec3d(tt.x(), tt.y(), tt.z())));

        // A geode to hold the cylinder
        osg::ref_ptr<osg::ShapeDrawable> cylinderDrawable( new osg::ShapeDrawable(cylinder) );
        geode->addDrawable(cylinderDrawable);

        // Set the color of the cylinder that extends between the two points.
        osg::ref_ptr<osg::Material> pMaterial( new osg::Material() );
        pMaterial->setDiffuse( osg::Material::FRONT, cc.color);
        geode->getOrCreateStateSet()->setAttribute( pMaterial, osg::StateAttribute::OVERRIDE );
    }

    osg::ref_ptr<osg::Group> pAddToThisGroup(new osg::Group());
    pAddToThisGroup->addChild(geode);
    return pAddToThisGroup;
}

} // namespace d3
