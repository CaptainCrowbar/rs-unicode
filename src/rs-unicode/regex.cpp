#include "rs-unicode/regex.hpp"
#include <algorithm>
#include <format>
#include <memory>

#if defined(PCRE2_CODE_UNIT_WIDTH) && PCRE2_CODE_UNIT_WIDTH != 8
    #error Requires PCRE2 to be compiled for UTF-8
#endif

#define PCRE2_CODE_UNIT_WIDTH 8

#include <pcre2.h>

#if ! PCRE2_CONFIG_UNICODE
    #error Requires PCRE2 to be compiled with Unicode support
#endif

namespace RS::Unicode {

    namespace {

        constexpr auto match_time_flags =
            Regex::anchor
            | Regex::full
            | Regex::notbol
            | Regex::notempty
            | Regex::notemptystart
            | Regex::noteol
            | Regex::partial;

        constexpr auto format_time_flags =
            Regex::anchor
            | Regex::full
            | Regex::global
            | Regex::notbol
            | Regex::notempty
            | Regex::notemptystart
            | Regex::noteol;

        std::string error_message(int code) {
            auto message = "PCRE error " + std::to_string(code);
            std::string buf(120, '\0');
            auto rc = pcre2_get_error_message(code, reinterpret_cast<PCRE2_UCHAR*>(buf.data()), buf.size());
            if (rc > 0) {
                buf.resize(static_cast<std::size_t>(rc));
                message += ": " + buf;
            }
            return message;
        }

        pcre2_code* pc(void* ptr) noexcept {
            return static_cast<pcre2_code*>(ptr);
        }

        std::uint32_t compile_options(RegexFlags flags) noexcept {
            std::uint32_t options = 0;
            if (!! (flags & RegexFlags::anchor))     { options |= PCRE2_ANCHORED; }
            if (! (flags & RegexFlags::byte))        { options |= PCRE2_UCP | PCRE2_UTF; }
            if (!! (flags & RegexFlags::dotall))     { options |= PCRE2_DOTALL; }
            if (!! (flags & RegexFlags::extended))   { options |= PCRE2_EXTENDED; }
            if (!! (flags & RegexFlags::firstline))  { options |= PCRE2_FIRSTLINE; }
            if (!! (flags & RegexFlags::full))       { options |= PCRE2_ANCHORED | PCRE2_ENDANCHORED; }
            if (!! (flags & RegexFlags::icase))      { options |= PCRE2_CASELESS; }
            if (!! (flags & RegexFlags::multiline))  { options |= PCRE2_MULTILINE; }
            if (!! (flags & RegexFlags::nocapture))  { options |= PCRE2_NO_AUTO_CAPTURE; }
            return options;
        }

        std::uint32_t match_options(RegexFlags flags) noexcept {
            std::uint32_t options = 0;
            if (!! (flags & RegexFlags::anchor))         { options |= PCRE2_ANCHORED; }
            if (!! (flags & RegexFlags::full))           { options |= PCRE2_ANCHORED | PCRE2_ENDANCHORED; }
            if (!! (flags & RegexFlags::notbol))         { options |= PCRE2_NOTBOL; }
            if (!! (flags & RegexFlags::notempty))       { options |= PCRE2_NOTEMPTY; }
            if (!! (flags & RegexFlags::notemptystart))  { options |= PCRE2_NOTEMPTY_ATSTART; }
            if (!! (flags & RegexFlags::noteol))         { options |= PCRE2_NOTEOL; }
            if (!! (flags & RegexFlags::partial))        { options |= PCRE2_PARTIAL_HARD; }
            return options;
        }

        std::uint32_t format_options(RegexFlags flags) noexcept {
            static constexpr std::uint32_t common_options =
                PCRE2_SUBSTITUTE_EXTENDED
                | PCRE2_SUBSTITUTE_OVERFLOW_LENGTH
                | PCRE2_SUBSTITUTE_UNSET_EMPTY;
            auto options = match_options(flags) | common_options;
            if (!! (flags & RegexFlags::global)) {
                options |= PCRE2_SUBSTITUTE_GLOBAL;
            }
            return options;
        }

    }

    // Regex class

    Regex::Regex(std::string_view pattern, RegexFlags flags):
    flags_(flags) {

        if (pattern.starts_with("(?b)")) {
            pattern = pattern.substr(4);
            flags_ |= byte;
        }

        auto pattern_ptr = reinterpret_cast<PCRE2_SPTR>(pattern.data());
        auto options = compile_options(flags_);
        auto error_code = 0;
        PCRE2_SIZE error_offset = 0;
        code_ = pcre2_compile(pattern_ptr, pattern.size(), options, &error_code, &error_offset, nullptr);

        if (code_ == nullptr) {
            throw error(error_code);
        }

    }

    Regex::~Regex() noexcept {
        pcre2_code_free(pc(code_));
    }

    Regex::Regex(const Regex& re):
    code_(pcre2_code_copy(pc(re.code_))),
    flags_(re.flags_) {
        if (code_ == nullptr) {
            throw error("pcre2_code_copy() failed");
        }
    }

    Regex::Regex(Regex&& re) noexcept:
    code_(std::exchange(re.code_, nullptr)),
    flags_(re.flags_) {}

    Regex& Regex::operator=(const Regex& re) {
        if (&re != this) {
            Regex temp(re);
            swap(temp);
        }
        return *this;
    }

    Regex& Regex::operator=(Regex&& re) noexcept {
        code_ = std::exchange(re.code_, nullptr);
        flags_ = re.flags_;
        return *this;
    }

    std::size_t Regex::groups() const noexcept {
        if (code_ == nullptr) {
            return 0;
        }
        std::uint32_t count = 0;
        if (code_ != nullptr) {
            pcre2_pattern_info(pc(code_), PCRE2_INFO_CAPTURECOUNT, &count);
        }
        return count + 1;
    }

