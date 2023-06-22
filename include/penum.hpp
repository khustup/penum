#pragma once

#include <tuple>
#include <variant>

namespace impl {

template <typename T, typename F>
struct case_handler
{
    case_handler(F f)
        : f_(std::move(f))
    {}

    F f_;
};

}

template <typename T, typename F>
inline auto CASE(F f)
{
    return impl::case_handler<T, F>(std::move(f));
}

#define PENUM_START(name) \
struct name { \
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
    using name = case_holder<id - 1>; \


#define PCASE(name, ...) \
PCASE_DEF(name, __COUNTER__, __VA_ARGS__)

#define PENUM_END \
    static constexpr inline int case_count = __COUNTER__ - __begin_counter - 1; \
\
private: \
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
    template <typename ... FS> \
    struct switch_handler \
    { \
        void static handle(const this_type& v) \
        {} \
    }; \
\
    template <int i, typename F, typename ... FS> \
    struct switch_handler<impl::case_handler<case_holder<i>, F>, FS ...> \
    { \
        void static handle(const this_type& v, impl::case_handler<case_holder<i>, F>& f, FS ... fs) \
        { \
            if (v.value_.index() == i) { \
                std::apply(f.f_, std::get<i>(v.value_)); \
            } else { \
                switch_handler<FS ...>::handle(v, fs ...); \
            } \
        } \
    }; \
\
public: \
    template <typename ... FS> \
    void pswitch(FS ... fs) const \
    { \
        switch_handler<FS ...>::handle(*this, fs ...); \
    } \
\
}; \
