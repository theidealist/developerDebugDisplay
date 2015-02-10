/////////////////////////////////////////////////////////////////
/// @file      Images.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2015.02.10
/// @brief     Display an images in 3D
///
/// @attention Copyright (C) 2015
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "Images.h"

#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/Geode>
#include <osg/TexEnv>

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Node> get(const ImageVec_t& images)
{
    osg::ref_ptr<osg::Group> rv( new osg::Group() );

    for ( const auto& image : images )
    {
        // compute the new image width and height
        double ww( static_cast<double>(image.image->s()) * image.scale/image.focalLengthX_pix );
        double hh( static_cast<double>(image.image->t()) * image.scale/image.focalLengthY_pix );

        // here we create a quad that will be texture mapped with the image
        osg::ref_ptr<osg::Vec3Array> quad( new osg::Vec3Array() );
        quad->push_back( osg::Vec3( -ww/2.0, -hh/2.0, image.scale ) );
        quad->push_back( osg::Vec3(  ww/2.0, -hh/2.0, image.scale ) );
        quad->push_back( osg::Vec3(  ww/2.0,  hh/2.0, image.scale ) );
        quad->push_back( osg::Vec3( -ww/2.0,  hh/2.0, image.scale ) );
        osg::ref_ptr<osg::Geometry> geo( new osg::Geometry() );
        geo->setVertexArray( quad );

        // here we create a drawing elelment to draw on
        osg::ref_ptr<osg::DrawElementsUInt>
            primitiveSet( new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0) );
        primitiveSet->push_back( 0 );
        primitiveSet->push_back( 1 );
        primitiveSet->push_back( 2 );
        primitiveSet->push_back( 3 );
        geo->addPrimitiveSet( primitiveSet );

        // the texture mappings
        osg::ref_ptr<osg::Vec2Array> texCoords( new osg::Vec2Array(4) );
        (*texCoords)[0].set( 0.0f, 0.0f );
        (*texCoords)[1].set( 1.0f, 0.0f );
        (*texCoords)[2].set( 1.0f, 1.0f );
        (*texCoords)[3].set( 0.0f, 1.0f );
        geo->setTexCoordArray( 0, texCoords );

        // now create the goede to hold our created geometry
        osg::ref_ptr<osg::Geode> geode( new osg::Geode() );
        geode->addDrawable( geo );

        // create the texture for the image
        osg::ref_ptr<osg::Texture2D> texture( new osg::Texture2D() );
        texture->setDataVariance(osg::Object::DYNAMIC);
        texture->setImage( image.image );

        // put this in decal mode
        osg::ref_ptr<osg::TexEnv> decalTexEnv( new osg::TexEnv() );
        decalTexEnv->setMode(osg::TexEnv::DECAL);

        // set the state set, lighting and such, so we actually display the image
        osg::ref_ptr<osg::StateSet> stateSet( geode->getOrCreateStateSet() );
        stateSet->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
        stateSet->setTextureAttribute(0, decalTexEnv);
        stateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
        geo->setStateSet(stateSet);

        // turn off any color binding - we want to use the texture
        geo->setColorBinding(osg::Geometry::BIND_OFF);

        // set our matrix as the provided camera matrix
        osg::ref_ptr<osg::MatrixTransform> xform(new osg::MatrixTransform());
        xform->setMatrix(image.cameraPose);
        xform->addChild(geode);

        // add this image and continue
        rv->addChild(xform);
    }

    return rv;
};

} // namespace d3
