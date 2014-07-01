#include "application.hpp"
#include "cluster.hpp"


int main(int  argc, char *  argv[])
{
    using mesosphere::service::application;
    using mesosphere::cluster::Cluster;

    application.init(argc, argv);

    return application.add_module(std::unique_ptr<Cluster>(new Cluster()))
                      .main();
}
