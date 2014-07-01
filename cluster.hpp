#ifndef MSESOSPHERE__CLUSTER__CLUSTER_HPP
#define MSESOSPHERE__CLUSTER__CLUSTER_HPP

// =============================================================================

#include "application.hpp"

#include <string>

// =============================================================================

namespace mesosphere {
namespace cluster    {

// =============================================================================

class Cluster
    : public mesosphere::service::Module
{
public:

    Cluster();
    virtual ~Cluster();


public:

    virtual bool  configure(boost::program_options::options_description &  config);
    virtual bool  start();
    virtual void  stop();


private:

    void  work();

}; // class Cluster

// =============================================================================

} // namespace cluster
} // namespace mesosphere

// =============================================================================

#endif // MSESOSPHERE__CLUSTER__CLUSTER_HPP
