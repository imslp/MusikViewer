//Copyright (C) 2018 Michail Montesatnos <musikviewer@imslp.org>
#ifndef IMPORTMIDI_TUPLET_FILTER_H
#define IMPORTMIDI_TUPLET_FILTER_H


namespace Ms {

class ReducedFraction;

namespace MidiTuplet {

struct TupletInfo;

void filterTuplets(std::vector<TupletInfo> &tuplets,
                   const ReducedFraction &basicQuant);

} // namespace MidiTuplet
} // namespace Ms


#endif // IMPORTMIDI_TUPLET_FILTER_H
