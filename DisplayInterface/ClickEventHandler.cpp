/////////////////////////////////////////////////////////////////
/// @file      ClickEventHandler.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2015.01.13
/// @brief     Handle Click Events
///
/// @attention Copyright (C) 2015
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "ClickEventHandler.h"

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
ClickEventHandler::ClickEventHandler() :
    Parent_t()
{
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
ClickEventHandler::~ClickEventHandler()
{
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
bool ClickEventHandler::add(const osgGA::GUIEventAdapter::MouseButtonMask& button,
                            const std::function<bool(const osgGA::GUIEventAdapter&)>& func,
                            const std::string& description)
{
    // add this click handler to the map
    m_clickFuncMap[button].push_back({button, func, description});

    return true;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
bool ClickEventHandler::handle(const osgGA::GUIEventAdapter& eventAdapter,
                               osgGA::GUIActionAdapter&)
{
    // find this handler
    ClickMap_t::iterator itt( m_clickFuncMap.find((osgGA::GUIEventAdapter::MouseButtonMask)eventAdapter.getButton()) );
    if ( m_clickFuncMap.end() == itt ) return false;

    // envoke the function(s)
    bool rv(true);
    for ( const auto& entry : itt->second )
        rv &= entry.func(eventAdapter);

    return rv;
};

} // namespace d3
