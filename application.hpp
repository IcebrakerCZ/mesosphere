#ifndef MESOSPHERE__SERVICE__APPLICATION_HPP
#define MESOSPHERE__SERVICE__APPLICATION_HPP

// =============================================================================

#include "log.hpp"

#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>

#include <string>
#include <vector>
#include <memory>

// =============================================================================

namespace mesosphere {
namespace service    {

// =============================================================================

class Module
    : public boost::noncopyable
{
public:

    Module(std::string const &  name);

    virtual ~Module();


    std::string const &  name() const;


public:

    virtual bool  configure(boost::program_options::options_description &  config);

    virtual bool  start();
    virtual void  stop ();


protected:

    std::string              name_;
    mesosphere::log::Module  log_;
}; // class Module

// =============================================================================

class Application
    : public boost::noncopyable
{
public:

    Application();

    int            main();

    Application &  init(int  argc, char *  argv[]);

    Application &  add_module(std::unique_ptr<Module>  module);


public:

    mesosphere::log::Module  get_log( std::string const &  facility_name
                                    , std::string const &  facility_ident = "");

    boost::asio::io_service &  io_service();

    void  received_signal(int  signum) throw();


private:

    bool  configure() throw();
    bool  start    () throw();
    void  stop     () throw();

    void  io_service_run() throw();

    void  process_signal(int  signum);


private:

    mesosphere::log::Module                          log_;
    std::vector<std::unique_ptr<Module> >            modules_;
    std::vector<std::unique_ptr<Module> >::iterator  i_modules_;

    boost::thread_group            threads_;
    boost::asio::io_service        io_service_;
    boost::asio::io_service::work  io_service_work_;

    int                                             argc_;
    char                                        **  argv_;
    boost::program_options::options_description     config_;
}; // class Application

/* Application singleton */
extern Application  application;

// =============================================================================

} // namespace service
} // namespace mesosphere

// =============================================================================

#endif // MESOSPHERE__SERVICE__APPLICATION_HPP
