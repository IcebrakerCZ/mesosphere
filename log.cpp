#include "log.hpp"

// =============================================================================

namespace mesosphere {
namespace log        {

// =============================================================================

std::string  StreamBase::program_name_;

// -----------------------------------------------------------------------------

std::ostream &  operator<<(std::ostream &  output, Level::type const &  level)
{
    static char const *  names[] = { "TRACE"  , "DEBUG", "INFO"
                                   , "WARNING", "ERROR", "CRITICAL"
                                   , "FATAL" };

    output << names[level];

    return output;
}

// =============================================================================

Module::Module( std::string const &  facility_name
              , std::string const &  facility_ident)
    : facility_name_(facility_name)
    , facility_ident_(facility_ident)
{}

// -----------------------------------------------------------------------------

Trace  Module::trace(std::string const &  session_ident)
{
    return Trace(facility_name_, facility_ident_, session_ident);
}

// -----------------------------------------------------------------------------

Debug  Module::debug(std::string const &  session_ident)
{
    return Debug(facility_name_, facility_ident_, session_ident);
}

// -----------------------------------------------------------------------------

Info  Module::info(std::string const &  session_ident)
{
    return Info(facility_name_, facility_ident_, session_ident);
}

// -----------------------------------------------------------------------------

Warning  Module::warning(std::string const &  session_ident)
{
    return Warning(facility_name_, facility_ident_, session_ident);
}

// -----------------------------------------------------------------------------

Error  Module::error(std::string const &  session_ident)
{
    return Error(facility_name_, facility_ident_, session_ident);
}

// -----------------------------------------------------------------------------

Critical  Module::critical(std::string const &  session_ident)
{
    return Critical(facility_name_, facility_ident_, session_ident);
}

// -----------------------------------------------------------------------------

Fatal  Module::fatal(std::string const &  session_ident)
{
    return Fatal(facility_name_, facility_ident_, session_ident);
}

// =============================================================================

} // namespace log
} // namespace mesosphere

// =============================================================================
