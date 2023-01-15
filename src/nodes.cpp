#include "nodes.hpp"


void Worker::receive_package(Package&& p) {

}

void Storehouse::receive_package(Package&& p) {

}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {

    auto iterat = std::find_if(receivers_.begin(), receivers_.end(), [r](const auto el) { return el == r; });
    if(iterat != receivers_.end()) {
        receivers_.erase(iterat);
    }
    // w innym wypadku rzucic wyjatek?
}
