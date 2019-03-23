//
// Created by Io on 2019-03-07.
//

#ifndef LINDENMAYER_SYMBOLTABLE_HPP
#define LINDENMAYER_SYMBOLTABLE_HPP
#include <map>
#include <string>
#include <tree/ParseTree.h>
#include "LCode/StorageType.hpp"

/*!
 * \brief Storag for static definitions
 */
class SymbolTable
{
    using NumberType = StorageTypeTrait<StorageType::Number>::Type;
public:
    struct FunctionSymbol
    {
        StorageType _returnType;
        std::vector<StorageType> _parameterTypes;
    };

    struct ConstantSymbol
    {
        NumberType const _value;
    };

    void clear();

    bool defineFunction(std::string const& identifier, FunctionSymbol&& symbol);
    bool isFunctionDefined(std::string const& identifier) const;
    FunctionSymbol const& getFunction(std::string const& identifier) const;

    bool defineType(std::string const& identifier, StorageType type);
    bool isTypeDefined(std::string const& identifier) const;
    StorageType getType(std::string const& identifier) const;

    bool defineConstant(std::string const& identifier, NumberType value);
    bool isConstantDefined(std::string const& identifier) const;
    ConstantSymbol const& getConstant(std::string const& identifier) const;
    std::size_t getConstantCount() const { return _constants.size(); }
private:
    std::map<std::string, StorageType> _types;
    std::map<std::string, FunctionSymbol> _functions;
    std::map<std::string, ConstantSymbol> _constants;
};

#endif //LINDENMAYER_SYMBOLTABLE_HPP
