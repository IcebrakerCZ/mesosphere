#include "application.hpp"
#include "cluster.hpp"
#include "cluster_new.hpp"

// =============================================================================

template <typename CLUSTER>
class ClusterTest1
    : public CLUSTER
{
protected:

void  work()
{
    this->add_job(1, 4);
    this->add_job(5, 2);

    this->add_node(2, 3);
    this->add_node(7, 3);

    this->add_node(2, 2);
}

}; // template class ClusterTest1

// =============================================================================

int main(int  argc, char *  argv[])
{
    using mesosphere::service::application;

    application.init(argc, argv);

    using mesosphere::cluster::Cluster;
    using mesosphere::cluster::ClusterNew;

    return application/*.add_module(std::unique_ptr<ClusterTest1<Cluster> >   (new ClusterTest1<Cluster>()))*/
                      .add_module(std::unique_ptr<ClusterTest1<ClusterNew> >(new ClusterTest1<ClusterNew>()))
                      .main();
}
