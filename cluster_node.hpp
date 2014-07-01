#ifndef MSESOSPHERE__CLUSTER__CLUSTER_NODE_HPP
#define MSESOSPHERE__CLUSTER__CLUSTER_NODE_HPP

// =============================================================================

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


    unsigned  units;
    unsigned  id;
}; // class node

// =============================================================================

} // namespace cluster
} // namespace mesosphere

// =============================================================================

#endif // MSESOSPHERE__CLUSTER__CLUSTER_NODE_HPP
