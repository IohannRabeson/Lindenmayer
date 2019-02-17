//
// Created by Io on 13/04/2018.
//

#include "GraphicsSceneTurtle2D.hpp"
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <limits>
#include <cmath>

GraphicsSceneTurtle2D::GraphicsSceneTurtle2D(QGraphicsScene* scene, QPen const& pen)
: m_scene(scene)
, m_pathItem(new QGraphicsPathItem)
, m_pen(pen)
, m_firstDraw(true)
{
    m_scene->addItem(m_pathItem.get());
}

GraphicsSceneTurtle2D::~GraphicsSceneTurtle2D() = default;

namespace
{
    template <typename T>
    static inline bool compareFloatingPoint(T left, T right, T epsilon = std::numeric_limits<T>::epsilon())
    {
        return (std::fabs(right - left) < epsilon);
    }

    static inline bool comparePoint(QPointF const& left, QPointF const& right, qreal epsilon = std::numeric_limits<qreal>::epsilon())
    {
        return compareFloatingPoint(left.x(), right.x(), epsilon) && compareFloatingPoint(left.y(), right.y(), epsilon);
    }
}

void GraphicsSceneTurtle2D::drawLine(double x0, double y0, double x1, double y1)
{
    if (m_firstPoints.size() == 0 || !compareFloatingPoint(x0, m_lastX1, 0.001) || !compareFloatingPoint(y0, m_lastY1, 0.001))
    {
        m_painterPath.moveTo(x0, y0);
    }
    if (m_firstPoints.size() < 2)
    {
        m_firstPoints.emplace_back(x0, y0);
    }
    m_lastX1 = x1;
    m_lastY1 = y1;
    m_firstDraw = false;
    m_painterPath.lineTo(x1, y1);
}

void GraphicsSceneTurtle2D::setPen(QPen const& pen)
{
    m_pen = pen;
}

void GraphicsSceneTurtle2D::start()
{
    m_firstDraw = true;
    m_painterPath = QPainterPath();
    m_firstPoints.clear();
}

void GraphicsSceneTurtle2D::finish()
{
    if (!m_firstPoints.empty() && comparePoint(m_firstPoints.front(), QPointF(m_lastX1, m_lastY1), 0.001))
    {
        // If the first point is the same than the last point, then this is likely a polygon
        // and we can try to close it.
        drawLine(m_firstPoints[0].x(), m_firstPoints[0].y(), m_firstPoints[1].x(), m_firstPoints[1].y());
    }
    m_pathItem->setPath(m_painterPath);
    m_pathItem->setPen(m_pen);
}