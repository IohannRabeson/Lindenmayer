//
// Created by Io on 11/04/2018.
//

#ifndef LINDENMAYER_SYMBOL_HPP
#define LINDENMAYER_SYMBOL_HPP
#include <cstdint>
#include <string>

namespace lcode
{
    /*!
     * \brief A symbol is an instruction identifier.
     *
     * Symbols are atomic, they can't be splitted. In L-Code, a symbol
     * is represented by an identifier.
     */
    class Symbol
    {
    public:
        using Integer = std::uint8_t;

        Symbol();
        explicit Symbol(Integer const identifier);

        bool isNull() const;

        friend std::string toString(Symbol const);
        friend bool operator == (Symbol const, Symbol const);
        friend bool operator != (Symbol const, Symbol const);
        friend bool operator <= (Symbol const, Symbol const);
        friend bool operator >= (Symbol const, Symbol const);
        friend bool operator < (Symbol const, Symbol const);
        friend bool operator > (Symbol const, Symbol const);
    private:
        Integer m_identifier;
    };
}

#endif //LINDENMAYER_SYMBOL_HPP
