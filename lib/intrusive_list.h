#pragma once

#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

namespace intrusive {

class default_tag;

template <typename T, typename Tag>
class list;

struct list_element_base {
  template <typename T, typename Tag>
  friend class list;

private:
  list_element_base* prev;
  list_element_base* next;

public:
  list_element_base();

  ~list_element_base();

  bool is_linked() const noexcept;

  void unlink() noexcept;

  list_element_base(list_element_base&& other);

  list_element_base& operator=(list_element_base&& other) noexcept;

  list_element_base([[maybe_unused]] const list_element_base& other);

  list_element_base& operator=(const list_element_base& other) noexcept;

  list_element_base* link_before(list_element_base* node) noexcept;

  list_element_base* unlink_and_link_neighbours() noexcept;
};

template <typename Tag = default_tag>
class list_element : private list_element_base {
  template <typename T, typename Tg>
  friend class list;
};

extern template class list_element<>;

template <typename T, typename Tag = default_tag>
class list {
  static_assert(std::is_base_of_v<list_element<Tag>, T>, "T must derive from list_element");

public:
  using node = list_element<Tag>;

  template <typename U>
  struct list_base_iterator {
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = U;
    using reference = U&;
    using pointer = U*;

    list_base_iterator() = default;
    list_base_iterator(const list_base_iterator& other) = default;
    ~list_base_iterator() = default;

    list_base_iterator(list_base_iterator&& other)
        : _current(std::exchange(other._current, nullptr)) {}

    operator list_base_iterator<const U>() const noexcept {
      return list_base_iterator<const U>(_current);
    }

    void swap(list_base_iterator& other) noexcept {
      std::swap(_current, other._current);
    }

    list_base_iterator& operator=(const list_base_iterator& other) noexcept {
      list_base_iterator tmp(other);
      swap(tmp);
      return *this;
    }

    list_base_iterator& operator=(list_base_iterator&& other) noexcept {
      list_base_iterator tmp(std::move(other));
      swap(tmp);
      return *this;
    }

    reference operator*() noexcept {
      return as_reference(_current);
    }

    reference operator*() const noexcept {
      return as_reference(_current);
    }

    pointer operator->() noexcept {
      return as_pointer(_current);
    }

    pointer operator->() const noexcept {
      return as_pointer(_current);
    }

    list_base_iterator& operator++() noexcept {
      _current = _current->next;
      return *this;
    }

    list_base_iterator operator++(int) noexcept {
      list_base_iterator tmp = *this;
      ++*this;
      return tmp;
    }

    list_base_iterator& operator--() noexcept {
      _current = _current->prev;
      return *this;
    }

    list_base_iterator operator--(int) noexcept {
      list_base_iterator tmp = *this;
      --*this;
      return tmp;
    }

    bool operator==(const list_base_iterator& other) const noexcept {
      return other._current == _current;
    }

  private:
    static pointer as_pointer(list_element_base* nd) noexcept {
      return static_cast<T*>(static_cast<node*>(nd));
    }

    static reference as_reference(list_element_base* nd) noexcept {
      return *as_pointer(nd);
    }

    list_base_iterator(list_element_base* nd)
        : _current(nd) {}

    list_element_base* _current;

    friend class list;
  };

  using iterator = list_base_iterator<T>;
  using const_iterator = list_base_iterator<const T>;

public:
  // O(1)
  list() noexcept = default;

  // O(1)
  ~list() = default;

  list(const list&) = delete;
  list& operator=(const list&) = delete;

  // O(1)
  list(list&& other) noexcept = default;

  // O(1)
  list& operator=(list&& other) noexcept = default;

  // O(1)
  bool empty() const noexcept {
    return !_sentinel.is_linked();
  }

  // O(n)
  size_t size() const noexcept {
    return std::distance(begin(), end());
  }

  // O(1)
  T& front() noexcept {
    return *begin();
  }

  // O(1)
  const T& front() const noexcept {
    return *begin();
  }

  // O(1)
  T& back() noexcept {
    return *--end();
  }

  // O(1)
  const T& back() const noexcept {
    return *--end();
  }

  // O(1)
  void push_front(T& value) noexcept {
    insert(begin(), value);
  }

  // O(1)
  void push_back(T& value) noexcept {
    insert(end(), value);
  }

  // O(1)
  void pop_front() noexcept {
    erase(begin());
  }

  // O(1)
  void pop_back() noexcept {
    erase(--end());
  }

  // O(1)
  void clear() noexcept {
    _sentinel.unlink();
  }

  // O(1)
  iterator begin() noexcept {
    return iterator(_sentinel.next);
  }

  // O(1)
  const_iterator begin() const noexcept {
    return const_iterator(_sentinel.next);
  }

  // O(1)
  iterator end() noexcept {
    return iterator(&_sentinel);
  }

  // O(1)
  const_iterator end() const noexcept {
    return const_iterator(_sentinel.prev->next);
  }

  // O(1)
  iterator insert(const_iterator pos, T& value) noexcept {
    return list_base_iterator<T>(pos._current->link_before(static_cast<node*>(&value)));
  }

  // O(1)
  iterator erase(const_iterator pos) noexcept {
    return list_base_iterator<T>(pos._current->unlink_and_link_neighbours());
  }

  iterator iterator_to(T& to) noexcept {
    return list_base_iterator<T>(&to);
  }

  const_iterator iterator_to(T const& to) noexcept {
    return list_base_iterator<T>(&to);
  }

  // O(1)
  void splice(const_iterator pos, [[maybe_unused]] list& other, const_iterator first, const_iterator last) noexcept {
    if (first == last) {
      return;
    }
    list_element_base* local = pos._current;
    list_element_base* there_begin = first._current;
    list_element_base* there_end = last._current;

    local->prev->next = there_begin;
    there_end->prev->next = local;
    there_begin->prev->next = there_end;

    std::swap(there_end->prev, there_begin->prev);
    std::swap(local->prev, there_begin->prev);
  }

private:
  node _sentinel;
};

} // namespace intrusive
