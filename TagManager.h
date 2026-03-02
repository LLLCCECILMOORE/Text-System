#pragma once
#include <QListWidget>
#include <QStringList>
#include <QMap>

class TagManager : public QListWidget {
    Q_OBJECT
public:
    explicit TagManager(QWidget *parent = nullptr);
    
    void addTagToNote(const QString &noteId, const QString &tag);
    void removeTagFromNote(const QString &noteId, const QString &tag);
    void editTag(const QString &oldTag, const QString &newTag); // 修改标签
    void removeTag(const QString &tag); // 删除标签（从所有笔记中移除）
    QStringList getTagsForNote(const QString &noteId) const;
    QStringList getAllTags() const;
    QStringList findNotesByTag(const QString &tag) const;
    
signals:
    void tagSelected(const QString &tag);
    
private slots:
    void onItemClicked(QListWidgetItem *item);
    
private:
    QMap<QString, QStringList> m_noteTags; // noteId -> tags
    QMap<QString, QStringList> m_tagNotes; // tag -> noteIds
};