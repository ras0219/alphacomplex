#pragma once
#include <iostream>

enum LoggingLevel {
  NONE = 0,
  ERROR = 1,
  WARNING = 2,
  DEBUG = 3,
  VERBOSE = 4,
};
template<LoggingLevel lvl>
struct LoggingPolicy {
  enum {
    level = lvl
  };
};

#include <streambuf>
#include <ostream>

template <class cT, class traits = std::char_traits<cT> >
class basic_nullbuf: public std::basic_streambuf<cT, traits> {
  typename traits::int_type overflow(typename traits::int_type c)
  {
    return traits::not_eof(c); // indicate success
  }
};

template <class cT, class traits = std::char_traits<cT> >
class basic_onullstream: public std::basic_ostream<cT, traits> {
public:
  basic_onullstream():
    std::basic_ios<cT, traits>(&m_sbuf),
    std::basic_ostream<cT, traits>(&m_sbuf)
  {
  }

private:
  basic_nullbuf<cT, traits> m_sbuf;
};

typedef basic_onullstream<char> onullstream;
typedef basic_onullstream<wchar_t> wonullstream;

static std::ostream& get_nullstr() {
  static onullstream* ostr = new onullstream();
  return *ostr;
}

static std::ostream& get_level_ostream (LoggingLevel base, LoggingLevel fn) {
  if(base >= fn) {
    switch (fn) {
    case ERROR:
      return std::cerr;
    default:
      return std::cout;
    }
  } else {
    return get_nullstr();
  }
}

const LoggingLevel default_log_level = LoggingLevel::ERROR;
typedef LoggingPolicy<default_log_level> logging_policy_t;

template <LoggingLevel lvl = default_log_level>
struct Logger : LoggingPolicy<lvl> {
  static ostream& error() { return get_level_ostream(lvl, LoggingLevel::ERROR); }
  static ostream& warning() { return get_level_ostream(lvl, LoggingLevel::WARNING); }
  static ostream& debug() { return get_level_ostream(lvl, LoggingLevel::DEBUG); }
  static ostream& verbose() { return get_level_ostream(lvl, LoggingLevel::VERBOSE); }
};

#define LOGGER Logger<>
