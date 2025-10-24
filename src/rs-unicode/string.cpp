#include "rs-unicode/string.hpp"
#include <algorithm>
#include <bit>
#include <cstdlib>
#include <unordered_set>

namespace rs = std::ranges;

namespace RS::Unicode {

    namespace {

        std::size_t terminal_width() noexcept {
            auto columns_str = std::getenv("COLUMNS");
            if (columns_str != nullptr && *columns_str != 0) {
                auto n_columns = std::strtoul(columns_str, nullptr, 10);
                if (n_columns > 0) {
                    return n_columns;
                }
            }
            return 80;
        }

    }

    // GraphemeIterator class

    GraphemeIterator& GraphemeIterator::operator++() {
        tail_ = tail_.substr(current_.size());
        next();
        return *this;
    }

    bool GraphemeIterator::operator==(const GraphemeIterator& i) const noexcept {
        return current_.data() == i.current_.data();
    }

    void GraphemeIterator::next() {

        current_ = tail_.substr(0, 0);

        if (tail_.empty()) {
            return;
        }

        auto c = U'\0';
        auto pos = decode_utf8_char(tail_.data(), c);
        auto gcb = grapheme_cluster_break(c);
        auto icb = indic_conjunct_break(c);
        auto icb_prefix = 0;
        auto epg = is_extended_pictographic(c);

        while (pos < tail_.size()) {

            // UAX #29: Unicode Text Segmentation
            // https://www.unicode.org/reports/tr29/tr29-43.html
            // Break at the start and end of text, unless the text is empty.
            // GB1 sot ÷ Any
            // GB2 Any ÷ eot

            auto prev_pos = pos;
            auto prev_gcb = gcb;
            auto prev_icb = icb;
            auto prev_epg = epg;
            pos += decode_utf8_char(tail_.data() + pos, c);
            gcb = grapheme_cluster_break(c);
            icb = indic_conjunct_break(c);

            if (prev_icb == Indic_Conjunct_Break::Consonant) {
                icb_prefix = 1;
            } else if (icb_prefix > 0 && prev_icb == Indic_Conjunct_Break::Linker) {
                ++icb_prefix;
            } else if (prev_icb != Indic_Conjunct_Break::Extend) {
                icb_prefix = 0;
            }

            if (gcb != Grapheme_Cluster_Break::Extend && gcb != Grapheme_Cluster_Break::ZWJ) {
                epg = is_extended_pictographic(c);
            }

            if (prev_gcb == Grapheme_Cluster_Break::CR && gcb == Grapheme_Cluster_Break::LF) {
                // Do not break between a CR and LF. Otherwise, break before and after controls.
                // GB3 CR × LF
            } else if (prev_gcb == Grapheme_Cluster_Break::Control || prev_gcb == Grapheme_Cluster_Break::CR
                    || prev_gcb == Grapheme_Cluster_Break::LF) {
                // GB4 (Control | CR | LF) ÷
                pos = prev_pos;
                break;
            } else if (gcb == Grapheme_Cluster_Break::Control || gcb == Grapheme_Cluster_Break::CR
                    || gcb == Grapheme_Cluster_Break::LF) {
                // GB5 ÷ (Control | CR | LF)
                pos = prev_pos;
                break;
            } else if (prev_gcb == Grapheme_Cluster_Break::L
                    && (gcb == Grapheme_Cluster_Break::L || gcb == Grapheme_Cluster_Break::V
                        || gcb == Grapheme_Cluster_Break::LV || gcb == Grapheme_Cluster_Break::LVT)) {
                // Do not break Hangul syllable sequences.
                // GB6 L × (L | V | LV | LVT)
            } else if ((prev_gcb == Grapheme_Cluster_Break::LV || prev_gcb == Grapheme_Cluster_Break::V)
                    && (gcb == Grapheme_Cluster_Break::V || gcb == Grapheme_Cluster_Break::T)) {
                // GB7 (LV | V) × (V | T)
            } else if ((prev_gcb == Grapheme_Cluster_Break::LVT || prev_gcb == Grapheme_Cluster_Break::T)
                    && gcb == Grapheme_Cluster_Break::T) {
                // GB8 (LVT | T) × T
            } else if (gcb == Grapheme_Cluster_Break::Extend || gcb == Grapheme_Cluster_Break::ZWJ) {
                // Do not break before extending characters or ZWJ.
                // GB9 × (Extend | ZWJ)
            } else if (gcb == Grapheme_Cluster_Break::SpacingMark) {
                // Do not break before SpacingMarks, or after Prepend characters.
                // GB9a × SpacingMark
            } else if (prev_gcb == Grapheme_Cluster_Break::Prepend) {
                // GB9b Prepend ×
            } else if (icb_prefix >= 2 && icb == Indic_Conjunct_Break::Consonant) {
                // Do not break within certain combinations with Indic_Conjunct_Break (InCB)=Linker.
                // GB9c \p{InCB=Consonant} [ \p{InCB=Extend} \p{InCB=Linker} ]* \p{InCB=Linker}
                //      [ \p{InCB=Extend} \p{InCB=Linker} ]* × \p{InCB=Consonant}
            } else if (prev_epg && prev_gcb == Grapheme_Cluster_Break::ZWJ && epg) {
                // Do not break within emoji modifier sequences or emoji zwj sequences.
                // GB11 \p{Extended_Pictographic} Extend* ZWJ × \p{Extended_Pictographic}
            } else if (prev_gcb == Grapheme_Cluster_Break::Regional_Indicator
                    && gcb == Grapheme_Cluster_Break::Regional_Indicator) {
                // Do not break within emoji flag sequences. That is, do not break
                // between regional indicator (RI) symbols if there is an odd
                // number of RI characters before the break point.
                // GB12 sot (RI RI)* RI × RI
                // GB13 [^RI] (RI RI)* RI × RI
                gcb = Grapheme_Cluster_Break::Other;
            } else {
                // Otherwise, break everywhere.
                // GB999 Any ÷ Any
                pos = prev_pos;
                break;
            }

        }

        current_ = tail_.substr(0, pos);

    }

