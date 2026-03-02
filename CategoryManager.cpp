#include "CategoryManager.h"
#include <QHeaderView>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>

CategoryManager::CategoryManager(QWidget *parent)
    : QTreeWidget(parent), m_fileSystemModel(new QFileSystemModel(this))
{
    setHeaderHidden(true);
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    setSelectionMode(QAbstractItemView::SingleSelection);

    connect(this, &QTreeWidget::itemClicked,
            this, &CategoryManager::onItemClicked);
}

void CategoryManager::setupFileSystem(const QString &rootPath, bool showSubFoldersOnly) {
    m_rootPath = rootPath;
    m_fileSystemModel->setRootPath(rootPath);
    m_fileSystemModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);

    // 找到当前根路径的顶级节点
    QTreeWidgetItem *rootItem = nullptr;
    for (int i = 0; i < topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = topLevelItem(i);
        if (item->data(0, Qt::UserRole).toString() == rootPath) {
            rootItem = item;
            break;
        }
    }

    // 如果没有找到，创建一个新的顶级节点
    if (!rootItem) {
        rootItem = new QTreeWidgetItem(this);
        rootItem->setText(0, QFileInfo(rootPath).fileName());
        rootItem->setData(0, Qt::UserRole, rootPath);
    }

    // 清空当前根路径的子节点
    while (rootItem->childCount() > 0) {
        delete rootItem->takeChild(0);
    }

    // 重新加载当前根路径的内容
    populateTree(rootPath, rootItem, showSubFoldersOnly);
}

void CategoryManager::populateTree(const QString &path, QTreeWidgetItem *parentItem, bool showSubFoldersOnly) {
    QFileInfoList entries = QDir(path).entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    for (const QFileInfo &entry : entries) {
        QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
        item->setText(0, entry.fileName());
        item->setData(0, Qt::UserRole, entry.absoluteFilePath());
        if (entry.isDir() && !showSubFoldersOnly) {
            populateTree(entry.absoluteFilePath(), item, showSubFoldersOnly);
        }
    }
}

QString CategoryManager::currentNotePath() const {
    QTreeWidgetItem *item = currentItem();
    return item ? item->data(0, Qt::UserRole).toString() : QString();
}

void CategoryManager::onItemClicked(QTreeWidgetItem *item) {
    if (!item) return;
    QString path = item->data(0, Qt::UserRole).toString();
    if (QFileInfo(path).isFile()) {
        emit noteSelected(path);
        if (!m_history.contains(path)) {
            m_history.append(path);
            emit historyUpdated(m_history);
        }
    } else {
        emit folderSelected(path);
    }
}