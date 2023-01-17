#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include <types.hpp>
#include <storage_types.hpp>

#include <iostream>
#include <list>
#include <algorithm>
#include <map>
#include <optional>
#include <memory>


enum class ReceiverType
{
    WORKER,
    STOREHOUSE     // to ma być wykorzystane potem
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

    ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : pg_(pg) {}
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();          // zabawa z prawdopodobieństwem
    const preferences_t& get_preferences() { return preferences_; };

private:
    ProbabilityGenerator pg_;
    preferences_t preferences_;
};


class PackageSender
{
public:
    ReceiverPreferences receiver_preferences_;
    PackageSender() = default;
    PackageSender(PackageSender&&) = default;
    PackageSender(Package&) = delete;
    void send_package();
    const std::optional<Package>& get_sending_buffer() const { return bufor_; }
protected:
    void push_package(Package&& p) { bufor_.emplace(std::move(p)); }
private:
    std::optional<Package> bufor_;
};


class Ramp : public PackageSender
{
    Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {}
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval_() const { return di_; }
    ElementID get_id() const { return id_; }
private:
    ElementID id_;
    TimeOffset di_;
};

class Worker : public PackageSender, public IPackageReceiver
{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : id_(id), pd_(pd), q_(std::move(q)), package_processing_start_time(0) {}

    virtual void receive_package(Package&& p) override;
    virtual ElementID get_id() const override { return id_; }

    void do_work(Time t);
    TimeOffset get_processing_duration() const { return pd_; }
    Time get_package_processing_start_time() const { return package_processing_start_time; }

    virtual IPackageStockpile::const_iterator begin() const override { return q_->cbegin(); }
    virtual IPackageStockpile::const_iterator end() const override { return q_->cend(); }
    virtual IPackageStockpile::const_iterator cbegin() const override { return q_->cbegin(); }
    virtual IPackageStockpile::const_iterator cend() const override { return q_->cend(); }

private:
    ElementID id_;
    TimeOffset pd_;
    std::unique_ptr<IPackageQueue> q_;

    Time package_processing_start_time;
};

class Storehouse : public IPackageReceiver
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

