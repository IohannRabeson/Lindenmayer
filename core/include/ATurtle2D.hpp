//
// Created by Io on 13/04/2018.
//

#ifndef LINDENMAYER_ATURTLE_HPP
#define LINDENMAYER_ATURTLE_HPP
#include <stack>
#include <memory>

namespace lcode
{
    class ATurtle2D
    {
        class ATurtle2DImp;
        struct State
        {
            double x = 0;
            double y = 0;
            /*!
             * \brief Direction angle in radians
             */
            double direction = 0;
        };
    public:
        ATurtle2D();
        ATurtle2D(ATurtle2D&&) = default;
        ATurtle2D& operator = (ATurtle2D&&) = default;
        virtual ~ATurtle2D();

        void reset();
        void setPosition(double const x, double const y);
        void setRotation(double const degrees);
        void advance(double const distance, bool const trace);
        void rotate(double const degrees);
        void push();
        void pop();

        State const& getCurrentState() const;
    private:
        State& getCurrentState();

        virtual void drawLine(double const x0, double const y0, double const x1, double const y1) = 0;
    private:
        std::unique_ptr<ATurtle2DImp> m_imp;
    };
}

#endif //LINDENMAYER_ATURTLE_HPP
