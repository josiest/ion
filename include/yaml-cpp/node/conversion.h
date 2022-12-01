#pragma once
#include <string>
#include <sstream>
#include <concepts>

namespace YAML::conversion {
inline bool IsInfinity(const std::string& input) {
  return input == ".inf" || input == ".Inf" || input == ".INF" ||
         input == "+.inf" || input == "+.Inf" || input == "+.INF";
}

inline bool IsNegativeInfinity(const std::string& input) {
  return input == "-.inf" || input == "-.Inf" || input == "-.INF";
}

inline bool IsNaN(const std::string& input) {
  return input == ".nan" || input == ".NaN" || input == ".NAN";
}

template <typename T>
concept OutputStreamable = requires(const T& t, std::stringstream & stream) {
    { stream << t };
};
template <typename T>
concept InputStreamable = requires(T& t, std::stringstream & stream) {
    { stream >> t };
};

template <OutputStreamable T> void
inner_encode(const T& rhs, std::stringstream& stream){
  stream << rhs;
}

// not ambiguous since C++ will prefer "more specialized" types
template <OutputStreamable T>
requires std::floating_point<T>
void inner_encode(const T& rhs, std::stringstream& stream){
  if (std::isnan(rhs)) {
    stream << ".nan";
  } else if (std::isinf(rhs)) {
    if (std::signbit(rhs)) {
      stream << "-.inf";
    } else {
      stream << ".inf";
    }
  } else {
    stream << rhs;
  }
}

template <InputStreamable T>
bool ConvertStreamTo(std::stringstream& stream, T& rhs) {
  if ((stream >> std::noskipws >> rhs) && (stream >> std::ws).eof()) {
    return true;
  }
  return false;
}

template <typename T>
requires (std::same_as<T, signed char> || std::same_as<T, unsigned char>)
bool ConvertStreamTo(std::stringstream& stream, T& rhs) {
  int num;
  if ((stream >> std::noskipws >> num) && (stream >> std::ws).eof()) {
    if (num >= (std::numeric_limits<T>::min)() &&
        num <= (std::numeric_limits<T>::max)()) {
      rhs = static_cast<T>(num);
      return true;
    }
  }
  return false;
}
}