//
// Created by Io on 2019-02-22.
//

#ifndef LINDENMAYER_LCODESYMBOLTABLE_HPP
#define LINDENMAYER_LCODESYMBOLTABLE_HPP
#include <string>
#include <map>
#include <variant>
#include <memory>
#include <functional>

#include "StorageType.hpp"

class SymbolTable
{
public:
    class ASymbol;
    using Symbol = std::unique_ptr<ASymbol>;
public:
    enum class SymbolType;

    template <typename T>
    class Constant;
    class IntegerConstant;
    class FloatConstant;
    class StringConstant;
    class BooleanConstant;
    class Function;
    class Type;

    template <typename S, typename ... A>
    static Symbol createSymbol(A&&...args)
    {
        static_assert( std::is_base_of<ASymbol, S>::value, "S must be derived from ASymbol" );
        return std::make_unique<S>(std::forward<A>(args)...);
    }

    bool defineType(std::string const& identifier);

    bool defineConstant(std::string const& identifier, CppType<StorageType::Integer> value);
    bool defineConstant(std::string const& identifier, CppType<StorageType::Float> value);
    bool defineConstant(std::string const& identifier, CppType<StorageType::Boolean> value);
    bool defineConstant(std::string const& identifier, CppType<StorageType::String> const& value);

    bool contains(std::string const& identifier) const;
    SymbolType symbolType(std::string const& identifier) const;
    bool define(std::string const& identifier, Symbol&& symbol);
private:
    std::map<std::string, Symbol> _symbols;
};

enum class SymbolTable::SymbolType
{
    Null,
    Constant,
    Function,
    Type
};

class SymbolTable::ASymbol
{
public:
    virtual ~ASymbol() = default;
    virtual SymbolType symbolType() const = 0;
};

template <typename T>
class SymbolTable::Constant : public ASymbol
{
    T const _storage;
    StorageType const _type;
public:
    using ValueType = T;

    explicit Constant(StorageType type, T const& value)
    : _storage(value)
    , _type(type)
    {
    }

    StorageType type() const
    {
        return _type;
    }

    SymbolType symbolType() const override
    {
        return SymbolType::Constant;
    }

    T const& value() const
    {
        return _storage;
    }
};

class SymbolTable::IntegerConstant final : public Constant<CppType<StorageType::Integer>>
{
public:
    explicit IntegerConstant(ValueType value);
};

class SymbolTable::FloatConstant final : public Constant<CppType<StorageType::Float>>
{
public:
    explicit FloatConstant(ValueType value);
};

class SymbolTable::StringConstant final : public Constant<CppType<StorageType::String>>
{
public:
    explicit StringConstant(ValueType const& value);
};

class SymbolTable::BooleanConstant final : public Constant<CppType<StorageType::Boolean>>
{
public:
    explicit BooleanConstant(ValueType value);
};

class SymbolTable::Function final : public ASymbol
{
    std::size_t const _parameterCount;
public:
    explicit Function(std::size_t parameterCount);
    SymbolType symbolType() const override;
    std::size_t parameterCount() const;
};

class SymbolTable::Type final : public ASymbol
{
    std::string const _typeName;
public:
    explicit Type(std::string const& typeName);
    virtual ~Type() = default;
    virtual SymbolType symbolType() const;
    std::string const& typeName() const;
};

#endif //LINDENMAYER_LCODESYMBOLTABLE_HPP