    // String segmentation iterators

    namespace {

        CharacterPredicate make_predicate(std::string_view delimiter) {
            auto set = decoded_utf8_view(delimiter) | rs::to<std::unordered_set>();
            return [set] (char32_t c) { return set.contains(c); };
        }

    }

    SplitIterator& SplitIterator::operator++() {
        next();
        return *this;
    }

    bool SplitIterator::operator==(const SplitIterator& i) const noexcept {
        return current_.data() == i.current_.data();
    }

    SplitIterator SplitIterator::words(std::string_view str) {
        return where(str, is_white_space);
    }

    SplitIterator SplitIterator::lines(std::string_view str) {
        SplitIterator it;
        it.tail_ = str;
        it.lines_ = true;
        it.next();
        return it;
    }

    SplitIterator SplitIterator::any(std::string_view str, std::string_view delimiter) {
        return where(str, make_predicate(delimiter));
    }

    SplitIterator SplitIterator::at(std::string_view str, std::string_view delimiter) {
        SplitIterator it;
        it.delimiter_ = delimiter;
        if (str.empty()) {
            // Do nothing
        } else if (it.delimiter_.empty()) {
            it.current_ = str;
        } else {
            auto pos = str.find(it.delimiter_);
            it.current_ = str.substr(0, pos);
            if (pos != npos) {
                it.tail_ = str.substr(pos);
            }
        }
        return it;
    }

    SplitIterator SplitIterator::where(std::string_view str, CharacterPredicate delimiter) {
        SplitIterator it;
        it.predicate_ = delimiter;
        it.tail_ = str;
        it.next();
        return it;
    }

