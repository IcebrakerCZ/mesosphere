#ifndef MSESOSPHERE__CLUSTER__CLUSTER_NEW_HPP
#define MSESOSPHERE__CLUSTER__CLUSTER_NEW_HPP

// =============================================================================

#include "cluster.hpp"

// =============================================================================

namespace mesosphere {
namespace cluster    {

// =============================================================================

class ClusterNew
    : public Cluster
{
public:

    ClusterNew();
    virtual ~ClusterNew();


/* Note: made protected from private just to support unit testing */
protected:

    /* Main logic */
    virtual void  add_node(unsigned  id, unsigned  units);
    virtual void  changed ();

}; // class ClusterNew

// =============================================================================

} // namespace cluster
} // namespace mesosphere

// =============================================================================

#endif // MSESOSPHERE__CLUSTER__CLUSTER_NEW_HPP
