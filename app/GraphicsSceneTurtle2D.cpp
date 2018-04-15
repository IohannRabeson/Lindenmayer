//
// Created by Io on 13/04/2018.
//

#include "GraphicsSceneTurtle2D.hpp"

GraphicsSceneTurtle2D::GraphicsSceneTurtle2D(QGraphicsScene* scene, QPen const& pen)
: m_scene(scene)
, m_pen(pen)
{
}

void GraphicsSceneTurtle2D::drawLine(QLineF const& line)
{
    m_scene->addLine(line, m_pen);
}

void GraphicsSceneTurtle2D::setPen(QPen const& pen)
{
    m_pen = pen;
}
