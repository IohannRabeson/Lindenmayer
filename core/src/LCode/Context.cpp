//
// Created by Io on 2019-03-09.
//

#include "LCode/Context.hpp"

bool getFunction(std::string const& identifier, Context::ScopeNode const* node, SymbolTable::FunctionSymbol const* symbol)
{
    bool result = false;
    std::function<bool(SymbolTable const&)> f = [&](SymbolTable const& symbols) -> bool
    {
        if (symbols.isFunctionDefined(identifier))
        {
            symbol = &symbols.getFunction(identifier);
            result = true;
            return false;
        }
        return true;
    };

    visitBottomUp(node, std::move(f));

    return result;
}
