#pragma once

#include "Save.hpp"
#include "MarkerData.h"

#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>

class TopographySource
{
public:
    virtual void init() = 0;
    virtual void imgui() = 0;
    virtual void render(sf::RenderWindow & window) = 0;
    virtual void processEvent(const sf::Event & event, const sf::Vector2f & mouse) = 0;
    virtual void save(Save & save) const = 0;
    virtual void load(const Save & save) = 0;

    virtual cv::Mat getTopography() = 0;
    virtual std::vector<MarkerData> getMarkers() { return {}; }
};