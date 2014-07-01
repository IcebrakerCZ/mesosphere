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

    /* Every node must have its unique ID and available job units */
    Node(unsigned  id, unsigned  units);

    ~Node();


    unsigned  id() const;


public:

    unsigned  units;


private:

    unsigned  id_;
}; // class node


// =============================================================================

} // namespace cluster
} // namespace mesosphere

// =============================================================================

#endif // MSESOSPHERE__CLUSTER__CLUSTER_NODE_HPP
