#include "merge_iterator.hpp"

#include <vector>
#include <algorithm>
#include <iostream>
#include <random>

std::default_random_engine e;
std::uniform_int_distribution<> d(10, 99);

int main()
{
  int const N = 10;
  std::vector<int> v1; v1.reserve(N);
  std::vector<int> v2; v2.reserve(N/2);

  e.seed(std::random_device{}());
  std::generate_n(std::back_inserter(v1), N, []{ return d(e); });
  std::generate_n(std::back_inserter(v2), N/2, []{ return d(e); });
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());
  
  std::cout << "v1: ";
  std::copy(v1.begin(), v1.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << '\n';

  std::cout << "v2: ";
  std::copy(v2.begin(), v2.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << '\n';

  auto b = make_merge_iterator(v1.begin(), v1.end(), v2.begin(), v2.end());
  auto e = make_merge_iterator(v1.end(), v2.end());

  std::cout << "Merged: ";
  std::copy(b, e, std::ostream_iterator<int>(std::cout, " "));
  std::cout << '\n';

  std::cout << "Adjacent find: ";
  auto it = std::adjacent_find(b, e);
  if (it != e) {
    std::cout << *it << '\n';
  } else {
    std::cout << "none\n";
  }
}
