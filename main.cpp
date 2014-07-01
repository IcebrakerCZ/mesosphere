#include "application.hpp"
#include "cluster.hpp"

// =============================================================================

class ClusterTest1
    : public mesosphere::cluster::Cluster
{
private:

void  work()
{
    add_node(2, 3);
    add_node(5, 42);
    add_node(7, 42);
    add_node(8, 42);

    add_job(3, 4);
    add_job(1, 4);
    add_job(4, 7);
    add_job(1, 3);
    add_job(5, 4);
    add_job(9, 3);
}

}; // class ClusterTest1

// =============================================================================

int main(int  argc, char *  argv[])
{
    using mesosphere::service::application;

    application.init(argc, argv);

    return application.add_module(std::unique_ptr<ClusterTest1>(new ClusterTest1()))
                      .main();
}