    void SplitIterator::next() {

        if (tail_.empty()) {

            current_ = {};

        } else if (lines_) {

            for (auto pos = 0uz;; ++pos) {
                pos = tail_.find_first_of("\r\n", pos);
                if (pos == npos) {
                    current_ = tail_;
                    tail_ = {};
                    break;
                } else if (tail_[pos] == '\n') {
                    current_ = tail_.substr(0, pos);
                    tail_ = tail_.substr(pos + 1);
                    break;
                } else if (tail_.size() - pos >= 2 && tail_[pos + 1] == '\n') {
                    current_ = tail_.substr(0, pos);
                    tail_ = tail_.substr(pos + 2);
                    break;
                }
            }


        } else if (predicate_) {

            auto ptr = tail_.data();
            auto len = tail_.size();
            auto pos = 0uz;
            auto c = U'\0';

            while (pos < len) {
                auto n_bytes = decode_utf8_char(ptr + pos, c);
                if (! predicate_(c)) {
                    break;
                }
                pos += n_bytes;
            }

            auto endpos = pos;

            while (endpos < len) {
                auto n_bytes = decode_utf8_char(ptr + endpos, c);
                if (predicate_(c)) {
                    break;
                }
                endpos += n_bytes;
            }

            tail_ = tail_.substr(pos);

            if (tail_.empty()) {
                current_ = {};
            } else {
                current_ = tail_.substr(0, endpos - pos);
            }

            tail_ = tail_.substr(current_.size());

        } else {

            tail_ = tail_.substr(delimiter_.size());
            auto pos = tail_.find(delimiter_);
            current_ = tail_.substr(0, pos);
            tail_ = tail_.substr(current_.size());

        }

    }

    // String length

    namespace {

        std::size_t character_columns(char32_t c, bool default_wide) {
            if (is_default_ignorable(c)) {
                return 0;
            } else if (auto gc = general_category(c);
                    gc == General_Category::Cc || gc == General_Category::Cf) {
                return 0;
            } else if (grapheme_cluster_break(c) == Grapheme_Cluster_Break::Regional_Indicator) {
                return 2;
            } else if (auto eaw = east_asian_width(c);
                    eaw == East_Asian_Width::F || eaw == East_Asian_Width::W) {
                return 2;
            } else if (eaw != East_Asian_Width::A || ! default_wide) {
                return 1;
            } else {
                return 2;
            }
        }

        std::size_t text_columns(std::string_view str, bool default_wide) {
            auto base = U'\0';
            auto width = 0uz;
            for (auto grapheme: grapheme_view(str)) {
                decode_utf8_char(grapheme.data(), base);
                width += character_columns(base, default_wide);
            }
            return width;
        }

        std::size_t utf16_units(std::string_view str) {

            auto units = 0uz;

            for (auto c: str) {

                if (is_utf8_leading_unit(c)) {

                    auto uc = static_cast<unsigned char>(c);

                    if (uc <= 0x7f || (uc >= 0xc2 && uc <= 0xef)) {
                        ++units;
                    } else if (uc >= 0xf0 && uc <= 0xf4) {
                        units += 2;
                    }

                }

            }

            return units;

        }

    }

    std::size_t length(std::string_view str, Unit u) {
        switch (u) {
            case Unit::bytes:      return str.size();
            case Unit::utf16:      return utf16_units(str);
            case Unit::scalars:    return static_cast<std::size_t>(rs::count_if(str, is_utf8_leading_unit));
            case Unit::graphemes:  return static_cast<std::size_t>(rs::distance(grapheme_view(str)));
            case Unit::columns:    return text_columns(str, false);
            case Unit::wide:       return text_columns(str, true);
            default:               std::unreachable();
        }
    }

    std::pair<std::size_t, std::size_t> line_and_column(std::string_view text, std::size_t pos, Unit u) noexcept {
        auto prefix = text.substr(0, pos);
        auto line = std::ranges::count(prefix, '\n');
        auto start_line = prefix.find_last_of('\n') + 1;
        auto last_line = prefix.substr(start_line);
        auto column = length(last_line, u);
        return {line, column};
    }

    // Case conversion functions

    namespace {

        void apply_casing(char32_t c, std::string& out, char32_t (*simple_mapping)(char32_t), unsigned special_index) {

            auto& special = special_casing(c);

            if (! special[special_index].empty()) {

                out += utf32_to_utf8(special[special_index]);

            } else {

                auto simple = simple_mapping(c);

                if (simple == 0) {
                    append_utf8(c, out);
                } else {
                    append_utf8(simple, out);
                }

            }

        }

