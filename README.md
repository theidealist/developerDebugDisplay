developerDebugDisplay
=====================

A simple 3D display based on QT and OpenSceneGraph aimed specifically at developers who need a quick debug tool that does 3D display without taking over your entire application.

Often when developing an algorithm which makes use of 3D spatial reasoning, it's useful to display what is going on. However, most well-designed applications will require low level algorithms to create messages and interfaces to a display front-end. However, often in the development cycle this is not practical, useful, or even desired. It is convenient at times to just draw something in 3D at a very low-level deep within some algorithm.

This is the target use-case of the developer debug display. The intent is to be very low overhead to just pop up a simple 3D display and start drawing stuff. Then, once debugging is complete, easily compile out all the calls to the library. We don't always want to have to create a test application that exercises and draws low level stuff and wraps in the "main-thread" of some gtk/qt thing.

The simplest thing to get up and running is to demonstrate the drawing of a couple of points. The code for this is in the test directory as a very simple test use-case. The namespace is di (display interface) and d3 (developer debug display).

The syntax is simply add( name, node ); where "name" is an std::string, and node is any osg::Node. The name can include scope operators ("::") for nesting things as the example below indicates. There are also several convenient wrappers to draw things like lines and points as indicated below.

... deep in the bowels of some algorithm we wish to display something ...

    // Draw a line starting at (0,0,0) to (1,1,1) with the next "default" color (just an osg::Vec4)
    // ddd::di() gets the display interface (singleton)
    // ddd::get(...) gets an osg::Node of type "Lines" where here, we just have a single line
    // This is all nested in the display under first -> line where we can turn on and off each nested element in a tree-view
    d3::di().add( "first::line", d3::get(d3::Line{osg::Vec3d(0,0,0), osg::Vec3d(1,1,1), ddd::nextColor()}) );

    // Here we draw a point at the first level nesting
    d3::di().add( "first::dot",  d3::get(d3::Point{osg::Vec3d(1,0,0), ddd::nextColor()}) );

    // Finally, we add something at a deeper level in the tree
    d3::di().add( "first::second::deep", d3::get(ddd::Point{osg::Vec3d(1,1,0), ddd::nextColor()}) );

... done ...

That's it! We now have a line and two points with only 3 lines of code. When this code is encountered, it will pop up a display (the display is not created unless something is added to it) and draw these elements.

For full documentation as well as some example code, download and run 'doxygen Doxyfile'
