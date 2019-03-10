//
// Created by Io on 2019-03-07.
//

#ifndef LINDENMAYER_SYMBOLTABLE_HPP
#define LINDENMAYER_SYMBOLTABLE_HPP
#include <map>
#include <string>
#include <variant>
#include <tree/ParseTree.h>
#include "LCode/StorageType.hpp"

/*!
 * \brief Storag for static definitions
 */
class SymbolTable
{
public:
    struct FunctionSymbol
    {
        StorageType _returnType;
        std::vector<StorageType> _parameterTypes;
    };

    void clear();

    bool defineFunction(std::string const& identifier, FunctionSymbol&& symbol);
    bool isFunctionDefined(std::string const& identifier) const;
    FunctionSymbol const& getFunction(std::string const& identifier) const;

    bool defineType(std::string const& identifier, StorageType type);
    bool isTypeDefined(std::string const& identifier) const;
    StorageType getType(std::string const& identifier) const;

    bool defineConstant(std::string const& identifier, StorageTypeTrait<StorageType::Number>::Type value);
    bool isConstantDefined(std::string const& identifier) const;
    StorageTypeTrait<StorageType::Number>::Type getConstantType(std::string const& identifier) const;
private:
    std::map<std::string, StorageType> _types;
    std::map<std::string, FunctionSymbol> _functions;
    std::map<std::string, StorageTypeTrait<StorageType::Number>::Type> _constants;
};

#endif //LINDENMAYER_SYMBOLTABLE_HPP
