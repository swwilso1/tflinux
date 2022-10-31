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

#ifndef TFMOUNTER_HPP
#define TFMOUNTER_HPP

#include <optional>
#include "TFFoundation.hpp"
#include "tfmounttable.hpp"
#include "tffilesystems.hpp"

using namespace TF::Foundation;

namespace TF::Linux
{

    class Mounter
    {
    public:
        using string_type = String;
        using file_system_type = FileSystem;

        Mounter() = default;

        static auto mount(const string_type & src, const string_type & dst, file_system_type fs_type, uint32_t flags,
                          const string_type & options) -> bool;

        static auto mount(const string_type & src, file_system_type fs_type, uint32_t flags,
                          const string_type & options) -> std::optional<string_type>;

        static auto mount_in_dir(const string_type & src, const string_type & dir, file_system_type fs_type,
                                 uint32_t flags, const string_type & options) -> std::optional<string_type>;

        static auto unmount(const string_type & target) -> bool;

        static auto unmount(const string_type & target, int flags) -> bool;
    };

} // namespace TF::Linux

#endif // TFMOUNTER_HPP