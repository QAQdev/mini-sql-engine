#ifndef MINISQL_BASIC_COMPARATOR_H
#define MINISQL_BASIC_COMPARATOR_H

template<typename T>
class BasicComparator {
public:
  inline int operator()(const T &l, const T &r) const {
    if (l < r) {
      return -1;
    } else if (l > r) {
      return 1;
    }
    return 0;
  }
};

#endif  // MINISQL_BASIC_COMPARATOR_H
