#ifndef CATEGORY_MANAGER_H
#define CATEGORY_MANAGER_H

#include <QTreeWidget>
#include <QFileSystemModel>
#include <QMap>
#include <QStringList>

class CategoryManager : public QTreeWidget {
    Q_OBJECT

public:
    explicit CategoryManager(QWidget *parent = nullptr);
    void setupFileSystem(const QString &rootPath, bool showSubFoldersOnly = false);
    QString currentNotePath() const;

signals:
    void noteSelected(const QString &filePath);
    void folderSelected(const QString &folderPath);
    void historyUpdated(const QStringList &history);

private slots:
    void onItemClicked(QTreeWidgetItem *item);

private:
    QFileSystemModel *m_fileSystemModel;
    QString m_rootPath;
    QStringList m_history;

    void populateTree(const QString &path, QTreeWidgetItem *parentItem, bool showSubFoldersOnly);
};

#endif // CATEGORY_MANAGER_H