#ifndef LAYERREFERENCEUTILS_HPP
#define LAYERREFERENCEUTILS_HPP

// Helpers for finding and mutating Package references (layerbox children) that
// point at a particular event by type+name. Used when an event is renamed or
// deleted so that stale references elsewhere in the project can be
// reconciled.

#include <QList>
#include <QPair>
#include <QString>
#include <algorithm>
#include <functional>

#include "event_struct.hpp"
#include "project_struct.hpp"
#include "../inst.hpp"

namespace LayerRefs {

// Associative list entry: key = owning (event type, owner index, layer index);
// value = package indices within that layer whose Package matches.
struct OwnerKey {
    Eventtype ownerType;
    int       ownerIndex;  // ignored when ownerType == top
    int       layerIndex;
};
using Assoc = QList<QPair<OwnerKey, QList<int>>>;

inline Assoc collect(const QString& typeNumStr, const QString& name) {
    Assoc refs;
    ProjectManager* pm = Inst::get_project_manager();

    auto scan = [&](HEvent& h, Eventtype ownerType, int ownerIndex) {
        for (int li = 0; li < h.event_layers.size(); ++li) {
            const QList<Package>& pkgs = h.event_layers[li].discrete_packages;
            QList<int> hits;
            for (int pi = 0; pi < pkgs.size(); ++pi) {
                if (pkgs[pi].event_type == typeNumStr &&
                    pkgs[pi].event_name == name) {
                    hits.append(pi);
                }
            }
            if (!hits.isEmpty())
                refs.append(qMakePair(OwnerKey{ownerType, ownerIndex, li}, hits));
        }
    };

    scan(pm->topevent(), top, -1);
    for (int i = 0; i < pm->highevents().size(); ++i)
        scan(pm->highevents()[i], high, i);
    for (int i = 0; i < pm->midevents().size(); ++i)
        scan(pm->midevents()[i], mid, i);
    for (int i = 0; i < pm->lowevents().size(); ++i)
        scan(pm->lowevents()[i], low, i);
    for (int i = 0; i < pm->bottomevents().size(); ++i)
        scan(pm->bottomevents()[i].event, bottom, i);

    return refs;
}

inline HEvent* resolveOwner(const OwnerKey& k) {
    ProjectManager* pm = Inst::get_project_manager();
    switch (k.ownerType) {
        case top:    return &pm->topevent();
        case high:   return (k.ownerIndex < pm->highevents().size())   ? &pm->highevents()[k.ownerIndex]         : nullptr;
        case mid:    return (k.ownerIndex < pm->midevents().size())    ? &pm->midevents()[k.ownerIndex]          : nullptr;
        case low:    return (k.ownerIndex < pm->lowevents().size())    ? &pm->lowevents()[k.ownerIndex]          : nullptr;
        case bottom: return (k.ownerIndex < pm->bottomevents().size()) ? &pm->bottomevents()[k.ownerIndex].event : nullptr;
        default:     return nullptr;
    }
}

inline void applyRename(const Assoc& refs, const QString& newName) {
    for (const auto& entry : refs) {
        HEvent* h = resolveOwner(entry.first);
        if (!h || entry.first.layerIndex >= h->event_layers.size()) continue;
        QList<Package>& pkgs = h->event_layers[entry.first.layerIndex].discrete_packages;
        for (int pi : entry.second)
            if (pi >= 0 && pi < pkgs.size()) pkgs[pi].event_name = newName;
    }
}

inline void applyDelete(const Assoc& refs) {
    for (const auto& entry : refs) {
        HEvent* h = resolveOwner(entry.first);
        if (!h || entry.first.layerIndex >= h->event_layers.size()) continue;
        QList<Package>& pkgs = h->event_layers[entry.first.layerIndex].discrete_packages;
        QList<int> desc = entry.second;
        std::sort(desc.begin(), desc.end(), std::greater<int>());
        for (int pi : desc)
            if (pi >= 0 && pi < pkgs.size()) pkgs.removeAt(pi);
    }
}

inline int totalCount(const Assoc& refs) {
    int n = 0;
    for (const auto& entry : refs) n += entry.second.size();
    return n;
}

} // namespace LayerRefs

#endif
