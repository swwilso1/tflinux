/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2022 to 2022 Tectiform Inc.

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

#ifndef TFITEMCOPIER_HPP
#define TFITEMCOPIER_HPP

#include "TFFoundation.hpp"

using namespace TF::Foundation;

namespace TF::Linux
{

    /**
     * Class to copy a file to another location.
     */
    class ItemCopier
    {
    public:
        using size_type = FileProperties::size_type;

        using string_type = String;

        using notifier_type = BasicProgressNotifier<size_type>;

        using interrupter_type = std::function<bool()>;

        /**
         * @brief constructor with source item and destination item
         * @param source a path to a file to copy
         * @param destination a path to the destination that will have a copy of @e src.
         */
        ItemCopier(const string_type & source, const string_type & destination);

        /**
         * @brief method to set the block size used for copying chunks of data from the source
         * to the destination.
         * @param size
         */
        void set_block_size(size_type size)
        {
            m_block_size = size;
        }

        /**
         * @brief method to set the notifier object that will be periodically updated with the progress of the copy.
         * @param notifier the notifier object.
         */
        void set_notifier(const notifier_type & notifier)
        {
            m_notifier = notifier;
        }

        /**
         * @brief method to get the size of the data that needs copied from the source.
         * @return the number of bytes needed to copy.
         */
        [[nodiscard]] size_type get_file_size() const
        {
            return m_file_size_in_bytes;
        }

        /**
         * @brief method to perform the copy of the source to the destination.
         * @param interrupter a callback function that the copier calls periodically to check if the copy should abort
         * early. The @e interrupter function should return true if the copier should abort early.
         */
        void copy(interrupter_type interrupter = nullptr);

    private:
        string_type m_source_item{};
        string_type m_destination_item{};
        size_type m_block_size{1048576};
        size_type m_file_size_in_bytes{0};
        notifier_type m_notifier{};
    };

} // namespace TF::Linux

#endif // TFITEMCOPIER_HPP
