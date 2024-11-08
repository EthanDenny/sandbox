#include "ParticleManager.h"
#include "VectorField.hpp"

void ParticleManager::update(const cv::Mat& data)
{
    const int pixelWidth = data.cols;
    const int pixelHeight = data.rows;
    const auto directions = VectorField::compute(data, cellSize, terrainWeight);

    if (m_particles.size() != particleCount)
    {
        m_particles.clear();
        m_particles.reserve(particleCount);
        for (int i = 0; i < particleCount; ++i)
        {
            m_particles.emplace_back(Particle{ (double)(rand() % pixelWidth), (double)(rand() % pixelHeight) });
        }
    }

    if (resetRequested)
    {
        for (auto& particle : m_particles)
        {
            particle.pos.x = rand() % pixelWidth;
            particle.pos.y = rand() % pixelHeight;
        }

        resetRequested = false;
    }

    auto clampPos = [&](sf::Vector2<double>& pos)
    {
        // TODO: Figure out why particles get stuck on the edges
        pos.x = std::clamp(pos.x, (double)cellSize, (double)pixelWidth);
        pos.y = std::clamp(pos.y, (double)cellSize, (double)(pixelHeight - cellSize - 1));
    };

    for (auto& particle : m_particles)
    {
        // In case the grid size changed
        clampPos(particle.pos);

        auto& dir = directions.get((size_t)(particle.pos.x / cellSize), (size_t)(particle.pos.y / cellSize));
        particle.pos.x += dir.x;
        particle.pos.y += dir.y;

        if (particle.pos.x >= pixelWidth - cellSize)
        {
            particle.pos.x = cellSize;
            particle.pos.y = rand() % pixelHeight;
        }

        particle.trail.push_back({ particle.pos.x, particle.pos.y });
        while (particle.trail.size() > trailLength)
        {
            particle.trail.erase(particle.trail.begin());
        }

        // In case the particle went out of bounds
        clampPos(particle.pos);
    }
}