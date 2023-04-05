#include "CentralPanel.h"

namespace pe
{
namespace gui
{
CentralPanel::CentralPanel (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                            std::shared_ptr<pe::dsp::LevelMeter<float> > inputLevelMeter,
                            std::shared_ptr<pe::dsp::LevelMeter<float> > clippingLevelMeter,
                            std::shared_ptr<pe::dsp::LevelMeter<float> > outputLevelMeter,
                            std::shared_ptr<Ticks> ticks)
	: juce::Component()
	, mClipMeter (parameters, inputLevelMeter, clippingLevelMeter, outputLevelMeter, ticks)
	, mControlPanel (parameters, inputLevelMeter, clippingLevelMeter)
	, mLinkingPanel (parameters)
	, mAnalyserComponent (parameters, inputLevelMeter, clippingLevelMeter, outputLevelMeter)
	, mScalingSwitch(ticks)
{
	addAndMakeVisible (mClipMeter);
	addAndMakeVisible (mControlPanel);
	addAndMakeVisible (mLinkingPanel);
	addAndMakeVisible (mAnalyserComponent);
	addAndMakeVisible (mScalingSwitch);
}

void CentralPanel::resized()
{
	auto const localBounds = getLocalBounds();

	mClipMeter.setBounds (localBounds);

	juce::Grid grid;
	using Track = juce::Grid::TrackInfo;
	using Fr = juce::Grid::Fr;
	using Item = juce::GridItem;
	grid.templateRows = { Track (Fr (3)), Track (Fr (2)) };
	grid.templateColumns = { Track (Fr (1)) };
	grid.items = { Item (mControlPanel), Item (mLinkingPanel) };
	auto const toRemoveFromTop = localBounds.proportionOfHeight (0.65f);
	auto const toRemoveFromSides = localBounds.proportionOfWidth (0.18f);
	grid.performLayout (localBounds
	                    .withTrimmedTop (toRemoveFromTop)
	                    .withTrimmedLeft (toRemoveFromSides)
	                    .withTrimmedRight (toRemoveFromSides));

	mAnalyserComponent.setBounds(localBounds
	                             .withWidth(localBounds.proportionOfWidth (0.28f))
	                             .withHeight(localBounds.proportionOfHeight (0.155f))
	                             .withX(localBounds.proportionOfWidth (0.1f))
	                             .withY(localBounds.proportionOfHeight (0.05f)));

	mScalingSwitch.setBounds(localBounds
	                         .withWidth(localBounds.proportionOfWidth(0.076f))
	                         .withHeight(localBounds.proportionOfWidth(0.076f))
	                         .withX(localBounds.proportionOfWidth (0.84f))
	                         .withY(localBounds.proportionOfHeight (0.71f)));
}
} // namespace gui
} // namespace pe