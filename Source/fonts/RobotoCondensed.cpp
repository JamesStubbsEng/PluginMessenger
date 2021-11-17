/*
  ==============================================================================

    RobotoCondensed.cpp
    Created: 7 Oct 2021 12:59:48pm
    Author:  James

  ==============================================================================
*/

#include "RobotoCondensed.h"

namespace PluginMessenger
{

//==============================================================================
namespace fonts
{


//==============================================================================

const juce::Font& robotoRegular()
{
    static const auto ptr = juce::Typeface::createSystemTypefaceFor (BinaryData::RobotoCondensedRegular_ttf, BinaryData::RobotoCondensedRegular_ttfSize);
    static const juce::Font f (ptr);
    return f;
}

const juce::Font& robotoBold()
{
    static const auto ptr = juce::Typeface::createSystemTypefaceFor (BinaryData::RobotoCondensedBold_ttf, BinaryData::RobotoCondensedBold_ttfSize);
    static const juce::Font f (ptr);
    return f;
}

} // namespace fonts

} // namespace PluginMessenger