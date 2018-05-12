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
    class ATurtle2D::ATurtle2DImp
    {
    public:
        std::stack<ATurtle2D::State> m_states;
    };

    ATurtle2D::ATurtle2D()
    : m_imp(new ATurtle2DImp)
    {
        m_imp->m_states.emplace();
    }

    ATurtle2D::~ATurtle2D()
    {
    }

    void ATurtle2D::setPosition(double const x, double const y)
    {
        m_imp->m_states.top().x = x;
        m_imp->m_states.top().y = y;
    }

    void ATurtle2D::setRotation(double const degrees)
    {
        m_imp->m_states.top().direction = degrees * Deg2Rad<double>;
    }

    void ATurtle2D::advance(double const distance, bool const trace)
    {
        auto& current = getCurrentState();
        auto const oldX = current.x;
        auto const oldY = current.y;

        current.x += std::cos(current.direction) * distance;
        current.y += std::sin(current.direction) * distance;

        if (trace)
        {
            drawLine(oldX, oldY, current.x, current.y);
        }
    }

    void ATurtle2D::rotate(double const degrees)
    {
        getCurrentState().direction += degrees * Deg2Rad<double>;
    }

    void ATurtle2D::push()
    {
        State state = getCurrentState();

        m_imp->m_states.push(state);
    }

    void ATurtle2D::pop()
    {
        assert( !m_imp->m_states.empty() );

        m_imp->m_states.pop();
    }

    auto ATurtle2D::getCurrentState() const -> State const&
    {
        assert( !m_imp->m_states.empty() );

        return m_imp->m_states.top();
    }

    ATurtle2D::State& ATurtle2D::getCurrentState()
    {
        assert( !m_imp->m_states.empty() );

        return m_imp->m_states.top();
    }

    void ATurtle2D::reset()
    {
        m_imp->m_states = std::stack<State>();
        m_imp->m_states.emplace();
    }
}