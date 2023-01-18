#include <factory.hpp>

class PackageSender;

bool Factory::is_consistent()
{
    std::map<const PackageSender*, NodeColor> colors;
    for(auto& x : ramps_)
        colors.emplace(&x, NodeColor::UNVISITED);
        
    for(auto& x : workers_)
        colors.emplace(&x, NodeColor::UNVISITED);
    
    for(const auto& x : ramps_)
    {
        try
        {
            has_reachable_storehouse(&x, colors);
        }
        catch(...)
        {
            return false;
        }
    }

    return true;
}

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors)
{
    if(node_colors[sender] == NodeColor::VERIFIED)
        return true;
    
    node_colors[sender] = NodeColor::VISITED;

    if(sender->receiver_preferences_.begin() == sender->receiver_preferences_.end())
        throw std::logic_error("Sender has no reachable receivers!");
    
    bool is_sender_has_at_least_one_receiver_diffrent_than_himself = false;

    for(auto& receiver : sender->receiver_preferences_)
    {
        if(receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE)
            is_sender_has_at_least_one_receiver_diffrent_than_himself = true;
        else if(receiver.first->get_receiver_type() == ReceiverType::WORKER)
        {
            IPackageReceiver* receiver_ptr = receiver.first;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if(sendrecv_ptr == sender)
                continue;
            
            is_sender_has_at_least_one_receiver_diffrent_than_himself = true;
            if(node_colors[sendrecv_ptr] == NodeColor::UNVISITED)
                has_reachable_storehouse(sendrecv_ptr, node_colors);
        }
    }

    node_colors[sender] = NodeColor::VERIFIED;

    if(is_sender_has_at_least_one_receiver_diffrent_than_himself)
        return true;
    else
        return false;
}