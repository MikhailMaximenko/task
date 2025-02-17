#include "intrusive_list.h"

#include <utility>

namespace intrusive {
list_element_base::list_element_base()
    : prev(this)
    , next(this) {}

list_element_base::~list_element_base() {
  unlink();
}

bool list_element_base::is_linked() const noexcept {
  return prev != this;
}

void list_element_base::unlink() noexcept {
  prev->next = next;
  next->prev = prev;
  next = prev = this;
}

list_element_base::list_element_base(list_element_base&& other)
    : list_element_base() {
  *this = std::move(other);
}

list_element_base& list_element_base::operator=(list_element_base&& other) noexcept {
  if (this == &other) {
    return *this;
  }
  unlink();
  if (!other.is_linked()) {
    return *this;
  }
  prev = std::exchange(other.prev, &other);
  next = std::exchange(other.next, &other);
  prev->next = this;
  next->prev = this;
  return *this;
}

list_element_base::list_element_base([[maybe_unused]] const list_element_base& other)
    : list_element_base() {}

list_element_base& list_element_base::operator=(const list_element_base& other) noexcept {
  if (this != &other) {
    unlink();
  }
  return *this;
}

list_element_base* list_element_base::link_before(list_element_base* node) noexcept {
  if (this == node) {
    return this;
  }
  node->unlink();
  node->prev = prev;
  node->next = this;
  prev->next = node;
  prev = node;
  return node;
}

list_element_base* list_element_base::unlink_and_link_neighbours() noexcept {
  if (!is_linked()) {
    return this;
  }
  prev->next = next;
  next->prev = prev;
  auto* nxt = next;
  unlink();
  return nxt;
}

} // namespace intrusive
