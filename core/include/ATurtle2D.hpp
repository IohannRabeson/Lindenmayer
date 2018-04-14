//
// Created by Io on 13/04/2018.
//

#ifndef LINDENMAYER_ATURTLE_HPP
#define LINDENMAYER_ATURTLE_HPP
#include <QLine>
#include <stack>

namespace lcode
{
    class ATurtle2D
    {
        struct State
        {
            qreal x = 0;
            qreal y = 0;
            /*!
             * \brief Direction angle in radians
             */
            qreal direction = 0;
        };
    public:
        ATurtle2D();
        virtual ~ATurtle2D();

        void reset();
        void setPosition(QPointF const& point);
        void setRotation(qreal const degrees);
        void advance(qreal const distance, bool const trace);
        void rotate(qreal const degrees);
        void push();
        void pop();

        State const& getCurrentState() const;
    private:
        State& getCurrentState();

        virtual void drawLine(QLineF const& line) = 0;
    private:
        std::stack<State> m_states;
    };
}

#endif //LINDENMAYER_ATURTLE_HPP
