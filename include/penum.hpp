#pragma once

#include <cassert>
#include <limits>
#include <tuple>
#include <type_traits>
#include <variant>

namespace impl {

struct default_holder
{
    static constexpr inline int id_v = std::numeric_limits<int>::max();
};

template <typename T, typename F>
struct case_handler
{
    static constexpr inline int id_v = T::id_v;

    case_handler(F f)
        : f_(std::move(f))
    {}

    F f_;
};

template <int x, int ... t>
struct contains
{};

template <int x, int h, int ... t>
struct contains<x, h, t...>
{
    static constexpr bool value = (x == h) || contains<x, t...>::value;
};

template <int x>
struct contains<x>
{
    static constexpr bool value = false;
};

template <int max, int... list>
struct contains_up_to
{
    static constexpr bool value = contains<max - 1, list...>::value && contains_up_to<max - 1, list...>::value;
};

template <int... list>
struct contains_up_to<0, list...>
{
    static constexpr bool value = true;
};

template <int ... list>
struct is_sorted_and_unique
{
    static constexpr bool value = true;
};

template <int f, int s, int ... tail>
struct is_sorted_and_unique<f, s, tail ...>
{
    static constexpr bool value = (f < s) && is_sorted_and_unique<s, tail ...>::value;
};

}

template <typename T, typename F>
inline auto CASE(F f)
{
    return impl::case_handler<T, F>(std::move(f));
}

template <typename F>
inline auto DEFAULT(F f)
{
    return impl::case_handler<impl::default_holder, F>(std::move(f));
}

#define PENUM(name) \
struct name { \
private: \
    using this_type = name; \
    static constexpr inline int __begin_counter = __COUNTER__; \
\
    template <int i> \
    struct case_holder; \
\
public: \
    template <int i> \
    name(case_holder<i> h) \
        : value_(std::in_place_index_t<i>(), h.data_) \
    {} \
\


#define PCASE_DEF(name, id, ...) \
private: \
    template <> \
    struct case_holder<id - 1> { \
        using type_t = typename std::tuple<__VA_ARGS__>; \
        constexpr static inline int id_v = id - 1 - __begin_counter; \
\
        template <typename ... Args> \
        case_holder(Args ... args) \
            : data_(std::forward<Args>(args) ...) \
        {} \
        template <> \
        case_holder() \
            : data_() \
        {} \
\
        type_t data_; \
    }; \
public: \
    using name = case_holder<id - 1>; \


#define PCASE(name, ...) \
PCASE_DEF(name, __COUNTER__, __VA_ARGS__)

#define PENUM_END \
private: \
    static constexpr inline int case_count = __COUNTER__ - __begin_counter - 1; \
\
    template <int i, typename ... Types> \
    struct variant_type : public variant_type<i - 1, typename case_holder<i>::type_t, Types ...> \
    {}; \
    template <typename ... Types> \
    struct variant_type<0, Types ...> \
    { \
        using type_t = std::variant<typename case_holder<0>::type_t, Types ...>; \
    }; \
\
    using data_type = typename variant_type<case_count - 1>::type_t; \
    data_type value_; \
\
private: \
    template <int, typename ... FS> \
    struct switch_handler \
    { \
        inline void static handle(const this_type&) \
        { \
            assert(false); \
        } \
    }; \
\
    template <int index, int i, typename F, typename ... FS> \
    struct switch_handler<index, impl::case_handler<case_holder<i>, F>, FS ...> \
    { \
        inline void static handle(const this_type& v, impl::case_handler<case_holder<i>, F>& f, FS ... fs) \
        { \
            if (v.value_.index() == i) { \
                return std::apply(f.f_, std::get<i>(v.value_)); \
            } \
            if constexpr (index != i) { \
                if (v.value_.index() == index) { \
                    return switch_handler<index, FS ...>::handle(v, fs ...); \
                } \
            } \
            if constexpr (index < case_count - 1) { \
                return switch_handler<index + 1, FS ...>::handle(v, fs ...); \
            } else { \
                assert(false); \
            } \
        } \
    }; \
\
    template <int index, typename F> \
    struct switch_handler<index, impl::case_handler<impl::default_holder, F>> \
    { \
        inline void static handle(const this_type& v, impl::case_handler<impl::default_holder, F>& f) \
        { \
            if (v.value_.index() == index) { \
                if constexpr (std::is_invocable_v<F>) { \
                    return f.f_(); \
                } else { \
                    std::apply(f.f_, std::get<index>(v.value_)); \
                } \
            } else { \
                if constexpr (index < case_count - 1) { \
                    switch_handler<index + 1, impl::case_handler<impl::default_holder, F>>::handle(v, f); \
                } else { \
                    assert(false); \
                } \
            } \
        } \
    }; \
\
public: \
    template <typename ... FS> \
    inline void pswitch(FS ... fs) const \
    { \
        static_assert(impl::contains_up_to<case_count, FS::id_v ...>::value || \
            impl::contains<impl::default_holder::id_v, FS::id_v ...>::value, \
            "pswitch requires all CASEs or DEFAULT to be specified."); \
        static_assert(impl::is_sorted_and_unique<FS::id_v ...>::value, \
            "pswitch requires CASEs to be ordered and DEFAULT specified at the end."); \
        switch_handler<0, FS ...>::handle(*this, fs ...); \
    } \
\
} \
