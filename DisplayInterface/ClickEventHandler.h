/////////////////////////////////////////////////////////////////
/// @file      ClickEventHandler.h
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

/// The ClickEventHandler is a type of gui event handler and responds when
/// mouse buttons are pressed. Generally, the add method adds a button to some
/// action with a description. The handle event is envoked for each button press
/// and the button funciton map is searched. If there is no registered funciton, or
/// the registered function returns false, the handler returns false implying
/// that the key was not (properly) handled by any registered functions.
class ClickEventHandler : public osgGA::GUIEventHandler
{
    typedef osgGA::GUIEventHandler Parent_t;

  public:

    /// @brief   Constructor
    ClickEventHandler();

    /// @brief   Destructor
    virtual ~ClickEventHandler();

    /// @brief   Add a key handler
    /// @param   button The mouse button used
    /// @param   func The function to envoke when that key is pressed
    /// @param   description The description of the key (for the help string)
    bool add(const osgGA::GUIEventAdapter::MouseButtonMask& button,
             const std::function<bool(const osgGA::GUIEventAdapter&)>& func,
             const std::string& description);

    /// @brief   Override the base's handle function
    /// @param   eventAdapter The osg gui adapter event
    virtual bool handle(const osgGA::GUIEventAdapter& eventAdapter,
                        osgGA::GUIActionAdapter&);

  private:

    /// Define a simple function map which maps keys to functions with a short description
    struct KeyFunctionMap
    {
        osgGA::GUIEventAdapter::MouseButtonMask ev;
        std::function<bool(const osgGA::GUIEventAdapter&)> func;
        std::string desc;
    };

    typedef std::map<osgGA::GUIEventAdapter::MouseButtonMask,
                     std::list<KeyFunctionMap>> ClickMap_t;

    /// The basic mapping from a key to a function map entry
    ClickMap_t m_clickFuncMap;
};

} // namespace d3

