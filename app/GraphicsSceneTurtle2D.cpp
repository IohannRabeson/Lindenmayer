//
// Created by Io on 13/04/2018.
//

#include "GraphicsSceneTurtle2D.hpp"

GraphicsSceneTurtle2D::GraphicsSceneTurtle2D(QGraphicsScene* scene, QPen const& pen)
: m_scene(scene)
, m_pen(pen)
{
}

void GraphicsSceneTurtle2D::drawLine(double x0, double y0, double x1, double y1)
{
    m_scene->addLine(QLine(x0, y0, x1, y1), m_pen);
}

void GraphicsSceneTurtle2D::setPen(QPen const& pen)
{
    m_pen = pen;
}
