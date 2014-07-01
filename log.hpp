#ifndef MESOSPHERE__LOG__LOG_HPP
#define MESOSPHERE__LOG__LOG_HPP

// =============================================================================

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <ostream>
#include <sstream>
#include <iostream>
#include <string>

#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <syscall.h>

// =============================================================================

namespace mesosphere {
namespace log        {

// =============================================================================

struct Level
{

enum type : int
{
      Trace
    , Debug
    , Info
    , Warning
    , Error
    , Critical
    , Fatal
}; // enum type

}; // struct Level


std::ostream &  operator<<(std::ostream &  output, Level::type const &  level);

// -----------------------------------------------------------------------------

struct StreamBase
{
    static std::string  program_name_;
}; // StreamBase


template <Level::type  LEVEL>
class Stream
    : public StreamBase
{
public:

    Stream( std::string const &  facility_name
          , std::string const &  facility_ident
          , std::string const &  session_ident)
        : moved_(false)
    {
        boost::posix_time::ptime  time = boost::posix_time::microsec_clock::universal_time();

        output_ << boost::posix_time::to_iso_extended_string(time) << '\t'
                << program_name_       << '\t'
                << getpid()            << '\t'
                << syscall(SYS_gettid) << '\t'
                << LEVEL               << '\t'
                << facility_name       << '\t'
                << facility_ident      << '\t'
                << session_ident       << '\t'
                ;
    }


    Stream(Stream &&  ref)
        : output_(ref.output_.str())
        , moved_(false)
    {
        ref.moved_ = true;
    }


    ~Stream()
    {
        if (moved_) {
            return;
        }

        if (LEVEL <= Level::Info) {
            output_ << '\n';
        } else {
            output_ << std::endl;
        }

        std::cout << output_.str();
    }


    template <typename TYPE>
    Stream &  operator<<(TYPE const &  value)
    {
        // Do not log invalid messages but continue
        boost::function<void ()>  func = boost::bind(&Stream::set_moved, this);

        output_ << value;

        func = boost::function<void ()>();

        return *this;
    }


private:

    void  set_moved()
    {
        moved_ = true;
    }



private:

    std::stringstream  output_;
    bool               moved_;

}; // class Stream

// =============================================================================

typedef Stream<Level::Trace>     Trace;
typedef Stream<Level::Debug>     Debug;
typedef Stream<Level::Info>      Info;
typedef Stream<Level::Warning>   Warning;
typedef Stream<Level::Error>     Error;
typedef Stream<Level::Critical>  Critical;
typedef Stream<Level::Fatal>     Fatal;

// =============================================================================

class Module
{
public:

    Module( std::string const &  facility_name
          , std::string const &  facility_ident = "");


    Trace     trace   (std::string const &  session_ident = "");
    Debug     debug   (std::string const &  session_ident = "");
    Info      info    (std::string const &  session_ident = "");
    Warning   warning (std::string const &  session_ident = "");
    Error     error   (std::string const &  session_ident = "");
    Critical  critical(std::string const &  session_ident = "");
    Fatal     fatal   (std::string const &  session_ident = "");


private:

    std::string  facility_name_;
    std::string  facility_ident_;
}; // class Module

// =============================================================================

} // namespace log
} // namespace mesosphere

// =============================================================================

#endif // MESOSPHERE__LOG__LOG_HPP