        void apply_lower_casing(char32_t c, std::string& out) {
            apply_casing(c, out, simple_lowercase_mapping, 0);
        }

        void apply_title_casing(char32_t c, std::string& out) {
            apply_casing(c, out, simple_titlecase_mapping, 1);
        }

        void apply_upper_casing(char32_t c, std::string& out) {
            apply_casing(c, out, simple_uppercase_mapping, 2);
        }

        void apply_lower_casing(std::string_view in, std::string& out) {
            for (auto c: decoded_utf8_view(in)) {
                apply_lower_casing(c, out);
            }
        }

        void apply_upper_casing(std::string_view in, std::string& out) {
            for (auto c: decoded_utf8_view(in)) {
                apply_upper_casing(c, out);
            }
        }

    }

    std::string to_casefold(std::string_view str) {

        std::string out;

        for (auto c: decoded_utf8_view(str)) {

            auto fold = case_folding(c);

            if (fold.empty()) {
                apply_lower_casing(c, out);
            } else {
                out += utf32_to_utf8(fold);
            }

        }

        return out;

    }

    std::string to_lowercase(std::string_view str) {
        std::string out;
        apply_lower_casing(str, out);
        return out;
    }

    std::string to_titlecase(std::string_view str) {

        std::string out;
        auto new_segment = true;

        for (auto c: decoded_utf8_view(str)) {
            if (is_white_space(c)) {
                append_utf8(c, out);
                new_segment = true;
            } else if (new_segment && is_alphanumeric(c)) {
                apply_title_casing(c, out);
                new_segment = false;
            } else {
                apply_lower_casing(c, out);
            }
        }

        return out;

    }

    std::string to_uppercase(std::string_view str) {
        std::string out;
        apply_upper_casing(str, out);
        return out;
    }

    // Normalization functions

    // References:
    // Unicode Standard 15.0, chapter 3.11, "Normalization Forms"
    // Unicode Standard Annex #15, "Unicode Normalization Forms"

    // Step 1: Decompose any characters that have canonical decompositions.
    // Decompose recursively until no more decompositions are possible.

    // Step 2: For each contiguous subsequence of non-starter characters, sort
    // the characters into ascending order by canonical combining class. This
    // is a stable sort, characters with the same combining class must not be
    // reordered.

    // For NFD, stop here. For NFC, continue with step 3.

    // Step 3: For each pair of characters that form a canonical composing
    // pair, replace the first with the composed character and delete the
    // second. A pair need not be adjacent to be composable, if the
    // characters between them are all non-starters with a lower combining
    // class than the second of the pair. Compose recursively until no more
    // compositions are possible.

    namespace {

        constexpr char32_t l_base = 0x1100;
        constexpr char32_t v_base = 0x1161;
        constexpr char32_t t_base = 0x11a7;
        constexpr char32_t s_base = 0xac00;
        constexpr char32_t n_count = 588;
        constexpr char32_t t_count = 28;

        bool is_starter(char32_t c) {
            return canonical_combining_class(c) == 0;
        };

        std::u32string hangul_decomposition(char32_t c) noexcept {

            auto hst = hangul_syllable_type(c);

            if (hst != Hangul_Syllable_Type::LV && hst != Hangul_Syllable_Type::LVT) {
                return {};
            }

            auto s_index = c - s_base;

            if (hst == Hangul_Syllable_Type::LV) {

                auto l_index = s_index / n_count;
                auto v_index = (s_index % n_count) / t_count;

                return {l_base + l_index, v_base + v_index};

            } else {

                auto lv_index = (s_index / t_count) * t_count;
                auto t_index = s_index % t_count;

                return {s_base + lv_index, t_base + t_index};

            }

        }

