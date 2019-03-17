//
// Created by Io on 2019-03-02.
//

#ifndef LINDENMAYER_DEFINITIONS_HPP
#define LINDENMAYER_DEFINITIONS_HPP

/*!
 * \brief LCode types identifiers
 * For now only the 'number' type is available.
 * Later I will probably add more type (boolean and string I guess).
 */
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

/*!
 * \brief Get the C++ type used to store a value of a LCode type specified by StorageTypeId
 * \tparam StorageTypeId
 * \code
 * using CppTypeIWillHaveToUseToStoreAValue = CppType<StorageType::Number>;
 * CppTypeIWillHaveToUseToStoreAValue myValueOfTypeNumber = 123;
 * \endcode
 */
template <StorageType StorageTypeId>
using CppType = typename StorageTypeTrait<StorageTypeId>::Type;

#endif //LINDENMAYER_DEFINITIONS_HPP
