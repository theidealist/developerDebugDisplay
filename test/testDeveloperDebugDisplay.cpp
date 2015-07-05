/////////////////////////////////////////////////////////////////
/// @file      testDeveloperDebugDisplay.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
/// @brief     Simple example for using the draw lib
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

/// The main draw object
#include <DDDisplayInterface/DisplayInterface.h>

/// The things to include for drawing
#include <DDDisplayObjects/Colors.h>
#include <DDDisplayObjects/Grids.h>
#include <DDDisplayObjects/Lines.h>
#include <DDDisplayObjects/Points.h>
#include <DDDisplayObjects/Triads.h>
#include <DDDisplayObjects/MeshGrid.h>
#include <DDDisplayObjects/Cylinders.h>
#include <DDDisplayObjects/Capsules.h>
#include <DDDisplayObjects/Cones.h>
#include <DDDisplayObjects/Spheres.h>
#include <DDDisplayObjects/HeightGrid.h>
#include <DDDisplayObjects/HeadsUpDisplay.h>
#include <DDDisplayObjects/CameraImages.h>
#include <DDDisplayObjects/Images.h>
#include <DDDisplayObjects/Voxels.h>

/// Fancier drawing stuff
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>

/// std stuff
#include <chrono>
#include <thread>
#include <iostream>
#include <sstream>

