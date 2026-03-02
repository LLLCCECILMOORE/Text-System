#ifndef COMPUTER_SOFT_MAINWINDOW_H
#define COMPUTER_SOFT_MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QListWidget>
#include <QToolBar>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QSplitter>
#include <QVBoxLayout>
#include <QFontComboBox>
#include <QComboBox>
#include <QToolButton>
#include <QLabel>
#include "NoteEditor.h"
#include "CategoryManager.h"
#include "TagManager.h" 

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addTagToItem(const QString& itemId, const QString& tag);
    void removeTagFromItem(const QString& itemId, const QString& tag);
    QStringList getTagsForItem(const QString& itemId) const;

private slots:
    void newNote();
    void newFolder();
    void openNote();
    void saveNote();
    void saveAsNote();
    void loadNoteFromList(QListWidgetItem *item);
    void deleteNote();
    void updateCharCount();
    void onFolderSelected(const QString &folderPath);
    void onNoteSelected(const QString &filePath);
    void updateHistoryList(const QStringList &history);
    void onHistorySelected(QListWidgetItem *item);
    void openFolder();
    void addFolderToHistory(const QString &filePath);
    void showAddTagDialog();
    void showEditTagDialog();
    void removeSelectedTag();
    void filterNotesByTag(const QString &tag = ""); // 可重载或默认参数
    void updateItemTagDisplay(QListWidget *listWidget, QListWidgetItem *item); // 更新笔记标签显示
    void updateCategoryItemTagDisplay(QTreeWidgetItem *item); // 更新文件夹标签显示

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar(); // 确保有这个声明
    void createCentralWidget();

    TagManager *tagManager; // 标签管理器
    QLineEdit *searchBar; // 搜索栏
    QToolBar *tagToolBar; // 标签工具栏
    QAction *addTagAct, *editTagAct, *removeTagAct; // 标签操作按钮

    // UI Components
    NoteEditor *textEdit;
    QListWidget *noteList;
    CategoryManager *categoryManager;
    QListWidget *historyList;
    QHash<QString, QStringList> m_itemTags; // 项目ID -> 标签列表

    // Menus and toolbars
    QMenu *fileMenu;
    QMenu *editMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *formatToolBar;

    // Actions
    QAction *newAct;
    QAction *newFolderAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *deleteAct;
    QAction *openFolderAct;

    // Format toolbar components
    QFontComboBox *fontCombo;
    QComboBox *sizeCombo;
    QToolButton *colorBtn;
    QAction *boldAct;
    QAction *italicAct;
    QAction *underlineAct;

    QLabel *charCountLabel;

    QString currentNotePath;
};

#endif // COMPUTER_SOFT_MAINWINDOW_H