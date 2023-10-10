/*
 * JellyFish Javascript Engine
 *
 * Copyright (c) 2023 Giorgi Kavrelishvili
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef JELLYFISH_H
#define JELLYFISH_H

#include <gc/gc.h> // Boehm GC
#include <string.h>
#include <stdlib.h> // abort()
#include <stdio.h>  // fprintf()

// Helper structure to transfer a `String` between C/C++ and Crystal.
typedef struct CrystalString
{ // C compatibility
  const char *ptr;
  int size;
} CrystalString;

// Compiler branching hint
#define bindgen_likely(x) __builtin_expect(!!(x), 1)

static __attribute__((noreturn)) void bindgen_fatal_panic(const char *message)
{
  fprintf(stderr, "Fatal error in bindings: %s\n", message);
  abort();
}

#ifdef __cplusplus
#include <gc/gc_cpp.h>
#include <quickjs.h>
#include <string>
#include <iostream>
#include <type_traits>
#include <utility>
#include <new>

// Break C++'s encapsulation to allow easy wrapping of protected methods.
#define protected public

template<int, typename Callable, typename Ret, typename... Args>
auto fnptr_(Callable&& c, Ret (*)(Args...))
{
    static std::decay_t<Callable> storage = std::forward<Callable>(c);
    static bool used = false;
    if(used)
    {
        using type = decltype(storage);
        storage.~type();
        new (&storage) type(std::forward<Callable>(c));
    }
    used = true;

    return [](Args... args) -> Ret {
        auto& c = *std::launder(&storage);
        return Ret(c(std::forward<Args>(args)...));
    };
}

template<typename Fn, int N = 0, typename Callable>
Fn* fnptr(Callable&& c)
{
    return fnptr_<N>(std::forward<Callable>(c), (Fn*)nullptr);
}

/* Wrapper for a Crystal `Proc`. */
template <typename T, typename... Args>
struct CrystalProcedure
{
  union
  {
    T(*withSelf)
    (void *, Args...);
    T(*withoutSelf)
    (Args...);
  };

  void *self;

  CrystalProcedure() : withSelf(nullptr), self(nullptr) {}

  inline bool isValid() const
  {
    return (withSelf != nullptr);
  }

  /* Fun fact: If the Crystal `Proc` doesn't capture any context, it won't
   * allocate any - But also don't expect any!  We have to accomodate for this
   * by only passing `this->self` iff it is non-NULL.
   */

  T operator()(Args... arguments) const
  {
    if (this->self)
    {
      return this->withSelf(this->self, arguments...);
    }
    else
    {
      return this->withoutSelf(arguments...);
    }
  }
};

template <typename T>
struct CrystalGCWrapper : public T, public gc_cleanup
{
  using T::T;
};

/// A simple wrapper around a non-pointer type that allows a single
/// dereference operation.
template <typename T>
struct bg_deref
{
  T data;

  template <typename... Args>
  bg_deref(Args &&...args) : data(std::forward<Args>(args)...) {}

  T operator*() && { return std::move(data); }
};

extern "C"
{
  JSRuntime *createRuntime();
  JSContext *createContext(JSRuntime *runtime);

  JSValue evaluateSourceCode(JSContext *context, const char * sourceCode, int evaluationFlags);

  JSValue getException(JSContext *context);

  CrystalString valueToCrystalString(JSContext *context, JSValue value);
}
#endif // __cplusplus
#endif // JELLYFISH_H