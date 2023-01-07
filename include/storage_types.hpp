#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include <package.hpp>
#include <list>

enum class PackageQueueType
{
    FIFO,
    LIFO
};

class IPackageStockpile {
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual void push(Package&& package) = 0;
    virtual bool empty() const = 0;
    virtual std::size_t size() const = 0;
    virtual ~IPackageStockpile() = default;

    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;
};

class IPackageQueue : public IPackageStockpile {
public:
    virtual PackageQueueType getPackageQueueType() const = 0;
    virtual Package pop() = 0;
};

class PackageQueue : public IPackageQueue {
public:
    PackageQueue(PackageQueueType type) : package_queue_type_(type) {}

    virtual void push(Package&& package) override;
    virtual bool empty() const override { return package_list_.empty(); }
    virtual std::size_t size() const override { return package_list_.size(); }
    virtual PackageQueueType getPackageQueueType() const override { return package_queue_type_; }
    virtual Package pop() override;

    virtual const_iterator begin() const override;
    virtual const_iterator end() const override;
    virtual const_iterator cbegin() const override;
    virtual const_iterator cend() const override;

private:
    PackageList package_list_;
    PackageQueueType package_queue_type_;
};

#endif //NETSIM_STORAGE_TYPES_HPP
