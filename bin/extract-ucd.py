#!/usr/bin/env python3

import collections
import os
import re
import sys

def check_dir(path):
    if not os.path.isdir(path):
        print("*** Directory not found: " + path)
        sys.exit(1)
    return path

project_root = check_dir(os.path.dirname(sys.path[0]))

if os.path.basename(project_root) != "rs-unicode":
    print("*** Unexpected project directory: " + project_root)
    sys.exit(1)

ucd_root = check_dir(os.path.join(project_root, "ucd"))
source_root = check_dir(os.path.join(project_root, "src/rs-unicode"))
test_root = check_dir(os.path.join(project_root, "src/test"))
property_list_file = os.path.join(source_root, "property-list.hpp")
unicode_data = "UnicodeData.txt"
unicode_points = 0x110000
unicode_range = range(unicode_points)
enum_tables = {} # property name => list of enumeration values
property_tables = {} # property name => list of property values by code point

##################################################

PropertyInfo = collections.namedtuple(
    "PropertyInfo",
    [
        "type",      # Data type (see below)
        "default",   # Default value
        "file",      # File path (relative to UCD root)
        "column",    # Column in file (0-based; only relevant if file is "UnicodeData.txt", otherwise None)
        "ucd_name",  # Name used in UCD file, when different from property name
    ]
)

PropertyType = collections.namedtuple(
    "PropertyType",
    [
        "cpp_type",     # C++ data type (None if this is the same as the property name)
        "cpp_headers",  # List of required C++ standard headers
    ],
    defaults = [[]]
)

PropertyCppInfo = collections.namedtuple(
    "PropertyCppInfo",
    [
        "file",      # C++ source file for property table
        "type",      # C++ property type
        "function",  # C++ function declaration
        "headers",   # List of required C++ standard headers
    ]
)

property_info_table = {

    "General_Category"                 : PropertyInfo("enum",   "Cn",     unicode_data,                           2,     None                            ),
    "Canonical_Combining_Class"        : PropertyInfo("int",    0,        unicode_data,                           3,     None                            ),
    "Canonical_Decomposition_Mapping"  : PropertyInfo("chars",  None,     unicode_data,                           5,     None                            ),
    "Simple_Uppercase_Mapping"         : PropertyInfo("char",   None,     unicode_data,                           12,    None                            ),
    "Simple_Lowercase_Mapping"         : PropertyInfo("char",   None,     unicode_data,                           13,    None                            ),
    "Simple_Titlecase_Mapping"         : PropertyInfo("char",   None,     unicode_data,                           14,    None                            ),
    "Default_Ignorable"                : PropertyInfo("bool",   False,    "DerivedCoreProperties.txt",            None,  "Default_Ignorable_Code_Point"  ),
    "Indic_Conjunct_Break"             : PropertyInfo("enum",   "None",   "DerivedCoreProperties.txt",            None,  "InCB"                          ),
    "XID_Start"                        : PropertyInfo("bool",   False,    "DerivedCoreProperties.txt",            None,  None                            ),
    "XID_Continue"                     : PropertyInfo("bool",   False,    "DerivedCoreProperties.txt",            None,  None                            ),
    "Full_Composition_Exclusion"       : PropertyInfo("bool",   False,    "DerivedNormalizationProps.txt",        None,  None                            ),
    "East_Asian_Width"                 : PropertyInfo("enum",   "N",      "EastAsianWidth.txt",                   None,  None                            ),
    "Line_Break"                       : PropertyInfo("enum",   "XX",     "LineBreak.txt",                        None,  None                            ),
    "Pattern_Syntax"                   : PropertyInfo("bool",   False,    "PropList.txt",                         None,  None                            ),
    "Pattern_White_Space"              : PropertyInfo("bool",   False,    "PropList.txt",                         None,  None                            ),
    "White_Space"                      : PropertyInfo("bool",   False,    "PropList.txt",                         None,  None                            ),
    "Grapheme_Cluster_Break"           : PropertyInfo("enum",   "Other",  "auxiliary/GraphemeBreakProperty.txt",  None,  None                            ),
    "Extended_Pictographic"            : PropertyInfo("bool",   False,    "emoji/emoji-data.txt",                 None,  None                            ),

}

property_type_table = {

    "bool"   : PropertyType("bool"),                                    # Binary property
    "char"   : PropertyType("char32_t"),                                # USV in hex
    "chars"  : PropertyType("std::u32string_view", ["<string_view>"]),  # Unicode string in hex
    "enum"   : PropertyType(None),                                      # Enumeration property
    "int"    : PropertyType("int"),                                     # Integer property

}

