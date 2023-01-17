#ifndef NETSIM_TYPES_HPP
#define NETSIM_TYPES_HPP

#include <list>
#include <helpers.hpp>

class Package;

using ElementID = int;
using PackageList = std::list<Package>;
using Time = int;
using TimeOffset = int;

#endif //NETSIM_TYPES_HPP
