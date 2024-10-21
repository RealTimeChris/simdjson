#ifndef SIMDJSON2_INTERNAL_ATOMIC_PTR_H
#define SIMDJSON2_INTERNAL_ATOMIC_PTR_H

#include "simdjson2/base.h"
#include <atomic>

namespace simdjson2 {
namespace internal {

template<typename T>
class atomic_ptr {
public:
  atomic_ptr(T *_ptr) : ptr{_ptr} {}

  operator const T*() const { return ptr.load(); }
  const T& operator*() const { return *ptr; }
  const T* operator->() const { return ptr.load(); }

  operator T*() { return ptr.load(); }
  T& operator*() { return *ptr; }
  T* operator->() { return ptr.load(); }
  atomic_ptr& operator=(T *_ptr) { ptr = _ptr; return *this; }

private:
  std::atomic<T*> ptr;
};

} // namespace internal
} // namespace simdjson2

#endif // SIMDJSON2_INTERNAL_ATOMIC_PTR_H
