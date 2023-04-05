#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../DSP/LevelMeter.h"
#include "Ticks.h"
#include "ControlPanel.h"
#include "LinkingPanel.h"
#include "analyser/AnalyserComponent.h"
#include "clipmeter/ClipMeter.h"
#include "scaling/ScalingSwitch.h"

namespace pe
{
namespace gui
{

class CentralPanel : public juce::Component
{
public:
CentralPanel (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
              std::shared_ptr<pe::dsp::LevelMeter<float> > inputLevelMeter,
              std::shared_ptr<pe::dsp::LevelMeter<float> > clippingLevelMeter,
              std::shared_ptr<pe::dsp::LevelMeter<float> > outputLevelMeter,
              std::shared_ptr<Ticks> ticks);

void resized() override;

private:
ClipMeter mClipMeter;
ControlPanel mControlPanel;
LinkingPanel mLinkingPanel;
AnalyserComponent mAnalyserComponent;
ScalingSwitch mScalingSwitch;

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CentralPanel)
};
} // namespace gui
} // namespace pe