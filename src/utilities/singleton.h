#ifndef SRC_UTILITIES_SINGLETON_H__
#define SRC_UTILITIES_SINGLETON_H__

#include <QObject>

template <typename T, typename D = T>
class Singleton : public QObject {
 private:
  friend D;
  static_assert(std::is_base_of<T, D>::value, "T should be a base type for D");

 public:
  static T &instance() {
    static D inst;
    return inst;
  }

 private:
  Singleton() = default;
  ~Singleton() = default;
  Singleton(const Singleton &) = delete;
  Singleton(Singleton &&) = delete;
  Singleton &operator=(const Singleton &) = delete;
  Singleton &operator=(Singleton &&) = delete;
};

#endif  // SRC_UTILITIES_SINGLETON_H__
