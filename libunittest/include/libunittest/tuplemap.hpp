/**
 * @brief Tuple function mapping.
 * @file tuplemap.hpp
 */
#pragma once
#include <tuple>
#include <functional>
/**
 * @brief Unit testing in C++
 */
namespace unittest {
/**
 * @brief Internal functionality, not relevant for most users
 */
namespace internals {
/**
 * @brief Type-level index list
 */
template<size_t ...> struct indices
{};
/**
 * @brief Forward declaration of the construct_range struct
 */
template<size_t ...> struct construct_range;
/**
 * @brief Partial specialization of construct_range struct for unfinished ranges.
 */
template< size_t end, size_t idx, size_t ...i >
struct construct_range<end, idx, i... >
     : construct_range<end, idx+1, i..., idx>
{
};
/**
 * @brief Partial specialization of construct_range struct for finished ranges.
 */
template< size_t end, size_t ...i >
struct construct_range< end, end, i... > {
    /**
     * @brief Type level index list
     */
    typedef indices< i... > type;
};
/**
 * @brief Wrapper struct index_range for construct_range struct.
 */
template< size_t b, size_t e >
struct index_range {
    /**
    * @brief Type level index list
    */
    typedef typename construct_range< e, b >::type type;
};
/**
 * @brief Partial specialization of tuple_for_each_index for empty index lists.
 */
template<typename F, typename T, typename ...Args>
void tuple_for_each_index(indices<>, const F&, T&, const Args&...) {
}
/**
 * @brief Partial specialization of tuple_for_each_index for empty index lists.
 */
template<typename F, typename T, typename ...Args>
void tuple_for_each_index(indices<>, const F&, const T&, const Args&...) {
}
/**
 * @brief Apply functor to each tuple element. Use tuple_for_each for actual application.
 * @param f Functor to apply.
 * @param t Tuple to apply f to.
 * @param args Additional arguments to pass to f.
 */
template<size_t i, size_t ...j, typename F, typename T, typename ...Args>
void tuple_for_each_index(indices<i,j...>, const F& f, T& t, const Args&... args) {
    f(std::get<i>(t), args...);
    unittest::internals::tuple_for_each_index(indices<j...>(), f, t, args...);
}
/**
 * @brief Apply functor to each tuple element. Use tuple_for_each for actual application.
 * @param f Functor to apply.
 * @param t Tuple to apply f to.
 * @param args Additional arguments to pass to f.
 */
template<size_t i, size_t ...j, typename F, typename T, typename ...Args>
void tuple_for_each_index(indices<i,j...>, const F& f, const T& t, const Args&... args) {
    f(std::get<i>(t), args...);
    unittest::internals::tuple_for_each_index(indices<j...>(), f, t, args...);
}
/**
 * @brief Apply functor to each tuple element.
 * @param f Functor to apply.
 * @param t Tuple to apply f to.
 * @param args Additional arguments to pass to f.
 */
template<typename F, typename T, typename ...Args>
void tuple_for_each(const F& f, T& t, const Args&... args) {
    constexpr size_t n = std::tuple_size<T>::value;
    typedef typename index_range<0,n>::type index_list;
    unittest::internals::tuple_for_each_index(index_list(), f, t, args...);
}
/**
 * @brief Apply functor to each tuple element.
 * @param f Functor to apply.
 * @param t Tuple to apply f to.
 * @param args Additional arguments to pass to f.
 */
template<typename F, typename T, typename ...Args>
void tuple_for_each(const F& f, const T& t, const Args&... args) {
    constexpr size_t n = std::tuple_size<T>::value;
    typedef typename index_range<0,n>::type index_list;
    unittest::internals::tuple_for_each_index(index_list(), f, t, args...);
}
/**
 * @brief Partial specialization of tuple_transform_index for empty index lists.
 */
template<typename F, typename T, typename R, typename ...Args>
void tuple_transform_index(indices<>, const F&, const T&, R&, const Args&...) {
}
/**
 * @brief Transform tuple elements using functor. Use tuple_transform for actual application.
 * @param f Functor to apply.
 * @param t Tuple to apply f to.
 * @param r Tuple holding resulting elements.
 * @param args Additional arguments to pass to f.
 */
template<size_t i, size_t ...j, typename F, typename T, typename R, typename ...Args>
void tuple_transform_index(indices<i,j...>, const F& f, const T& t, R& r, const Args&... args) {
    std::get<i>(r) = f(std::get<i>(t), args...);
    unittest::internals::tuple_transform_index(indices<j...>(), f, t, r, args...);
}
/**
 * @brief Transform tuple elements using functor.
 * @param f Functor to apply.
 * @param t Tuple to apply f to.
 * @param r Tuple holding resulting elements.
 * @param args Additional arguments to pass to f.
 */
template<typename F, typename T, typename R, typename ...Args>
void tuple_transform(const F& f, const T& t, R& r, const Args&... args) {
    constexpr size_t n = std::tuple_size<T>::value;
    typedef typename index_range<0,n>::type index_list;
    unittest::internals::tuple_transform_index(index_list(), f, t, r, args...);
}

} // internals
} // unittest
