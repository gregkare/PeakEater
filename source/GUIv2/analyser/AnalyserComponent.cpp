#include "AnalyserComponent.h"

#include <JuceHeader.h>
#include <cmath>

#include "../Utils.h"
#include "../../DSP/ClippingFunctions.h"
#include "../ColourScheme.h"

namespace pe
{
namespace gui
{
namespace
{
// GUI configuration
int constexpr gBorderWidth = 1;
int constexpr gBorderRadius = 10;
} // namespace
AnalyserComponent::AnalyserComponent (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                                      std::shared_ptr<pe::dsp::LevelMeter<float> > inputLevelMeter,
                                      std::shared_ptr<pe::dsp::LevelMeter<float> > clippingLevelMeter,
                                      std::shared_ptr<pe::dsp::LevelMeter<float> > outputLevelMeter)
	: mClipTypeComponent(parameters)
	, mPeakAnalyzerComponent(inputLevelMeter, clippingLevelMeter, outputLevelMeter)
{
	addAndMakeVisible(mClipTypeComponent);
	addAndMakeVisible(mPeakAnalyzerComponent);
}

AnalyserComponent::~AnalyserComponent()
{
	setLookAndFeel (nullptr);
}

void AnalyserComponent::resized()
{
	juce::Grid grid;
	using Track = juce::Grid::TrackInfo;
	using Fr = juce::Grid::Fr;
	using Item = juce::GridItem;
	grid.templateRows = { Track (Fr (1)) };
	grid.templateColumns = { Track (Fr (2)), Track (Fr (3)) };
	grid.items = { Item (mClipTypeComponent), Item (mPeakAnalyzerComponent).withMargin({5, 0, 5, 2}) };
	grid.performLayout (getLocalBounds());
}

void AnalyserComponent::paint (juce::Graphics& g)
{
	auto bounds = getLocalBounds().toFloat().reduced (gBorderWidth);
	g.setColour (colourscheme::BackgroundSecondary.withAlpha(0.9f));
	g.fillRoundedRectangle (bounds, gBorderRadius);
	g.setColour (colourscheme::BackgroundTertiary.withAlpha(0.5f));
	g.drawRoundedRectangle (bounds, gBorderRadius, gBorderWidth);
}
} // namespace gui
} // namespace pe