        char32_t hangul_composition(std::u32string_view chars) noexcept {

            auto hst0 = hangul_syllable_type(chars[0]);
            auto hst1 = hangul_syllable_type(chars[1]);

            if (hst0 == Hangul_Syllable_Type::L && hst1 == Hangul_Syllable_Type::V) {

                auto l_index = chars[0] - l_base;
                auto v_index = chars[1] - v_base;
                auto lv_index = l_index * n_count + v_index * t_count;

                return s_base + lv_index;

            } else if (hst0 == Hangul_Syllable_Type::LV && hst1 == Hangul_Syllable_Type::T) {

                auto lv = hangul_decomposition(chars[0]);
                auto l_index = lv[0] - l_base;
                auto v_index = lv[1] - v_base;
                auto t_index = chars[1] - t_base;
                auto lv_index = l_index * n_count + v_index * t_count;

                return s_base + lv_index + t_index;

            } else {

                return 0;

            }

        }

        std::u32string apply_decomposition(std::u32string_view str) {

            std::u32string result;
            result.reserve(str.size() + str.size() / 10);
            auto in = str.begin();
            auto in_end = str.end();
            auto out_pos = 0uz;
            std::u32string decomp;
            std::u32string_view decomp_view;

            while (in != in_end || out_pos < result.size()) {

                if (out_pos == result.size()) {

                    result += *in++;

                } else {

                    auto c = result[out_pos];

                    if (is_hangul(c)) {
                        decomp = hangul_decomposition(c);
                        decomp_view = decomp;
                    } else {
                        decomp_view = canonical_decomposition_mapping(c);
                    }

                    if (decomp_view.empty()) {
                        ++out_pos;
                    } else {
                        result[out_pos] = decomp_view[0];
                        if (decomp_view.size() == 2) {
                            result.insert(out_pos + 1, 1, decomp_view[1]);
                        }
                    }

                }

            }

            return result;

        }

        void apply_ordering(std::u32string& str) {

            auto i = str.begin();
            auto j = i;
            auto end = str.end();

            while (j != end) {

                i = std::find_if_not(j, end, is_starter);

                if (i == end) {
                    break;
                }

                j = std::find_if(i, end, is_starter);

                std::stable_sort(i, j, [] (char32_t a, char32_t b) {
                    return canonical_combining_class(a) < canonical_combining_class(b);
                });

            }

        }

        void apply_composition(std::u32string& str) {

            if (str.size() < 2) {
                return;
            }

            auto i = str.begin();
            auto end = str.end();

            while (i != end) {

                i = std::find_if(i, end, is_starter);

                if (i == end) {
                    break;
                }

                auto cc = 0;
                auto was_combined = false;

                for (auto j = i + 1; j != end; ++j) {

                    auto prev_cc = cc;
                    cc = canonical_combining_class(*j);

                    if (prev_cc == 0 || prev_cc < cc) {

                        auto hangul = is_hangul(*i);

                        if (is_hangul(*j) != hangul) {
                            break;
                        }

                        std::u32string chars{*i, *j};
                        char32_t comp;

                        if (hangul) {
                            comp = hangul_composition(chars);
                        } else {
                            comp = canonical_composition_mapping(chars);
                        }

                        if (comp != 0) {
                            *i = comp;
                            str.erase(j);
                            was_combined = true;
                            break;
                        }

                    }

                    if (cc == 0) {
                        break;
                    }

                }

                if (! was_combined) {
                    ++i;
                }

            }

        }

    }

    std::string to_nfc(std::string_view str) {
        auto str32 = utf8_to_utf32(str);
        auto result = apply_decomposition(str32);
        apply_ordering(result);
        apply_composition(result);
        return utf32_to_utf8(result);
    }

    std::string to_nfd(std::string_view str) {
        auto str32 = utf8_to_utf32(str);
        auto result = apply_decomposition(str32);
        apply_ordering(result);
        return utf32_to_utf8(result);
    }

    // Subscripts and superscripts

    std::optional<std::string> to_subscript(std::string_view str) {
        std::string out;
        for (auto ch: decoded_utf8_view(str)) {
            auto sub = subscript_character(ch);
            if (sub == 0) {
                return {};
            }
            append_utf8(sub, out);
        }
        return out;
    }

