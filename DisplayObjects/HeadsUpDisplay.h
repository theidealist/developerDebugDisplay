/////////////////////////////////////////////////////////////////
/// @file      HeadsUpDisplay.h
/// @author    Chris L Baker (clb) <chris@chimail.net>
/// @date      2014.06.19
/// @brief     Setup and draw a simple HUD
///
/// @attention Copyright (C) 2014
/// @attention All rights reserved
/////////////////////////////////////////////////////////////////

#ifndef  _DDD__DISPLAY_OBJECTS__HEADS_UP_DISPLAY_H_
#define  _DDD__DISPLAY_OBJECTS__HEADS_UP_DISPLAY_H_

#include <osgText/Text>

namespace d3
{

class HeadsUpDisplay
{
  public:

    /// @brief   The possible positions for the HUD within the window
    enum class Position
    {
        TOP = 0,
        BOTTOM,
        LEFT,
        RIGHT,
        CENTER
    };

    /// @brief   Constructor
    /// @param   width Width of HUD as a fraction of the window's width
    /// @param   height Height of the HUD as a fraction of the window's height
    /// @param   position The position of the HUD in the window. Currently
    ///          supported are TOP, BOTTOM, LEFT, RIGHT, and CENTER.
    /// @param   initText The initial text to display
    /// @param   initTextColor The initial color of the text
    /// @param   initShow Should the HUD be displayed initially
    HeadsUpDisplay(const float& width = 1.0,
                   const float& height = 0.05,
                   const Position& position = Position::BOTTOM,
                   const std::string& initText = "",
                   const osg::Vec4& initTextColor = osg::Vec4(0.8, 0.8, 0.8, 0.5),
                   const bool& initShow = true);

    /// @brief   Destructor
    ~HeadsUpDisplay();

    /// @brief   Access to the display root
    const osg::ref_ptr<osg::Group>& get() const { return m_root; };

    /// @brief   Set/Change the text
    /// @param   text The text to update the display with
    inline void setText(const std::string& text) { m_text->setText(text);   };

    /// @brief   Set/Change the color of the text
    /// @param   color The color to set the text in the hud
    inline void setTextColor(const osg::Vec4& color) { m_text->setColor(color); };

    /// @brief   Set/Change the color of the background
    /// @param   color The color to change the background to
    void setBackgroundColor(const osg::Vec4& color);

    /// @brief   Set the background to the original default color
    void setDefaultBackgrounColor();

    /// @brief   Method to show/hide the HUD
    /// @param   display true means to show, false means to hide
    void show(const bool& display);

    /// @brief   See if the HUD is currently displayed
    /// @return  boolean True if the HUD is currently displayed
    bool isShown() const;

  private:

    /// Keep the text as a member so we can change the text and color
    osg::ref_ptr<osgText::Text>         m_text;

    /// Keep track of the background geometry so we can change the colors when desired
    osg::ref_ptr<osg::Geometry>         m_hudBackgroundGeometry;

    /// The root of the display
    osg::ref_ptr<osg::Group>            m_root;
};

/// @brief   get an osg node from a single hud display
/// @param   hud The hud to get an osg representation from
/// @return  osg::ref_ptr<osg::Node> The osg::Node rep of the hud for the
///          di().add() call
inline osg::ref_ptr<osg::Node> get(const HeadsUpDisplay& hud)
{
    return hud.get();
};

} // namespace d3

#endif   // _DDD__DISPLAY_OBJECTS__HEADS_UP_DISPLAY_H_
