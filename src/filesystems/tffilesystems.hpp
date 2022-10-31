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

#ifndef TFFILESYSTEMS_HPP
#define TFFILESYSTEMS_HPP

#include "TFFoundation.hpp"

using namespace TF::Foundation;

namespace TF::Linux
{

    enum class FileSystem
    {
        EXT2,
        EXT3,
        EXT4,
        FAT16,
        FAT32,
        NTFS,
        HFSPLUS,
        APFS,
        CIFS,

        UNKNOWN
    };

    auto operator<<(std::ostream & o, const FileSystem & fs) -> std::ostream &;

    auto stringToFileSystemType(const TF::Foundation::String & s) -> FileSystem;

    auto fileSystemTypeToString(FileSystem t) -> String;

    auto fileSystemTypeIsMountable(FileSystem t) -> bool;

    template<typename INTEGER, typename = std::enable_if<std::is_integral<INTEGER>::value>>
    auto valueIsFileSystemType(INTEGER i) -> bool
    {
        if (i > 0 && i < static_cast<int>(FileSystem::UNKNOWN))
        {
            return true;
        }
        return false;
    }

    auto fileSystemTypeIsFormattable(FileSystem t) -> bool;

} // namespace TF::Linux

#endif // TFFILESYSTEMS_HPP
