#ifndef MSESOSPHERE__CLUSTER__CLUSTER_HPP
#define MSESOSPHERE__CLUSTER__CLUSTER_HPP

// =============================================================================

#include "cluster_node.hpp"
#include "cluster_job.hpp"

#include "application.hpp"

#include <boost/thread.hpp>

#include <string>
#include <vector>
#include <map>

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


/* Note: made protected from private just to support unit testing */
protected:

    void  add_node(unsigned  id, unsigned  units);
    void  add_job (unsigned  id, unsigned  units);

    /* Main logic */
    void  changed();

    /* Send job to node */
    void  process_job(Node &  node, Job &  job);

    void  print_nodes() const;
    void  print_jobs () const;
    void  print_stats() const;


private:

    /* Function just for unit testing cluster logic inside the framework */
    virtual void  work();


private:

    boost::mutex              mutex_;
    std::map<unsigned, Node>  nodes_;
    std::vector<Job>          jobs_;

    std::map<unsigned, unsigned long long>  units_stats_;
}; // class Cluster

// =============================================================================

} // namespace cluster
} // namespace mesosphere

// =============================================================================

#endif // MSESOSPHERE__CLUSTER__CLUSTER_HPP
