#pragma once

#include <AK/HashMap.h>
#include <LibCore/CNotifier.h>
#include <LibGUI/GModel.h>
#include <sys/stat.h>
#include <time.h>

class TemplateListModel final : public GModel
    , public Weakable<TemplateListModel> {
public:
    static NonnullRefPtr<TemplateListModel> create(const StringView& path);
    virtual ~TemplateListModel() override;

    enum Column {
        Filename = 0,
        Icon,
        Name,
        Description,
        __Count,
    };

    virtual int row_count(const GModelIndex& = GModelIndex()) const override;
    virtual int column_count(const GModelIndex& = GModelIndex()) const override;
    virtual String column_name(int column) const override;
    virtual ColumnMetadata column_metadata(int column) const override;
    virtual GVariant data(const GModelIndex&, Role = Role::Display) const override;
    virtual void update() override;

    String path() const { return m_path; }

    struct Entry {
        String filename;
        GIcon icon;
        String name;
        String description;
        String full_path(const TemplateListModel& model) const { return String::format("%s/%s", model.path().characters(), filename.characters()); }
    };

    const Entry& entry(int index) const
    {
        return m_entries[index];
    }

private:
    TemplateListModel(const StringView& path);

    String m_path;
    Vector<Entry> m_entries;
};
