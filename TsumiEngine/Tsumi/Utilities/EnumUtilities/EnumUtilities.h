#pragma once

#include <type_traits>


// Enumのサイズを取得するメタ関数
template<typename E, typename = void>
struct EnumSize;

// 通常のenumとenum classの両方に対応するための特殊化
template<typename E>
struct EnumSize<E, std::void_t<decltype(E::Count)>> {
    static constexpr std::size_t value = static_cast<std::size_t>(E::Count);
};