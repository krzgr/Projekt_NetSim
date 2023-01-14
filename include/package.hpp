#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP

#include <algorithm>
#include <set>
#include <types.hpp>

class Package
{

private:
    ElementID id_;
    static std::set<ElementID> assigned_IDs_;
    static std::set<ElementID> free_IDs_;

public:
    Package();
    Package(ElementID id) : id_(id) { assigned_IDs_.insert(id_); }
    Package(Package&& package) = default;
    Package(Package& package) = delete;
    Package& operator=(Package&& other) noexcept;
    Package operator=(Package& other) = delete;
    ElementID get_id() const { return id_; }
    ~Package();

};

#endif //NETSIM_PACKAGE_HPP
