#include "cluster_new.hpp"

#include <boost/foreach.hpp>

#include <utility>

// =============================================================================

namespace mesosphere {
namespace cluster    {

// =============================================================================

ClusterNew::ClusterNew()
    : Cluster("cluster_new")
{
    log_.trace() << "Constructor";
}

// -----------------------------------------------------------------------------

ClusterNew::~ClusterNew()
{
    log_.trace() << "Destructor";
}

// -----------------------------------------------------------------------------

void  ClusterNew::add_node(unsigned  id,  unsigned  units)
{
    boost::mutex::scoped_lock  lock(mutex_);

    log_.debug() << "Adding node " << id << " with " << units << " units";

    Node &  node = nodes_[id];

    node.id = id;

    node.units += units;

    log_.debug() << "Have node " << node.id << " with " << node.units << " units";

    if (node.job != NULL) {
        log_.trace() << "Checking job " << node.job->id() << " with " << node.job->units() << " units"
                     << " on saved node " << node.id << " with " << node.units << " units";

        if (node.units >= node.job->units()) {
            log_.trace() << "Processing job " << node.job->id() << " with " << node.job->units() << " units"
                         << " on saved node " << node.id << " with " << node.units << " units";

            process_job(node, *node.job);
            node.units -= node.job->units();
            node.job.reset();
        }
    }

    changed();
}

// -----------------------------------------------------------------------------

void  ClusterNew::changed()
{
    if (jobs_.empty()) {
        print_nodes();
        print_jobs();
        log_.debug() << "changed: No more jobs in queue";
        return;
    }

    Job  &  job  = jobs_.front();
    Node *  node = NULL;

    /* Find suitable node */
    std::map<unsigned, Node>::iterator  i_nodes = nodes_.begin();
    for (; i_nodes != nodes_.end(); ++i_nodes) {
        /* Skip already saved nodes */
        if (i_nodes->second.job != NULL) {
            log_.trace() << "Skipping saved node " << i_nodes->second.id << " with job "
                         << i_nodes->second.job->id();
            continue;
        }

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

        i_nodes = nodes_.begin();
        for (; i_nodes != nodes_.end(); ++i_nodes) {
            /* Skip already saved node */
            if (i_nodes->second.job != NULL) {
                log_.trace() << "Skipping saved node " << i_nodes->second.id << " with job "
                             << i_nodes->second.job->id();
                continue;
            }

            node = &i_nodes->second;
            break;
        }

        /* Save a node for bigger job if we stll have spare node to use */
        if (node != NULL) {
            node->job.reset(new Job(job));
            jobs_.erase(jobs_.begin());

            log_.trace() << "Saved job " << node->job->id() << " with " << node->job->units() << " units"
                         << " on node " << node->id << " with " << node->units << " units";
        }
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
