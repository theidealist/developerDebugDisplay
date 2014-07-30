/////////////////////////////////////////////////////////////////
/// @file      HeadsUpDisplay.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.06.19
/// @brief     Setup and draw a simple HUD
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "HeadsUpDisplay.h"

#include <DDDisplayInterface/DisplayInterface.h>

#include <osg/Projection>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osgText/Text>

#include <iostream>

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
HeadsUpDisplay::HeadsUpDisplay(const float& width /* = 1.0 */,
                               const float& height /* = 0.04 */,
                               const Position& position /*= Position::BOTTOM */,
                               const std::string& initText /* = "" */,
                               const osg::Vec4& initTextColor /* = osg::Vec4(0.8, 0.8, 0.8, 0.5) */,
                               const bool& initShow /* = true */) :
    m_text( new osgText::Text() ),
    m_hudBackgroundGeometry( new osg::Geometry() ),
    m_root(new osg::Group())
{
    // Projection node for defining view frustrum for HUD:
    osg::ref_ptr<osg::Projection> hudProjectionMatrix( new osg::Projection() );
    hudProjectionMatrix->setMatrix(osg::Matrix::ortho2D(0,640,0,480));

    // For the HUD model view matrix use an identity matrix:
    osg::ref_ptr<osg::MatrixTransform> hudModelViewMatrix( new osg::MatrixTransform() );
    hudModelViewMatrix->setMatrix(osg::Matrix::identity());
    hudModelViewMatrix->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    hudProjectionMatrix->addChild(hudModelViewMatrix);
    m_root->addChild(hudProjectionMatrix);

    // The background is a polygon
    osg::ref_ptr<osg::DrawElementsUInt>
        hudBackgroundIndices( new osg::DrawElementsUInt(osg::PrimitiveSet::POLYGON, 0) );
    hudBackgroundIndices->push_back(0);
    hudBackgroundIndices->push_back(1);
    hudBackgroundIndices->push_back(2);
    hudBackgroundIndices->push_back(3);

    float ww(640), hh(480);

    // setup the hud background
    osg::ref_ptr<osg::Vec3Array> hudBackgroundVertices( new osg::Vec3Array() );
    osg::ref_ptr<osg::Vec2Array> texcoords( new osg::Vec2Array() );
    switch ( position )
    {
    case Position::TOP:
        hudBackgroundVertices->push_back( {ww-ww*width,   hh-0*height,   -1} );
        hudBackgroundVertices->push_back( {ww-0*width,    hh-0*height,   -1} );
        hudBackgroundVertices->push_back( {ww-0*width,    hh-hh*height,  -1} );
        hudBackgroundVertices->push_back( {ww-ww*width,   hh-hh*height,  -1} );

        // set the texture coordinates for the background
        texcoords->push_back( {0.0, 1.0} );
        texcoords->push_back( {0.0, 0.0} );
        texcoords->push_back( {1.0, 0.0} );
        texcoords->push_back( {1.0, 1.0} );
        break;

    case Position::BOTTOM:
        hudBackgroundVertices->push_back( {0*width,    0*height,   -1} );
        hudBackgroundVertices->push_back( {ww*width,   0*height,   -1} );
        hudBackgroundVertices->push_back( {ww*width,   hh*height,  -1} );
        hudBackgroundVertices->push_back( {0*width,    hh*height,  -1} );

        // set the texture coordinates for the background
        texcoords->push_back( {0.0, 1.0} );
        texcoords->push_back( {0.0, 0.0} );
        texcoords->push_back( {1.0, 0.0} );
        texcoords->push_back( {1.0, 1.0} );
        break;

    case Position::LEFT:
        hudBackgroundVertices->push_back( {0*width,    0*height,   -1} );
        hudBackgroundVertices->push_back( {ww*width,   0*height,   -1} );
        hudBackgroundVertices->push_back( {ww*width,   hh*height,  -1} );
        hudBackgroundVertices->push_back( {0*width,    hh*height,  -1} );

        // set the texture coordinates for the background
        texcoords->push_back( {0.0, 0.0} );
        texcoords->push_back( {0.0, 1.0} );
        texcoords->push_back( {1.0, 0.0} );
        texcoords->push_back( {1.0, 1.0} );
        break;

    case Position::RIGHT:
        hudBackgroundVertices->push_back( {ww-ww*width,   hh-0*height,   -1} );
        hudBackgroundVertices->push_back( {ww-0*width,    hh-0*height,   -1} );
        hudBackgroundVertices->push_back( {ww-0*width,    hh-hh*height,  -1} );
        hudBackgroundVertices->push_back( {ww-ww*width,   hh-hh*height,  -1} );

        // set the texture coordinates for the background
        texcoords->push_back( {0.0, 0.0} );
        texcoords->push_back( {0.0, 1.0} );
        texcoords->push_back( {1.0, 0.0} );
        texcoords->push_back( {1.0, 1.0} );
        break;

    case Position::CENTER:
        hudBackgroundVertices->push_back( {(ww-ww*width)/2.0f,   (hh-hh*height)/2.0f,   -1} );
        hudBackgroundVertices->push_back( {(ww+ww*width)/2.0f,   (hh-hh*height)/2.0f,   -1} );
        hudBackgroundVertices->push_back( {(ww+ww*width)/2.0f,   (hh+hh*height)/2.0f,   -1} );
        hudBackgroundVertices->push_back( {(ww-ww*width)/2.0f,   (hh+hh*height)/2.0f,   -1} );

        // set the texture coordinates for the background
        texcoords->push_back( {0.0, 0.0} );
        texcoords->push_back( {0.0, 1.0} );
        texcoords->push_back( {1.0, 0.0} );
        texcoords->push_back( {1.0, 1.0} );
        break;
    }

    // setup the background colors - elegant fade
    setDefaultBackgrounColor();

    // set the normals for the background
    osg::ref_ptr<osg::Vec3Array> hudnormals( new osg::Vec3Array() );
    hudnormals->push_back( {0.0, 0.0, 1.0} );

    // create the actual geometry for the hud
    m_hudBackgroundGeometry->setTexCoordArray(0,texcoords);
    m_hudBackgroundGeometry->setNormalArray(hudnormals);
    m_hudBackgroundGeometry->setNormalBinding(osg::Geometry::BIND_OVERALL);
    m_hudBackgroundGeometry->addPrimitiveSet(hudBackgroundIndices);
    m_hudBackgroundGeometry->setVertexArray(hudBackgroundVertices);
    //hudBackgroundGeometry->setColorArray(hudcolors);
    m_hudBackgroundGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

    // create the actual texture
    osg::ref_ptr<osg::Texture2D> hudTexture( new osg::Texture2D() );
    hudTexture->setDataVariance(osg::Object::DYNAMIC);

    // fancy stateset stuff
    osg::ref_ptr<osg::StateSet> hudStateSet( new osg::StateSet() );
    hudStateSet->setTextureAttributeAndModes(0, hudTexture, osg::StateAttribute::ON);
    hudStateSet->setMode(GL_BLEND,osg::StateAttribute::ON);
    hudStateSet->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
    hudStateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    hudStateSet->setRenderBinDetails( 11, "RenderBin");

    // create the geode
    osg::ref_ptr<osg::Geode> hudGeode( new osg::Geode() );
    hudModelViewMatrix->addChild( hudGeode );
    hudGeode->addDrawable(m_hudBackgroundGeometry);
    hudGeode->setStateSet(hudStateSet);

    // setup the text
    m_text->setCharacterSize(12);
    m_text->setFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf");
    m_text->setText(initText);
    m_text->setAxisAlignment(osgText::Text::SCREEN);
    m_text->setColor(initTextColor);

    switch ( position )
    {
    case Position::TOP:     m_text->setPosition( osg::Vec3(10,hh-10,-1.5) );        break;
    case Position::BOTTOM:  m_text->setPosition( osg::Vec3(10,10,-1.5) );           break;
    case Position::RIGHT:   m_text->setPosition( osg::Vec3(ww-6*initText.size(),hh-10,-1.5) );     break;
    case Position::LEFT:    m_text->setPosition( osg::Vec3(0.0, 0.0,-1.5) );        break;
    case Position::CENTER:  m_text->setPosition( osg::Vec3(ww/2.0-ww*width/2.0+10,hh/2.0,-1.5) ); break;
    default:                m_text->setPosition( osg::Vec3(ww/2.0,hh/2.0,-1.5) );   break;
    }

    // add the text to the geode
    hudGeode->addDrawable( m_text );

    // set the init to show
    show(initShow);
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
HeadsUpDisplay::~HeadsUpDisplay()
{
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void HeadsUpDisplay::setBackgroundColor(const osg::Vec4& color)
{
    // setup the background colors - user defined
    osg::ref_ptr<osg::Vec4Array> hudcolors( new osg::Vec4Array() );
    hudcolors->push_back( color );
    hudcolors->push_back( color );
    hudcolors->push_back( color );
    hudcolors->push_back( color );

    // set the color in the backgroun geometry
    di().lock();
    m_hudBackgroundGeometry->setColorArray(hudcolors);
    di().unlock();
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void HeadsUpDisplay::setDefaultBackgrounColor()
{
    // setup the background colors - elegant fade
    osg::ref_ptr<osg::Vec4Array> hudcolors( new osg::Vec4Array() );
    hudcolors->push_back( {0.2, 0.8, 0.2, 0.1} );
    hudcolors->push_back( {0.2, 0.8, 0.2, 0.1} );
    hudcolors->push_back( {0.2, 0.2, 0.8, 0.1} );
    hudcolors->push_back( {0.2, 0.2, 0.8, 0.1} );

    // set the color in the backgroun geometry
    di().lock();
    m_hudBackgroundGeometry->setColorArray(hudcolors);
    di().unlock();
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void HeadsUpDisplay::show(const bool& display)
{
    // set the node mask based on the display parameter
    m_root->setNodeMask(display?~0:0);
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
bool HeadsUpDisplay::isShown() const
{
    // is the node mask something non-zero
    return (m_root->getNodeMask()==~0u?true:false);
};

} // namespace d3
