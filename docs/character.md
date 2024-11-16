# Character Properties

_[Unicode Library by Ross Smith](index.html)_

```c++
#include "rs-unicode/character.hpp"
namespace RS::Unicode;
```

## Contents

* TOC
{:toc}

## Constants

```c++
constexpr std::size_t npos = std::string::npos;
```

Defined for convenience.

```c++
constexpr char32_t min_surrogate     = 0xd800;
constexpr char32_t max_surrogate     = 0xdfff;
constexpr char32_t byte_order_mark   = 0xfeff;
constexpr char32_t replacement_char  = 0xfffd;
constexpr char32_t max_bmp           = 0xffff;
constexpr char32_t max_unicode       = 0x10'ffff;
constexpr char32_t not_unicode       = 0x11'0000;
```

Some useful code points for reference.

## Unicode enumeration properties

For all of these enumerations, the first listed name is the default value, and
has the integer value zero.

```c++
enum class East_Asian_Width: unsigned char {
    N,   // Neutral (default)
    A,   // Ambiguous
    F,   // Fullwidth
    H,   // Halfwidth
    Na,  // Narrow
    W,   // Wide
};

enum class General_Category: unsigned char {
    // Other (C*)
    Cn,  // Unassigned (default)
    Cc,  // Control
    Cf,  // Format
    Co,  // Private Use
    Cs,  // Surrogate
    // Letter (L*)
    LC,  // Cased Letter
    Ll,  // Lowercase letter
    Lm,  // Modifier letter
    Lo,  // Other letter
    Lt,  // Titlecase letter
    Lu,  // Uppercase letter
    // Mark (M*)
    Mc,  // Spacing mark
    Me,  // Enclosing mark
    Mn,  // Nonspacing mark
    // Number (N*)
    Nd,  // Decimal number
    Nl,  // Letter number
    No,  // Other number
    // Punctuation (P*)
    Pc,  // Connector punctuation
    Pd,  // Dash punctuation
    Pe,  // Close punctuation
    Pf,  // Final punctuation
    Pi,  // Initial punctuation
    Po,  // Other punctuation
    Ps,  // Open punctuation
    // Symbol (S*)
    Sc,  // Currency symbol
    Sk,  // Modifier symbol
    Sm,  // Math symbol
    So,  // Other symbol
    // Separator (Z*)
    Zl,  // Line separator
    Zp,  // Paragraph separator
    Zs,  // Space separator
};

enum class Grapheme_Cluster_Break: unsigned char {
    Other, // Default
    CR,
    Control,
    Extend,
    L,
    LF,
    LV,
    LVT,
    Prepend,
    Regional_Indicator,
    SpacingMark,
    T,
    V,
    ZWJ,
};

enum class Hangul_Syllable_Type: unsigned char {
    NA, // Default
    L,
    LV,
    LVT,
    T,
    V,
};

enum class Indic_Conjunct_Break: unsigned char {
    None, // Default
    Case_Ignorable,
    Consonant,
    Default_Ignorable_Code_Point,
    Extend,
    Grapheme_Base,
    Grapheme_Extend,
    Grapheme_Link,
    Linker,
};

enum class Line_Break: unsigned char {
    // Other characters
    XX,   // Unknown (default)
    AI,   // Ambiguous (alphabetic or ideographic)
    AK,   // Aksara
    AL,   // Alphabetic
    AP,   // Aksara pre-base
    AS,   // Aksara start
    CJ,   // Conditional Japanese starter
    EB,   // Emoji base
    EM,   // Emoji modifier
    H2,   // Hangul LV syllable
    H3,   // Hangul LVT syllable
    HL,   // Hebrew letter
    ID,   // Ideographic
    JL,   // Hangul L Jamo
    JV,   // Hangul V Jamo
    JT,   // Hangul T Jamo
    RI,   // Regional indicator
    SA,   // Complex context dependent (South East Asian)
    VF,   // Virama final
    VI,   // Virama
    // Non-tailorable line breaking classes
    BK,   // Mandatory break
    CR,   // Carriage return
    LF,   // Line feed
    CM,   // Combining mark
    NL,   // Next line
    SG,   // Surrogate
    WJ,   // Word joiner
    ZW,   // Zero width space
    GL,   // Non-breaking (glue)
    SP,   // Space
    ZWJ,  // Zero width joiner
    // Break opportunities
    B2,   // Break opportunity before and after
    BA,   // Break after
    BB,   // Break before
    HY,   // Hyphen
    CB,   // Contingent break opportunity
    // Characters prohibiting certain breaks
    CL,   // Close punctuation
    CP,   // Close parenthesis
    EX,   // Exclamation/interrogation
    IN,   // Inseparable
    NS,   // Nonstarter
    OP,   // Open punctuation
    QU,   // Quotation
    // Numeric context
    IS,   // Infix numeric separator
    NU,   // Numeric digits
    PO,   // Postfix numeric
    PR,   // Prefix numeric
    SY,   // Symbols allowing break after
};
```

## Character property functions

```c++
bool is_alphanumeric(char32_t c); // General_Category L* or N*
bool is_ascii(char32_t c); // Code points up to 0x7f
bool is_astral(char32_t c); // Code points from 0x1'0000 to 0x10'ffff
bool is_bmp(char32_t c); // Code points up to 0xffff, excluding surrogates
bool is_default_ignorable(char32_t c);
bool is_extended_pictographic(char32_t c);
bool is_full_composition_exclusion(char32_t c);
bool is_hangul(char32_t c);
bool is_noncharacter(char32_t c); // Surrogates and designated noncharacters
bool is_pattern_syntax(char32_t c);
bool is_pattern_white_space(char32_t c);
bool is_private_use(char32_t c);
bool is_surrogate(char32_t c);
bool is_unicode(char32_t c); // Up to 0x10'ffff, excluding surrogates
bool is_white_space(char32_t c);
bool is_xid_continue(char32_t c);
bool is_xid_nonstart(char32_t c);
bool is_xid_start(char32_t c);
```

Boolean valued properties.

```c++
char32_t simple_lowercase_mapping(char32_t c);
char32_t simple_titlecase_mapping(char32_t c);
char32_t simple_uppercase_mapping(char32_t c);
```

Character valued properties.

```c++
East_Asian_Width east_asian_width(char32_t c);
General_Category general_category(char32_t c);
Grapheme_Cluster_Break grapheme_cluster_break(char32_t c);
Hangul_Syllable_Type hangul_syllable_type(char32_t c);
Indic_Conjunct_Break indic_conjunct_break(char32_t c);
Line_Break line_break(char32_t c);
```

Enumeration valued properties.

```c++
int canonical_combining_class(char32_t c);
char primary_category(char32_t c); // From General_Category
```

Integer valued properties.

## Compound character property functions

```c++
char32_t canonical_composition_mapping(std::u32string_view str);
    // Canonical composition if one exists, otherwise 0
std::u32string_view canonical_decomposition_mapping(char32_t c);
std::u32string_view case_folding(char32_t c);
const std::array<std::u32string_view, 3>& special_casing(char32_t c);
```

Properties of, or returning, strings.
