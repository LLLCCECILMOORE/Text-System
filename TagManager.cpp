#include "TagManager.h"
#include <QInputDialog>
#include <QMessageBox>

TagManager::TagManager(QWidget *parent)
    : QListWidget(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    connect(this, &QListWidget::itemClicked, this, &TagManager::onItemClicked);
}

void TagManager::addTagToNote(const QString &noteId, const QString &tag) {
    if (!m_noteTags[noteId].contains(tag)) {
        m_noteTags[noteId].append(tag);
        m_tagNotes[tag].append(noteId);
        
        if (findItems(tag, Qt::MatchExactly).isEmpty()) {
            addItem(tag);
        }
    }
}


void TagManager::removeTagFromNote(const QString &noteId, const QString &tag) {
    m_noteTags[noteId].removeAll(tag);
    m_tagNotes[tag].removeAll(noteId);
    
    if (m_tagNotes[tag].isEmpty()) {
        delete takeItem(row(findItems(tag, Qt::MatchExactly).first()));
    }
}

// TagManager.cpp
void TagManager::editTag(const QString &oldTag, const QString &newTag) {
    if (oldTag == newTag || !m_tagNotes.contains(oldTag)) return;
    
    // 更新笔记-标签映射
    QStringList noteIds = m_tagNotes.take(oldTag);
    for (const QString &noteId : noteIds) {
        m_noteTags[noteId].removeAll(oldTag);
        m_noteTags[noteId].append(newTag);
    }
    
    // 更新标签-笔记映射
    m_tagNotes.insert(newTag, noteIds);
    
    // 更新列表显示
    int row = currentRow();
    QListWidgetItem *item = takeItem(row);
    delete item;
    addItem(newTag);
    setCurrentRow(row);
}

void TagManager::removeTag(const QString &tag) {
    if (!m_tagNotes.contains(tag)) return;
    
    // 从所有笔记中移除标签
    const QStringList noteIds = m_tagNotes.take(tag);
    for (const QString &noteId : noteIds) {
        m_noteTags[noteId].removeAll(tag);
    }
    
    // 删除列表项
    QList<QListWidgetItem*> items = findItems(tag, Qt::MatchExactly);
    foreach (QListWidgetItem *item, items) {
        delete takeItem(row(item));
    }
}

QStringList TagManager::getTagsForNote(const QString &noteId) const {
    return m_noteTags.value(noteId);
}

QStringList TagManager::getAllTags() const {
    return m_tagNotes.keys();
}

QStringList TagManager::findNotesByTag(const QString &tag) const {
    return m_tagNotes.value(tag);
}

void TagManager::onItemClicked(QListWidgetItem *item) {
    emit tagSelected(item->text());
}