    std::optional<std::string> to_superscript(std::string_view str) {
        std::string out;
        for (auto ch: decoded_utf8_view(str)) {
            auto super = superscript_character(ch);
            if (super == 0) {
                return {};
            }
            append_utf8(super, out);
        }
        return out;
    }

    // Other string functions

    namespace {

        std::size_t character_width(char32_t c, Unit u) {
            switch (u) {
                case Unit::bytes:      return utf8_length(c);
                case Unit::utf16:      return c <= 0xffff ? 1 : 2;
                case Unit::scalars:    return 1;
                case Unit::graphemes:  return 1;
                case Unit::columns:    return character_columns(c, false);
                case Unit::wide:       return character_columns(c, true);
                default:               std::unreachable();
            }
        }

        std::string pad_helper(std::string_view str, std::size_t to_length,
                char32_t padding, Unit u, bool right) {

            std::string str_obj {str};
            auto str_len = length(str, u);

            if (str_len >= to_length) {
                return str_obj;
            }

            auto pad_len = character_width(padding, u);

            if (pad_len == 0) {
                return str_obj;
            }

            auto n = (to_length - str_len) / pad_len;
            auto pad_str = utf32_to_utf8(std::u32string(n, padding));

            if (right) {
                return str_obj + pad_str;
            } else {
                return pad_str + str_obj;
            }

        }

    }

    std::string indent(std::string_view str, std::size_t n, char32_t c) {

        if (str.empty()) {
            return {};
        }

        std::string out{str};
        auto margin = repeat(c, n);
        auto pos = 0uz;

        while (pos < out.size()) {

            if (out[pos] != '\n') {
                out.insert(pos, margin);
                pos += margin.size();
            }

            pos = out.find('\n', pos);

            if (pos == npos) {
                break;
            }

            ++pos;

        }

        return out;

    }

    std::string pad_left(std::string_view str, std::size_t to_length,
            char32_t padding, Unit u) {
        return pad_helper(str, to_length, padding, u, false);
    }

    std::string pad_right(std::string_view str, std::size_t to_length,
            char32_t padding, Unit u) {
        return pad_helper(str, to_length, padding, u, true);
    }

    std::pair<std::string_view, std::string_view> partition_words(std::string_view str) noexcept {
        return partition_where(str, is_white_space);
    }

    std::pair<std::string_view, std::string_view> partition_lines(std::string_view str) noexcept {

        auto lf = str.find('\n');

        if (lf == npos) {
            return {str, {}};
        }

        std::string_view left;
        auto right = str.substr(lf + 1);

        if (lf > 0 && str[lf - 1] == '\r') {
            left = str.substr(0, lf - 1);
        } else {
            left = str.substr(0, lf);
        }

        return {left, right};

    }

    std::pair<std::string_view, std::string_view> partition_any(std::string_view str,
            std::string_view delimiter) noexcept {
        return partition_where(str, make_predicate(delimiter));
    }

    std::pair<std::string_view, std::string_view> partition_at(std::string_view str,
            std::string_view delimiter) noexcept {

        auto pos = str.find(delimiter);
        std::string_view left, right;

        if (pos == npos) {
            left = str;
        } else {
            left = str.substr(0, pos);
            right = str.substr(pos + delimiter.size());
        }

        return {left, right};

    }

    std::pair<std::string_view, std::string_view> partition_where(std::string_view str,
            CharacterPredicate delimiter) noexcept {

        auto utf32 = decoded_utf8_view(str);
        auto i = rs::find_if(utf32, delimiter);
        std::string_view left, right;

        if (i == rs::end(utf32)) {
            left = str;
        } else {
            auto tail = rs::subrange{i, rs::end(utf32)};
            auto j = rs::find_if_not(tail, delimiter);
            left = {str.begin(), i.view().begin()};
            right = {j.view().begin(), str.end()};
        }

        return {left, right};

    }

    std::string repeat(char32_t c, std::size_t n) {
        std::string str;
        append_utf8(c, str);
        return repeat(str, n);
    }