    Regex::match Regex::search(std::string_view subject, RegexFlags flags, std::size_t offset) const {

        struct free_match {
            void operator()(pcre2_match_data* ptr) const noexcept {
                pcre2_match_data_free(ptr);
            }
        };

        auto bad_flags = flags & ~ match_time_flags;

        if (bad_flags != RegexFlags::none) {
            throw error(std::format("Invalid flags in regex match: 0x{:x}",
                static_cast<std::uint16_t>(bad_flags)));
        } else if (code_ == nullptr || offset > subject.size()) {
            return {};
        }

        auto options = match_options(flags | flags_);
        auto match_ptr = pcre2_match_data_create_from_pattern(pc(code_), nullptr);

        if (match_ptr == nullptr) {
            throw error("pcre2_match_data_create_from_pattern() failed");
        }

        std::unique_ptr<pcre2_match_data, free_match> match_guard(match_ptr);
        match result;
        result.subject_ = subject;
        auto subject_ptr = reinterpret_cast<PCRE2_SPTR>(subject.data());
        auto rc = pcre2_match(pc(code_), subject_ptr, subject.size(), offset, options, match_ptr, nullptr);

        if (rc > 0) {
            result.groups_.resize(static_cast<std::size_t>(rc));
        } else if (rc == PCRE2_ERROR_NOMATCH) {
            return {};
        } else if (rc == PCRE2_ERROR_PARTIAL) {
            result.groups_.resize(1);
            result.partial_ = true;
        } else {
            throw error(rc);
        }

        auto ov = pcre2_get_ovector_pointer(match_ptr);

        for (auto& group: result.groups_) {
            auto pos = *ov++;
            auto endpos = *ov++;
            if (pos == PCRE2_UNSET) {
                group = {npos, npos};
            } else {
                group = {pos, endpos};
            }
        }

        return result;

    }

    Regex::match Regex::operator()(std::string_view subject, RegexFlags flags, std::size_t offset) const {
        return search(subject, flags, offset);
    }

    std::string Regex::format(std::string_view subject, std::string_view replace, RegexFlags flags) const {

        auto bad_flags = flags & ~ format_time_flags;

        if (bad_flags != RegexFlags::none) {
            throw error(std::format("Invalid flags in regex format: 0x{:x}",
                static_cast<std::uint16_t>(bad_flags)));
        } else if (code_ == nullptr) {
            return std::string{subject};
        }

        std::uint32_t options = format_options(flags | flags_);
        auto subject_ptr = reinterpret_cast<PCRE2_SPTR>(subject.data());
        auto replace_ptr = reinterpret_cast<PCRE2_SPTR>(replace.data());
        std::string out(2 * subject.size(), '\0');
        int rc = 0;

        for (auto i = 0; i < 2; ++i) {

            auto out_ptr = reinterpret_cast<PCRE2_UCHAR*>(out.data());
            PCRE2_SIZE outlen = out.size();
            rc = pcre2_substitute(pc(code_), subject_ptr, subject.size(), 0, options, nullptr, nullptr,
                replace_ptr, replace.size(), out_ptr, &outlen);

            if (rc < 0 && (rc != PCRE2_ERROR_NOMEMORY || outlen == PCRE2_UNSET)) {
                break;
            }

            out.resize(static_cast<std::size_t>(outlen));

            if (rc >= 0) {
                return out;
            }

        }

        throw error(rc);

    }

    void Regex::swap(Regex& re) noexcept {
        std::swap(code_, re.code_);
        std::swap(flags_, re.flags_);
    }

    std::pair<int, int> Regex::pcre_version() noexcept {
        return {PCRE2_MAJOR, PCRE2_MINOR};
    }

    // Exceptions

    Regex::error::error(const std::string& message):
    std::runtime_error("PCRE error: " + message) {}

    Regex::error::error(int code):
    std::runtime_error(error_message(code)), code_(code) {}

    // Match class

    bool Regex::match::matched(std::size_t index) const noexcept {
        return index < groups_.size() && groups_[index].first != npos;
    }

    std::size_t Regex::match::pos(std::size_t index) const noexcept {
        return index < groups_.size() ? groups_[index].first : npos;
    }

    std::size_t Regex::match::endpos(std::size_t index) const noexcept {
        return index < groups_.size() ? groups_[index].second : npos;
    }

    std::string_view Regex::match::str(std::size_t index) const noexcept {
        if (! matched(index)) {
            return {};
        }
        auto i = groups_[index].first;
        auto j = groups_[index].second;
        return subject_.substr(i, j - i);
    }

    // Match iterator class

    Regex::match_iterator::match_iterator(const Regex& regex, std::string_view subject,
        RegexFlags flags, std::size_t offset):
    regex_(&regex),
    subject_(subject),
    flags_(flags) {
        next(offset);
    }

    void Regex::match_iterator::next(std::size_t pos) {
        match_ = regex_->search(subject_, flags_, pos);
    }

    // Split iterator class

    Regex::split_iterator::split_iterator(const Regex& regex, std::string_view subject,
        RegexFlags flags, std::size_t offset):
    regex_(&regex),
    subject_(subject),
    restart_(offset),
    flags_(flags) {
        next();
    }

    void Regex::split_iterator::next() {
        if (restart_ == npos) {
            current_ = {};
        } else {
            auto next_match = regex_->search(subject_, flags_, restart_);
            if (next_match) {
                current_ = subject_.substr(restart_, next_match.pos() - restart_);
                restart_ = next_match.endpos();
            } else {
                current_ = subject_.substr(restart_);
                restart_ = npos;
            }
        }
    }

}
