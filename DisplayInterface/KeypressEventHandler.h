/////////////////////////////////////////////////////////////////
/// @file      KeypressEventHandler.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.06.20
/// @brief     Handle Keypress Events
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#ifndef  _DDD__DISPLAY_INTERFACE__KEYPRESS_EVENT_HANDLER_H_
#define  _DDD__DISPLAY_INTERFACE__KEYPRESS_EVENT_HANDLER_H_

#include <osgGA/GUIEventHandler>

#include <functional>
#include <iostream>
#include <string>
#include <map>

namespace d3
{

/// The KeypressEventHandler is a type of gui event handler and responds when
/// keys are pressed. Generally, the add method adds a key to some action with a
/// description. The handle event is envoked for each key press and the key
/// funciton map is searched. If there is no registered funciton, or the
/// registered function returns false, the handler returns false implying that
/// the key was not (properly) handled by any registered functions.
class KeypressEventHandler : public osgGA::GUIEventHandler
{
    typedef osgGA::GUIEventHandler Parent_t;

  public:

    /// @brief   Constructor
    KeypressEventHandler();

    /// @brief   Destructor
    virtual ~KeypressEventHandler();

    /// @brief   Add a key handler
    /// @param   key The key that gets pressed
    /// @param   func The function to envoke when that key is pressed
    /// @param   description The description of the key (for the help string)
    bool add(const osgGA::GUIEventAdapter::KeySymbol& key,
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
        osgGA::GUIEventAdapter::KeySymbol key;
        std::function<bool(const osgGA::GUIEventAdapter&)> func;
        std::string desc;
    };

    typedef std::map<char, std::list<KeyFunctionMap>> KeyMap_t;

    /// The basic mapping from a key to a function map entry
    KeyMap_t m_keyFuncMap;
};

} // namespace d3

#endif   // _DDD__DISPLAY_INTERFACE__KEYPRESS_EVENT_HANDLER_H_
