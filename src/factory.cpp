#include "factory.hpp"

class PackageSender;

bool Factory::is_consistent() {

    std::map<PackageSender*, NodeColor> colors;
    for(const auto it=ramp_cbegin(); it!=ramp_cend(); it++) {
        try {
            colors[*it] = NodeColor::UNVISITED;
        } catch(std::logic_error) {
            return false;
        }
    }
    for(const auto it=worker_cbegin(); it!=worker_cend(); it++) {
        try {
            colors[*it] = NodeColor::UNVISITED;
        } catch(std::logic_error) {
            return false;
        }
    }
    //do dokonczenia

}

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) {

    if(node_colors[sender] == NodeColor::VERIFIED) {
        return true;
    }
    node_colors[sender] = NodeColor::VISITED;

    //do dokonczenia

}
