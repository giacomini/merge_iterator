#ifndef MERGE_ITERATOR_HPP
#define MERGE_ITERATOR_HPP

#include <iterator>
#include <algorithm>
#include <cassert>
#include <array>
#include <type_traits>

// all_same taken from
// https://stackoverflow.com/questions/18017543/c11-variable-number-of-arguments-same-specific-type

template<typename... T>
struct all_same : std::false_type { };

template<>
struct all_same<> : std::true_type { };

template<typename T>
struct all_same<T> : std::true_type { };

template<typename T, typename... Ts>
struct all_same<T, T, Ts...> : all_same<T, Ts...> { };

template<typename... T>
struct front;

template<typename T, typename... R>
struct front<T, R...> { using type = T; };

template<typename Iterator, size_t N>
class merge_iterator
{
  std::array<Iterator, N*2> iterators;
  int which;
 public:
  using value_type = typename std::iterator_traits<Iterator>::value_type;
  using iterator_category = std::forward_iterator_tag;
  using difference_type = typename std::iterator_traits<Iterator>::difference_type;
  using reference = typename std::iterator_traits<Iterator>::reference;
  using pointer = typename std::iterator_traits<Iterator>::pointer;

  merge_iterator()
      : iterators{}, which{-1}
  {}

  template<typename... It, typename std::enable_if<all_same<It...>::value && sizeof...(It) == N, void*>::type = nullptr>
  merge_iterator(It... l)
      : iterators{}
      , which(-1)
  {
    std::array<Iterator, N> its { l... };
    for (int i = 0; i != N; ++i) {
      iterators[2*i] = its[i];
      iterators[2*i+1] = its[i];
    }
  }

  template<typename... It, typename std::enable_if<all_same<It...>::value &&  sizeof...(It) == N*2, void*>::type = nullptr>
  merge_iterator(It... l)
      : iterators{l...}
      , which(-1)
  {
    which = find_min_element();
  }

  reference operator*()
  {
    assert(which % 2 == 0);
    return *iterators[which];
  }

  pointer operator->()
  {
    assert(which % 2 == 0);
    return iterators[which];
  }

  merge_iterator& operator++()
  {
    assert(which == -1 || (which % 2 == 0 && (iterators[which] != iterators[which+1])));

    if (which != -1)
    {
      ++iterators[which];
      which = find_min_element();
    }

    return *this;
  }

  merge_iterator operator++(int)
  {
    merge_iterator result = *this;
    ++*this;
    return result;
  }

  friend bool operator==(merge_iterator const& l, merge_iterator const& r)
  {
    return l.iterators == r.iterators;
  }

 private:
  int find_min_element()
  {
    int n = 0;
    std::array<int, N> a;
    for (int i = 0; i != 2*N; i += 2) {
      if (iterators[i] != iterators[i+1]) {
        a[n++] = i;
      }
    }
    if (n) {
      auto it = std::min_element(a.begin(), a.begin() + n, [this](int i, int j) { return *iterators[i] < *iterators[j]; });
      return *it;
    } else {
      return -1;
    }
  }

};

template<typename Iterator, size_t N>
bool operator!=(merge_iterator<Iterator, N> const& l, merge_iterator<Iterator, N> const& r)
{
  return !(l == r);
}

template<typename... Iterator>
auto make_begin_merge_iterator(Iterator... its)
{
  static_assert(sizeof...(Iterator) % 2 == 0, "there must be an even number of iterators");
  return merge_iterator<typename front<Iterator...>::type, sizeof...(Iterator) / 2>{its...};
}

template<typename... Iterator>
auto make_end_merge_iterator(Iterator... its)
{
  return merge_iterator<typename front<Iterator...>::type, sizeof...(Iterator)>{its...};
}

#endif
