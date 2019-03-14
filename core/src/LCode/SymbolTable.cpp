//
// Created by Io on 2019-03-07.
//

#include "LCode/SymbolTable.hpp"

void SymbolTable::clear()
{
    _types.clear();
    _functions.clear();
    _constants.clear();
}

bool SymbolTable::defineFunction(std::string const& identifier, SymbolTable::FunctionSymbol&& symbol)
{
    return _functions.emplace(identifier, std::move(symbol)).second;
}

bool SymbolTable::isFunctionDefined(std::string const& identifier) const
{
    return _functions.find(identifier) != _functions.end();
}

SymbolTable::FunctionSymbol const& SymbolTable::getFunction(std::string const& identifier) const
{
    assert( isFunctionDefined(identifier) );
    auto it = _functions.find(identifier);
    return it->second;
}

bool SymbolTable::defineType(std::string const& identifier, StorageType type)
{
    return _types.emplace(identifier, type).second;
}

bool SymbolTable::isTypeDefined(std::string const& identifier) const
{
    return _types.find(identifier) != _types.end();
}

StorageType SymbolTable::getType(std::string const& identifier) const
{
    assert( isTypeDefined(identifier) );
    return _types.at(identifier);
}

bool SymbolTable::defineConstant(std::string const& identifier, NumberType value)
{
    return _constants.emplace(identifier, ConstantSymbol{value}).second;
}

bool SymbolTable::isConstantDefined(std::string const& identifier) const
{
    return _constants.find(identifier) != _constants.end();
}

SymbolTable::ConstantSymbol const& SymbolTable::getConstant(std::string const& identifier) const
{
    return _constants.at(identifier);
}