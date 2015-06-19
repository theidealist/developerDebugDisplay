/////////////////////////////////////////////////////////////////
/// @file      MotionEventHandler.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2015.01.13
/// @brief     Handle Click Events
///
/// @attention Copyright (C) 2015
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "MotionEventHandler.h"

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
MotionEventHandler::MotionEventHandler() :
    Parent_t()
{
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
MotionEventHandler::~MotionEventHandler()
{
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
bool MotionEventHandler::add(const std::function<bool(const osgGA::GUIEventAdapter&)>& func,
                             const std::string& description)
{
    // add this click handler to the map
    m_motionFuncs.push_back({func, description});

    return true;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
bool MotionEventHandler::handle(const osgGA::GUIEventAdapter& eventAdapter,
                                osgGA::GUIActionAdapter&)
{
    // make sure we are in a motion event
    if ( osgGA::GUIEventAdapter::MOVE != eventAdapter.getEventType() )
        return false;
    
    // we only handle motions, if a button is clicked, don't handle it
    if ( 0 != eventAdapter.getButton() )
        return false;

    // envoke the function(s)
    bool rv(false);
    if ( not m_motionFuncs.empty() )
    {
        rv = true;
        for ( const auto& ff : m_motionFuncs )
            rv &= ff.func(eventAdapter);
    }

    return rv;
};

} // namespace d3