property_cpp_info_table = {} # property name => derived info

for prop in property_info_table:

    info = property_info_table[prop]
    cpp_file_tag = prop.lower().replace("_", "-")
    cpp_file = f"ucd-{cpp_file_tag}.cpp"
    function_prefix = "is_" if info.type == "bool" else ""
    type_info = property_type_table[info.type]
    cpp_type = type_info.cpp_type if type_info.cpp_type else prop
    cpp_function = f"{cpp_type} {function_prefix}{prop.lower()}(char32_t c)"
    property_cpp_info_table[prop] = PropertyCppInfo(cpp_file, cpp_type, cpp_function, type_info.cpp_headers)

##################################################

# Remove leading indentation from every line in a multiline string. This
# allows multiline strings to be used inline without violating the
# indentation of surrounding code.

# The first line must be empty (after the leading triple quote) and is always
# stripped. The last line must consist only of whitespace (before the
# trailing triple quote) and indicates how much indentation to strip from all
# other lines.

# (textwrap.dedent() almost does what I want here, but it doesn't provide any
# way to leave partial indentation on every line.)

def undent(text):

    if not text or text.endswith("\n"):
        return text

    if not text.startswith("\n"):
        raise ValueError("Invalid text for undent(): Does not start with LF")

    lines = text.splitlines()
    indent = lines[-1]
    del lines[0]
    del lines[-1]
    undented = []

    for line in lines:
        if line and not line.startswith(indent):
            raise ValueError("Invalid text for undent(): Line does not start with expected indentation")
        undented.append(line.removeprefix(indent).rstrip())

    result = "\n".join(undented)
    result += "\n"
    return result

##################################################

# Parse a hex code point range.

def parse_code_range(text):
    left, mid, right = text.strip().partition("..")
    first = int(left, 16)
    if right:
        last = int(right, 16)
    else:
        last = first
    return (first, last)

##################################################

# Format a list of code points as an unquoted C++ string literal.
# Mode Q encloses string in quotes, mode U in U"...".

def hex_codes_to_cpp(codes, mode=None):
    if mode == "Q":
        cpp = '"'
    elif mode == "U":
        cpp = 'U"'
    else:
        cpp = ""
    for code in codes:
        if len(code) <= 4:
            cpp += "\\u" + code.lower().rjust(4, "0")
        else:
            cpp += "\\U" + code.lower().rjust(8, "0")
    if mode == "Q" or mode == "U":
        cpp += '"'
    return cpp

##################################################

# Convert a UCD property value to a C++ literal.

def ucd_value_to_cpp(ucd_value, property_name, property_type):

    if ucd_value is None:
        return "{}"
    elif property_type == "char":
        return f"0x{ucd_value.lower()}"
    elif property_type == "chars":
        return hex_codes_to_cpp(ucd_value.split(), "U")
    elif property_type == "enum":
        return f"{property_name}::{ucd_value}"
    else:
        return str(ucd_value).lower()

##################################################

# Extract property values from "UnicodeData.txt". This writes the results into
# the property_tables structure. The subscript and superscript properties
# require special handling and are covered in another function.

def extract_properties_from_unicode_data():

    print(f"Extracting {unicode_data}...")
    file_path = os.path.join(ucd_root, unicode_data)
    first_code = None
    column_table = {} # column number => property name

    for prop in property_info_table:
        info = property_info_table[prop]
        if info.column:
            column_table[info.column] = prop
        property_tables[prop] = [info.default] * unicode_points

    for line in open(file_path):

        columns = line.rstrip().split(";")
        code = int(columns[0], 16)
        name = columns[1]

        for col in column_table:
            value = columns[col]
            if value and not value.startswith("<"):
                property_tables[column_table[col]][code] = value

        if name.endswith(" First>"):
            first_code = code
        elif name.endswith(" Last>"):
            for col in column_table:
                code_map = property_tables[column_table[col]]
                value = code_map[code]
                for cp in range(first_code, code):
                    code_map[cp] = value
            first_code = None

##################################################

# Extract property values from a UCD file (other than "UnicodeData.txt"). This
# writes the results into the property_tables structure.

#   - First field is always the code point range
#   - If line contains 2 fields:
#       - If property type is "bool":
#           - Second field is the property name
#       - Any other property type:
#           - Second field is the property value
#           - Property name is implicit (file contains only one property)
#   - If line contains 3 fields:
#       - Subsequent fields are the property name and value