    std::string repeat(std::string_view str, std::size_t n) {

        auto mask = std::bit_floor(n);
        std::string out;

        for (;;) {
            if ((n & mask) != 0) {
                out += str;
            }
            if ((mask >>= 1) == 0) {
                break;
            }
            out += out;
        }

        return out;

    }

    std::string replace(std::string_view source, std::string_view target,
            std::string_view replacement, std::size_t number) {

        std::string out;
        auto i = 0uz;
        auto source_size = source.size();
        auto target_size = target.size();
        auto count = 0uz;

        while (i < source_size) {

            if (count >= number) {
                out.append(source, i);
                break;
            }

            auto j = source.find(target, i);
            out.append(source, i, j - i);

            if (j == npos) {
                break;
            }

            out += replacement;
            i = j + target_size;
            ++count;

        }

        return out;

    }

    std::string_view trim(std::string_view str) {
        return trim_where(str, is_white_space);
    }

    std::string_view trim_chars(std::string_view str, std::string_view chars) {
        return trim_right_chars(trim_left_chars(str, chars), chars);
    }

    std::string_view trim_str(std::string_view str, std::string_view substr) {
        return trim_right_str(trim_left_str(str, substr), substr);
    }

    std::string_view trim_where(std::string_view str, CharacterPredicate pred) {
        return trim_right_where(trim_left_where(str, pred), pred);
    }

    std::string_view trim_left(std::string_view str) {
        return trim_left_where(str, is_white_space);
    }

    std::string_view trim_left_chars(std::string_view str, std::string_view chars) {
        auto uchars = utf8_to_utf32(chars);
        return trim_left_where(str, [&uchars] (char32_t c) { return uchars.contains(c); });
    }

    std::string_view trim_left_str(std::string_view str, std::string_view substr) {
        if (str.starts_with(substr)) {
            return str.substr(substr.size());
        } else {
            return str;
        }
    }

    std::string_view trim_left_where(std::string_view str, CharacterPredicate pred) {
        auto chars = decoded_utf8_view(str);
        auto it = rs::find_if_not(chars, pred);
        return std::string_view(it.view().begin(), str.end());
    }

    std::string_view trim_right(std::string_view str) {
        return trim_right_where(str, is_white_space);
    }

    std::string_view trim_right_chars(std::string_view str, std::string_view chars) {
        auto uchars = utf8_to_utf32(chars);
        return trim_right_where(str, [&uchars] (char32_t c) { return uchars.contains(c); });
    }

    std::string_view trim_right_str(std::string_view str, std::string_view substr) {
        if (str.ends_with(substr)) {
            return str.substr(0, str.size() - substr.size());
        } else {
            return str;
        }
    }

    std::string_view trim_right_where(std::string_view str, CharacterPredicate pred) {
        auto chars = decoded_utf8_view(str);
        auto begin = rs::begin(chars);
        auto it = rs::end(chars);
        while (it != begin) {
            --it;
            if (! pred(*it)) {
                ++it;
                break;
            }
        }
        return std::string_view(str.begin(), it.view().begin());
    }

    std::string word_wrap(std::string_view text, std::size_t width,
            std::size_t indent1, std::size_t indent2, Unit u) {

        if (width == npos) {
            width = terminal_width() - 1;
        }

        if (indent2 == npos) {
            indent2 = indent1;
        }

        std::string out;
        auto current_width = 0uz;
        auto indent = indent1;

        for (auto word: split_words(text)) {

            auto word_size = length(word, u);

            if (current_width + word_size > width) {
                out.back() = '\n';
                current_width = 0;
            }

            if (current_width == 0) {
                out.append(indent, ' ');
                current_width = indent;
                indent = indent2;
            }

            out += word;
            out += ' ';
            current_width += word_size + 1;

        }

        if (! out.empty() && out.back() == ' ') {
            out.back() = '\n';
        }

        return out;

    }

    // String search functions

    std::string_view find_first_of(std::string_view str, char32_t c) noexcept {
        auto utf32 = decoded_utf8_view(str);
        auto it = rs::find(utf32, c);
        return it == utf32.end() ? std::string_view() : it.view();
    }

    std::string_view find_first_of(std::string_view str, std::string_view chars) noexcept {
        return find_first_of(str, utf8_to_utf32(chars));
    }

