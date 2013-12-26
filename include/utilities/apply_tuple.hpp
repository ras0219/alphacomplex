#pragma once

#include <type_traits>
#include <tuple>

namespace rasutil {
  template<size_t N>
  struct _apply {
    template<class F, class...Args, class...Args2>
    inline auto apply(F& f, std::tuple<Args...>& tup, Args2&&...args2)
      -> decltype(_apply<N - 1>::apply(f, tup, std::get<N - 1>(tup), std::forward<Args2>(args2)...))
    {
      return _apply<N - 1>::apply(f, tup, std::get<N - 1>(tup), std::forward<Args2>(args2)...);
    }
  };

  template<>
  struct _apply<0> {
    template<class F, class...Args, class...Args3>
    inline std::result_of<F(Args3...)> apply(F& f, std::tuple<Args...>&, Args3&&...args3) {
      return f(std::forward<Args3>(args3)...);
    }
  };

  template<class F, class...Args, class...Args2>
  auto apply_tuple(F& f, std::tuple<Args...>& tup, Args2&&...args2)
    -> decltype(_apply<std::tuple_size<std::tuple<Args...>>::value>::apply(f, tup, std::forward<Args2>(args2)...))
  {
    return _apply<std::tuple_size<std::tuple<Args...>>::value>::apply(f, tup, std::forward<Args2>(args2)...);
  }
}
