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

#ifndef TFKEYVALUECONFIGFILE_HPP
#define TFKEYVALUECONFIGFILE_HPP

#include <unordered_map>
#include "TFFoundation.hpp"

using namespace TF::Foundation;

namespace TF::Linux
{

    /**
     * KeyValueConfigFile represents a basic configuration file that uses key/value pairs separated by an '='
     * character.
     */
    class KeyValueConfigFile
    {
    public:
        using string_type = String;
        using key_type = String;
        using value_type = String;
        using key_value_table_type = std::unordered_map<key_type, value_type>;

        /**
         * @brief constructor with the name of the configuration file.
         * @param config_file the file name.
         */
        explicit KeyValueConfigFile(const string_type & config_file);

        /**
         * @brief method to load the key/value pairs in the file.
         * @return the table of key/value pairs.
         */
        [[nodiscard]] auto load_configuration() const -> key_value_table_type;

        /**
         * @brief method to write a table of key/value pairs to the configuration file.
         * @param table the table of pairs.
         */
        void save_configuration(const key_value_table_type & table);

        /**
         * @brief method to check if the configuration file exists.
         * @return true if the file exists.
         */
        [[nodiscard]] auto file_exists() -> bool;

    private:
        string_type m_config_file{};
    };

} // namespace TF::Linux

#endif // TFKEYVALUECONFIGFILE_HPP
