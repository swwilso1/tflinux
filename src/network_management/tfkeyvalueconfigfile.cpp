/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2023 to 2023 Tectiform Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


******************************************************************************/

#include "tfkeyvalueconfigfile.hpp"

namespace TF::Linux
{

    KeyValueConfigFile::KeyValueConfigFile(const string_type & config_file) : m_config_file{config_file} {}

    auto KeyValueConfigFile::load_configuration() const -> key_value_table_type
    {
        key_value_table_type table{};
        auto read_handle = FileHandle::fileHandleForReadingAtPath(m_config_file);
        while (! read_handle.atEndOfFile())
        {
            auto line = read_handle.readLine();

            if (line.empty())
            {
                continue;
            }

            // Skip over lines that start with a comment marker (#).
            if (line.first() == '#')
            {
                continue;
            }

            // If the line ends with a newline character, crop it off.
            if (line.last() == '\n')
            {
                line = line.substringToIndex(line.length() - 1);
            }

            // now split the line on the '=' character.
            auto line_components = line.split("=");
            if (line_components.size() != 2)
            {
                // Discard any lines that do not have an '=' character.
                continue;
            }

            table.insert(std::make_pair(line_components[0], line_components[1]));
        }
        return table;
    }

    void KeyValueConfigFile::save_configuration(const key_value_table_type & table)
    {
        if (table.size() > 0)
        {
            auto fh = FileHandle::fileHandleForWritingAtPath(m_config_file, true);
            std::for_each(table.cbegin(), table.cend(), [&fh](const std::pair<key_type, value_type> & pair) -> void {
                string_type new_line{"# Ignore this line\n"};
                if ((! pair.first.empty()) && (! pair.second.empty()))
                {
                    new_line = pair.first + "=" + pair.second + "\n";
                }
                else if ((! pair.first.empty()) && pair.second.empty())
                {
                    new_line = pair.first + "\n";
                }
                fh.writeData(new_line.getAsData());
            });
        }
    }

    auto KeyValueConfigFile::file_exists() -> bool
    {
        FileManager manager{};
        return manager.fileExistsAtPath(m_config_file);
    }

} // namespace TF::Linux
