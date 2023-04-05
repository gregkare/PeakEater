#include "LevelMeterComponent.h"

#include "../Utils.h"
#include "../ColourScheme.h"

#include <juce_graphics/juce_graphics.h>
#include <stdint.h>

namespace pe
{
namespace gui
{
namespace
{
float smoothDbValue (float const& dB, juce::LinearSmoothedValue<float>& smoother)
{
	smoother.skip (1);
	if (dB < smoother.getCurrentValue())
	{
		smoother.setTargetValue (dB);
	}
	else
	{
		smoother.setCurrentAndTargetValue (dB);
	}
	return smoother.getCurrentValue();
}
template <typename T>
std::string gToStringWithPrecision (const T a_value, const int n = 6)
{
	std::ostringstream out;
	out.precision (n);
	out << std::fixed << a_value;
	return out.str();
}
} // namespace
LevelMeterComponent::LevelMeterComponent (
	std::shared_ptr<pe::dsp::LevelMeter<float> > levelMeter
	, std::shared_ptr<Ticks> ticks
	, std::string const& name)
	: juce::Component()
	, mName (name)
	, mMeterTimer (std::bind (&LevelMeterComponent::onTimerTick, this))
	, mTicks (ticks)
	, mLevelMeter (levelMeter)
	, mRange (-36.0f, 0.0f, 0.1f, 1.0f)
	, mSmoothedGain (levelMeter->getDecibels())
	, mLabel()
{
	auto const refreshRate = 60;
	mSmoothedGain.reset (refreshRate, 0.5f);
	mMeterTimer.startTimerHz (refreshRate);
}

LevelMeterComponent::~LevelMeterComponent()
{
	setLookAndFeel (nullptr);
}

void LevelMeterComponent::resized()
{
}

void LevelMeterComponent::paint (juce::Graphics& g)
{
	g.fillAll (colourscheme::BackgroundPrimary);

	auto const bounds = getLocalBounds();
	auto const width = static_cast<float> (bounds.getWidth());
	auto const height = static_cast<float> (bounds.getHeight());

	auto const dB = smoothDbValue (mLevelMeter->getDecibels(), mSmoothedGain);
	auto const poxY = gDbToYPos (dB, height, mTicks->isLinear());
	auto const poxX = 0.0f;

	const juce::Point<float> gradientPoint1 = { 0.0f, static_cast<float> (bounds.getBottom()) };
	const juce::Point<float> gradientPoint2 = { 0.0f, static_cast<float> (bounds.getY()) };
	juce::ColourGradient gradient (colourscheme::ForegroundPrimary, gradientPoint1, colourscheme::ForegroundSecondary, gradientPoint2, false);
	gradient.addColour(0.8f, colourscheme::ForegroundSecondary);
	g.setGradientFill (gradient);
	g.fillRect (poxX, poxY, width, height - poxY);

	juce::Colour dbValueTextColour = colourscheme::TextFocusLevel2;
	juce::Colour nameTextClour = colourscheme::TextFocusLevel2;
	if (!isEnabled())
	{
		dbValueTextColour = dbValueTextColour.withAlpha (0.5f);
		nameTextClour = nameTextClour.withAlpha (0.5f);
	}

	// Draw level ticks
	drawTicks (mTicks->getTicksList(), colourscheme::TextFocusLevel3, g);

	// Draw dB value
	auto const padding = 15.0f;
	auto const valueTextSize = calculateTextSize (getTopLevelComponent()->getBounds().getWidth(), getTopLevelComponent()->getBounds().getHeight());
	g.setFont (valueTextSize);
	g.setColour (dbValueTextColour);
	g.drawText (
		std::to_string (static_cast<int> (dB)) + "dB",
		0.0f,
		static_cast<int>(padding),
		static_cast<int>(width),
		static_cast<int>(height),
		juce::Justification::centredTop,
		true);
	auto const textSize = calculatePrimaryTextSize (getTopLevelComponent()->getBounds().getWidth(), getTopLevelComponent()->getBounds().getHeight());

	// Draw name
	g.setFont (textSize);
	g.setColour (nameTextClour);
	g.drawText (
		mName,
		0.0f,
		0.0f,
		static_cast<int>(width),
		static_cast<int>(height - padding),
		juce::Justification::centredBottom,
		true);
}

void LevelMeterComponent::drawTicks (std::vector<float> const& ticksLevels, juce::Colour const&& colour, juce::Graphics& g)
{
	drawTicks (ticksLevels, colour, g);
}

void LevelMeterComponent::drawTicks (std::vector<float> const& ticksLevels, juce::Colour const& colour, juce::Graphics& g)
{
	auto const bounds = getBounds();
	auto const height = static_cast<float> (bounds.getHeight());
	auto const tickWidth = static_cast<float> (bounds.getWidth());
	for (auto const& tickLevel : ticksLevels)
	{
		auto const yPos = gDbToYPos (tickLevel, height, mTicks->isLinear());
		juce::Point<float> start (0.0f, yPos);
		juce::Point<float> end (tickWidth, yPos);
		juce::Line<float> line (start, end);
		g.setColour (colour);
		g.drawLine (line, 0.5f);
	}
}

void LevelMeterComponent::onTimerTick()
{
	repaint();
}
} // namespace gui
} // namespace pe