def extract_property_from_ucd_file(prop):

    print(f"Extracting {prop}...")
    info = property_info_table[prop]
    key = info.ucd_name if info.ucd_name else prop
    file_path = os.path.join(ucd_root, info.file)
    code_map = [info.default] * unicode_points
    property_tables[prop] = code_map

    for line in open(file_path):

        left, mid, right = line.partition("#")
        left = left.strip()

        if not left:
            continue

        fields = [field.strip() for field in left.split(";")]
        n_fields = len(fields)

        if n_fields == 2:
            if info.type == "bool" and fields[1] != key:
                continue
        elif n_fields == 3:
            if fields[1] != key:
                continue
        else:
            raise ValueError(f"Unexpected line in UCD file ({info.file}): {line.rstrip()}")

        code_range = parse_code_range(fields[0])

        if n_fields == 3:
            value = fields[2]
        elif info.type == "bool":
            value = True
        else:
            value = fields[1]

        for code in range(code_range[0], code_range[1] + 1):
            code_map[code] = value

##################################################

# Write an extracted property table to a C++ source file.

def write_property_table(prop):

    info = property_info_table[prop]
    is_binary = info.type == "bool"
    cpp_info = property_cpp_info_table[prop]
    cpp_path = os.path.join(source_root, cpp_info.file)
    code_map = property_tables[prop]
    print(f"Writing {cpp_info.file}...")

    if is_binary:
        cpp_map_type = "BinaryPropertyMap"
    else:
        cpp_map_type = f"GeneralPropertyMap<{cpp_info.type}>"

    with open(cpp_path, "w", newline="\n") as cpp:

        cpp.write(undent(f"""
            // This file is automatically generated
            // Do not edit

            #include "rs-unicode/property-list.hpp"
            """))

        for header in cpp_info.headers:
            cpp.write(f"#include {header}\n")

        cpp.write(undent(f"""

            namespace RS::Unicode {{

                {cpp_info.function} {{

                    static const {cpp_map_type} map {{

            """))

        if is_binary:

            start_code = None
            start_value = False

            for code in unicode_range:
                value = code_map[code]
                if start_value and not value:
                    cpp.write(f"            {{ {hex(start_code)}, {hex(code - 1)} }},\n")
                if value != start_value:
                    start_code = code
                    start_value = value

            if start_value:
                cpp.write(f"            {{ {hex(start_code)}, {hex(unicode_range[-1])} }},\n")

        else:

            prev_value = None

            for code in unicode_range:
                value = code_map[code]
                if code == 0 or value != prev_value:
                    value_str = ucd_value_to_cpp(value, prop, info.type)
                    cpp.write(f"            {{ {hex(code)}, {value_str} }},\n")
                    prev_value = value

        cpp.write(undent("""

                    };

                    return map[c];

                }

            }
            """))

##################################################

# Extract all required properties from the UCD files.

extract_properties_from_unicode_data()

for prop in sorted(property_info_table):
    if not property_info_table[prop].column:
        extract_property_from_ucd_file(prop)

# Generate the property tables as C++ files

for prop in property_info_table:
    write_property_table(prop)

# Compile lists of values for the enumeration properties. The default value is
# always first, the rest are in alphabetical order.

print("Extracting enumeration tables...")

for prop in property_info_table:

    info = property_info_table[prop]

    if info.type != "enum":
        continue

    value_set = set(property_tables[prop])
    value_set.discard(info.default)
    table = [info.default]
    table.extend(sorted(value_set))
    enum_tables[prop] = table

##################################################

# Generate the canonical composition mapping file.

output_file = f"ucd-canonical-composition-mapping.cpp"
output_path = os.path.join(source_root, output_file)
decomposition_map = property_tables["Canonical_Decomposition_Mapping"]
exclusion_list = property_tables["Full_Composition_Exclusion"]
composition_mapping_function = "char32_t canonical_composition_mapping(std::u32string_view str)"
print(f"Writing {output_file}...")

with open(output_path, "w", newline="\n") as cpp:

    cpp.write(undent(f"""
        // This file is automatically generated
        // Do not edit

        #include "rs-unicode/property-list.hpp"
        #include <string_view>
        #include <unordered_map>

        namespace RS::Unicode {{

            {composition_mapping_function} {{

                static const std::unordered_map<std::u32string_view, char32_t> map {{

        """))

    for code in unicode_range:
        value = decomposition_map[code]
        if value is not None and not exclusion_list[code]:
            chars = value.split()
            if len(chars) == 2:
                cpp_value = hex_codes_to_cpp(chars, "U")
                cpp.write(f"            {{ {cpp_value}, {hex(code)} }},\n")

    cpp.write(undent("""

                };

                auto it = map.find(str);

                if (it == map.end()) {
                    return {};
                } else {
                    return it->second;
                }

            }

        }
        """))

