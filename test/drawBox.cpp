/////////////////////////////////////////////////////////////////
/// @file      drawBox.cpp
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
#include <DDDisplayObjects/Grids.h>    // for ground()
#include <DDDisplayObjects/Triads.h>   // for the world origin
#include <DDDisplayObjects/Colors.h>   // gets various colors
#include <DDDisplayObjects/Capsules.h> // to draw the actual box
#include <DDDisplayObjects/HeadsUpDisplay.h> // flashing awesome

/// std stuff
#include <thread>

int main()
{
    // draws a "standard" grid on the ground (z=0 plane)
    d3::di().add( "Ground", d3::ground() );

    // draws an rgb triad at the origin
    d3::di().add( "Origin", d3::origin() );

    // create a simple HUD
    static const float width(0.13), height(0.07);
    static const d3::HeadsUpDisplay::Position position(d3::HeadsUpDisplay::Position::CENTER);
    static const std::string initText("AWESOME");
    d3::HeadsUpDisplay hud(width, height, position, initText);
    hud.setBackgroundColor(d3::black());

    // add the hud to the display
    d3::di().add( "HUD", d3::get(hud) );

    // create a little thread to "flash" the hud
    std::thread thrd
        ([&]()
         {
             while ( d3::di().running() )
             {
                 // we are messing directly with things in the render thread -
                 // make sure we lock the display interface
                 if ( d3::di().try_lock() )
                 {
                     hud.show(not hud.isShown());
                     d3::di().unlock();
                 }

                 // flash at about 2Hz
                 usleep(500000);
             }
         });

    // create the box as a set of capsules
    static const double radius(0.1);
    d3::CapsuleVec_t capsules;

    // The bottom square
    capsules.push_back({{-1.0, -1.0, -1.0}, { 1.0, -1.0, -1.0}, radius, d3::nextColor()});
    capsules.push_back({{ 1.0, -1.0, -1.0}, { 1.0,  1.0, -1.0}, radius, d3::nextColor()});
    capsules.push_back({{ 1.0,  1.0, -1.0}, {-1.0,  1.0, -1.0}, radius, d3::nextColor()});
    capsules.push_back({{-1.0,  1.0, -1.0}, {-1.0, -1.0, -1.0}, radius, d3::nextColor()});

    // The top square
    capsules.push_back({{-1.0, -1.0,  1.0}, { 1.0, -1.0,  1.0}, radius, d3::nextColor()});
    capsules.push_back({{ 1.0, -1.0,  1.0}, { 1.0,  1.0,  1.0}, radius, d3::nextColor()});
    capsules.push_back({{ 1.0,  1.0,  1.0}, {-1.0,  1.0,  1.0}, radius, d3::nextColor()});
    capsules.push_back({{-1.0,  1.0,  1.0}, {-1.0, -1.0,  1.0}, radius, d3::nextColor()});

    // Joins the top to the bottom
    capsules.push_back({{-1.0, -1.0, -1.0}, {-1.0, -1.0,  1.0}, radius, d3::nextColor()});
    capsules.push_back({{ 1.0, -1.0, -1.0}, { 1.0, -1.0,  1.0}, radius, d3::nextColor()});
    capsules.push_back({{ 1.0,  1.0, -1.0}, { 1.0,  1.0,  1.0}, radius, d3::nextColor()});
    capsules.push_back({{-1.0,  1.0, -1.0}, {-1.0,  1.0,  1.0}, radius, d3::nextColor()});

    // add the capsules to the display
    d3::di().add( "box", d3::get(capsules) );

    // wait for the drawing to close
    d3::di().blockForClose();

    return EXIT_SUCCESS;
};
