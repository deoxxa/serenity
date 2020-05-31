/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <AK/ByteBuffer.h>
#include <AK/HexDump.h>
#include <ctype.h>

namespace AK {

void hex_dump(const ByteBuffer& buffer)
{
    hex_dump(buffer.data(), buffer.size());
}

void hex_dump(const u8* buffer, size_t size)
{
    char chars[17] { 0 };

    for (size_t i { 0 }; i < size; ++i) {
        dbgprintf("%02X ", buffer[i]);

        if (isprint(buffer[i]))
            chars[i % 16] = buffer[i];
        else
            chars[i % 16] = '.';

        if ((i + 1) % 8 == 0 || i + 1 == size) {
            dbgprintf(" ");

            if ((i + 1) % 16 == 0) {
                dbgprintf("|  %s \n", chars);
            } else if (i + 1 == size) {
                chars[(i + 1) % 16] = '\0';

                if ((i + 1) % 16 <= 8)
                    dbgprintf(" ");

                for (size_t j = (i + 1) % 16; j < 16; ++j)
                    dbgprintf("   ");

                dbgprintf("|  %s \n", chars);
            }
        }
    }
}

}