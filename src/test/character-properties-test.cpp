#include "rs-unicode/character.hpp"
#include "rs-unicode/encoding.hpp"
#include "rs-core/unit-test.hpp"
#include <cstdint>

using namespace RS::Unicode;

namespace {

    template <typename T> constexpr auto to_u32(T t) noexcept { return static_cast<std::uint32_t>(t); }

}

void test_rs_unicode_character_properties_general_category() {

    using GC = General_Category;

    TEST_EQUAL(general_category(0),          GC::Cc);  TEST_EQUAL(primary_category(0),          'C');  //
    TEST_EQUAL(general_category(U'\n'),      GC::Cc);  TEST_EQUAL(primary_category(U'\n'),      'C');  //
    TEST_EQUAL(general_category(U' '),       GC::Zs);  TEST_EQUAL(primary_category(U' '),       'Z');  //
    TEST_EQUAL(general_category(U'!'),       GC::Po);  TEST_EQUAL(primary_category(U'!'),       'P');  //
    TEST_EQUAL(general_category(U'$'),       GC::Sc);  TEST_EQUAL(primary_category(U'$'),       'S');  //
    TEST_EQUAL(general_category(U'('),       GC::Ps);  TEST_EQUAL(primary_category(U'('),       'P');  //
    TEST_EQUAL(general_category(U')'),       GC::Pe);  TEST_EQUAL(primary_category(U')'),       'P');  //
    TEST_EQUAL(general_category(U'+'),       GC::Sm);  TEST_EQUAL(primary_category(U'+'),       'S');  //
    TEST_EQUAL(general_category(U'-'),       GC::Pd);  TEST_EQUAL(primary_category(U'-'),       'P');  //
    TEST_EQUAL(general_category(U'/'),       GC::Po);  TEST_EQUAL(primary_category(U'/'),       'P');  //
    TEST_EQUAL(general_category(U'0'),       GC::Nd);  TEST_EQUAL(primary_category(U'0'),       'N');  //
    TEST_EQUAL(general_category(U'9'),       GC::Nd);  TEST_EQUAL(primary_category(U'9'),       'N');  //
    TEST_EQUAL(general_category(U'A'),       GC::Lu);  TEST_EQUAL(primary_category(U'A'),       'L');  //
    TEST_EQUAL(general_category(U'Z'),       GC::Lu);  TEST_EQUAL(primary_category(U'Z'),       'L');  //
    TEST_EQUAL(general_category(U'^'),       GC::Sk);  TEST_EQUAL(primary_category(U'^'),       'S');  //
    TEST_EQUAL(general_category(U'_'),       GC::Pc);  TEST_EQUAL(primary_category(U'_'),       'P');  //
    TEST_EQUAL(general_category(U'a'),       GC::Ll);  TEST_EQUAL(primary_category(U'a'),       'L');  //
    TEST_EQUAL(general_category(U'z'),       GC::Ll);  TEST_EQUAL(primary_category(U'z'),       'L');  //
    TEST_EQUAL(general_category(0xa6),       GC::So);  TEST_EQUAL(primary_category(0xa6),       'S');  // broken bar
    TEST_EQUAL(general_category(0xaa),       GC::Lo);  TEST_EQUAL(primary_category(0xaa),       'L');  // feminine ordinal indicator
    TEST_EQUAL(general_category(0xab),       GC::Pi);  TEST_EQUAL(primary_category(0xab),       'P');  // left-pointing double angle quotation mark
    TEST_EQUAL(general_category(0xad),       GC::Cf);  TEST_EQUAL(primary_category(0xad),       'C');  // soft hyphen
    TEST_EQUAL(general_category(0xb2),       GC::No);  TEST_EQUAL(primary_category(0xb2),       'N');  // superscript two
    TEST_EQUAL(general_category(0xbb),       GC::Pf);  TEST_EQUAL(primary_category(0xbb),       'P');  // right-pointing double angle quotation mark
    TEST_EQUAL(general_category(0xf7),       GC::Sm);  TEST_EQUAL(primary_category(0xf7),       'S');  // division sign
    TEST_EQUAL(general_category(0x1c5),      GC::Lt);  TEST_EQUAL(primary_category(0x1c5),      'L');  // latin capital letter d with small letter z with caron
    TEST_EQUAL(general_category(0x2b0),      GC::Lm);  TEST_EQUAL(primary_category(0x2b0),      'L');  // modifier letter small h
    TEST_EQUAL(general_category(0x2b9),      GC::Lm);  TEST_EQUAL(primary_category(0x2b9),      'L');  // modifier letter prime
    TEST_EQUAL(general_category(0x2c5),      GC::Sk);  TEST_EQUAL(primary_category(0x2c5),      'S');  // modifier letter down arrowhead
    TEST_EQUAL(general_category(0x300),      GC::Mn);  TEST_EQUAL(primary_category(0x300),      'M');  // combining grave accent
    TEST_EQUAL(general_category(0x309),      GC::Mn);  TEST_EQUAL(primary_category(0x309),      'M');  // combining hook above
    TEST_EQUAL(general_category(0x378),      GC::Cn);  TEST_EQUAL(primary_category(0x378),      'C');  // unassigned
    TEST_EQUAL(general_category(0x488),      GC::Me);  TEST_EQUAL(primary_category(0x488),      'M');  // combining cyrillic hundred thousands sign
    TEST_EQUAL(general_category(0x5d1),      GC::Lo);  TEST_EQUAL(primary_category(0x5d1),      'L');  // hebrew letter bet
    TEST_EQUAL(general_category(0x6fd),      GC::So);  TEST_EQUAL(primary_category(0x6fd),      'S');  // arabic sign sindhi ampersand
    TEST_EQUAL(general_category(0x903),      GC::Mc);  TEST_EQUAL(primary_category(0x903),      'M');  // devanagari sign visarga
    TEST_EQUAL(general_category(0x94e),      GC::Mc);  TEST_EQUAL(primary_category(0x94e),      'M');  // devanagari vowel sign prishthamatra e
    TEST_EQUAL(general_category(0x9f7),      GC::No);  TEST_EQUAL(primary_category(0x9f7),      'N');  // bengali currency numerator four
    TEST_EQUAL(general_category(0x9fb),      GC::Sc);  TEST_EQUAL(primary_category(0x9fb),      'S');  // bengali ganda mark
    TEST_EQUAL(general_category(0x16ee),     GC::Nl);  TEST_EQUAL(primary_category(0x16ee),     'N');  // runic arlaug symbol
    TEST_EQUAL(general_category(0x180e),     GC::Cf);  TEST_EQUAL(primary_category(0x180e),     'C');  // mongolian vowel separator
    TEST_EQUAL(general_category(0x1f8d),     GC::Lt);  TEST_EQUAL(primary_category(0x1f8d),     'L');  // greek capital letter alpha with dasia and oxia and prosgegrammeni
    TEST_EQUAL(general_category(0x2006),     GC::Zs);  TEST_EQUAL(primary_category(0x2006),     'Z');  // six-per-em space
    TEST_EQUAL(general_category(0x2014),     GC::Pd);  TEST_EQUAL(primary_category(0x2014),     'P');  // em dash
    TEST_EQUAL(general_category(0x2028),     GC::Zl);  TEST_EQUAL(primary_category(0x2028),     'Z');  // line separator
    TEST_EQUAL(general_category(0x2029),     GC::Zp);  TEST_EQUAL(primary_category(0x2029),     'Z');  // paragraph separator
    TEST_EQUAL(general_category(0x207d),     GC::Ps);  TEST_EQUAL(primary_category(0x207d),     'P');  // superscript left parenthesis
    TEST_EQUAL(general_category(0x2166),     GC::Nl);  TEST_EQUAL(primary_category(0x2166),     'N');  // roman numeral seven
    TEST_EQUAL(general_category(0x2309),     GC::Pe);  TEST_EQUAL(primary_category(0x2309),     'P');  // right ceiling
    TEST_EQUAL(general_category(0x2e0c),     GC::Pi);  TEST_EQUAL(primary_category(0x2e0c),     'P');  // left raised omission bracket
    TEST_EQUAL(general_category(0x2e21),     GC::Pf);  TEST_EQUAL(primary_category(0x2e21),     'P');  // right vertical bar with quill
    TEST_EQUAL(general_category(0xa670),     GC::Me);  TEST_EQUAL(primary_category(0xa670),     'M');  // combining cyrillic ten millions sign
    TEST_EQUAL(general_category(0xd800),     GC::Cs);  TEST_EQUAL(primary_category(0xd800),     'C');  // non private use high surrogate
    TEST_EQUAL(general_category(0xdfff),     GC::Cs);  TEST_EQUAL(primary_category(0xdfff),     'C');  // low surrogate
    TEST_EQUAL(general_category(0xe000),     GC::Co);  TEST_EQUAL(primary_category(0xe000),     'C');  // private use
    TEST_EQUAL(general_category(0xff3f),     GC::Pc);  TEST_EQUAL(primary_category(0xff3f),     'P');  // fullwidth low line
    TEST_EQUAL(general_category(0x10'fffd),  GC::Co);  TEST_EQUAL(primary_category(0x10'fffd),  'C');  // plane 16 private use
    TEST_EQUAL(general_category(0x11'0000),  GC::Cn);  TEST_EQUAL(primary_category(0x11'0000),  'C');  // not unicode

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

    TEST_EQUAL(east_asian_width(0),       East_Asian_Width::N);
    TEST_EQUAL(east_asian_width(U' '),    East_Asian_Width::Na);
    TEST_EQUAL(east_asian_width(U'A'),    East_Asian_Width::Na);
    TEST_EQUAL(east_asian_width(0xa1),    East_Asian_Width::A);
    TEST_EQUAL(east_asian_width(0x1100),  East_Asian_Width::W);
    TEST_EQUAL(east_asian_width(0x20a9),  East_Asian_Width::H);
    TEST_EQUAL(east_asian_width(0x3000),  East_Asian_Width::F);

}

