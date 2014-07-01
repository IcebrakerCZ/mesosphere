#ifndef MSESOSPHERE__CLUSTER__CLUSTER_JOB_HPP
#define MSESOSPHERE__CLUSTER__CLUSTER_JOB_HPP

// =============================================================================

#include <string>

// =============================================================================

namespace mesosphere {
namespace cluster    {

// =============================================================================

class Job
{
public:

    /* Every job must have its unique ID and requested units */
    Job(unsigned long long  id, unsigned  units);

    ~Job();


    unsigned            units() const;
    unsigned long long  id   () const;


private:

    unsigned            units_;
    unsigned long long  id_;
}; // class job

// =============================================================================

} // namespace cluster
} // namespace mesosphere

// =============================================================================

#endif // MSESOSPHERE__CLUSTER__CLUSTER_JOB_HPP
