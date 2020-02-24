#ifndef Array_renderers_html_h
#define Array_renderers_html_h


template <typename ArrayType>
class HTMLArrayRenderer {
public:
  struct BitTranslation {
    unsigned origin;
    unsigned destination;

    //  Equal
    //  Not Equal
    //  Less
    //  Less or Equal
    //  Greater
    //  Greater or Equal

    bool origin_eq(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.origin == rhs.origin; }
    bool origin_ne(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.origin != rhs.origin; }
    bool origin_lt(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.origin <  rhs.origin; }
    bool origin_le(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.origin <= rhs.origin; }
    bool origin_gt(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.origin >  rhs.origin; }
    bool origin_ge(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.origin >= rhs.origin; }

    bool destination_eq(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.destination == rhs.destination; }
    bool destination_ne(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.destination != rhs.destination; }
    bool destination_lt(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.destination <  rhs.destination; }
    bool destination_le(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.destination <= rhs.destination; }
    bool destination_gt(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.destination >  rhs.destination; }
    bool destination_ge(const BitTranslation& lhs, const BitTranslation& rhs) { return lhs.destination >= rhs.destination; }

  };


private:

};


#endif
