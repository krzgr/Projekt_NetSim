#include <nodes.hpp>

void ReceiverPreferences::add_receiver(IPackageReceiver* r)
{
    preferences_.emplace(std::make_pair(r, 0));
    const double probability = 1.0 / preferences_.size();
    for(auto& x : preferences_)
        x.second = probability;
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r)
{
    if(preferences_.find(r) != preferences_.end())
    {
        preferences_.erase(r);
        const double probability = 1.0 / preferences_.size();
        for(auto& x : preferences_)
            x.second = probability;
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver()
{   
    const double random_val = pg_();
    double min_val = 0.0;

    for(const auto& x : preferences_)
    {
        min_val += x.second;
        if(random_val <= min_val)
            return x.first;
    }

    return nullptr; // control reaches end of non-void function [-Wreturn-type]
}

void PackageSender::send_package()
{
    if(bufor_.has_value())
    {
        IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
        receiver->receive_package(std::move(bufor_.value()));
        bufor_.reset();
    }
}


void Ramp::deliver_goods(Time t)
{
    if(t % di_ == 0)
        send_package();
    else if(get_sending_buffer().has_value() == false)
        push_package(Package());
}


void Worker::do_work(Time t)
{
    if(t >= package_processing_start_time + pd_)
        send_package();
    
    if(get_sending_buffer().has_value() == false && q_->size() != 0)
    {
        push_package(q_->pop());
        package_processing_start_time = t;
    }
}

void Worker::receive_package(Package&& p)
{
    q_->push(std::move(p));
}


void Storehouse::receive_package(Package&& p) 
{
    d_->push(std::move(p));
}