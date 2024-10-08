#pragma once

#include "opencv2/core.hpp"
#include <iostream>

namespace HeatMap
{
	constexpr float CelciusFactor = 100.f;
	constexpr float KelvinDiff = 273.15f;

	enum class Algorithms
	{
		Average,
		HeatEquation
	};

	static const char* AlgorithmNames[] = {
		"Average",
		"Heat Equation"
	};

	class HeatSource
	{
		// All temperatures are Celcius, in the range [0.0, 100.0]
		// However, they are stored as [0.0, 1.0]
		float temp;

	public:
		cv::Rect area;

		HeatSource(const cv::Rect& area, const float temp) : area(area)
		{
			setTempCelcius(temp);
		}

		HeatSource(const cv::Point& position, const cv::Size& size, const float temp) : area(cv::Rect2i{ position, size })
		{
			setTempCelcius(temp);
		}

		void setTempCelcius(const float temp)
		{
			this->temp = temp / CelciusFactor;
		}

		float getTempCelcius() const
		{
			return temp * CelciusFactor;
		}

		void setTempKelvin(const float temp)
		{
			this->temp = (temp + KelvinDiff) / CelciusFactor;
		}

		float getTempKelvin() const
		{
			return (temp * CelciusFactor) - KelvinDiff;
		}

		float getTempRaw() const
		{
			return temp;
		}
	};

	class Grid
	{
		cv::Mat temps{};
		std::vector<HeatSource> sources{};
		bool restartRequested = false;
		int stepsRequested = 0;

	public:
		Algorithms algorithm = Algorithms::HeatEquation;

		Grid(std::vector<HeatSource> sources = {}) : sources(sources) {}

		const cv::Mat& data() const
		{
			return temps;
		}

		void requestStep(int count = 1)
		{
			if (count > 0)
			{
				stepsRequested = count;
			}
		}

		void reset()
		{
			stepsRequested = 0;
			restartRequested = true;
		}

		void addSource(const HeatSource& source)
		{
			sources.push_back(source);
			updateSources();
		}

		void clearSources()
		{
			sources.clear();
		}

		void update(const cv::Mat& kMat);
		void updateSources();
		float getNewTemp(int i, int j, const cv::Mat& kMat = {});
	};
}
