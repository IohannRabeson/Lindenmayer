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

class SymbolTable
{
public:
    class ASymbol;
    using Symbol = std::unique_ptr<ASymbol>;
public:
    using IntegerStorage = std::int64_t;
    using FloatStorage = double;
    using BooleanStorage = bool;
    using StringStorage = std::string;

    enum class SymbolType;
    enum class ConstantType;

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
    bool defineConstant(std::string const& identifier, IntegerStorage value);
    bool defineConstant(std::string const& identifier, FloatStorage value);
    bool defineConstant(std::string const& identifier, BooleanStorage value);
    bool defineConstant(std::string const& identifier, StringStorage const& value);

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

enum class SymbolTable::ConstantType
{
    Null,
    Integer,
    Float,
    Boolean,
    String
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
public:
    using ValueType = T;

    explicit Constant(T const& value)
        : _storage(value)
    {
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

class SymbolTable::IntegerConstant final : public Constant<std::int64_t>
{
public:
    using Constant::Constant;
};

class SymbolTable::FloatConstant final : public Constant<double>
{
public:
    using Constant::Constant;
};

class SymbolTable::StringConstant final : public Constant<std::string>
{
public:
    using Constant::Constant;
};

class SymbolTable::BooleanConstant final : public Constant<bool>
{
public:
    using Constant::Constant;
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
