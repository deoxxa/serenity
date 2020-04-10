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

#include <Kernel/Devices/DiskPartition.h>
#include <Kernel/FileSystem/FileDescription.h>

// #define OFFD_DEBUG

namespace Kernel {

NonnullRefPtr<DiskPartition> DiskPartition::create(BlockDevice& device, unsigned block_offset, unsigned block_limit)
{
    return adopt(*new DiskPartition(device, block_offset, block_limit));
}

DiskPartition::DiskPartition(BlockDevice& device, unsigned block_offset, unsigned block_limit)
    : BlockDevice(100, 0, device.block_size())
    , m_device(device)
    , m_block_offset(block_offset)
    , m_block_limit(block_limit)
{
}

DiskPartition::~DiskPartition()
{
}

bool DiskPartition::read_blocks(unsigned index, u16 count, u8* out)
{
#ifdef OFFD_DEBUG
    klog() << "DiskPartition::read_blocks " << index << " (really: " << (m_block_offset + index) << ") count=" << count;
#endif

    return m_device->read_blocks(m_block_offset + index, count, out);
}

bool DiskPartition::write_blocks(unsigned index, u16 count, const u8* data)
{
#ifdef OFFD_DEBUG
    klog() << "DiskPartition::write_blocks " << index << " (really: " << (m_block_offset + index) << ") count=" << count;
#endif

    return m_device->write_blocks(m_block_offset + index, count, data);
}

ssize_t DiskPartition::read(FileDescription& fd, u8* outbuf, ssize_t len)
{
    unsigned adjust = m_block_offset * block_size();

#ifdef OFFD_DEBUG
    klog() << "DiskPartition::read offset=" << fd.offset() << " adjust=" << adjust << " len=" << len;
#endif

    fd.seek(adjust, SEEK_CUR);
    auto nread = m_device->read(fd, outbuf, len);
    fd.seek(0 - adjust, SEEK_CUR);
    return nread;
}

bool DiskPartition::can_read(const FileDescription& fd) const
{
    unsigned adjust = m_block_offset * block_size();

#ifdef OFFD_DEBUG
    klog() << "DiskPartition::can_read offset=" << fd.offset() << " adjust=" << adjust;
#endif

    const_cast<FileDescription&>(fd).seek(adjust, SEEK_CUR);
    auto result = m_device->can_read(fd);
    const_cast<FileDescription&>(fd).seek(0 - adjust, SEEK_CUR);
    return result;
}

ssize_t DiskPartition::write(FileDescription& fd, const u8* inbuf, ssize_t len)
{
    unsigned adjust = m_block_offset * block_size();

#ifdef OFFD_DEBUG
    klog() << "DiskPartition::write offset=" << fd.offset() << " adjust=" << adjust << " len=" << len;
#endif

    fd.seek(adjust, SEEK_CUR);
    auto nwritten = m_device->write(fd, inbuf, len);
    fd.seek(0 - adjust, SEEK_CUR);
    return nwritten;
}

bool DiskPartition::can_write(const FileDescription& fd) const
{
    unsigned adjust = m_block_offset * block_size();

#ifdef OFFD_DEBUG
    klog() << "DiskPartition::can_write offset=" << fd.offset() << " adjust=" << adjust;
#endif

    const_cast<FileDescription&>(fd).seek(adjust, SEEK_CUR);
    auto result = m_device->can_write(fd);
    const_cast<FileDescription&>(fd).seek(0 - adjust, SEEK_CUR);
    return result;
}

const char* DiskPartition::class_name() const
{
    return "DiskPartition";
}

}
