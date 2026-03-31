#include "map.h"

template <typename T, typename H>
class OpenHashTable : public Map<T, H> {
    using Map<T, H>::Pair;

};