    std::string_view find_first_of(std::string_view str, std::u32string_view chars) noexcept {
        auto utf32 = decoded_utf8_view(str);
        auto it = rs::find_first_of(utf32, chars);
        return it == utf32.end() ? std::string_view() : it.view();
    }

    std::string_view find_first_of(std::string_view str, CharacterPredicate pred) {
        auto utf32 = decoded_utf8_view(str);
        auto it = rs::find_if(utf32, pred);
        return it == utf32.end() ? std::string_view() : it.view();
    }

    std::string_view find_first_not_of(std::string_view str, char32_t c) noexcept {
        return find_first_not_of(str, [=] (char32_t c2) { return c == c2; });
    }

    std::string_view find_first_not_of(std::string_view str, std::string_view chars) noexcept {
        return find_first_not_of(str, utf8_to_utf32(chars));
    }

    std::string_view find_first_not_of(std::string_view str, std::u32string_view chars) noexcept {
        return find_first_not_of(str, [=] (char32_t c) { return chars.contains(c); });
    }

    std::string_view find_first_not_of(std::string_view str, CharacterPredicate pred) {
        auto utf32 = decoded_utf8_view(str);
        auto it = rs::find_if_not(utf32, pred);
        return it == utf32.end() ? std::string_view() : it.view();
    }

    std::string_view find_last_of(std::string_view str, char32_t c) noexcept {
        auto utf32 = decoded_utf8_view(str);
        auto it = rs::find_last(utf32, c).begin();
        return it == utf32.end() ? std::string_view() : it.view();
    }

    std::string_view find_last_of(std::string_view str, std::string_view chars) noexcept {
        return find_last_of(str, utf8_to_utf32(chars));
    }

    std::string_view find_last_of(std::string_view str, std::u32string_view chars) noexcept {
        return find_last_of(str, [=] (char32_t c) { return chars.contains(c); });
    }

    std::string_view find_last_of(std::string_view str, CharacterPredicate pred) {
        auto utf32 = decoded_utf8_view(str);
        auto it = rs::find_last_if(utf32, pred).begin();
        return it == utf32.end() ? std::string_view() : it.view();
    }

    std::string_view find_last_not_of(std::string_view str, char32_t c) noexcept {
        return find_last_not_of(str, [=] (char32_t c2) { return c == c2; });
    }

    std::string_view find_last_not_of(std::string_view str, std::string_view chars) noexcept {
        return find_last_not_of(str, utf8_to_utf32(chars));
    }

    std::string_view find_last_not_of(std::string_view str, std::u32string_view chars) noexcept {
        return find_last_not_of(str, [=] (char32_t c) { return chars.contains(c); });
    }

    std::string_view find_last_not_of(std::string_view str, CharacterPredicate pred) {
        auto utf32 = decoded_utf8_view(str);
        auto it = rs::find_last_if_not(utf32, pred).begin();
        return it == utf32.end() ? std::string_view() : it.view();
    }

    // String literals

    namespace Literals {

        std::string operator""_doc(const char* ptr, std::size_t len) {

            std::string_view source(ptr, len);
            auto lines = split_lines(source) | rs::to<std::vector>();

            if (lines.empty()) {
                return {};
            }

            auto margin = std::min(lines.back().find_first_not_of("\t "),
                lines.back().size());

            for (auto& line: lines) {
                auto spaces = line.find_first_not_of("\t ");
                if (spaces == npos) {
                    line = {};
                } else {
                    margin = std::min(margin, spaces);
                }
            }

            if (! lines.empty() && lines.front().empty()) {
                lines.erase(lines.begin());
            }

            if (! lines.empty() && lines.back().empty()) {
                lines.pop_back();
            }

            std::string result;

            for (auto& line: lines) {
                if (! line.empty()) {
                    result.append(line, margin, npos);
                }
                result += "\n";
            }

            return result;

        }

        std::vector<std::string> operator""_qw(const char* ptr, std::size_t len) {
            return reify(split_words({ptr, len}));
        }

    }

}