##################################################

# Generate the case folding file.

# Fields in "CaseFolding.txt":
#   [0] Code point
#   [1] Status
#   [2] Case folded

# We are only interested in status C or F. Skip any entries that are the same
# as Simple_Uppercase_Mapping.

input_file = f"CaseFolding.txt"
input_path = os.path.join(ucd_root, input_file)
output_file = f"ucd-case-folding.cpp"
output_path = os.path.join(source_root, output_file)
case_folding_function = "std::u32string_view case_folding(char32_t c)"
case_folding = {} # code point => list of code points
print(f"Extracting {input_file}...")

for line in open(input_path):
    left, mid, right = line.partition("#")
    left = left.strip()
    if not left:
        continue
    fields = [field.strip() for field in left.split(";")]
    if len(fields) < 3 or (fields[1] != "C" and fields[1] != "F"):
        continue
    code = int(fields[0], 16)
    value = fields[2]
    if value != property_tables["Simple_Lowercase_Mapping"][code]:
        case_folding[code] = value

print(f"Writing {output_file}...")

with open(output_path, "w", newline="\n") as cpp:

    cpp.write(undent(f"""
        // This file is automatically generated
        // Do not edit

        #include "rs-unicode/property-list.hpp"
        #include <string_view>
        #include <unordered_map>

        namespace RS::Unicode {{

            {case_folding_function} {{

                static const std::unordered_map<char32_t, std::u32string_view> map {{

        """))

    for code in sorted(case_folding):
        value = hex_codes_to_cpp(case_folding[code].split(), "U")
        cpp.write(f"            {{ {hex(code)}, {value} }},\n")

    cpp.write(undent("""

                };

                auto it = map.find(c);

                if (it == map.end()) {
                    return {};
                } else {
                    return it->second;
                }

            }

        }
        """))

##################################################

# Generate the special casing file.

# Fields in "SpecialCasing.txt":
#   [0] Code point
#   [1] Lower case
#   [2] Title case
#   [3] Upper case
#   [4] Conditions

input_file = f"SpecialCasing.txt"
input_path = os.path.join(ucd_root, input_file)
output_file = f"ucd-special-casing.cpp"
output_path = os.path.join(source_root, output_file)
special_casing_result = "std::array<std::u32string_view, 3>"
special_casing_function = f"const {special_casing_result}& special_casing(char32_t c)"

print(f"Extracting {input_file}...")
special_casing = {} # code point => 3 lists of code points

for line in open(input_path):
    left, mid, right = line.partition("#")
    left = left.strip()
    if not left:
        continue
    fields = [field.strip() for field in left.split(";")]
    if len(fields) >= 5 and fields[4]:
        continue
    code = int(fields[0], 16)
    special_casing[code] = fields[1:4]

print(f"Writing {output_file}...")

with open(output_path, "w", newline="\n") as cpp:

    cpp.write(undent(f"""
        // This file is automatically generated
        // Do not edit

        #include "rs-unicode/property-list.hpp"
        #include <array>
        #include <string_view>
        #include <unordered_map>

        namespace RS::Unicode {{

            {special_casing_function} {{

                static const std::unordered_map<char32_t, {special_casing_result}> map {{

        """))

    for code in sorted(special_casing):
        fields = special_casing[code]
        strings = [hex_codes_to_cpp(field.split(), "U") for field in fields]
        cpp.write(f"            {{ {hex(code)}, {{ {strings[0]}, {strings[1]}, {strings[2]} }} }},\n")

    cpp.write(undent(f"""

                }};

                static const {special_casing_result} empty;

                auto it = map.find(c);

                if (it == map.end()) {{
                    return empty;
                }} else {{
                    return it->second;
                }}

            }}

        }}
        """))

##################################################

# Generate the subscript and superscript properties file.

print(f"Extracting subscripts and superscripts...")
input_path = os.path.join(ucd_root, unicode_data)
output_file = f"ucd-subscript-superscript.cpp"
output_path = os.path.join(source_root, output_file)
sub_super_result = "std::pair<char32_t, char32_t>"
sub_super_function = f"{sub_super_result} subscript_superscript_base_table(char32_t c)"
sub_super_pattern = re.compile("<su(?:b|per)> ([0-9a-f]+)", re.IGNORECASE)
sub_super_base_chars = set() # base characters
subscript_table = {} # base character => subscript
superscript_table = {} # base character => superscript

