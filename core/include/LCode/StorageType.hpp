//
// Created by Io on 2019-03-02.
//

#ifndef LINDENMAYER_DEFINITIONS_HPP
#define LINDENMAYER_DEFINITIONS_HPP

enum class StorageType : std::size_t
{
    Null,
    Boolean,
    Integer,
    Float,
    String
};

template <StorageType StorageTypeId>
struct StorageTypeTrait;

template <>
struct StorageTypeTrait<StorageType::Null>
{
    using Type = void;
};

template <>
struct StorageTypeTrait<StorageType::Integer>
{
    using Type = std::int64_t;
    static Type fromText(std::string const& str)
    {
        return std::stoll(str);
    }
};

template <>
struct StorageTypeTrait<StorageType::Float>
{
    using Type = double;
    static Type fromText(std::string const& str)
    {
        return std::stod(str);
    }
};

template <>
struct StorageTypeTrait<StorageType::Boolean>
{
    using Type = bool;
    static Type fromText(std::string const& str)
    {
        return (str == "true");
    }
};

template <>
struct StorageTypeTrait<StorageType::String>
{
    using Type = std::string;

    static Type fromText(std::string const& str)
    {
        return str;
    }
};

template <StorageType StorageTypeId>
using CppType = typename StorageTypeTrait<StorageTypeId>::Type;

#endif //LINDENMAYER_DEFINITIONS_HPP
