/////////////////////////////////////////////////////////////////
/// @file      MotionEventHandler.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2015.01.13
/// @brief     Handle Click Events
///
/// @attention Copyright (C) 2015
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#pragma once

#include <osgGA/GUIEventHandler>

#include <functional>
#include <iostream>
#include <string>
#include <map>

namespace d3
{

/// The MotionEventHandler is a type of gui event handler and responds when
/// mouse is moved. Generally, the add method adds a motion event for the mouse
/// action with a description. The handle event is envoked during motion.
class MotionEventHandler : public osgGA::GUIEventHandler
{
    typedef osgGA::GUIEventHandler Parent_t;

  public:

    /// @brief   Constructor
    MotionEventHandler();

    /// @brief   Destructor
    virtual ~MotionEventHandler();

    /// @brief   Add a key handler
    /// @param   func The function to envoke during motion
    /// @param   description The description of the key (for the help string)
    bool add(const std::function<bool(const osgGA::GUIEventAdapter&)>& func,
             const std::string& description);

    /// @brief   Override the base's handle function
    /// @param   eventAdapter The osg gui adapter event
    virtual bool handle(const osgGA::GUIEventAdapter& eventAdapter,
                        osgGA::GUIActionAdapter&);

  private:

    /// Define a simple function map which maps keys to functions with a short description
    struct MotionFunc
    {
        std::function<bool(const osgGA::GUIEventAdapter&)> func;
        std::string desc;
    };

    /// The list of functions to run during a motion event
    typedef std::vector<MotionFunc> MotionFuncs_t;
    MotionFuncs_t m_motionFuncs;
};

} // namespace d3

