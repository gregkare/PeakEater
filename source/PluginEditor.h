/*
   ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

   ==============================================================================
 */
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "GUIv2/MainComponent.h"
#include "PluginProcessor.h"

namespace pe
{
//==============================================================================
class PeakEaterAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
PeakEaterAudioProcessorEditor (PeakEaterAudioProcessor& audioProcessor,
                               std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                               std::shared_ptr<pe::dsp::LevelMeter<float> > inputLevelMeter,
                               std::shared_ptr<pe::dsp::LevelMeter<float> > clippingLevelMeter,
                               std::shared_ptr<pe::dsp::LevelMeter<float> > outputLevelMeter);
~PeakEaterAudioProcessorEditor() override;

//==============================================================================
void paint (juce::Graphics&) override;
void resized() override;

private:
//==============================================================================
pe::gui::MainComponent mMainComponent;

//==============================================================================
JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PeakEaterAudioProcessorEditor)
};
} // namespace pe