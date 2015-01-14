/////////////////////////////////////////////////////////////////
/// @file      MainPage.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.05.20
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

/// @mainpage The Developer Debug Display Main Intro
///
/// @section intro_sec Introduction
///
/// First, let me describe what this collection of modest classes does \b *not*
/// claim to be.
///
/// - This is \b not a testing suite
///
/// - This is \b not an application developer tool where you build functionality
/// and end-user enterprise applications using a wysiwyg layout editor
///
/// - This is \b not going to meet everyone's needs in all situations and is \b
/// not intended to be perfectly efficient in every possible way
///
/// What then \b is this tool and why should you care? If you are a developer
/// and work with visual 3D data from a sensor, or using a robot, or some type
/// of manipulator, or planning or are developing some type of algorithm that
/// lives in a massive system and you need to \b see what you are doing,
/// visually in 3D - this tool with be indispensible for you.
///
/// I created this tool because I work in robotics and I do everything from
/// perception to manipulation, planning and control. I need a way to see what I
/// am doing. Finding the closest point between a point and a bounded plan? It's
/// pretty easy to code that up and run some tests to see if you are doing it
/// right. It's about 1000x faster to do that and debug it if you have a way to
/// visually display what's going on.
///
/// There are several ways to display information in 3D. You can write out files
/// and load them in other displayers. You can take your full application and
/// drop it in an event-handler based system and have a "mode" where you run
/// things with a display on. You can write messaging schema and publish status
/// to be displayed by another appliation live. All these things are well and
/// good and have their use-cases. I'm not intending to replace those. In fact,
/// using this as a end-user display is probably a really bad idea. The better
/// way would be to design a messaging interface and publish updates to be
/// displayed by a remote reader of those messages (i.e. model-view-controller
/// design pattern).
///
/// What this suite tries to accomplish is the ability to be developing and
/// debugging a very low-level algorithm where the overhead of generating
/// messages for publishing is not the appropriate action. We may not even want
/// to boil information to the top of our main application for display within an
/// event-loop system. So, we'd like to be able to, at any point in the code, no
/// matter how deep into several function calls we find ourselves, quickly and
/// easily just draw something. Be it points, lines, spheres, triangles, full
/// surfaces, full models... whatever. \b That is the objective here.
///
/// So, the way we accomplish this is to create a class which just waits for us
/// to draw something. A full tree-based structure with a full tree based
/// display controller. We can add widgets at will where helpful to accomplish
/// our debugging and then when we're all done debugging and we know things are
/// working well, we can just compile it all away.
///
/// @section intro_0 Quick Start
///
/// So, to get started on this is pretty trivial. Assuming you have downloaded,
/// built the dependencies, built this package, setup your application to link
/// to this library (DDDisplayInterface) and possible the display objects
/// (DDDisplayObjects), you are ready to go. The display objects just have some
/// easy and convenient wrappers to get things going quickly, such as points and
/// lines. Fundamentally, there's nothing going on in the display objects that
/// you can't do from scratch - the display interface just takes
/// osg::ref_ptr<osg::Node> objects.
///
/// Let's say we are working on an algorithm that finds the closest point
/// between a point and a line. We'd like to draw the point and the line. Here's
/// all we need to do (note, this can be in the main(), but really imagine this
/// being in some very low level algorith that has no access or notion of from
/// whence it was called.)
///
/// Imagine we have defined some point type Point_t and some line type Line_t
/// for our algorithm as follows
///
/// @code
/// struct Point_t
/// {
///     float x;
///     float y;
///     float z;
/// };
///
/// struct Line_t
/// {
///     Point_t begin;
///     Point_t end;
/// };
/// @endcode
///
/// Now, we have defined some function in some class called MyAwesomeClass which
/// will find the closest point between the point and the line. The code to do
/// this is pretty trivial and I won't show it here - let's just say we want to
/// draw the point and the line in a 3D display
///
/// @code
/// // ... other includes ...
/// #include <DDDisplayInterface/DisplayInterface.h>
/// #include <DDDisplayObjects/Points.h>
/// #include <DDDisplayObjects/Lines.h>
/// // ... other includes ...
///
/// void MyAwesomeClass::findClosest(const Point_t& point, const Line_t& line)
/// {
///     // draw the line
///     d3::di().add( "line",
///                   d3::get(d3::Line{{line.begin.x,line.begin.y,line.begin.z},
///                                    {line.end.x,  line.end.y,  line.end.z},
///                                    {1.0, 0.0, 0.0, 1.0}}) );
///
///     // draw the point
///     d3::di().add( "point",
///                   d3::get(d3::Point{{point.x, point.y, point.z},
///                                    {0.0, 1.0, 0.0, 1.0}}) );
///
///     // ... do other stuff to find the closest point ...
/// }
/// @endcode
///
/// That's it. So, what's going on here? d3 is the namespace (as in developer
/// debug display == ddd == d3). di() is the namespace function which gets the
/// static singleon from the display interface ( == di ). add(...) is the method
/// used to add stuff to the display. The first argument to the add method is
/// the name of the thing we're adding. This is used not only to keep the
/// objects straight, but also for the purpose of building a tree-view where we
/// can selectively turn objects on and off in the 3D display. Then, there are
/// several overloaded namespace functions "get()" which all return an
/// osg::ref_ptr<osg::Node> object to hand to the add method. For example, one
/// takes a d3::Line object which is created with two points and a color. The
/// second example is a d3::Point object which takes an osg::Vec3d point and a
/// color. So, drawing simple stuff is just that easy.
///
/// Notice the include paths are all plural (i.e. Points.h). This is because
/// each of the d3::get() methods are formulated to receive std::vector<OBJ>
/// where OBJ is a Point or a Line or something being drawn. when calling with a
/// single object (as in the above example) we simplly create a length-1 vector
/// and forward the call to the more general call. This allows us to efficiently
/// make use of OpenGL and OSG when creating large lists of objects. So, if we
/// wanted to draw a bunch of points, like a pcl::PointCloud from a Velodyne, we
/// could easily do something like this:
///
/// @code
/// // ... other includes ...
/// #include <DDDisplayInterface/DisplayInterface.h>
/// #include <DDDisplayObjects/Points.h>
/// // ... other includes ...
///
/// void MyAwesomeClass::findPersonInData(const PointData_t& pointData)
/// {
///     //// display the raw input data
///     // @todo: color by height
///     static const osg::Vec4 color(1.0, 0.0, 1.0, 1.0);
///
///     // build the point display vector
///     d3::PointVec_t displayPoints;
///     for ( const auto& pt : pointData )
///         displayPoints.push_back({{pt.x, pt.y, pt.z}, color});
///
///     // draw the actual points
///     d3::di().add( "input", d3::get(displayPoints) );
///
///     // ... do other stuff to find a person in some data ...
/// }
/// @endcode
///
/// There is a test app called testDeveloperDebugDisplay.cpp with many more
/// examples that can also be reviewed and run.
