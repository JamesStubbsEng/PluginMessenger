/*
  ==============================================================================

    MessageDisplayRow.cpp
    Created: 17 Nov 2021 6:28:03pm
    Author:  James

  ==============================================================================
*/

#include "MessageDisplayRow.h"

MessageDisplayRow::MessageDisplayRow(StringRef senderName, StringRef messageBody, bool hasHeader, bool isMessageOutgoing) 
    : senderName(senderName), messageBody(messageBody), hasHeader(hasHeader), isMessageOutgoing(isMessageOutgoing)
{
    robotoBold.setHeight(17);
    robotoRegular.setHeight(17);
}

void MessageDisplayRow::paint(juce::Graphics& g)
{
    auto pillColour = isMessageOutgoing ? Colours::mediumpurple : Colours::cadetblue;
    auto headerYOffset = hasHeader ? headerHeight : 0;
    //Draw header
    if (hasHeader)
    {
        g.setFont(robotoBold);
        g.setColour(pillColour);
        g.fillEllipse(pillMarginX, headerHeight / 4, 20, 20);

        g.setColour(Colours::white);
        if(senderName.isNotEmpty()){}
            g.drawText(senderName.substring(0, 1).toUpperCase(), pillMarginX, headerHeight / 4, 20, 20, Justification::centred, false);

        g.setColour(pillColour.withAlpha(0.5f));
        g.drawText(senderName, pillMarginX + 40, headerHeight / 4, getParentWidth() - pillMarginX - 80, 20, Justification::centredLeft);
    }

    //Draw message bubble
    g.setColour(pillColour);
    g.fillRoundedRectangle(pillMarginX, pillMarginY + headerYOffset, messageWidth + 2*messagePaddingX, messageHeight + 2*messagePaddingY, 6);

    //Draw message
    g.setFont(robotoRegular);
    g.setColour(Colours::white);
    g.drawMultiLineText(messageBody, pillMarginX + messagePaddingX, pillMarginY + messagePaddingY + 3*robotoRegular.getHeight()/4 + headerYOffset, messageWidth, Justification::topLeft);
}

void MessageDisplayRow::resized()
{
    configureSize();
}

void MessageDisplayRow::parentSizeChanged()
{
    configureSize();
}

void MessageDisplayRow::configureSize()
{
    if (getParentComponent() == nullptr || getParentWidth() == 0)
        return;

    int startIndex = 0;
    int endIndex = 0;
    int previousEndIndex = 0;
    messageHeight = robotoRegular.getHeight();
    String stringToFindHeightOf = messageBody;
    while (endIndex < stringToFindHeightOf.length())
    {
        endIndex = stringToFindHeightOf.indexOfChar(endIndex, ' ');
        if (endIndex == -1)
            endIndex = stringToFindHeightOf.length();
        auto subString = stringToFindHeightOf.substring(startIndex, endIndex);
        if (robotoRegular.getStringWidth(subString) > getParentWidth() - 2 * pillMarginX - 2 * messagePaddingX)
        {
            messageHeight += robotoRegular.getHeight();
            startIndex = previousEndIndex;
            endIndex = previousEndIndex;
        }
        previousEndIndex = endIndex;
        endIndex++;
    }

    if (messageHeight == robotoRegular.getHeight())
        messageWidth = robotoRegular.getStringWidth(stringToFindHeightOf);
    else
        messageWidth = getParentWidth() - 2 * pillMarginX - 2 * messagePaddingX;

    auto componentWidth = messageWidth + 2 * pillMarginX + 2 * messagePaddingX;
    auto componentHeight = messageHeight + pillMarginY + 2 * messagePaddingY;

    if (hasHeader)
    {
        componentHeight += headerHeight;
        setSize(getParentWidth(), componentHeight);
    }
    else
        setSize(componentWidth, componentHeight);   
}

const String& MessageDisplayRow::getSenderName()
{
    return senderName;
}
