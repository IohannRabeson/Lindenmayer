//
// Created by Io on 13/04/2018.
//

#include "ATurtle2D.hpp"

#include <cmath>
#include <cassert>

namespace
{
    template<class T>
    constexpr T Pi = T(3.1415926535897932385L);

    template<class T>
    constexpr T Deg2Rad = Pi<T> / 180;

    template <class T>
    T deg2rad(T const degrees)
    {
        return degrees * Deg2Rad<T>;
    }
}

namespace lcode
{
    ATurtle2D::ATurtle2D()
    {
        m_states.emplace();
    }

    ATurtle2D::~ATurtle2D()
    {
    }

    void ATurtle2D::setPosition(QPointF const& point)
    {
        m_states.top().x = point.x();
        m_states.top().y = point.y();
    }

    void ATurtle2D::setRotation(qreal const degrees)
    {
        m_states.top().direction = degrees * Deg2Rad<qreal>;
    }

    void ATurtle2D::advance(qreal const distance, bool const trace)
    {
        auto& current = getCurrentState();
        auto const old = current;

        current.x += std::cos(current.direction) * distance;
        current.y += std::sin(current.direction) * distance;

        if (trace)
        {
            drawLine(QLineF(old.x, old.y, current.x, current.y));
        }
    }

    void ATurtle2D::rotate(qreal const degrees)
    {
        getCurrentState().direction += degrees * Deg2Rad<qreal>;
    }

    void ATurtle2D::push()
    {
        State state = getCurrentState();

        m_states.push(state);
    }

    void ATurtle2D::pop()
    {
        assert( !m_states.empty() );

        m_states.pop();
    }

    auto ATurtle2D::getCurrentState() const -> State const&
    {
        assert( !m_states.empty() );

        return m_states.top();
    }

    ATurtle2D::State& ATurtle2D::getCurrentState()
    {
        assert( !m_states.empty() );

        return m_states.top();
    }

    void ATurtle2D::reset()
    {
        m_states = std::stack<State>();
        m_states.emplace();
    }
}