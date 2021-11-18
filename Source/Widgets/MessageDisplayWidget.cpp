/*
  ==============================================================================

    MessageDisplayWidget.cpp
    Created: 17 Nov 2021 6:26:52pm
    Author:  James

  ==============================================================================
*/

#include "MessageDisplayWidget.h"

MessageDisplayWidget::MessageDisplayWidget()
{
    viewport.setScrollBarsShown(true, false);
    viewport.setScrollBarThickness(6);
    viewport.setViewedComponent(&viewedComponent, true);
    addAndMakeVisible(&viewport);
}


void MessageDisplayWidget::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(1);
    g.setColour(getLookAndFeel().findColour(TextEditor::backgroundColourId));
    g.fillRoundedRectangle(bounds, 6);
    g.setColour(getLookAndFeel().findColour(TextEditor::outlineColourId));
    g.drawRoundedRectangle(bounds, 6, 2);
}

void MessageDisplayWidget::resized()
{
}

void MessageDisplayWidget::parentSizeChanged()
{
    viewport.setBounds(0, 3, getWidth() - 5 , getHeight() - 10);
    positionListRows();
}

void MessageDisplayWidget::addRowComponent(StringRef senderName, StringRef messageBody, bool isMessageOutgoing)
{
    auto rowsLastElement = rows.getLast();
    auto hasHeader = true;
    if(rowsLastElement != nullptr)
        hasHeader = rows.getLast()->getSenderName() != senderName;
    MessageDisplayRow* mDR = new MessageDisplayRow(senderName, messageBody, hasHeader, isMessageOutgoing);

    viewedComponent.addAndMakeVisible(rows.add(mDR));
    mDR->configureSize();
    positionListRows();
}

void MessageDisplayWidget::positionListRows()
{
    int y = 0;
    int x = 0;
    int width = viewport.getWidth();

    for (int i = 0; i < rows.size(); i++)
    {
        int height = rows[i]->getHeight();
        rows[i]->setTopLeftPosition(x, y);
        y += height;
    }

    viewedComponent.setSize(width, y);
}

void MessageDisplayWidget::clear()
{
    rows.clear();
    positionListRows();
}
