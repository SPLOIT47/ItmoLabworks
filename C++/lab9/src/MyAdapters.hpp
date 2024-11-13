#include <utility>
#include <concepts>

template<typename T>
struct is_pair : std::false_type {};

template <class T1, class T2>
struct is_pair<std::pair<T1, T2>> : std::true_type {};

template<class T>
concept ContainerType = requires(T t) {
    { t.begin() } -> std::forward_iterator;
    { t.end() } -> std::forward_iterator;
};

template<class T>
concept Iterator = requires(T t) {
    {t.operator++()} -> std::same_as<T&>;
};

template<typename UnaryOperator, typename ValueType>
concept UnaryOperatorReturnsValueType = requires(UnaryOperator op, ValueType value) {
  { op(value) } -> std::same_as<ValueType>;
};

template<typename UnaryPredicate, typename T>
concept PredicateReturnsType = requires(UnaryPredicate pred, T t) {
    { pred(t) } -> std::same_as<bool>;
};

/* TRANSFORM */
template<Iterator InputIterator, Iterator OutputIterator, class UnaryOperator>
OutputIterator my_transform(InputIterator begin, InputIterator end, OutputIterator result, UnaryOperator op) {
  for (; begin != end; ++begin, ++result) {
    *result = op(*begin);
  }

  return result;
}

template<class UnaryOperator, template<typename, typename...> class Container, typename T>
class MyTransform {
public:
    explicit MyTransform(UnaryOperator op) : op_(op) {
        static_assert(UnaryOperatorReturnsValueType<UnaryOperator, T>,
                      "UnaryOperator must return the same type as the container's value_type.");
    }

    template<ContainerType Cont>
    auto operator()(Cont&& cont) const {
        Container<T> result;
        my_transform(cont.begin(), cont.end(), std::inserter(result, result.end()), op_);
        return result;
    }

private:
    UnaryOperator op_;
};

template<template<typename, typename...> class Container, typename T, class UnaryOperator>
MyTransform<UnaryOperator, Container, T> my_transform(UnaryOperator op) {
  return MyTransform<UnaryOperator, Container, T>(op);
}

template<ContainerType Cont, class UnaryOperator, template<typename, typename...> class Container, typename T>
auto operator|(Cont&& cont, MyTransform<UnaryOperator, Container, T> transform) {
    return transform(std::forward<Cont>(cont));
}

/* FILTER */
template<Iterator InputIterator, Iterator OutputIterator, class UnaryPredicate>
OutputIterator my_filter(InputIterator begin, InputIterator end, OutputIterator result, UnaryPredicate op) {
    for (; begin != end; ++begin) {
        if (op(*begin)) {
            *result = *begin;
            ++result;
        }
    }
    return result;
}

template<class UnaryPredicate, template<typename, typename...> class Container, typename T>
class MyFilter {
public:
    explicit MyFilter(UnaryPredicate op) : op_(op) {
        static_assert(PredicateReturnsType<UnaryPredicate, T>,
                      "Predicate must be a predicate.");
    }

    template<ContainerType Cont>
    auto operator()(Cont&& cont) {
        Container<T> result;
        my_filter(cont.begin(), cont.end(), std::inserter(result, result.end()), op_);
        return result;
    }

private:
    UnaryPredicate op_;
};

template<template<typename, typename...> class Container, typename T, class UnaryPredicate>
MyFilter<UnaryPredicate, Container, T> my_filter(UnaryPredicate op) {
  return MyFilter<UnaryPredicate, Container, T>(op);
}

template<ContainerType Cont, class UnaryPredicate, template<typename, typename...> class Container, typename T>
auto operator|(Cont&& cont, MyFilter<UnaryPredicate, Container, T> filter) {
    return filter(std::forward<Cont>(cont));
}

/* TAKE */
template<Iterator InputIterator, Iterator OutputIterator>
OutputIterator my_take(InputIterator begin, InputIterator end, OutputIterator result, int N) {
    for (; N != 0 && begin != end; ++begin, ++result, --N) {
        *result = *begin;
    }
    return result;
}


template<template<typename, typename...> class Container, typename T>
class MyTake {
public:
    explicit MyTake(int N) : N_(N) {}

    template<ContainerType Cont>
    auto operator()(Cont&& cont) {
        Container<T> result;
        my_take(cont.begin(), cont.end(), std::inserter(result, result.end()), N_);
        return result;
    }
private:
    int N_;
};

template<template<typename, typename...> class Container, typename T>
MyTake<Container, T> my_take(int N) {
    return MyTake<Container, T>(N);
}

template<class Cont, template<typename, typename...> class Container, typename T>
auto operator|(Cont&& cont, MyTake<Container, T> take) {
    return take(std::forward<Cont>(cont));
}

/* DROP */

template<Iterator InputIterator, Iterator OutputIterator>
OutputIterator my_drop(InputIterator begin, InputIterator end, OutputIterator result, int N) {
    for (; N != 0 && begin != end; ++begin, --N) {}

    for (; begin != end; ++begin) {
        *result = *begin;
    }
    return result;
}

