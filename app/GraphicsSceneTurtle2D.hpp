//
// Created by Io on 13/04/2018.
//

#ifndef LINDENMAYER_GRAPHICSSCENETURTLE2D_HPP
#define LINDENMAYER_GRAPHICSSCENETURTLE2D_HPP

#include <ATurtle2D.hpp>

#include <QPainterPath>
#include <QPen>

class QGraphicsPathItem;
class QGraphicsScene;

class GraphicsSceneTurtle2D final : public lcode::ATurtle2D
{
public:
    explicit GraphicsSceneTurtle2D(QGraphicsScene* scene, QPen const& pen = QPen(Qt::SolidLine));
    ~GraphicsSceneTurtle2D();

    void setPen(QPen const& pen);
    void start() override;
    void finish() override;
private:
    void drawLine(double x0, double y0, double x1, double y1) override;
private:
    QGraphicsScene* const m_scene;
    std::unique_ptr<QGraphicsPathItem> const m_pathItem;
    QPen m_pen;
    QPainterPath m_painterPath;
    std::vector<QPointF> m_firstPoints;
    QPointF m_lastPoint;
    bool m_firstDraw = false;
    double m_lastX1;
    double m_lastY1;
};

#endif //LINDENMAYER_GRAPHICSSCENETURTLE2D_HPP
