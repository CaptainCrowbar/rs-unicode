#include "rs-unicode/legacy.hpp"
#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <format>
#include <system_error>
#include <unordered_set>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <iconv.h>
#endif

namespace RS::Unicode {

    #ifdef _WIN32

        namespace {

            std::wstring mbcs_to_wstring(std::string_view input, std::uint32_t cp) {

                if (input.empty()) {
                    return {};
                }

                std::wstring output(input.size(), '\0');

                for (;;) {
                    auto rc = MultiByteToWideChar(cp, 0, input.data(), input.size(),
                        output.data(), output.size());
                    if (rc > 0) {
                        output.resize(static_cast<std::size_t>(rc));
                        return output;
                    }
                    auto err = GetLastError();
                    if (rc == 0 && err != ERROR_INSUFFICIENT_BUFFER) {
                        throw std::system_error(static_cast<int>(err),
                            std::generic_category(), std::format("Code page {}", key));
                    }
                    output.resize(2 * output.size());
                }

            }

            std::string wstring_to_mbcs(std::wstring_view input, std::uint32_t cp) {

                if (input.empty()) {
                    return {};
                }

                static const std::unordered_set<std::uint32_t> special_pages {
                    42,
                    50220, 50221, 50222, 50225, 50227, 50229,
                    57002, 57003, 57004, 57005, 57006, 57007, 57008, 57009, 57010, 57011,
                    65000,
                };

                std::uint32_t flags = special_pages.contains(cp) ? 0 : WC_NO_BEST_FIT_CHARS;
                std::wstring output(input.size(), L'\0');

                for (;;) {
                    auto rc = WideCharToMultiByte(cp, flags, input.data(), input.size(),
                        output.data(), output.size(), nullptr, nullptr);
                    if (rc > 0) {
                        output.resize(static_cast<std::size_t>(rc));
                        return output;
                    }
                    auto err = GetLastError();
                    if (rc == 0 && err != ERROR_INSUFFICIENT_BUFFER) {
                        throw std::system_error(static_cast<int>(err),
                            std::generic_category(), std::format("Code page {}", key));
                    }
                    output.resize(2 * output.size());
                }

            }

        }

        LegacyEncoding::LegacyEncoding(const std::string& name) {

            static const std::unordered_set<std::string> code_page_prefixes {
                "",
                "CP",
                "IBM",
                "IBMCP",
                "MS",
                "MSCP",
                "WINDOWS",
            };

            auto fail = [&name] {
                throw std::system_error(ERROR_INVALID_PARAMETER, std::generic_category(),
                    std::format("Code page {}", name));
            };

            std::string prefix, suffix;

            for (auto c: name) {
                if (c >= '0' && c <= '9') {
                    suffix += c;
                } else if (! suffix.empty()) {
                    fail();
                } else if (c >= 'A' && c <= 'Z') {
                    prefix += c;
                } else if (c >= 'a' && c <= 'z') {
                    prefix += static_cast<char>(c - 0x20);
                } else if (c < ' ' || c > '~') {
                    fail();
                }
            }

            if (! code_page_prefixes.contains(prefix) || suffix.size() > 5) {
                fail();
            }

            auto page = static_cast<std::uint32_t>(std::strtoul(suffix.data(), nullptr, 10));
            *this = LegacyEncoding(page);

        }

        LegacyEncoding::LegacyEncoding(std::uint32_t key):
        page_(key) {
            auto rc = MultiByteToWideChar(key, 0, "", -1, nullptr, 0);
            auto err = GetLastError();
            if (rc == 0) {
                throw std::system_error(static_cast<int>(err),
                    std::generic_category(), std::format("Code page {}", key));
            }
        }

        std::string LegacyEncoding::from_utf8(std::string_view input) const {
            auto wstr = mbcs_to_wstring(input, page_);
            return wstring_to_mbcs(wstr, CP_UTF8);
        }

        std::string LegacyEncoding::to_utf8(std::string_view input) const {
            auto wstr = mbcs_to_wstring(input, CP_UTF8);
            return wstring_to_mbcs(wstr, page_);
        }

    #else

        namespace {

            const inline auto iconv_fail = reinterpret_cast<iconv_t>(std::intptr_t{-1});
            constexpr const char* utf = "utf-8";

            class Iconv {
            public:
                Iconv(const char* to, const char* from);
                ~Iconv();
                Iconv(const Iconv&) = delete;
                Iconv(Iconv&&) = delete;
                Iconv& operator=(const Iconv&) = delete;
                Iconv& operator=(Iconv&&) = delete;
                std::string operator()(std::string_view input, std::string_view replace);
            private:
                iconv_t cd_;
            };

            Iconv::Iconv(const char* to, const char* from) {
                errno = 0;
                cd_ = iconv_open(to, from);
                auto err = errno;
                if (cd_ == iconv_fail) {
                    throw std::system_error(err, std::system_category(),
                        std::format("{} => {}", from, to));
                }
            }

            Iconv::~Iconv() {
                if (cd_ != iconv_fail) {
                    iconv_close(cd_);
                }
            }

            std::string Iconv::operator()(std::string_view input, std::string_view replace) {

                if (input.empty()) {
                    return {};
                }

                auto inbuf = const_cast<char*>(input.data()); // Posix breakage
                auto inbytesleft = input.size();
                auto outbytesleft = 2 * inbytesleft;
                std::string output(outbytesleft, '\0');
                auto outbuf = output.data();

                auto reallocate = [&] () {
                    auto offset = outbuf - output.data();
                    outbytesleft += output.size();
                    output.resize(2 * output.size());
                    outbuf = output.data() + offset;
                };

                while (inbytesleft > 0) {

                    errno = 0;
                    iconv(cd_, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
                    auto err = errno;

                    switch (err) {

                        case 0:
                            // Conversion successfully completed
                            break;

                        case E2BIG:
                            // Output buffer is too short
                            reallocate();
                            break;

                        case EILSEQ:
                        case EINVAL:
                            // Invalid encoding in input
                            ++inbuf;
                            --inbytesleft;
                            if (outbytesleft < replace.size()) {
                                reallocate();
                            }
                            std::memcpy(outbuf, replace.data(), replace.size());
                            outbuf += replace.size();
                            outbytesleft -= replace.size();
                            break;

                        default:
                            // Some other error we can't handle
                            throw std::system_error(err, std::system_category());

                    }

                }

                output.resize(output.size() - outbytesleft);

                return output;

            }

        }

        LegacyEncoding::LegacyEncoding(const std::string& name):
        name_(name) {
            Iconv ic(utf, name.data());
        }

        LegacyEncoding::LegacyEncoding(std::uint32_t key):
        name_(std::to_string(key)) {
            Iconv ic(utf, name_.data());
        }

        std::string LegacyEncoding::from_utf8(std::string_view input) const {
            Iconv ic(name_.data(), utf);
            return ic(input, "?");
        }

        std::string LegacyEncoding::to_utf8(std::string_view input) const {
            Iconv ic(utf, name_.data());
            return ic(input, "\xef\xbf\xbd"); // U+FFFD
        }

    #endif

}
