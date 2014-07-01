#ifndef MSESOSPHERE__CLUSTER__CLUSTER_NODE_HPP
#define MSESOSPHERE__CLUSTER__CLUSTER_NODE_HPP

// =============================================================================

#include "cluster_job.hpp"

#include <boost/shared_ptr.hpp>

#include <string>

// =============================================================================

namespace mesosphere {
namespace cluster    {

// =============================================================================

class Node
{
public:

    Node();
    ~Node();


    unsigned                units;
    unsigned                id;
    boost::shared_ptr<Job>  job;
}; // class node

// =============================================================================

} // namespace cluster
} // namespace mesosphere

// =============================================================================

#endif // MSESOSPHERE__CLUSTER__CLUSTER_NODE_HPP
