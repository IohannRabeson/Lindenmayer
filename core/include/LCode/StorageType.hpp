//
// Created by Io on 2019-03-02.
//

#ifndef LINDENMAYER_DEFINITIONS_HPP
#define LINDENMAYER_DEFINITIONS_HPP

enum class StorageType : std::size_t
{
    Null,
    Number,
};

template <StorageType StorageTypeId>
struct StorageTypeTrait;

template <>
struct StorageTypeTrait<StorageType::Null>
{
    using Type = void;
};

template <>
struct StorageTypeTrait<StorageType::Number>
{
    using Type = double;
    static Type fromText(std::string const& str)
    {
        return std::stod(str);
    }
};

template <StorageType StorageTypeId>
using CppType = typename StorageTypeTrait<StorageTypeId>::Type;

#endif //LINDENMAYER_DEFINITIONS_HPP
