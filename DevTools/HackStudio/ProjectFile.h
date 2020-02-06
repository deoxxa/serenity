#pragma once

#include <AK/ByteBuffer.h>
#include <AK/NonnullRefPtr.h>
#include <AK/RefCounted.h>
#include <AK/String.h>
#include <LibGUI/GTextDocument.h>

class ProjectFile : public RefCounted<ProjectFile> {
public:
    static NonnullRefPtr<ProjectFile> construct_with_name_and_path(const String& name, const String& path)
    {
        return adopt(*new ProjectFile(name, path));
    }

    const String& name() const { return m_name; }
    const String& path() const { return m_path; }

    const GTextDocument& document() const;

private:
    explicit ProjectFile(const String& name, const String& path)
        : m_name(name)
        , m_path(path)
    {
    }

    String m_name;
    String m_path;
    mutable RefPtr<GTextDocument> m_document;
};
