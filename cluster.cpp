#include "cluster.hpp"

#include <boost/foreach.hpp>

#include <utility>

// =============================================================================

namespace mesosphere {
namespace cluster    {

// =============================================================================

Cluster::Cluster(std::string const &  name)
    : mesosphere::service::Module(name)
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

void  Cluster::add_node(unsigned  id,  unsigned  units)
{
    boost::mutex::scoped_lock  lock(mutex_);

    log_.debug() << "Adding node " << id << " with " << units << " units";

    Node &  node = nodes_[id];

    node.id = id;

    node.units += units;

    changed();
}

// -----------------------------------------------------------------------------

void  Cluster::add_job(unsigned  id,  unsigned  units)
{
    boost::mutex::scoped_lock  lock(mutex_);

    log_.debug() << "Adding job " << id << " with " << units << " units";

    jobs_.push_back(Job(id, units));

    ++(units_stats_.insert(std::make_pair(units, static_cast<unsigned long long>(0))).first->second);

    changed();
    print_stats();
}

// -----------------------------------------------------------------------------

void  Cluster::work()
{
}

// -----------------------------------------------------------------------------

void  Cluster::process_job(Node &  node, Job &  job)
{
    log_.info() << "Sending job " << job.id() << " to node " << node.id;
}

// -----------------------------------------------------------------------------

void  Cluster::print_nodes() const
{
    BOOST_FOREACH(auto  node_pair, nodes_) {
        log_.debug() << "node " << node_pair.second.id
                     << ": " << node_pair.second.units << " units";
    }
}

// -----------------------------------------------------------------------------

void  Cluster::print_jobs() const
{
    BOOST_FOREACH(auto  job, jobs_) {
        log_.debug() << "job  " << job.id()
                     << ": " << job.units() << " units";
    }
}

// -----------------------------------------------------------------------------

void  Cluster::print_stats() const
{
    BOOST_FOREACH(auto  unit_pair, units_stats_) {
        log_.debug() << "unit " << unit_pair.first
                     << ": " << unit_pair.second << " hits";
    }
}

// -----------------------------------------------------------------------------

void  Cluster::changed()
{
    if (jobs_.empty()) {
        log_.debug() << "changed: No more jobs in queue";
        return;
    }

    Job  &  job  = jobs_.front();
    Node *  node = NULL;

    /* Find suitable node */
    std::map<unsigned, Node>::iterator  i_nodes = nodes_.begin();
    for (; i_nodes != nodes_.end(); ++i_nodes) {
        /* Check if there is enough available units at the node*/
        if (i_nodes->second.units >= job.units()) {
            /* But use node with availale units closest to job units */
            if (   node == NULL
                || node->units > i_nodes->second.units
               ) {
                node = &(i_nodes->second);

                /* Stop searching if we found perfect match */
                if (node->units == job.units()) {
                    break;
                }
            }
        }
    }

    if (node == NULL) {
        log_.info() << "No suitable node found for job " << job.id();
    } else {
        /* Send job to node and update the node available units
         * and remove the job from pending jobs list
         */
        process_job(*node, job);

        jobs_.erase(jobs_.begin());
        node->units -= job.units();
    }

    print_nodes();
    print_jobs();
}

// =============================================================================

} // namespace cluster
} // namespace mesosphere

// =============================================================================
