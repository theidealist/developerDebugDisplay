/////////////////////////////////////////////////////////////////
/// @file      dsp.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.06.03
/// @brief     Simple example for using the draw lib
///
/// @attention Copyright (C) 2014
/// @attention
/// @attention
/// @attention All rights reserved
/// @attention
/////////////////////////////////////////////////////////////////

/// The main draw object
#include <DDDisplayInterface/DisplayInterface.h>

/// The things to include for drawing
#include <DDDisplayObjects/Grids.h>
#include <DDDisplayObjects/Triads.h>

/// osg
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/Material>

/// boost
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

/// std
#include <thread>
#include <chrono>

namespace po = boost::program_options;

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
bool setupOptions(int argc, char** argv,
                  po::variables_map& vm)
{
    // define the options
    po::options_description desc("Options");
    desc.add_options()
        ("help,h", "Print this help message")
        ("file,f", po::value<boost::filesystem::path>(), "The file to load and display")
        ("scale,s", po::value<double>()->default_value(1.0), "The scale to apply to the model" )
        ;

    po::positional_options_description positionalOptions;

    po::store(po::command_line_parser(argc, argv).
              options(desc).positional(positionalOptions).run(), vm);
    po::notify(vm);

    // check for help on the command line
    if ( vm.count("help") )
    {
        std::cout << desc << "\n";
        exit(0);
    }

    return true;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
    // setup options
    po::variables_map vm;
    setupOptions(argc, argv, vm);

    // make sure we have a file to read
    if ( not vm.count("file") )
    {
        std::cerr << "No file specified: try --help for help" << std::endl;
        return EXIT_FAILURE;
    }

    // get the scale
    double scale(vm["scale"].as<double>());

    // add common stuff
    d3::di().add( "ground", d3::ground(0.1, 5.0) );
    d3::di().add( "triad", d3::origin() );

    // read in the osg model
    osg::ref_ptr<osg::Node> node( osgDB::readNodeFile(vm["file"].as<boost::filesystem::path>().string()) );

    // make a scaling transform
    osg::ref_ptr<osg::MatrixTransform> xform(new osg::MatrixTransform(osg::Matrix::scale(osg::Vec3d(scale,scale,scale))));
    xform->addChild(node);
    if ( node )
        d3::di().add( vm["file"].as<boost::filesystem::path>().string(), xform );

    // wait for close
    d3::di().blockForClose();

    // we're done
    return EXIT_SUCCESS;
}
