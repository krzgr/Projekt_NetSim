#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP

#include <iostream>
#include <algorithm>
#include <map>
#include "types.hpp"
#include "nodes.hpp"

enum class NodeColor
{
    UNVISITED,
    VISITED,
    VERIFIED
};

template <typename Node>

class NodeCollection
{
public:
    using NodeContainer = typename std::list<Node>;
    using iterator = typename NodeContainer::iterator;
    using const_iterator = typename NodeContainer::const_iterator;

private:
    NodeContainer collection_;

public:

    void add(Node&& node) { collection_.template emplace_back(std::move(node)); }
    void remove_by_id(ElementID id) {std::remove_if(collection_.begin(), collection_.end(),
                                                    [&id](const auto& item){ return item.get_id() == id; });}

    // nie wiem troche jak sobie z tym poradzic
    typename NodeContainer::iterator find_by_id(ElementID id) { return std::find_if(collection_.begin(), collection_.end(),
                                                                                    [&id](const auto& item){ return item.get_id() == id; }); }
    typename NodeContainer::const_iterator find_by_id(ElementID id) { return std::find_if(collection_.cbegin(), collection_.cend(),
                                                                                          [&id](const auto& item){ return item.get_id() == id;}); }

    iterator begin() { return collection_.begin(); }
    iterator end() { return collection_.end(); }
    const_iterator begin() const { return collection_.cbegin(); };
    const_iterator end() const { return collection_.cend(); }
    const_iterator cbegin() const { return collection_.cbegin(); }
    const_iterator cend() const { return collection_.cend(); }
};


class Factory
{

    static void remove_receiver(NodeCollection<Node>& collection, ElementID id) { collection.remove_by_id(id); }; // tez nie wiem czemu blad
    NodeCollection<Ramp> ramps_;
    NodeCollection<Worker> workers_;
    NodeCollection<Storehouse> storehouses_;

public:

    bool is_consistent();
    void do_deliveries(Time);
    void do_package();
    void do_work(Time);

    void add_ramp(Ramp&& ramp) { ramps_.add(ramp); } //czy ramps_.add(std::move(ramp))
    void remove_ramp(ElementID id);
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) { return ramps_.find_by_id(id); };
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) { return ramps_.find_by_id(id); };
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return ramps_.cbegin(); }
    NodeCollection<Ramp>::const_iterator ramp_cend() const { return ramps_.cend(); }

    void add_ramp(Worker&& worker) { workers_.add(worker); } //czy tutaj workers_.add(std::move(worker))
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id);
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id);
    NodeCollection<Worker>::const_iterator worker_cbegin() const { return workers_.cbegin(); }
    NodeCollection<Worker>::const_iterator worker_cend() const { return workers_.cend(); }

    void add_storehouse(Storehouse&& storehouse) { storehouses_.add(storehouse); }//czy storehouses_.add(std::move(storehouse))
    void remove_storehouse(ElementID id);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id);
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id);
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return storehouses_.cbegin(); }
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return storehouses_.cend(); }
};

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors);

#endif //NETSIM_FACTORY_HPP