void test_rs_unicode_character_properties_grapheme_cluster_break() {

    TEST_EQUAL(grapheme_cluster_break(0),         Grapheme_Cluster_Break::Control);
    TEST_EQUAL(grapheme_cluster_break(0x000a),    Grapheme_Cluster_Break::LF);
    TEST_EQUAL(grapheme_cluster_break(0x000d),    Grapheme_Cluster_Break::CR);
    TEST_EQUAL(grapheme_cluster_break(0x0041),    Grapheme_Cluster_Break::Other);
    TEST_EQUAL(grapheme_cluster_break(0x0300),    Grapheme_Cluster_Break::Extend);
    TEST_EQUAL(grapheme_cluster_break(0x0600),    Grapheme_Cluster_Break::Prepend);
    TEST_EQUAL(grapheme_cluster_break(0x0903),    Grapheme_Cluster_Break::SpacingMark);
    TEST_EQUAL(grapheme_cluster_break(0x1100),    Grapheme_Cluster_Break::L);
    TEST_EQUAL(grapheme_cluster_break(0x1160),    Grapheme_Cluster_Break::V);
    TEST_EQUAL(grapheme_cluster_break(0x11a8),    Grapheme_Cluster_Break::T);
    TEST_EQUAL(grapheme_cluster_break(0x200d),    Grapheme_Cluster_Break::ZWJ);
    TEST_EQUAL(grapheme_cluster_break(0xa97c),    Grapheme_Cluster_Break::L);
    TEST_EQUAL(grapheme_cluster_break(0xac00),    Grapheme_Cluster_Break::LV);
    TEST_EQUAL(grapheme_cluster_break(0xac01),    Grapheme_Cluster_Break::LVT);
    TEST_EQUAL(grapheme_cluster_break(0xd788),    Grapheme_Cluster_Break::LV);
    TEST_EQUAL(grapheme_cluster_break(0xd7a3),    Grapheme_Cluster_Break::LVT);
    TEST_EQUAL(grapheme_cluster_break(0xd7c6),    Grapheme_Cluster_Break::V);
    TEST_EQUAL(grapheme_cluster_break(0xd7fb),    Grapheme_Cluster_Break::T);
    TEST_EQUAL(grapheme_cluster_break(0x1'1f02),  Grapheme_Cluster_Break::Prepend);
    TEST_EQUAL(grapheme_cluster_break(0x1'd16d),  Grapheme_Cluster_Break::Extend);
    TEST_EQUAL(grapheme_cluster_break(0x1'f1e6),  Grapheme_Cluster_Break::Regional_Indicator);
    TEST_EQUAL(grapheme_cluster_break(0x1'f1ff),  Grapheme_Cluster_Break::Regional_Indicator);
    TEST_EQUAL(grapheme_cluster_break(0xe'01ef),  Grapheme_Cluster_Break::Extend);
    TEST_EQUAL(grapheme_cluster_break(0xe'0fff),  Grapheme_Cluster_Break::Control);

}

void test_rs_unicode_character_properties_normalization_quick_check() {

    TEST_EQUAL(nfc_quick_check(0x0041),  NFC_Quick_Check::Y);  // latin capital letter a
    TEST_EQUAL(nfc_quick_check(0x03B1),  NFC_Quick_Check::Y);  // greek small letter alpha
    TEST_EQUAL(nfc_quick_check(0x20AC),  NFC_Quick_Check::Y);  // euro sign
    TEST_EQUAL(nfc_quick_check(0x0300),  NFC_Quick_Check::M);  // combining grave accent
    TEST_EQUAL(nfc_quick_check(0x0330),  NFC_Quick_Check::M);  // combining tilde below
    TEST_EQUAL(nfc_quick_check(0x0653),  NFC_Quick_Check::M);  // arabic maddah above
    TEST_EQUAL(nfc_quick_check(0x0343),  NFC_Quick_Check::N);  // combining greek koronis
    TEST_EQUAL(nfc_quick_check(0x0958),  NFC_Quick_Check::N);  // devanagari letter qa
    TEST_EQUAL(nfc_quick_check(0x0F73),  NFC_Quick_Check::N);  // tibetan vowel sign ii
    TEST_EQUAL(nfd_quick_check(0x0041),  NFD_Quick_Check::Y);  // latin capital letter a
    TEST_EQUAL(nfd_quick_check(0x03B1),  NFD_Quick_Check::Y);  // greek small letter alpha
    TEST_EQUAL(nfd_quick_check(0x20AC),  NFD_Quick_Check::Y);  // euro sign
    TEST_EQUAL(nfd_quick_check(0x00C0),  NFD_Quick_Check::N);  // latin capital letter a with grave
    TEST_EQUAL(nfd_quick_check(0x00FF),  NFD_Quick_Check::N);  // latin small letter y with diaeresis
    TEST_EQUAL(nfd_quick_check(0x0343),  NFD_Quick_Check::N);  // combining greek koronis

}

void test_rs_unicode_character_properties_simple_lowercase_mapping() {

    TEST_EQUAL(to_u32(simple_lowercase_mapping(0x0020)),  0u);       // space
    TEST_EQUAL(to_u32(simple_lowercase_mapping(0x0041)),  0x0061u);  // latin capital letter a
    TEST_EQUAL(to_u32(simple_lowercase_mapping(0x005a)),  0x007au);  // latin capital letter z
    TEST_EQUAL(to_u32(simple_lowercase_mapping(0x0061)),  0u);       // latin small letter a
    TEST_EQUAL(to_u32(simple_lowercase_mapping(0x007a)),  0u);       // latin small letter z
    TEST_EQUAL(to_u32(simple_lowercase_mapping(0x00c0)),  0x00e0u);  // latin capital letter a with grave
    TEST_EQUAL(to_u32(simple_lowercase_mapping(0x00c9)),  0x00e9u);  // latin capital letter e with acute
    TEST_EQUAL(to_u32(simple_lowercase_mapping(0x00ce)),  0x00eeu);  // latin capital letter i with circumflex
    TEST_EQUAL(to_u32(simple_lowercase_mapping(0x00d5)),  0x00f5u);  // latin capital letter o with tilde
    TEST_EQUAL(to_u32(simple_lowercase_mapping(0x00dc)),  0x00fcu);  // latin capital letter u with diaeresis
    TEST_EQUAL(to_u32(simple_lowercase_mapping(0x00e0)),  0u);       // latin small letter a with grave
    TEST_EQUAL(to_u32(simple_lowercase_mapping(0x00e9)),  0u);       // latin small letter e with acute
    TEST_EQUAL(to_u32(simple_lowercase_mapping(0x00ee)),  0u);       // latin small letter i with circumflex
    TEST_EQUAL(to_u32(simple_lowercase_mapping(0x00f5)),  0u);       // latin small letter o with tilde
    TEST_EQUAL(to_u32(simple_lowercase_mapping(0x00fc)),  0u);       // latin small letter u with diaeresis
    TEST_EQUAL(to_u32(simple_lowercase_mapping(0x01c4)),  0x01c6u);  // latin capital letter dz with caron
    TEST_EQUAL(to_u32(simple_lowercase_mapping(0x01c5)),  0x01c6u);  // latin capital letter d with small letter z with caron
    TEST_EQUAL(to_u32(simple_lowercase_mapping(0x01c6)),  0u);       // latin small letter dz with caron

}

void test_rs_unicode_character_properties_simple_titlecase_mapping() {

    TEST_EQUAL(to_u32(simple_titlecase_mapping(0x0020)),  0u);       // space
    TEST_EQUAL(to_u32(simple_titlecase_mapping(0x0041)),  0u);       // latin capital letter a
    TEST_EQUAL(to_u32(simple_titlecase_mapping(0x005a)),  0u);       // latin capital letter z
    TEST_EQUAL(to_u32(simple_titlecase_mapping(0x0061)),  0x0041u);  // latin small letter a
    TEST_EQUAL(to_u32(simple_titlecase_mapping(0x007a)),  0x005au);  // latin small letter z
    TEST_EQUAL(to_u32(simple_titlecase_mapping(0x00c0)),  0u);       // latin capital letter a with grave
    TEST_EQUAL(to_u32(simple_titlecase_mapping(0x00c9)),  0u);       // latin capital letter e with acute
    TEST_EQUAL(to_u32(simple_titlecase_mapping(0x00ce)),  0u);       // latin capital letter i with circumflex
    TEST_EQUAL(to_u32(simple_titlecase_mapping(0x00d5)),  0u);       // latin capital letter o with tilde
    TEST_EQUAL(to_u32(simple_titlecase_mapping(0x00dc)),  0u);       // latin capital letter u with diaeresis
    TEST_EQUAL(to_u32(simple_titlecase_mapping(0x00e0)),  0x00c0u);  // latin small letter a with grave
    TEST_EQUAL(to_u32(simple_titlecase_mapping(0x00e9)),  0x00c9u);  // latin small letter e with acute
    TEST_EQUAL(to_u32(simple_titlecase_mapping(0x00ee)),  0x00ceu);  // latin small letter i with circumflex
    TEST_EQUAL(to_u32(simple_titlecase_mapping(0x00f5)),  0x00d5u);  // latin small letter o with tilde
    TEST_EQUAL(to_u32(simple_titlecase_mapping(0x00fc)),  0x00dcu);  // latin small letter u with diaeresis
    TEST_EQUAL(to_u32(simple_titlecase_mapping(0x01c4)),  0x01c5u);  // latin capital letter dz with caron
    TEST_EQUAL(to_u32(simple_titlecase_mapping(0x01c5)),  0x01c5u);  // latin capital letter d with small letter z with caron
    TEST_EQUAL(to_u32(simple_titlecase_mapping(0x01c6)),  0x01c5u);  // latin small letter dz with caron

}

void test_rs_unicode_character_properties_simple_uppercase_mapping() {

    TEST_EQUAL(to_u32(simple_uppercase_mapping(0x0020)),  0u);       // space
    TEST_EQUAL(to_u32(simple_uppercase_mapping(0x0041)),  0u);       // latin capital letter a
    TEST_EQUAL(to_u32(simple_uppercase_mapping(0x005a)),  0u);       // latin capital letter z
    TEST_EQUAL(to_u32(simple_uppercase_mapping(0x0061)),  0x0041u);  // latin small letter a
    TEST_EQUAL(to_u32(simple_uppercase_mapping(0x007a)),  0x005au);  // latin small letter z
    TEST_EQUAL(to_u32(simple_uppercase_mapping(0x00c0)),  0u);       // latin capital letter a with grave
    TEST_EQUAL(to_u32(simple_uppercase_mapping(0x00c9)),  0u);       // latin capital letter e with acute
    TEST_EQUAL(to_u32(simple_uppercase_mapping(0x00ce)),  0u);       // latin capital letter i with circumflex
    TEST_EQUAL(to_u32(simple_uppercase_mapping(0x00d5)),  0u);       // latin capital letter o with tilde
    TEST_EQUAL(to_u32(simple_uppercase_mapping(0x00dc)),  0u);       // latin capital letter u with diaeresis
    TEST_EQUAL(to_u32(simple_uppercase_mapping(0x00e0)),  0x00c0u);  // latin small letter a with grave
    TEST_EQUAL(to_u32(simple_uppercase_mapping(0x00e9)),  0x00c9u);  // latin small letter e with acute
    TEST_EQUAL(to_u32(simple_uppercase_mapping(0x00ee)),  0x00ceu);  // latin small letter i with circumflex
    TEST_EQUAL(to_u32(simple_uppercase_mapping(0x00f5)),  0x00d5u);  // latin small letter o with tilde
    TEST_EQUAL(to_u32(simple_uppercase_mapping(0x00fc)),  0x00dcu);  // latin small letter u with diaeresis
    TEST_EQUAL(to_u32(simple_uppercase_mapping(0x01c4)),  0u);       // latin capital letter dz with caron
    TEST_EQUAL(to_u32(simple_uppercase_mapping(0x01c5)),  0x01c4u);  // latin capital letter d with small letter z with caron
    TEST_EQUAL(to_u32(simple_uppercase_mapping(0x01c6)),  0x01c4u);  // latin small letter dz with caron

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
    TEST(is_extended_pictographic(0x1'f004));  // mahjong red dragon
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

void test_rs_unicode_character_properties_subscripts_and_superscripts() {

    TEST_EQUAL(to_u32(subscript_character(U'0')),    0x2080u);  // U+0030 digit zero
    TEST_EQUAL(to_u32(superscript_character(U'0')),  0x2070u);  // U+0030 digit zero
    TEST_EQUAL(to_u32(subscript_character(U'A')),    0u);       // U+0041 latin capital letter a
    TEST_EQUAL(to_u32(superscript_character(U'A')),  0x1d2cu);  // U+0041 latin capital letter a
    TEST_EQUAL(to_u32(subscript_character(U'Z')),    0u);       // U+005a latin capital letter z
    TEST_EQUAL(to_u32(superscript_character(U'Z')),  0u);       // U+005a latin capital letter z
    TEST_EQUAL(to_u32(subscript_character(U'a')),    0x2090u);  // U+0061 latin small letter a
    TEST_EQUAL(to_u32(superscript_character(U'a')),  0x1d43u);  // U+0061 latin small letter a
    TEST_EQUAL(to_u32(subscript_character(U'z')),    0u);       // U+007a latin small letter z
    TEST_EQUAL(to_u32(superscript_character(U'z')),  0x1dbbu);  // U+007a latin small letter z

}

void test_rs_unicode_character_properties_identifiers() {

    TEST(! is_xid_start(0));
    TEST(! is_xid_start(U'!'));
    TEST(! is_xid_start(U'0'));
    TEST(is_xid_start(U'A'));
    TEST(! is_xid_start(U'_'));
    TEST(! is_xid_start(U'¡'));
    TEST(is_xid_start(U'Ω'));
    TEST(! is_xid_start(max_unicode));

    TEST(! is_xid_continue(0));
    TEST(! is_xid_continue(U'!'));
    TEST(is_xid_continue(U'0'));
    TEST(is_xid_continue(U'A'));
    TEST(is_xid_continue(U'_'));
    TEST(! is_xid_continue(U'¡'));
    TEST(is_xid_continue(U'Ω'));
    TEST(! is_xid_continue(max_unicode));

    TEST(! is_id_compat_math_start(0));
    TEST(! is_id_compat_math_start(U'A'));
    TEST(! is_id_compat_math_start(U'²'));
    TEST(! is_id_compat_math_start(U'¹'));
    TEST(is_id_compat_math_start(U'∂'));
    TEST(is_id_compat_math_start(U'∇'));
    TEST(is_id_compat_math_start(U'∞'));
    TEST(! is_id_compat_math_start(max_unicode));

    TEST(! is_id_compat_math_continue(0));
    TEST(! is_id_compat_math_continue(U'A'));
    TEST(is_id_compat_math_continue(U'²'));
    TEST(is_id_compat_math_continue(U'¹'));
    TEST(is_id_compat_math_continue(U'∂'));
    TEST(is_id_compat_math_continue(U'∇'));
    TEST(is_id_compat_math_continue(U'∞'));
    TEST(! is_id_compat_math_continue(max_unicode));

    TEST(! is_combined_id_start(0));
    TEST(! is_combined_id_start(U'!'));
    TEST(! is_combined_id_start(U'0'));
    TEST(is_combined_id_start(U'A'));
    TEST(is_combined_id_start(U'_'));
    TEST(! is_combined_id_start(U'¡'));
    TEST(! is_combined_id_start(U'²'));
    TEST(! is_combined_id_start(U'¹'));
    TEST(is_combined_id_start(U'Ω'));
    TEST(is_combined_id_start(U'∂'));
    TEST(is_combined_id_start(U'∇'));
    TEST(is_combined_id_start(U'∞'));
    TEST(! is_combined_id_start(max_unicode));

    TEST(! is_combined_id_continue(0));
    TEST(! is_combined_id_continue(U'!'));
    TEST(is_combined_id_continue(U'0'));
    TEST(is_combined_id_continue(U'A'));
    TEST(is_combined_id_continue(U'_'));
    TEST(! is_combined_id_continue(U'¡'));
    TEST(is_combined_id_continue(U'²'));
    TEST(is_combined_id_continue(U'¹'));
    TEST(is_combined_id_continue(U'Ω'));
    TEST(is_combined_id_continue(U'∂'));
    TEST(is_combined_id_continue(U'∇'));
    TEST(is_combined_id_continue(U'∞'));
    TEST(! is_combined_id_continue(max_unicode));

}
