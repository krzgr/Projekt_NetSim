#include <package.hpp>

std::set<ElementID> Package::assigned_IDs_ = {};
std::set<ElementID> Package::free_IDs_ = {};

Package::Package()
{
    if(assigned_IDs_.empty())
        free_IDs_.insert(1);
    
    if(!free_IDs_.empty())
    {
        id_ = *free_IDs_.begin();
        free_IDs_.erase(free_IDs_.begin());
        assigned_IDs_.insert(id_);
    }
    else
    {
        id_ = *std::max_element(assigned_IDs_.begin(), assigned_IDs_.end()) + 1;
        assigned_IDs_.insert(id_);
    }
}

Package& Package::operator=(Package&& other) noexcept
{
    // Też nie jestem pewien, czy tego potrzebujemy
    // Lepiej skorzystać z konstruktora przenoszącego Package(Package&& package)
    assigned_IDs_.erase(id_);
    free_IDs_.insert(id_);
    id_ = other.id_;
    return *this;
}

Package::~Package()
{
    assigned_IDs_.erase(id_);
    free_IDs_.insert(id_);
}