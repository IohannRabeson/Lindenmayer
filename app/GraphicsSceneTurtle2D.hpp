//
// Created by Io on 13/04/2018.
//

#ifndef LINDENMAYER_GRAPHICSSCENETURTLE2D_HPP
#define LINDENMAYER_GRAPHICSSCENETURTLE2D_HPP

#include <ATurtle2D.hpp>

#include <QGraphicsScene>

class GraphicsSceneTurtle2D final : public lcode::ATurtle2D
{
public:
    explicit GraphicsSceneTurtle2D(QGraphicsScene* scene, QPen const& pen = QPen(Qt::SolidLine));

    void setPen(QPen const& pen);
private:
    void drawLine(double x0, double y0, double x1, double y1) override;
private:
    QGraphicsScene* const m_scene;
    QPen m_pen;
};

#endif //LINDENMAYER_GRAPHICSSCENETURTLE2D_HPP
