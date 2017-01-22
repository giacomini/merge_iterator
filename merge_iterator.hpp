#ifndef MERGE_ITERATOR_HPP
#define MERGE_ITERATOR_HPP

#include <iterator>
#include <vector>
#include <algorithm>
#include <cassert>
#include <array>
#include <iostream>
#include <random>

template<typename Iterator>
class merge_iterator
{
  std::array<Iterator, 2> first;
  std::array<Iterator, 2> last;
  int which;
 public:
  using value_type = typename std::iterator_traits<Iterator>::value_type;
  using iterator_category = std::forward_iterator_tag;
  using difference_type = typename std::iterator_traits<Iterator>::difference_type;
  using reference = typename std::iterator_traits<Iterator>::reference;
  using pointer = typename std::iterator_traits<Iterator>::pointer;

  merge_iterator(Iterator l1 = Iterator{}, Iterator l2 = Iterator{})
      : first{l1, l2}
      , last{l1, l2}
      , which(-1)
  {}
  merge_iterator(Iterator f1, Iterator l1, Iterator f2, Iterator l2)
      : first{f1, f2}
      , last{l1, l2}
      , which(-1)
  {
    which = find_min_element();
  }

  reference operator*()
  {
    assert(which != -1);
    return *first[which];
  }

  pointer operator->()
  {
    assert(which != -1);
    return first[which];
  }

  merge_iterator& operator++()
  {
    assert(which == -1 || which != -1 && (first[which] != last[which]));

    if (which != -1)
    {
      ++first[which];
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
    return l.first == r.first;
  }

 private:
  int find_min_element()
  {
    int i = 0;
    std::array<int, 2> a;
    if (first[0] != last[0]) {
      a[i++] = 0;
    }
    if (first[1] != last[1]) {
      a[i++] = 1;
    }
    if (i) {
      auto it = std::min_element(a.begin(), a.begin() + i, [this](int i, int j) { return *first[i] < *first[j]; });
      return *it;
    } else {
      return -1;
    }
  }
};

template<typename Iterator>
bool operator!=(merge_iterator<Iterator> const& l, merge_iterator<Iterator> const& r)
{
  return !(l == r);
}

template<typename Iterator>
auto make_merge_iterator(Iterator f1, Iterator l1, Iterator f2, Iterator l2)
{
  return merge_iterator<Iterator>{f1, l1, f2, l2};
}

template<typename Iterator>
auto make_merge_iterator(Iterator l1, Iterator l2)
{
  return merge_iterator<Iterator>{l1, l2};
}

#endif
