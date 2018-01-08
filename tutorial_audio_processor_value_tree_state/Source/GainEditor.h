/*
 ==============================================================================

 This file is part of the JUCE library.
 Copyright (c) 2015 - ROLI Ltd.

 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3

 Details of these licenses can be found at: www.gnu.org/licenses

 JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

 ------------------------------------------------------------------------------

 To release a closed-source product which uses JUCE, commercial licenses are
 available: visit www.juce.com for more information.

 ==============================================================================
 */

class GainEditor : public AudioProcessorEditor
{
public:
    enum
    {
        paramControlHeight = 40,
        paramLabelWidth    = 80,
        paramSliderWidth   = 300
    };

    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    
    GainEditor (AudioProcessor& parent, AudioProcessorValueTreeState& vts)
        : AudioProcessorEditor (parent),
          valueTreeState (vts)
    {
        gainLabel.setText ("Gain", dontSendNotification);
        addAndMakeVisible (gainLabel);
        
        addAndMakeVisible (gainSlider);
        gainAttachment = new SliderAttachment (valueTreeState, "gain", gainSlider);
        
        invertButton.setButtonText ("Invert Phase");
        addAndMakeVisible (invertButton);
        invertAttachment = new ButtonAttachment (valueTreeState, "invertPhase", invertButton);
        
        setSize (paramSliderWidth + paramLabelWidth, paramControlHeight * 2);
    }

    ~GainEditor()
    {
    }

    void resized() override
    {
        Rectangle<int> r = getLocalBounds();
        
        {
            Rectangle<int> gainRect = r.removeFromTop (paramControlHeight);
            gainLabel.setBounds (gainRect.removeFromLeft (paramLabelWidth));
            gainSlider.setBounds (gainRect);
        }
        
        invertButton.setBounds (r.removeFromTop (paramControlHeight));
    }

    void paint (Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    }
    
private:
    AudioProcessorValueTreeState& valueTreeState;
    
    Label gainLabel;
    Slider gainSlider;
    ScopedPointer<SliderAttachment> gainAttachment;
    
    ToggleButton invertButton;
    ScopedPointer<ButtonAttachment> invertAttachment;
};
