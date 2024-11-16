#include "rs-unicode/character.hpp"
#include "rs-unicode/encoding.hpp"
#include "test/unit-test.hpp"
#include <cstdint>

using namespace RS::Unicode;

void test_rs_unicode_character_properties_general_category() {

    using GC = General_Category;

    TEST_EQUAL(int(general_category(0)),          int(GC::Cc));  TEST_EQUAL(primary_category(0),          'C');  //
    TEST_EQUAL(int(general_category(U'\n')),      int(GC::Cc));  TEST_EQUAL(primary_category(U'\n'),      'C');  //
    TEST_EQUAL(int(general_category(U' ')),       int(GC::Zs));  TEST_EQUAL(primary_category(U' '),       'Z');  //
    TEST_EQUAL(int(general_category(U'!')),       int(GC::Po));  TEST_EQUAL(primary_category(U'!'),       'P');  //
    TEST_EQUAL(int(general_category(U'$')),       int(GC::Sc));  TEST_EQUAL(primary_category(U'$'),       'S');  //
    TEST_EQUAL(int(general_category(U'(')),       int(GC::Ps));  TEST_EQUAL(primary_category(U'('),       'P');  //
    TEST_EQUAL(int(general_category(U')')),       int(GC::Pe));  TEST_EQUAL(primary_category(U')'),       'P');  //
    TEST_EQUAL(int(general_category(U'+')),       int(GC::Sm));  TEST_EQUAL(primary_category(U'+'),       'S');  //
    TEST_EQUAL(int(general_category(U'-')),       int(GC::Pd));  TEST_EQUAL(primary_category(U'-'),       'P');  //
    TEST_EQUAL(int(general_category(U'/')),       int(GC::Po));  TEST_EQUAL(primary_category(U'/'),       'P');  //
    TEST_EQUAL(int(general_category(U'0')),       int(GC::Nd));  TEST_EQUAL(primary_category(U'0'),       'N');  //
    TEST_EQUAL(int(general_category(U'9')),       int(GC::Nd));  TEST_EQUAL(primary_category(U'9'),       'N');  //
    TEST_EQUAL(int(general_category(U'A')),       int(GC::Lu));  TEST_EQUAL(primary_category(U'A'),       'L');  //
    TEST_EQUAL(int(general_category(U'Z')),       int(GC::Lu));  TEST_EQUAL(primary_category(U'Z'),       'L');  //
    TEST_EQUAL(int(general_category(U'^')),       int(GC::Sk));  TEST_EQUAL(primary_category(U'^'),       'S');  //
    TEST_EQUAL(int(general_category(U'_')),       int(GC::Pc));  TEST_EQUAL(primary_category(U'_'),       'P');  //
    TEST_EQUAL(int(general_category(U'a')),       int(GC::Ll));  TEST_EQUAL(primary_category(U'a'),       'L');  //
    TEST_EQUAL(int(general_category(U'z')),       int(GC::Ll));  TEST_EQUAL(primary_category(U'z'),       'L');  //
    TEST_EQUAL(int(general_category(0xa6)),       int(GC::So));  TEST_EQUAL(primary_category(0xa6),       'S');  // broken bar
    TEST_EQUAL(int(general_category(0xaa)),       int(GC::Lo));  TEST_EQUAL(primary_category(0xaa),       'L');  // feminine ordinal indicator
    TEST_EQUAL(int(general_category(0xab)),       int(GC::Pi));  TEST_EQUAL(primary_category(0xab),       'P');  // left-pointing double angle quotation mark
    TEST_EQUAL(int(general_category(0xad)),       int(GC::Cf));  TEST_EQUAL(primary_category(0xad),       'C');  // soft hyphen
    TEST_EQUAL(int(general_category(0xb2)),       int(GC::No));  TEST_EQUAL(primary_category(0xb2),       'N');  // superscript two
    TEST_EQUAL(int(general_category(0xbb)),       int(GC::Pf));  TEST_EQUAL(primary_category(0xbb),       'P');  // right-pointing double angle quotation mark
    TEST_EQUAL(int(general_category(0xf7)),       int(GC::Sm));  TEST_EQUAL(primary_category(0xf7),       'S');  // division sign
    TEST_EQUAL(int(general_category(0x1c5)),      int(GC::Lt));  TEST_EQUAL(primary_category(0x1c5),      'L');  // latin capital letter d with small letter z with caron
    TEST_EQUAL(int(general_category(0x2b0)),      int(GC::Lm));  TEST_EQUAL(primary_category(0x2b0),      'L');  // modifier letter small h
    TEST_EQUAL(int(general_category(0x2b9)),      int(GC::Lm));  TEST_EQUAL(primary_category(0x2b9),      'L');  // modifier letter prime
    TEST_EQUAL(int(general_category(0x2c5)),      int(GC::Sk));  TEST_EQUAL(primary_category(0x2c5),      'S');  // modifier letter down arrowhead
    TEST_EQUAL(int(general_category(0x300)),      int(GC::Mn));  TEST_EQUAL(primary_category(0x300),      'M');  // combining grave accent
    TEST_EQUAL(int(general_category(0x309)),      int(GC::Mn));  TEST_EQUAL(primary_category(0x309),      'M');  // combining hook above
    TEST_EQUAL(int(general_category(0x378)),      int(GC::Cn));  TEST_EQUAL(primary_category(0x378),      'C');  // unassigned
    TEST_EQUAL(int(general_category(0x488)),      int(GC::Me));  TEST_EQUAL(primary_category(0x488),      'M');  // combining cyrillic hundred thousands sign
    TEST_EQUAL(int(general_category(0x5d1)),      int(GC::Lo));  TEST_EQUAL(primary_category(0x5d1),      'L');  // hebrew letter bet
    TEST_EQUAL(int(general_category(0x6fd)),      int(GC::So));  TEST_EQUAL(primary_category(0x6fd),      'S');  // arabic sign sindhi ampersand
    TEST_EQUAL(int(general_category(0x903)),      int(GC::Mc));  TEST_EQUAL(primary_category(0x903),      'M');  // devanagari sign visarga
    TEST_EQUAL(int(general_category(0x94e)),      int(GC::Mc));  TEST_EQUAL(primary_category(0x94e),      'M');  // devanagari vowel sign prishthamatra e
    TEST_EQUAL(int(general_category(0x9f7)),      int(GC::No));  TEST_EQUAL(primary_category(0x9f7),      'N');  // bengali currency numerator four
    TEST_EQUAL(int(general_category(0x9fb)),      int(GC::Sc));  TEST_EQUAL(primary_category(0x9fb),      'S');  // bengali ganda mark
    TEST_EQUAL(int(general_category(0x16ee)),     int(GC::Nl));  TEST_EQUAL(primary_category(0x16ee),     'N');  // runic arlaug symbol
    TEST_EQUAL(int(general_category(0x180e)),     int(GC::Cf));  TEST_EQUAL(primary_category(0x180e),     'C');  // mongolian vowel separator
    TEST_EQUAL(int(general_category(0x1f8d)),     int(GC::Lt));  TEST_EQUAL(primary_category(0x1f8d),     'L');  // greek capital letter alpha with dasia and oxia and prosgegrammeni
    TEST_EQUAL(int(general_category(0x2006)),     int(GC::Zs));  TEST_EQUAL(primary_category(0x2006),     'Z');  // six-per-em space
    TEST_EQUAL(int(general_category(0x2014)),     int(GC::Pd));  TEST_EQUAL(primary_category(0x2014),     'P');  // em dash
    TEST_EQUAL(int(general_category(0x2028)),     int(GC::Zl));  TEST_EQUAL(primary_category(0x2028),     'Z');  // line separator
    TEST_EQUAL(int(general_category(0x2029)),     int(GC::Zp));  TEST_EQUAL(primary_category(0x2029),     'Z');  // paragraph separator
    TEST_EQUAL(int(general_category(0x207d)),     int(GC::Ps));  TEST_EQUAL(primary_category(0x207d),     'P');  // superscript left parenthesis
    TEST_EQUAL(int(general_category(0x2166)),     int(GC::Nl));  TEST_EQUAL(primary_category(0x2166),     'N');  // roman numeral seven
    TEST_EQUAL(int(general_category(0x2309)),     int(GC::Pe));  TEST_EQUAL(primary_category(0x2309),     'P');  // right ceiling
    TEST_EQUAL(int(general_category(0x2e0c)),     int(GC::Pi));  TEST_EQUAL(primary_category(0x2e0c),     'P');  // left raised omission bracket
    TEST_EQUAL(int(general_category(0x2e21)),     int(GC::Pf));  TEST_EQUAL(primary_category(0x2e21),     'P');  // right vertical bar with quill
    TEST_EQUAL(int(general_category(0xa670)),     int(GC::Me));  TEST_EQUAL(primary_category(0xa670),     'M');  // combining cyrillic ten millions sign
    TEST_EQUAL(int(general_category(0xd800)),     int(GC::Cs));  TEST_EQUAL(primary_category(0xd800),     'C');  // non private use high surrogate
    TEST_EQUAL(int(general_category(0xdfff)),     int(GC::Cs));  TEST_EQUAL(primary_category(0xdfff),     'C');  // low surrogate
    TEST_EQUAL(int(general_category(0xe000)),     int(GC::Co));  TEST_EQUAL(primary_category(0xe000),     'C');  // private use
    TEST_EQUAL(int(general_category(0xff3f)),     int(GC::Pc));  TEST_EQUAL(primary_category(0xff3f),     'P');  // fullwidth low line
    TEST_EQUAL(int(general_category(0x10'fffd)),  int(GC::Co));  TEST_EQUAL(primary_category(0x10'fffd),  'C');  // plane 16 private use
    TEST_EQUAL(int(general_category(0x11'0000)),  int(GC::Cn));  TEST_EQUAL(primary_category(0x11'0000),  'C');  // not unicode

}

void test_rs_unicode_character_properties_caonical_combining_class() {

    TEST_EQUAL(canonical_combining_class(0),            0);    //
    TEST_EQUAL(canonical_combining_class('A'),          0);    //
    TEST_EQUAL(canonical_combining_class(0x334),        1);    // combining tilde overlay
    TEST_EQUAL(canonical_combining_class(0x94d),        9);    // devanagari sign virama
    TEST_EQUAL(canonical_combining_class(0x316),        220);  // combining grave accent below
    TEST_EQUAL(canonical_combining_class(0x300),        230);  // combining grave accent
    TEST_EQUAL(canonical_combining_class(max_unicode),  0);    //

}

void test_rs_unicode_character_properties_canonical_decomposition_mapping() {

    TEST_EQUAL(utf32_to_utf8(canonical_decomposition_mapping(' ')),     "");              //
    TEST_EQUAL(utf32_to_utf8(canonical_decomposition_mapping('0')),     "");              //
    TEST_EQUAL(utf32_to_utf8(canonical_decomposition_mapping('9')),     "");              //
    TEST_EQUAL(utf32_to_utf8(canonical_decomposition_mapping('A')),     "");              //
    TEST_EQUAL(utf32_to_utf8(canonical_decomposition_mapping('Z')),     "");              //
    TEST_EQUAL(utf32_to_utf8(canonical_decomposition_mapping('a')),     "");              //
    TEST_EQUAL(utf32_to_utf8(canonical_decomposition_mapping('z')),     "");              //
    TEST_EQUAL(utf32_to_utf8(canonical_decomposition_mapping(0x00c0)),  "\u0041\u0300");  // latin capital letter a with grave
    TEST_EQUAL(utf32_to_utf8(canonical_decomposition_mapping(0x00c9)),  "\u0045\u0301");  // latin capital letter e with acute
    TEST_EQUAL(utf32_to_utf8(canonical_decomposition_mapping(0x00ce)),  "\u0049\u0302");  // latin capital letter i with circumflex
    TEST_EQUAL(utf32_to_utf8(canonical_decomposition_mapping(0x00d5)),  "\u004f\u0303");  // latin capital letter o with tilde
    TEST_EQUAL(utf32_to_utf8(canonical_decomposition_mapping(0x00dc)),  "\u0055\u0308");  // latin capital letter u with diaeresis
    TEST_EQUAL(utf32_to_utf8(canonical_decomposition_mapping(0x00e0)),  "\u0061\u0300");  // latin small letter a with grave
    TEST_EQUAL(utf32_to_utf8(canonical_decomposition_mapping(0x00e9)),  "\u0065\u0301");  // latin small letter e with acute
    TEST_EQUAL(utf32_to_utf8(canonical_decomposition_mapping(0x00ee)),  "\u0069\u0302");  // latin small letter i with circumflex
    TEST_EQUAL(utf32_to_utf8(canonical_decomposition_mapping(0x00f5)),  "\u006f\u0303");  // latin small letter o with tilde
    TEST_EQUAL(utf32_to_utf8(canonical_decomposition_mapping(0x00fc)),  "\u0075\u0308");  // latin small letter u with diaeresis

}

void test_rs_unicode_character_properties_east_asian_width() {

    TEST_EQUAL(int(east_asian_width(0)),       int(East_Asian_Width::N));
    TEST_EQUAL(int(east_asian_width(U' ')),    int(East_Asian_Width::Na));
    TEST_EQUAL(int(east_asian_width(U'A')),    int(East_Asian_Width::Na));
    TEST_EQUAL(int(east_asian_width(0xa1)),    int(East_Asian_Width::A));
    TEST_EQUAL(int(east_asian_width(0x1100)),  int(East_Asian_Width::W));
    TEST_EQUAL(int(east_asian_width(0x20a9)),  int(East_Asian_Width::H));
    TEST_EQUAL(int(east_asian_width(0x3000)),  int(East_Asian_Width::F));

}

void test_rs_unicode_character_properties_grapheme_cluster_break() {

    TEST_EQUAL(int(grapheme_cluster_break(0)),         int(Grapheme_Cluster_Break::Control));
    TEST_EQUAL(int(grapheme_cluster_break(0x000a)),    int(Grapheme_Cluster_Break::LF));
    TEST_EQUAL(int(grapheme_cluster_break(0x000d)),    int(Grapheme_Cluster_Break::CR));
    TEST_EQUAL(int(grapheme_cluster_break(0x0041)),    int(Grapheme_Cluster_Break::Other));
    TEST_EQUAL(int(grapheme_cluster_break(0x0300)),    int(Grapheme_Cluster_Break::Extend));
    TEST_EQUAL(int(grapheme_cluster_break(0x0600)),    int(Grapheme_Cluster_Break::Prepend));
    TEST_EQUAL(int(grapheme_cluster_break(0x0903)),    int(Grapheme_Cluster_Break::SpacingMark));
    TEST_EQUAL(int(grapheme_cluster_break(0x1100)),    int(Grapheme_Cluster_Break::L));
    TEST_EQUAL(int(grapheme_cluster_break(0x1160)),    int(Grapheme_Cluster_Break::V));
    TEST_EQUAL(int(grapheme_cluster_break(0x11a8)),    int(Grapheme_Cluster_Break::T));
    TEST_EQUAL(int(grapheme_cluster_break(0x200d)),    int(Grapheme_Cluster_Break::ZWJ));
    TEST_EQUAL(int(grapheme_cluster_break(0xa97c)),    int(Grapheme_Cluster_Break::L));
    TEST_EQUAL(int(grapheme_cluster_break(0xac00)),    int(Grapheme_Cluster_Break::LV));
    TEST_EQUAL(int(grapheme_cluster_break(0xac01)),    int(Grapheme_Cluster_Break::LVT));
    TEST_EQUAL(int(grapheme_cluster_break(0xd788)),    int(Grapheme_Cluster_Break::LV));
    TEST_EQUAL(int(grapheme_cluster_break(0xd7a3)),    int(Grapheme_Cluster_Break::LVT));
    TEST_EQUAL(int(grapheme_cluster_break(0xd7c6)),    int(Grapheme_Cluster_Break::V));
    TEST_EQUAL(int(grapheme_cluster_break(0xd7fb)),    int(Grapheme_Cluster_Break::T));
    TEST_EQUAL(int(grapheme_cluster_break(0x1'1f02)),  int(Grapheme_Cluster_Break::Prepend));
    TEST_EQUAL(int(grapheme_cluster_break(0x1'd16d)),  int(Grapheme_Cluster_Break::SpacingMark));
    TEST_EQUAL(int(grapheme_cluster_break(0x1'f1e6)),  int(Grapheme_Cluster_Break::Regional_Indicator));
    TEST_EQUAL(int(grapheme_cluster_break(0x1'f1ff)),  int(Grapheme_Cluster_Break::Regional_Indicator));
    TEST_EQUAL(int(grapheme_cluster_break(0xe'01ef)),  int(Grapheme_Cluster_Break::Extend));
    TEST_EQUAL(int(grapheme_cluster_break(0xe'0fff)),  int(Grapheme_Cluster_Break::Control));

}

void test_rs_unicode_character_properties_simple_lowercase_mapping() {

    TEST_EQUAL(static_cast<std::uint32_t>(simple_lowercase_mapping(0x0020)),  0u);       // space
    TEST_EQUAL(static_cast<std::uint32_t>(simple_lowercase_mapping(0x0041)),  0x0061u);  // latin capital letter a
    TEST_EQUAL(static_cast<std::uint32_t>(simple_lowercase_mapping(0x005a)),  0x007au);  // latin capital letter z
    TEST_EQUAL(static_cast<std::uint32_t>(simple_lowercase_mapping(0x0061)),  0u);       // latin small letter a
    TEST_EQUAL(static_cast<std::uint32_t>(simple_lowercase_mapping(0x007a)),  0u);       // latin small letter z
    TEST_EQUAL(static_cast<std::uint32_t>(simple_lowercase_mapping(0x00c0)),  0x00e0u);  // latin capital letter a with grave
    TEST_EQUAL(static_cast<std::uint32_t>(simple_lowercase_mapping(0x00c9)),  0x00e9u);  // latin capital letter e with acute
    TEST_EQUAL(static_cast<std::uint32_t>(simple_lowercase_mapping(0x00ce)),  0x00eeu);  // latin capital letter i with circumflex
    TEST_EQUAL(static_cast<std::uint32_t>(simple_lowercase_mapping(0x00d5)),  0x00f5u);  // latin capital letter o with tilde
    TEST_EQUAL(static_cast<std::uint32_t>(simple_lowercase_mapping(0x00dc)),  0x00fcu);  // latin capital letter u with diaeresis
    TEST_EQUAL(static_cast<std::uint32_t>(simple_lowercase_mapping(0x00e0)),  0u);       // latin small letter a with grave
    TEST_EQUAL(static_cast<std::uint32_t>(simple_lowercase_mapping(0x00e9)),  0u);       // latin small letter e with acute
    TEST_EQUAL(static_cast<std::uint32_t>(simple_lowercase_mapping(0x00ee)),  0u);       // latin small letter i with circumflex
    TEST_EQUAL(static_cast<std::uint32_t>(simple_lowercase_mapping(0x00f5)),  0u);       // latin small letter o with tilde
    TEST_EQUAL(static_cast<std::uint32_t>(simple_lowercase_mapping(0x00fc)),  0u);       // latin small letter u with diaeresis
    TEST_EQUAL(static_cast<std::uint32_t>(simple_lowercase_mapping(0x01c4)),  0x01c6u);  // latin capital letter dz with caron
    TEST_EQUAL(static_cast<std::uint32_t>(simple_lowercase_mapping(0x01c5)),  0x01c6u);  // latin capital letter d with small letter z with caron
    TEST_EQUAL(static_cast<std::uint32_t>(simple_lowercase_mapping(0x01c6)),  0u);       // latin small letter dz with caron

}

void test_rs_unicode_character_properties_simple_titlecase_mapping() {

    TEST_EQUAL(static_cast<std::uint32_t>(simple_titlecase_mapping(0x0020)),  0u);       // space
    TEST_EQUAL(static_cast<std::uint32_t>(simple_titlecase_mapping(0x0041)),  0u);       // latin capital letter a
    TEST_EQUAL(static_cast<std::uint32_t>(simple_titlecase_mapping(0x005a)),  0u);       // latin capital letter z
    TEST_EQUAL(static_cast<std::uint32_t>(simple_titlecase_mapping(0x0061)),  0x0041u);  // latin small letter a
    TEST_EQUAL(static_cast<std::uint32_t>(simple_titlecase_mapping(0x007a)),  0x005au);  // latin small letter z
    TEST_EQUAL(static_cast<std::uint32_t>(simple_titlecase_mapping(0x00c0)),  0u);       // latin capital letter a with grave
    TEST_EQUAL(static_cast<std::uint32_t>(simple_titlecase_mapping(0x00c9)),  0u);       // latin capital letter e with acute
    TEST_EQUAL(static_cast<std::uint32_t>(simple_titlecase_mapping(0x00ce)),  0u);       // latin capital letter i with circumflex
    TEST_EQUAL(static_cast<std::uint32_t>(simple_titlecase_mapping(0x00d5)),  0u);       // latin capital letter o with tilde
    TEST_EQUAL(static_cast<std::uint32_t>(simple_titlecase_mapping(0x00dc)),  0u);       // latin capital letter u with diaeresis
    TEST_EQUAL(static_cast<std::uint32_t>(simple_titlecase_mapping(0x00e0)),  0x00c0u);  // latin small letter a with grave
    TEST_EQUAL(static_cast<std::uint32_t>(simple_titlecase_mapping(0x00e9)),  0x00c9u);  // latin small letter e with acute
    TEST_EQUAL(static_cast<std::uint32_t>(simple_titlecase_mapping(0x00ee)),  0x00ceu);  // latin small letter i with circumflex
    TEST_EQUAL(static_cast<std::uint32_t>(simple_titlecase_mapping(0x00f5)),  0x00d5u);  // latin small letter o with tilde
    TEST_EQUAL(static_cast<std::uint32_t>(simple_titlecase_mapping(0x00fc)),  0x00dcu);  // latin small letter u with diaeresis
    TEST_EQUAL(static_cast<std::uint32_t>(simple_titlecase_mapping(0x01c4)),  0x01c5u);  // latin capital letter dz with caron
    TEST_EQUAL(static_cast<std::uint32_t>(simple_titlecase_mapping(0x01c5)),  0x01c5u);  // latin capital letter d with small letter z with caron
    TEST_EQUAL(static_cast<std::uint32_t>(simple_titlecase_mapping(0x01c6)),  0x01c5u);  // latin small letter dz with caron

}

void test_rs_unicode_character_properties_simple_uppercase_mapping() {

    TEST_EQUAL(static_cast<std::uint32_t>(simple_uppercase_mapping(0x0020)),  0u);       // space
    TEST_EQUAL(static_cast<std::uint32_t>(simple_uppercase_mapping(0x0041)),  0u);       // latin capital letter a
    TEST_EQUAL(static_cast<std::uint32_t>(simple_uppercase_mapping(0x005a)),  0u);       // latin capital letter z
    TEST_EQUAL(static_cast<std::uint32_t>(simple_uppercase_mapping(0x0061)),  0x0041u);  // latin small letter a
    TEST_EQUAL(static_cast<std::uint32_t>(simple_uppercase_mapping(0x007a)),  0x005au);  // latin small letter z
    TEST_EQUAL(static_cast<std::uint32_t>(simple_uppercase_mapping(0x00c0)),  0u);       // latin capital letter a with grave
    TEST_EQUAL(static_cast<std::uint32_t>(simple_uppercase_mapping(0x00c9)),  0u);       // latin capital letter e with acute
    TEST_EQUAL(static_cast<std::uint32_t>(simple_uppercase_mapping(0x00ce)),  0u);       // latin capital letter i with circumflex
    TEST_EQUAL(static_cast<std::uint32_t>(simple_uppercase_mapping(0x00d5)),  0u);       // latin capital letter o with tilde
    TEST_EQUAL(static_cast<std::uint32_t>(simple_uppercase_mapping(0x00dc)),  0u);       // latin capital letter u with diaeresis
    TEST_EQUAL(static_cast<std::uint32_t>(simple_uppercase_mapping(0x00e0)),  0x00c0u);  // latin small letter a with grave
    TEST_EQUAL(static_cast<std::uint32_t>(simple_uppercase_mapping(0x00e9)),  0x00c9u);  // latin small letter e with acute
    TEST_EQUAL(static_cast<std::uint32_t>(simple_uppercase_mapping(0x00ee)),  0x00ceu);  // latin small letter i with circumflex
    TEST_EQUAL(static_cast<std::uint32_t>(simple_uppercase_mapping(0x00f5)),  0x00d5u);  // latin small letter o with tilde
    TEST_EQUAL(static_cast<std::uint32_t>(simple_uppercase_mapping(0x00fc)),  0x00dcu);  // latin small letter u with diaeresis
    TEST_EQUAL(static_cast<std::uint32_t>(simple_uppercase_mapping(0x01c4)),  0u);       // latin capital letter dz with caron
    TEST_EQUAL(static_cast<std::uint32_t>(simple_uppercase_mapping(0x01c5)),  0x01c4u);  // latin capital letter d with small letter z with caron
    TEST_EQUAL(static_cast<std::uint32_t>(simple_uppercase_mapping(0x01c6)),  0x01c4u);  // latin small letter dz with caron

}

void test_rs_unicode_character_properties_full_composition_exclusion() {

    TEST(! is_full_composition_exclusion(0));       // null
    TEST(! is_full_composition_exclusion(0x0041));  // latin capital letter a
    TEST(! is_full_composition_exclusion(0x0391));  // greek capital letter alpha
    TEST(is_full_composition_exclusion(0x037e));    // greek question mark
    TEST(is_full_composition_exclusion(0x0958));    // devanagari letter qa
    TEST(is_full_composition_exclusion(0x2000));    // en quad
    TEST(is_full_composition_exclusion(0xf900));    // cjk compatibility ideograph f900
    TEST(is_full_composition_exclusion(0x2'fa1d));  // cjk compatibility ideograph 2fa1d

}

void test_rs_unicode_character_properties_extended_pictographic() {

    TEST(! is_extended_pictographic(0));       // null
    TEST(! is_extended_pictographic(0x0041));  // latin capital letter a
    TEST(is_extended_pictographic(0x00a9));    // copyright
    TEST(! is_extended_pictographic(0x0391));  // greek capital letter alpha
    TEST(is_extended_pictographic(0x2139));    // information
    TEST(is_extended_pictographic(0x3299));    // japanese secret button
    TEST(is_extended_pictographic(0x1'f000));  // mahjong tile east wind
    TEST(is_extended_pictographic(0x1'faf8));  // rightwards pushing hand

}

void test_rs_unicode_character_properties_pattern_syntax() {

    TEST(! is_pattern_syntax(0));
    TEST(is_pattern_syntax('!'));
    TEST(! is_pattern_syntax('0'));
    TEST(! is_pattern_syntax('A'));
    TEST(is_pattern_syntax(0xa1)); // inverted exclamation mark
    TEST(! is_pattern_syntax(0x3a9)); // greek capital letter omega
    TEST(! is_pattern_syntax(max_unicode));

}

void test_rs_unicode_character_properties_xid_continue() {

    TEST(! is_xid_continue(0));
    TEST(! is_xid_continue('!'));
    TEST(is_xid_continue('0'));
    TEST(is_xid_continue('A'));
    TEST(! is_xid_continue(0xa1)); // inverted exclamation mark
    TEST(is_xid_continue(0x3a9)); // greek capital letter omega
    TEST(! is_xid_continue(max_unicode));

}

void test_rs_unicode_character_properties_xid_start() {

    TEST(! is_xid_start(0));
    TEST(! is_xid_start('!'));
    TEST(! is_xid_start('0'));
    TEST(is_xid_start('A'));
    TEST(! is_xid_start(0xa1)); // inverted exclamation mark
    TEST(is_xid_start(0x3a9)); // greek capital letter omega
    TEST(! is_xid_start(max_unicode));

}

void test_rs_unicode_character_properties_noncharacter() {

    TEST(! is_noncharacter(0));
    TEST(! is_noncharacter(U'A'));
    TEST(! is_noncharacter(0xd7ff));
    TEST(is_noncharacter(0xd800));
    TEST(is_noncharacter(0xdfff));
    TEST(! is_noncharacter(0xe000));
    TEST(! is_noncharacter(0xfdcf));
    TEST(is_noncharacter(0xfdd0));
    TEST(is_noncharacter(0xfdef));
    TEST(! is_noncharacter(0xfdf0));
    TEST(! is_noncharacter(0xfffd));
    TEST(is_noncharacter(0xfffe));
    TEST(is_noncharacter(0xffff));
    TEST(! is_noncharacter(0x1'0000));
    TEST(! is_noncharacter(0x1'fffd));
    TEST(is_noncharacter(0x1'fffe));
    TEST(is_noncharacter(0x1'ffff));
    TEST(! is_noncharacter(0x2'0000));
    TEST(! is_noncharacter(0x10'fffd));
    TEST(is_noncharacter(0x10'fffe));
    TEST(is_noncharacter(0x10'ffff));
    TEST(is_noncharacter(0x11'0000));

}

void test_rs_unicode_character_properties_private_use() {

    TEST(! is_private_use(0));
    TEST(! is_private_use(0xdfff));
    TEST(is_private_use(0xe000));
    TEST(is_private_use(0xf8ff));
    TEST(! is_private_use(0xf900));
    TEST(! is_private_use(0xe'ffff));
    TEST(is_private_use(0xf'0000));
    TEST(is_private_use(0xf'fffd));
    TEST(! is_private_use(0xf'fffe));
    TEST(! is_private_use(0xf'ffff));
    TEST(is_private_use(0x10'0000));
    TEST(is_private_use(0x10'fffd));
    TEST(! is_private_use(0x10'fffe));
    TEST(! is_private_use(0x11'0000));

}