for line in open(input_path):

    columns = line.rstrip().split(";")
    decomposition = columns[5]
    match = sub_super_pattern.fullmatch(decomposition)

    if not match:
        continue

    code = int(columns[0], 16)
    base_code = int(match[1], 16)
    is_subscript = decomposition[3] == "b"
    sub_super_base_chars.add(base_code)

    if is_subscript:
        subscript_table[base_code] = code
    else:
        superscript_table[base_code] = code

print(f"Writing {output_file}...")

with open(output_path, "w", newline="\n") as cpp:

    cpp.write(undent(f"""
        // This file is automatically generated
        // Do not edit

        #include "rs-unicode/property-list.hpp"
        #include <unordered_map>
        #include <utility>

        namespace RS::Unicode {{

            {sub_super_function} {{

                static const std::unordered_map<char32_t, {sub_super_result}> map {{

        """))

    for code in sorted(sub_super_base_chars):
        if code in subscript_table:
            sub = subscript_table[code]
        else:
            sub = 0
        if code in superscript_table:
            super = superscript_table[code]
        else:
            super = 0
        cpp.write(f"            {{ {hex(code)}, {{ {hex(sub)}, {hex(super)} }} }},\n")

    cpp.write(undent(f"""

                }};

                auto it = map.find(c);

                if (it == map.end()) {{
                    return {{0, 0}};
                }} else {{
                    return it->second;
                }}

            }}

        }}
        """))

##################################################

# Write interfaces for all generated property tables to a C++ header file.

print(f"Writing {os.path.basename(property_list_file)}...")

with open(property_list_file, "w", newline="\n") as hpp:

    hpp.write(undent("""
        // This file is automatically generated
        // Do not edit
        // Do not include this file directly
        // Include "rs-unicode/character.hpp" instead

        #include "rs-unicode/enum.hpp"
        #include "rs-unicode/property-map.hpp"
        #include <array>
        #include <string_view>
        #include <utility>

        namespace RS::Unicode {

        """))

    for prop in sorted(enum_tables):
        values = enum_tables[prop]
        hpp.write(f"    RS_UNICODE_ENUM({prop}, unsigned char,\n")
        for value in values:
            hpp.write(f"        {value},\n")
        hpp.write(f"    )\n\n")

    for prop in sorted(property_cpp_info_table):
        info = property_cpp_info_table[prop]
        if info.type != "bool":
            hpp.write(f"    {info.function};\n")

    hpp.write("\n")

    for prop in sorted(property_cpp_info_table):
        info = property_cpp_info_table[prop]
        if info.type == "bool":
            hpp.write(f"    {info.function};\n")

    hpp.write(undent(f"""

            {composition_mapping_function};
            {case_folding_function};
            {special_casing_function};
            {sub_super_function};
        """))

    gc_values = enum_tables["General_Category"]
    primary_letters = gc_values[0][0]

    for gc in sorted(gc_values[1:]):
        primary_letters += gc[0]

    hpp.write(undent(f"""

            inline char primary_category(char32_t c) {{
                auto index = static_cast<unsigned>(general_category(c));
                return "{primary_letters}"[index];
            }}

        }}
        """))

##################################################

# Generate the grapheme break test file.

input_file = f"auxiliary/GraphemeBreakTest.txt"
input_path = os.path.join(ucd_root, input_file)
output_file = f"ucd-grapheme-break-test.cpp"
output_path = os.path.join(test_root, output_file)

print(f"Extracting {input_file}...")
test_cases = [] # Each test case is a list of lists of hex code points

for line in open(input_path):
    left, mid, right = line.partition("#")
    left = left.strip()
    if not left:
        continue
    test_case = []
    for group in left.strip("÷").split("÷"):
        test_case.append([x.strip() for x in group.strip().split("×")])
    test_cases.append(test_case)

print(f"Writing {output_file}...")

