//
// Created by Io on 2019-02-22.
//

#include "LCode/SymbolTable.hpp"

bool SymbolTable::contains(std::string const& identifier) const
{
    return _symbols.find(identifier) != _symbols.end();
}

SymbolTable::SymbolType SymbolTable::symbolType(std::string const& identifier) const
{
    auto const it = _symbols.find(identifier);

    return it != _symbols.end() ? it->second->symbolType() : SymbolType::Null;
}

bool SymbolTable::define(std::string const& identifier, SymbolTable::Symbol&& symbol)
{
    return _symbols.emplace(identifier, std::move(symbol)).second;
}

bool SymbolTable::defineType(std::string const& identifier)
{
    return define(identifier, createSymbol<Type>(identifier));
}

bool SymbolTable::defineConstant(std::string const& identifier, StringStorage const& value)
{
    return define(identifier, createSymbol<StringConstant>(value));
}

bool SymbolTable::defineConstant(std::string const& identifier, BooleanStorage value)
{
    return define(identifier, createSymbol<BooleanConstant>(value));
}

bool SymbolTable::defineConstant(std::string const& identifier, FloatStorage value)
{
    return define(identifier, createSymbol<FloatConstant>(value));
}

bool SymbolTable::defineConstant(std::string const& identifier, IntegerStorage value)
{
    return define(identifier, createSymbol<IntegerConstant>(value));
}

//
// class SymbolTable::Function
//

SymbolTable::Function::Function(std::size_t parameterCount)
    : _parameterCount(parameterCount)
{
}

SymbolTable::SymbolType SymbolTable::Function::symbolType() const
{
    return SymbolType::Function;
}

std::size_t SymbolTable::Function::parameterCount() const
{
    return _parameterCount;
}

//
// class SymbolTable::Type
//

SymbolTable::Type::Type(std::string const& typeName)
    : _typeName(typeName)
{
}

SymbolTable::SymbolType SymbolTable::Type::symbolType() const
{
    return SymbolType::Type;
}

std::string const& SymbolTable::Type::typeName() const
{
    return _typeName;
}
