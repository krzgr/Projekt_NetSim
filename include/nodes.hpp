#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include <iostream>
#include <list>
#include "types.hpp"
#include "storage_types.hpp"
#include <algorithm>
#include <map>
#include <optional>
#include <memory>

enum class ReceiverType
{
    WORKER,
    STOREHOUSE
};

class IPackageReceiver
{
public:
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
};

class ReceiverPreferences
{
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    preferences_t preferences_;

    ReceiverPreferences(ProbabilityGenerator pg) : pg_(std::move(pg)) {}
    void add_receiver(IPackageReceiver* r) { receivers_.push_back(r); } //przechowujemy wskazniki czy po id?
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver() {};
    preferences_t& get_preferences() {};

private:
    ProbabilityGenerator pg_;
    std::list<IPackageReceiver*> receivers_;

};

class PackageSender
{
public:

    ReceiverPreferences receiver_preferences_;
    PackageSender(PackageSender&&) = default;
    PackageSender(Package&) = delete;
    void send_package() {};                                                                      //do napisania
    std::optional<Package>& get_sending_buffer() const { return (std::optional<Package>&) bufor_; }
protected:
    void push_package(Package&& p) { bufor_.emplace(std::move(p)); }
private:
    std::optional<Package> bufor_; // nie dziala

};

class Ramp : public PackageSender
{
    Ramp(PackageSender&& other, ElementID id, TimeOffset di) : PackageSender(std::move(other)), id_(id), di_(di) {}
    void deliver_goods(Time t) {};
    TimeOffset get_delivery_interval_() const {};
    ElementID get_id() const { return id_; }
private:
    ElementID id_;
    TimeOffset di_;

};

class Worker : public PackageSender, public IPackageQueue, public IPackageReceiver
{
public:
    Worker(PackageSender&& other, ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : PackageSender(std::move(other)), id_(id), pd_(pd), q_(std::move(q)) {}

    virtual void receive_package(Package&& p) override;
    virtual ElementID get_id() const override { return id_; }

    void do_work(Time t){};
    TimeOffset get_processing_duration() const {return pd_;};
    Time get_package_processing_start_time() const {};
    virtual IPackageStockpile::const_iterator begin() const override { return q_->cbegin(); }
    virtual IPackageStockpile::const_iterator end() const override { return q_->cend(); }
    virtual IPackageStockpile::const_iterator cbegin() const override { return q_->cbegin(); };
    virtual IPackageStockpile::const_iterator cend() const override { return q_->cend(); };


private:
    ElementID id_;
    TimeOffset pd_;
    std::unique_ptr<IPackageQueue> q_;
};

class Storehouse : public IPackageReceiver, public IPackageStockpile
{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d) : id_(id), d_(std::move(d)) {};

    virtual void receive_package(Package&& p) override;
    virtual ElementID get_id() const override { return id_; }
    virtual IPackageStockpile::const_iterator begin() const override { return d_->cbegin(); }
    virtual IPackageStockpile::const_iterator end() const override { return d_->cend(); }
    virtual IPackageStockpile::const_iterator cbegin() const override { return d_->cbegin(); };
    virtual IPackageStockpile::const_iterator cend() const override { return d_->cend(); };
private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};
#endif

