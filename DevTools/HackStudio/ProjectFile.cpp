#include "ProjectFile.h"
#include <AK/FileSystemPath.h>
#include <LibCore/CFile.h>
#include <string.h>

const GTextDocument& ProjectFile::document() const
{
    if (!m_document) {
        m_document = GTextDocument::create(nullptr);
        auto file = CFile::construct(m_path);
        if (!file->open(CFile::ReadOnly)) {
            ASSERT_NOT_REACHED();
        }
        m_document->set_text(file->read_all());
    }
    return *m_document;
}
