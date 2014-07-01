#include "cluster.hpp"

// =============================================================================

namespace mesosphere {
namespace cluster    {

// =============================================================================

Cluster::Cluster()
    : mesosphere::service::Module("cluster")
{
    log_.trace() << "Constructor";
}

// -----------------------------------------------------------------------------

Cluster::~Cluster()
{
    log_.trace() << "Destructor";
}

// -----------------------------------------------------------------------------

bool  Cluster::configure(boost::program_options::options_description &  config)
{
    log_.trace() << "configure";
    return true;
}

// -----------------------------------------------------------------------------

bool  Cluster::start()
{
    log_.trace() << "start";

    mesosphere::service::application.io_service().post(boost::bind(&Cluster::work, this));

    return true;
}

// -----------------------------------------------------------------------------

void  Cluster::stop()
{
    log_.trace() << "stop";
}

// -----------------------------------------------------------------------------

void  Cluster::work()
{
    log_.info() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
}

// =============================================================================

} // namespace cluster
} // namespace mesosphere

// =============================================================================
