#include "TemplateListModel.h"
#include <AK/FileSystemPath.h>
#include <AK/StringBuilder.h>
#include <LibCore/CConfigFile.h>
#include <LibCore/CDirIterator.h>
#include <LibDraw/GraphicsBitmap.h>
#include <LibGUI/GPainter.h>
#include <LibThread/BackgroundAction.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <unistd.h>

static RefPtr<GraphicsBitmap> render_icon(const StringView& path)
{
    auto bitmap = GraphicsBitmap::load_from_file(path);
    if (!bitmap)
        return nullptr;
    auto icon = GraphicsBitmap::create(bitmap->format(), { 32, 32 });
    Painter painter(*icon);
    painter.draw_scaled_bitmap(icon->rect(), *bitmap, bitmap->rect());
    return icon;
}

NonnullRefPtr<TemplateListModel> TemplateListModel::create(const StringView& path)
{
    auto model = adopt(*new TemplateListModel(path));
    model->update();
    return model;
}

TemplateListModel::TemplateListModel(const StringView& path) : m_path(path)
{
}

TemplateListModel::~TemplateListModel()
{
}

int TemplateListModel::row_count(const GModelIndex&) const
{
    return m_entries.size();
}

int TemplateListModel::column_count(const GModelIndex&) const
{
    return Column::__Count;
}

String TemplateListModel::column_name(int column) const
{
    switch (column) {
    case Column::Filename:
        return "Filename";
    case Column::Icon:
        return "";
    case Column::Name:
        return "Name";
    case Column::Description:
        return "Description";
    }
    ASSERT_NOT_REACHED();
}

GModel::ColumnMetadata TemplateListModel::column_metadata(int column) const
{
    switch (column) {
    case Column::Filename:
        return { 120, TextAlignment::CenterLeft };
    case Column::Icon:
        return { 16, TextAlignment::Center, nullptr, GModel::ColumnMetadata::Sortable::False };
    case Column::Name:
        return { 120, TextAlignment::CenterLeft };
    case Column::Description:
        return { 300, TextAlignment::CenterLeft };
    }
    ASSERT_NOT_REACHED();
}

GVariant TemplateListModel::data(const GModelIndex& index, Role role) const
{
    ASSERT(is_valid(index));
    auto& entry = this->entry(index.row());
    if (role == Role::Custom) {
        ASSERT(index.column() == Column::Name);
        return entry.full_path(*this);
    }
    if (role == Role::DragData) {
        if (index.column() == Column::Name) {
            StringBuilder builder;
            builder.append("file://");
            builder.append(entry.full_path(*this));
            return builder.to_string();
        }
        return {};
    }
    if (role == Role::Sort) {
        switch (index.column()) {
        case Column::Filename:
            return entry.filename;
        case Column::Icon:
            return 0;
        case Column::Name:
            return entry.name;
        case Column::Description:
            return entry.description;
        }
        ASSERT_NOT_REACHED();
    }
    if (role == Role::Display) {
        switch (index.column()) {
        case Column::Filename:
            return entry.filename;
        case Column::Icon:
            return entry.icon;
        case Column::Name:
            return entry.name;
        case Column::Description:
            return entry.description;
        }
    }
    if (role == Role::Icon) {
        return entry.icon;
    }
    return {};
}

void TemplateListModel::update()
{
    CDirIterator di(m_path, CDirIterator::SkipDots);
    if (di.has_error()) {
        fprintf(stderr, "CDirIterator: %s\n", di.error_string());
        exit(1);
    }

    m_entries.clear();

    while (di.has_next()) {
        String filename = di.next_path();

        Entry entry;
        entry.filename = String::format("%s/%s/template.ini", m_path.characters(), filename.characters());

        struct stat st;
        int rc = stat(entry.filename.characters(), &st);
        if (rc < 0) {
            perror("lstat");
            continue;
        }
        if (!S_ISREG(st.st_mode)) {
            continue;
        }

        auto ini = CConfigFile::open(entry.filename);
        if (!ini->has_group("HackStudioTemplate")) {
            continue;
        }

        entry.icon = GIcon::default_icon("filetype-unknown");
        entry.name = ini->read_entry("HackStudioTemplate", "Name", "Untitled Template");
        entry.description = ini->read_entry("HackStudioTemplate", "Description", "No description available.");

        auto icon_path = ini->read_entry("HackStudioTemplate", "Icon", "");
        if (!icon_path.is_empty()) {
            auto bitmap = render_icon(icon_path);
            if (bitmap) {
                entry.icon = GIcon(move(bitmap));
            }
        }

        m_entries.append(move(entry));
    }

    did_update();
}
