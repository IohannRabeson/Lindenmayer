//
// Created by Io on 11/04/2018.
//

#ifndef LINDENMAYER_PROGRAM_HPP
#define LINDENMAYER_PROGRAM_HPP
#include "Module.hpp"
#include "Rewrite.hpp"
#include "ModuleTable.hpp"

#include <vector>

namespace lcode
{
    class ProgramPrivate;

    // TODO: remove that when we can use std::optional
    template <class T>
    class Optional
    {
    public:
        Optional() : m_isValid(false) {}
        template <class U = T> Optional(U&& value): m_value(std::forward<U>(value)), m_isValid(true) {}
        template <class U = T> Optional<T>& operator = (U&& value)
        {
            m_value = std::forward<U>(value);
            m_isValid = true;
            return *this;
        }

        void assign(T const& value) { m_value = value; m_isValid = true; }
        bool isValid() const { return m_isValid; }
        T getValue() const { return m_value; }
    private:
        T m_value;
        bool m_isValid = false;
    };

    class Program
    {
        class LoadFromLCode;
    public:
        struct Error
        {
            std::string message;
            std::size_t line;
            std::size_t charIndex;
        };

        struct Content
        {
            RewriteRules rewriteRules;
            Modules axiom;
            Optional<unsigned int> iterations;
            Optional<float> distance;
            Optional<float> angle;
            Optional<float> initialAngle;
            std::vector<Error> errors;
        };

        class ALoader;

        std::vector<Error> load(ALoader&& loader);
        std::vector<Error> loadFromLCode(std::string const& lcode, ModuleTable const& table);
        Content const& content() const { return m_content; }
        Modules rewrite(unsigned int const iterations) const;
    private:
        Content m_content;
    };

    class Program::ALoader
    {
    public:
        virtual ~ALoader();
        virtual Content load() = 0;
    };
}

#endif //LINDENMAYER_PROGRAM_HPP
