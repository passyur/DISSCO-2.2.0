#ifndef ENVELOPE_LIBRARY_PALETTE_EVENT_HPP
#define ENVELOPE_LIBRARY_PALETTE_EVENT_HPP

#include <QString>
#include "EnvelopeLibraryEntry.hpp"

// Minimal interface to be used as an IEvent* in the palette
class EnvelopeLibraryPaletteEvent /* : public IEvent */ {
public:
    EnvelopeLibraryPaletteEvent(EnvelopeLibraryEntry* entry)
        : entry(entry) {}

    EnvelopeLibraryEntry* getEntry() const { return entry; }

    // For display in the palette
    QString getName() const { return entry ? entry->getNumberString() : QString(); }
    int getNumber() const { return entry ? entry->number : -1; }

    // Add more methods as needed for editing
    // ...

    // Example: set name/number (if supported)
    void setNumber(int num) { if (entry) entry->number = num; }

private:
    EnvelopeLibraryEntry* entry;
};

#endif // ENVELOPE_LIBRARY_PALETTE_EVENT_HPP 