#include "application.hpp"

#include "log.hpp"

#include <cstdlib>

#include <libgen.h>
#include <signal.h>

// =============================================================================

namespace mesosphere {
namespace service    {

// =============================================================================

Application  application;

// =============================================================================

static void  application_signal_handler(int  signum)
{
    application.received_signal(signum);
}

// =============================================================================

Module::Module(std::string const &  name)
    : name_(name)
    , log_(name)
{
}

// -----------------------------------------------------------------------------

Module::~Module()
{
}

// -----------------------------------------------------------------------------

std::string const &  Module::name() const
{
    return name_;
}

// -----------------------------------------------------------------------------

bool  Module::configure(boost::program_options::options_description &  config)
{
    log_.trace() << "configure not defined";
    return true;
}

// -----------------------------------------------------------------------------

bool  Module::start()
{
    log_.trace() << "start not defined";
    return true;
}

// -----------------------------------------------------------------------------

void  Module::stop()
{
    log_.trace() << "stop not defined";
}

// =============================================================================

Application::Application()
    : log_("application")
    , io_service_work_(io_service_)
{
}

// -----------------------------------------------------------------------------

int  Application::main()
{
    if (!configure()) {
        return EXIT_FAILURE;
    }

    if (io_service_.stopped()) {
        return EXIT_SUCCESS;
    }

    signal(SIGTERM, &application_signal_handler);
    signal(SIGINT , &application_signal_handler);

    if (!start()) {
        stop();
        return EXIT_FAILURE + 1;
    }

    io_service_run();

    threads_.join_all();

    modules_.clear();

    return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------

bool  Application::configure() throw()
try {
    i_modules_ = modules_.begin();

    for (; i_modules_ != modules_.end(); ++i_modules_) {
        if (!(*i_modules_)->configure(config_)) {
            return false;
        }
    }


//    config_.add_options()
//            ( "help,h"
//            , "produce this nice helpful message"
//            )
//            ;
//
//    boost::program_options::variables_map  vars;
//
//    boost::program_options::store(boost::program_options::parse_command_line(argc_, argv_, config_), vars);
//
//    vars.notify();
//
//    if (vars.find("help") != vars.end()) {
//        log_.info() << "HELP";
//        io_service_.stop();
//    }

    return true;
} catch (std::exception &  error) {
    log_.fatal() << "Failed to configure " << (*i_modules_)->name()
                 << " due to std exception: " << error.what();
    return false;
} catch (...) {
    log_.fatal() << "Failed to configure " << (*i_modules_)->name()
                 << " due to unknown exception: ";
    return false;
}

// -----------------------------------------------------------------------------

bool  Application::start() throw()
try {
    for (int i = 0; i < 10; ++i) {
        threads_.create_thread(boost::bind(&Application::io_service_run, this));
    }

    i_modules_ = modules_.begin();

    for (; i_modules_ != modules_.end(); ++i_modules_) {
        if (!(*i_modules_)->start()) {
            return false;
        }
    }

    return true;
} catch (std::exception &  error) {
    log_.fatal() << "Failed to start " << (*i_modules_)->name()
                 << " due to std exception: " << error.what();
    return false;
} catch (...) {
    log_.fatal() << "Failed to start " << (*i_modules_)->name()
                 << " due to unknown exception: ";
    return false;
}

// -----------------------------------------------------------------------------

void  Application::stop() throw()
try {
    if (i_modules_ == modules_.end()) {
        --i_modules_;
    }

    for (; i_modules_ >= modules_.begin(); --i_modules_) {
        (*i_modules_)->stop();
    }

    io_service_.stop();
} catch (std::exception &  error) {
    log_.fatal() << "Failed to stop " << (*i_modules_)->name()
                 << " due to std exception: " << error.what();
} catch (...) {
    log_.fatal() << "Failed to stop " << (*i_modules_)->name()
                 << " due to unknown exception: ";
}

// -----------------------------------------------------------------------------

Application &  Application::init(int  argc, char *  argv[])
{
    mesosphere::log::StreamBase::program_name_ = basename(argv[0]);

    argc_ = argc;
    argv_ = argv;

    return *this;
}

// -----------------------------------------------------------------------------

Application &  Application::add_module(std::unique_ptr<Module>  module)
{
    modules_.push_back(std::move(module));

    return *this;
}

// -----------------------------------------------------------------------------

mesosphere::log::Module  Application::get_log( std::string const &  facility_name
                                             , std::string const &  facility_ident)
{
    return mesosphere::log::Module(facility_name, facility_ident);
}

// -----------------------------------------------------------------------------

boost::asio::io_service &  Application::io_service()
{
    return io_service_;
}

// -----------------------------------------------------------------------------

void  Application::received_signal(int  signum) throw()
{
    io_service().post(boost::bind(&Application::process_signal, this, signum));
}

// -----------------------------------------------------------------------------

void  Application::process_signal(int  signum)
{
    log_.trace() << "Processing signal: " << signum;

    if (signum == SIGTERM || signum == SIGINT) {
        log_.info() << "Received SIGTERM or SIGINT so going to stop the application";
        stop();
    }
}

// -----------------------------------------------------------------------------

void  Application::io_service_run() throw()
{
    log_.trace() << "Thread started";

    boost::system::error_code  ec;

    while (!io_service_.stopped()) {
        io_service_.run(ec);

        if (ec.value() != 0) {
            log_.error() << "io_service_.run error " << ec.value() << ": " << ec.message();
            ec.clear();
        }
    }

    log_.trace() << "Thread finished";
}

// =============================================================================

} // namespace service
} // namespace mesosphere

// =============================================================================
