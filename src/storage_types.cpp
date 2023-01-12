#include "storage_types.hpp"

void PackageQueue::push(Package&& package)
{
    package_list_.emplace_back(std::move(package));
}

Package PackageQueue::pop()
{
    switch(package_queue_type_) {
        case(PackageQueueType::FIFO): {
            Package p = std::move(package_list_.front());
            package_list_.pop_front();
            return p;
        }
        case(PackageQueueType::LIFO): {
            Package p = std::move(package_list_.back());
            package_list_.pop_back();
            return p;
        }
    }
    return Package(); // W celu wyciszenia błędu "error: control reaches end of non-void function"
}

PackageQueue::const_iterator PackageQueue::begin() const
{
    return package_list_.cbegin();
}

PackageQueue::const_iterator PackageQueue::end() const
{
    return package_list_.cend();
}

PackageQueue::const_iterator PackageQueue::cbegin() const
{
    return package_list_.cbegin();
}

PackageQueue::const_iterator PackageQueue::cend() const
{
    return package_list_.cend();
}