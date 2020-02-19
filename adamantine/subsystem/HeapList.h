#pragma once

#include <vector>
#include <algorithm>
#include <functional>

template<typename T>
using ItemPredicate = std::function<bool(T*)>;

template<typename T>
class HeapList {
public:
  T* operator[](int index) {
    return list[index];
  }

  ~HeapList() {
    free();
  }

  typename std::vector<T*>::const_iterator begin() const {
    return list.begin();
  }

  typename std::vector<T*>::const_iterator end() const {
    return list.end();
  }

  void free() {
    for (auto* item : list) {
      delete item;
    }

    list.clear();
  }

  int length() const {
    return list.size();
  }

  void push(T* item) {
    list.push_back(item);
  }

  void remove(T* item) {
    list.erase(std::remove(list.begin(), list.end(), item), list.end());

    delete item;
  }

  void removeWhere(ItemPredicate<T> predicate) {
    unsigned int i = 0;

    while (i < list.size()) {
      auto* item = list[i];

      if (predicate(item)) {
        remove(item);
      } else {
        i++;
      }
    }
  }

private:
  std::vector<T*> list;
};