template<template<typename, typename...> class Container, typename T>
class MyDrop {
public:
    explicit MyDrop(int N) : N_(N) {}

    template<ContainerType Cont>
    auto operator()(Cont&& cont) {
        Container<T> result;
        my_drop(cont.begin(), cont.end(), std::inserter(result, result.end()), N_);
        return result;
    }
private:
    int N_;
};

template<template<typename, typename...> class Container, typename T>
MyDrop<Container, T> my_drop(int N) {
    return MyDrop<Container, T>(N);
}

template<ContainerType Cont, template<typename, typename...> class Container, typename T>
auto operator|(Cont&& cont, MyDrop<Container, T> drop) {
    return drop(std::forward<Cont>(cont));
}

/* REVERSE */

template<Iterator InputIterator, Iterator OutputIterator>
OutputIterator my_reverse(InputIterator begin, InputIterator end, OutputIterator result) {
    --end;
    for(; end != begin; --end, ++result) {
        *result = *end;
    }
    *result = *end;
    return result;
}

template<template<typename, typename...> class Container, typename T>
class MyReverse {
public:
    MyReverse() = default;

    template<class Cont>
    auto operator()(Cont&& cont) {
        Container<T> result;
        my_reverse(cont.begin(), cont.end(), std::inserter(result, result.end()));
        return result;
    }
};

template<template<typename, typename...> class Container, typename T>
MyReverse<Container, T> my_reverse() {
    return MyReverse<Container, T>();
}

template<ContainerType Cont, template<typename, typename...> class Container, typename T>
auto operator|(Cont&& cont, MyReverse<Container, T> reverse) {
    return reverse(std::forward<Cont>(cont));
}

/* KEYS */
template <Iterator InputIterator, Iterator OutputIterator>
OutputIterator my_keys_impl(InputIterator begin, InputIterator end, OutputIterator result, std::false_type) {
    for (; begin != end; ++begin, ++result) {
        *result = *begin;
    }
    return result;
}

template <Iterator InputIterator, Iterator OutputIterator>
OutputIterator my_keys_impl(InputIterator begin, InputIterator end, OutputIterator result, std::true_type) {
    for (; begin != end; ++begin, ++result) {
        *result = begin->first;
    }
    return result;
}

template <Iterator InputIterator, Iterator OutputIterator>
OutputIterator my_keys(InputIterator begin, InputIterator end, OutputIterator result) {
    using ValueType = typename std::iterator_traits<InputIterator>::value_type;
    if constexpr (is_pair<ValueType>::value) {
        return my_keys_impl(begin, end, result, std::true_type{});
    } else {
        return my_keys_impl(begin, end, result, std::false_type{});
    }
}


template<template<typename, typename...> class Container, typename T>
class MyKeys {
public:
    MyKeys() = default;

    template<ContainerType Cont>
    auto operator()(Cont&& cont) {
        Container<T> result;
        my_keys(cont.begin(), cont.end(), std::inserter(result, result.end()));
        return result;
    }
};

template<template<typename, typename...> class Container, typename T>
MyKeys<Container, T> my_keys() {
    return MyKeys<Container, T>();
}

template<ContainerType Cont, template<typename, typename...> class Container, typename T>
auto operator|(Cont&& cont, MyKeys<Container, T> keys) {
    return keys(std::forward<Cont>(cont));
}

/* VALUES */
template <Iterator InputIterator, Iterator OutputIterator>
OutputIterator my_values_impl(InputIterator begin, InputIterator end, OutputIterator result, std::false_type) {
    for (; begin != end; ++begin, ++result) {
        *result = *begin;
    }
    return result;
}

template <Iterator InputIterator, Iterator OutputIterator>
OutputIterator my_values_impl(InputIterator begin, InputIterator end, OutputIterator result, std::true_type) {
    for (; begin != end; ++begin, ++result) {
        *result = begin->second;
    }
    return result;
}

template <Iterator InputIterator, Iterator OutputIterator>
OutputIterator my_values(InputIterator begin, InputIterator end, OutputIterator result) {
    using ValueType = typename std::iterator_traits<InputIterator>::value_type;
    if constexpr (is_pair<ValueType>::value) {
        return my_values_impl(begin, end, result, std::true_type{});
    } else {
        return my_values_impl(begin, end, result, std::false_type{});
    }
}



template<template<typename, typename...> class Container, typename T>
class MyValues {
public:
    MyValues() = default;

    template<ContainerType Cont>
    auto operator()(Cont&& cont) {
        Container<T> result;
        my_values(cont.begin(), cont.end(), std::inserter(result, result.end()));
        return result;
    }
};

template<template<typename, typename...> class Container, typename T>
MyValues<Container, T> my_values() {
    return MyValues<Container, T>();
}


template<ContainerType Cont, template<typename, typename...> class Container, typename T>
auto operator|(Cont&& cont, MyValues<Container, T> values) {
    return values(std::forward<Cont>(cont));
}
