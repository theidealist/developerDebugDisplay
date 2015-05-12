/////////////////////////////////////////////////////////////////
/// @file      KeypressEventHandler.cpp
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.06.20
/// @brief     Handle Keypress Events
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#include "KeypressEventHandler.h"

namespace d3
{

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
KeypressEventHandler::KeypressEventHandler() :
    Parent_t()
{
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
KeypressEventHandler::~KeypressEventHandler()
{
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
bool KeypressEventHandler::add(const osgGA::GUIEventAdapter::KeySymbol& key,
                               const std::function<bool(const osgGA::GUIEventAdapter&)>& func,
                               const std::string& description)
{
    // add this key handler to the map
    m_keyFuncMap[key].push_back({key, func, description});

    return true;
};

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
bool KeypressEventHandler::handle(const osgGA::GUIEventAdapter& eventAdapter,
                                  osgGA::GUIActionAdapter&)
{
    // find this handler
    KeyMap_t::iterator itt( m_keyFuncMap.find(eventAdapter.getKey()) );
    if ( m_keyFuncMap.end() == itt ) return false;

    // envoke the function(s)
    bool rv(true);
    for ( const auto& entry : itt->second )
        rv &= entry.func(eventAdapter);

    return rv;
};

} // namespace d3