with open(output_path, "w", newline="\n") as cpp:

    cpp.write(undent("""
        // This file is automatically generated
        // Do not edit

        #include "rs-unicode/string.hpp"
        #include "test/unit-test.hpp"
        #include <cstddef>
        #include <string>
        #include <vector>

        using namespace RS::Unicode;

        namespace {

            void run_test(const std::string& test_case, std::size_t index) {

                auto prefix = std::to_string(index) + ':';
                auto expect = prefix + test_case;
                std::string str;

                for (auto c: test_case) {
                    if (c != ';') {
                        str += c;
                    }
                }

                std::string result = prefix;

                for (auto g: grapheme_view(str)) {
                    result += g;
                    result += ';';
                }

                TEST_EQUAL(result, expect);

            }

            const auto& test_cases() {

                static const std::vector<std::string> list = {

        """));

    for test_case in test_cases:
        groups_cpp = [hex_codes_to_cpp(group) for group in test_case]
        expect_cpp = ";".join(groups_cpp)
        expect_cpp = f'"{expect_cpp};"'
        cpp.write(f"            {expect_cpp},\n")

    cpp.write(undent("""

                };

                return list;

            }

        }

        void test_rs_unicode_string_grapheme_breaks() {

            for (auto i = 0uz; i < test_cases().size(); ++i) {
                run_test(test_cases()[i], i);
            }

        }
        """))

##################################################

# Generate the normalization test file.

input_file = f"NormalizationTest.txt"
input_path = os.path.join(ucd_root, input_file)
output_file = f"ucd-normalization-test.cpp"
output_path = os.path.join(test_root, output_file)

print(f"Extracting {input_file}...")
test_cases = [] # Each test case is 3 lists of hex code points

for line in open(input_path):
    left, mid, right = line.partition("#")
    left = left.strip()
    if not left:
        continue
    test_case = [field.strip() for field in left.split(";")]
    if len(test_case) >= 3:
        test_cases.append(test_case[:3])

print(f"Writing {output_file}...")

with open(output_path, "w", newline="\n") as cpp:

    cpp.write(undent("""
        // This file is automatically generated
        // Do not edit

        #include "rs-unicode/string.hpp"
        #include "test/unit-test.hpp"
        #include <array>
        #include <cstddef>
        #include <string>
        #include <string_view>
        #include <vector>

        using namespace RS::Unicode;

        namespace {

            using TestCase = std::array<std::string_view, 3>;

            void run_test(const TestCase& test_case, std::size_t index) {

                auto prefix = std::to_string(index) + ':';
                auto expect_nfc = prefix;
                auto expect_nfd = prefix;
                expect_nfc += test_case[1];
                expect_nfd += test_case[2];
                auto test_nfc = prefix + to_nfc(test_case[0]);
                auto test_nfd = prefix + to_nfd(test_case[0]);

                TEST_EQUAL(test_nfc, expect_nfc);
                TEST_EQUAL(test_nfd, expect_nfd);

            }

            const auto& test_cases() {

                static const std::vector<TestCase> list = {

        """));

    for test_case in test_cases:
        strings = [hex_codes_to_cpp(group.split(), "Q") for group in test_case]
        cpp.write(f"            {{ {strings[0]}, {strings[1]}, {strings[2]} }},\n")

    cpp.write(undent("""

                };

                return list;

            }

        }

        void test_rs_unicode_string_normalization() {

            for (auto i = 0uz; i < test_cases().size(); ++i) {
                run_test(test_cases()[i], i);
            }

        }
        """))

##################################################

# Get the UCD version number.

# # PropList-16.0.0.txt

# inline std::array<int, 3> unicode_version() noexcept {
#     return { 0, 1, 12 };
# }

input_file = f"PropList.txt"
input_path = os.path.join(ucd_root, input_file)
output_file = f"version.hpp"
output_path = os.path.join(source_root, output_file)

with open(input_path, "r") as source:
    line = source.readline()

match = re.match(r"# [A-Za-z]+-(\d+)\.(\d+)\.(\d+)\.txt", line)

if not match:
    raise ValueError(f"Unexpected format for first line: {repr(line)}")

x = int(match[1])
y = int(match[2])
z = int(match[3])

with open(output_path, "r") as header:
    body = header.read()

match = re.search(r"std::array<int, 3> unicode_version\(\)", body)

if not match:
    raise ValueError(f"Can't parse version header: unicode_version() function not found")

pos = match.end()
pattern = re.compile(r"return *\{ *\d+, *\d+, *\d+ *\};")
match = pattern.search(body, pos)

if not match:
    raise ValueError(f"Can't parse version header: unicode_version() body not found")

new_version = f"return {{ {x}, {y}, {z} }};"
body = body[:match.start()] + new_version + body[match.end():]

with open(output_path, "w") as header:
    header.write(body)
