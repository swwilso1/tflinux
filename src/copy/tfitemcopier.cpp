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

#include <sys/sendfile.h>
#include "tfitemcopier.hpp"

namespace TF::Linux
{

    ItemCopier::ItemCopier(const string_type & source, const string_type & destination) :
        m_source_item{source}, m_destination_item{destination}
    {
        FileManager fm{};
        auto properties = fm.propertiesForItemAtPath(source);
        m_file_size_in_bytes = properties.size;
    }

    void ItemCopier::copy(interrupter_type interrupter)
    {
        auto read_handle = FileHandle::fileHandleForReadingAtPath(m_source_item, true);
        auto write_handle = FileHandle::fileHandleForWritingAtPath(m_destination_item, true);
        auto read_descriptor = read_handle.fileDescriptor();
        auto write_descriptor = write_handle.fileDescriptor();
        auto bytes_still_to_transfer{m_file_size_in_bytes};
        bool finished{false};
        bool interruped{false};
        off_t offset{0};

        while (! finished)
        {
            auto bytes_to_transfer = bytes_still_to_transfer >= m_block_size ? m_block_size : bytes_still_to_transfer;
            auto bytes_sent = sendfile(write_descriptor, read_descriptor, &offset, bytes_to_transfer);

            if (bytes_sent == 0)
            {
                finished = true;
                continue;
            }

            if (bytes_sent < 0)
            {
                throw std::system_error{errno, std::system_category(), "Error transferring data with sendfile"};
            }

            auto typed_bytes_sent = static_cast<decltype(bytes_still_to_transfer)>(bytes_sent);

            m_notifier.notify(typed_bytes_sent);
            bytes_still_to_transfer -= typed_bytes_sent;
            finished = bytes_still_to_transfer == 0;

            if (! finished && interrupter)
            {
                finished = interruped = interrupter();
            }
        }

        if (! interruped)
        {
            write_handle.flush();
        }
    }
} // namespace TF::Linux