int main(int argc, char* argv[])
{
    // bulid a cell array
    d3::VoxelVec_t cells;
    osg::Vec3d halfCell{0.1, 0.1, 0.5};
    for ( double xx(10.0) ; xx<=20.0 ; xx+=0.1 )
        for ( double yy(10.0) ; yy<=15.0 ; yy+=0.1 )
            for ( double zz(0.0) ; zz<=5.0 ; zz+=0.5 )
                cells.emplace_back(d3::Voxel{osg::Vec3d{xx,yy,zz}-halfCell,
                                             osg::Vec3d{xx,yy,zz}+halfCell,
                                             {1.0, 1.0, 0.0, 1.0}});
    d3::di().add( "Cell Array", d3::get(cells) );
    
    // the scope here is to make sure that the image given to the display is
    // held by the display even when this one goes out of scope
    {
        osg::ref_ptr<osg::Image> img(osgDB::readImageFile("logo.png"));
        if ( img )
        {
            osg::Matrix pp(osg::Matrix::translate(3,2,1) * osg::Matrix::rotate(1.0, osg::Vec3{1,0.2,0.1}));
            d3::di().add( "image::frame", d3::get(d3::CameraImage{pp, img, 600.0, 600.0, 5.0}) );
            d3::di().add( "image::camera", d3::get(d3::Triad{pp}) );
            d3::di().add( "iamge::pic", d3::get(d3::Image{img, {{{0.0,0.0,0.0},{15.0,0.0,0.0},{15.0,10.0,0.0},{0.0,10.0,0.0}}}}) );
        }
    }

    d3::di().add( "ground", d3::ground() );
    d3::di().add( "origin", d3::origin() );

    d3::di().add( "cone", d3::get(d3::Cone({{3.0, 3.0, 3.0}, 0.2, 1.0, {1.0, 0.0, 0.5, 0.2}})) );

    // You can add to the draw object like this...
    d3::di().add( "first::line",
                   d3::get(d3::Line{osg::Vec3d(0,0,0),
                                    osg::Vec3d(1,1,1),
                                    d3::nextColor()}) );
    d3::di().add( "first::dot",
                   d3::get(d3::Point{osg::Vec3d(1,0,0), d3::nextColor()}) );


    d3::di().add( "first::second::deep",
                   d3::get(d3::Point{osg::Vec3d(1,1,0), d3::nextColor()}) );

    d3::di().add( "first::cylinder",
                   d3::get(d3::Cylinder{osg::Vec3d(3,3,3), osg::Vec3d(5,5,5), 0.2, osg::Vec4(0,1,1,1)}) );

    d3::di().add( "first::capsule",
                   d3::get(d3::Capsule{osg::Vec3d(-5,-5,5), osg::Vec3d(-3,-3,3), 0.3, osg::Vec4(1,1,0,1)}) );

    d3::di().add( "first::sphere",
                   d3::get(d3::Sphere{osg::Vec3d(1,2,3), 0.1, osg::Vec4(0.2, 0.3, 0.4, 0.5)}) );

    d3::PointVec_t pts;
    for ( double xx(-1.0) ; xx<=1.0 ; xx+=0.1 )
        for ( double yy(-1.0) ; yy<=1.0 ; yy+=0.1 )
            pts.push_back(d3::Point{{xx,yy,0.0}, {0,0,1,1}});
    d3::di().add( "cloud", d3::get(pts) );

    d3::PointVec_t cpts;
    for ( double xx(-1.0) ; xx<=1.0 ; xx+=0.1 )
        for ( double yy(-1.0) ; yy<=1.0 ; yy+=0.1 )
            cpts.push_back(d3::Point{{xx,yy,3.0}, d3::nextColor()});
    d3::di().add( "color cloud", d3::get(cpts) );

    d3::di().add( 'j',
                  [&](const osgGA::GUIEventAdapter& ev)->bool
                  {
                      if (osgGA::GUIEventAdapter::KEYDOWN == ev.getEventType())
                      {
                          std::cout << "Typed j" << std::endl;
                          return true;
                      }
                      return false;
                  },
                  "Typing \'j\' is cool" );

    d3::di().add( 'j',
                  [&](const osgGA::GUIEventAdapter& ev)->bool
                   {
                       if (osgGA::GUIEventAdapter::KEYDOWN == ev.getEventType())
                       {
                           std::cout << "Typed j - again" << std::endl;
                           return true;
                       }
                       return false;
                   },
                   "Typing \'j\' twice is really cool" );

    d3::di().add( 'k',
                  [&](const osgGA::GUIEventAdapter& ev)->bool
                   {
                       if (osgGA::GUIEventAdapter::KEYDOWN == ev.getEventType())
                       {
                           std::cout << "Typed k" << std::endl;
                           return true;
                       }
                       return false;
                   },
                   "Typing \'k\' is cool" );

    d3::di().add( osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON,
                  [&](const osgGA::GUIEventAdapter& event)->bool
                  {
                      //std::cout << "event type: " << event.getEventType() << std::endl;
                      if ( osgGA::GUIEventAdapter::RELEASE != event.getEventType() )
                          return false;

                      if ( osgGA::GUIEventAdapter::MODKEY_SHIFT & event.getModKeyMask() )
                          std::cout << "shift is down" << std::endl;

                      if ( osgGA::GUIEventAdapter::MODKEY_CTRL & event.getModKeyMask() )
                          std::cout << "ctrl is down" << std::endl;

                      //std::cout << "mod mask: " << event.getModKeyMask() << std::endl;
                      if ( osgGA::GUIEventAdapter::DOUBLECLICK == event.getEventType() )
                          std::cout << "double" << std::endl;
                      return false;
                  },
                  "Right Click Test" );

    d3::di().add( [&](const osgGA::GUIEventAdapter& event)->bool
                  {
                      std::cout << "motion" << std::endl;
                      return false;
                  },
                  "Motion Event Handler" );

    bool replace(true);
    d3::di().add( "par::not::appender", d3::get(d3::Point{osg::Vec3d(1,0,0), d3::white()}), replace);
    d3::di().add( "par::not::appender", d3::get(d3::Point{osg::Vec3d(2,0,0), d3::white()}), replace);
    d3::di().add( "par::not::appender", d3::get(d3::Point{osg::Vec3d(3,0,0), d3::white()}), replace);

    replace = false;
    d3::di().add( "par::appender", d3::get(d3::Point{osg::Vec3d(4,0,0), d3::white()}), replace);
    d3::di().add( "par::appender", d3::get(d3::Point{osg::Vec3d(5,0,0), d3::white()}), replace);
    d3::di().add( "par::appender", d3::get(d3::Point{osg::Vec3d(6,0,0), d3::white()}), replace);

    d3::HeadsUpDisplay hud0;//(0.5, 0.1,  d3::HeadsUpDisplay::Position::BOTTOM);
    d3::HeadsUpDisplay hud1(1.0, 0.04, d3::HeadsUpDisplay::Position::TOP);
    d3::HeadsUpDisplay hud2(0.04, 1.0, d3::HeadsUpDisplay::Position::LEFT);
    d3::HeadsUpDisplay hud3(0.04, 1.0, d3::HeadsUpDisplay::Position::RIGHT);
    d3::di().add( "Hud0", hud0.get() );
    d3::di().add( "Hud1", hud1.get() );
    d3::di().add( "Hud2", hud2.get() );
    d3::di().add( "Hud3", hud3.get() );

    d3::HeadsUpDisplay hud4(0.1, 0.1, d3::HeadsUpDisplay::Position::CENTER);
    d3::di().add( "Hud4", hud4.get() );
    hud4.setBackgroundColor({1,0.1,0.1,0.8});
    hud4.setText("PAUSED");

    hud0.setBackgroundColor({0,1,0,1});
    hud3.setBackgroundColor({1,0,0,1});

    osg::ref_ptr<osg::MatrixTransform> MT(new osg::MatrixTransform());
    MT->setMatrix(osg::Matrix::translate(osg::Vec3d(0.0, 0.0, 3.0)));
    MT->addChild(d3::get(d3::Grid{osg::Vec2(0.1, 0.3),
                                    osg::Vec2(10.0, 20.0),
                                    osg::Vec4(0.0, 0.2, 0.0, 1.0)}));
    d3::di().add( "grid", MT );

    d3::di().add( "first::pose",
                   d3::get(d3::Triad{osg::Matrix::translate(osg::Vec3d(0.2, 0.3, -0.1))}) );

    // try making a mesh
    d3::PointVec_t points;
    for ( double xx(-1.0) ; xx<=1.0 ; xx+=0.01 )
        for ( double yy(-2.0) ; yy<=2.0 ; yy+=0.01 )
            points.push_back(d3::Point{osg::Vec3d(xx,yy,0.1), osg::Vec4(1,0,1,0.3)});
    uint width( 4/0.01 );
    d3::di().add( "mesh::filled", d3::get(d3::MeshGrid{points, std::vector<osg::Vec3d>(1, {0,0,1}), width, true}) );
    d3::di().add( "mesh::wireframe", d3::get(d3::MeshGrid{points, std::vector<osg::Vec3d>(1, {0,0,1}), width, false}) );
    d3::di().add( "mesh::height", d3::get(d3::HeightGrid{points, 0.01, 0.01, osg::Vec3d(-1.0, -2.0, 0.0), width, osg::Vec4(0,1,1,0.5)}) );

    osg::ref_ptr<osg::Node> point( d3::get(d3::Point{osg::Vec3d(0,0,0), d3::nextColor()}) );
    osg::ref_ptr<osg::MatrixTransform> pointXform(new osg::MatrixTransform());
    pointXform->addChild(point);
    d3::di().add( "tracked point", pointXform );
    d3::di().track(point);

    double xOffset(0.0);
    double direction = 0.01;

    uint count(0);
    while ( d3::di().running() )
    {
        if ( ++count > 1000 ) break;
        xOffset += direction;
        if ( xOffset >  3.0 ) direction = -direction;
        if ( xOffset < -3.0 ) direction = -direction;

        std::stringstream ss;
        ss << "Count: " << count;
        hud0.setText(ss.str());
        hud0.setTextColor(d3::nextColor());

        if ( d3::di().try_lock() )
        {
            pointXform->setMatrix(osg::Matrix::translate(xOffset, 0, 0));
            d3::di().unlock();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    hud0.show(false);

    d3::di().lock();
    osg::ref_ptr<osg::Node> root( d3::di().getRootGroup() );
    if ( root )
    {
        std::cout << "modifying the root node" << std::endl;
        osg::ref_ptr<osg::MatrixTransform>
            scaleXform(new osg::MatrixTransform(osg::Matrix::scale(osg::Vec3d(1.0,1.0,0.1))));

        scaleXform->addChild(root);
        if ( not d3::di().setRootGroup(scaleXform) )
            std::cerr << "Could not set the root group" << std::endl;
    }
    d3::di().unlock();

    // wait for close
    d3::di().blockForClose();

    // we're done
    return EXIT_SUCCESS;
}
