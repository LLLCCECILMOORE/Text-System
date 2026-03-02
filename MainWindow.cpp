#include "MainWindow.h"
#include "NoteEditor.h"
#include <QApplication>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QSplitter>
#include <QVBoxLayout>
#include <QColorDialog>
#include <QTextDocument>
#include <QTextCursor>
#include "CategoryManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    textEdit(nullptr),
    noteList(nullptr),
    categoryManager(nullptr),
    historyList(nullptr),
    tagManager(nullptr),      // 新增初始化
    searchBar(nullptr),        // 新增初始化
    tagToolBar(nullptr)        // 新增初始化
{
    try {
        qDebug() << "Initializing MainWindow...";
        
        createActions();
        qDebug() << "Actions created";
        
        createMenus();
        qDebug() << "Menus created";
        
        createCentralWidget();
        if (!textEdit) {
            qCritical() << "Failed to create textEdit!";
            return;
        }
        if (!noteList) {
            qCritical() << "Failed to create noteList!";
            return;
        }
        if (!categoryManager) {
            qCritical() << "Failed to create categoryManager!";
            return;
        }
        if (!historyList) {
            qCritical() << "Failed to create historyList!";
            return;
        }
        qDebug() << "Central widget created";
        
        createToolBars(); 
        qDebug() << "Toolbars created";
        
        createStatusBar();
        qDebug() << "Status bar created";
        
        qDebug() << "UI components initialized:";
        qDebug() << "TextEdit:" << textEdit;
        qDebug() << "NoteList:" << noteList;
        qDebug() << "CategoryManager:" << categoryManager;
        qDebug() << "HistoryList:" << historyList;
        
        setWindowTitle(tr("笔记应用程序"));
        resize(1200, 800); // 确保窗口大小足够大
        
        qDebug() << "MainWindow initialized successfully";
    } catch (const std::exception &e) {
        qCritical() << "Exception in MainWindow constructor:" << e.what();
    } catch (...) {
        qCritical() << "Unknown exception in MainWindow constructor";
    }
}

MainWindow::~MainWindow() {
    // 析构函数的实现
    qDebug() << "Destroying MainWindow...";
}

void MainWindow::createActions() {
    qDebug() << "Creating actions...";
    newAct = new QAction(tr("新建笔记"), this);
    newFolderAct = new QAction(tr("新建文件夹"), this);
    openAct = new QAction(tr("打开笔记"), this);
    saveAct = new QAction(tr("保存笔记"), this);
    saveAsAct = new QAction(tr("另存为"), this);
    exitAct = new QAction(tr("退出"), this);
    cutAct = new QAction(tr("剪切"), this);
    copyAct = new QAction(tr("复制"), this);
    pasteAct = new QAction(tr("粘贴"), this);
    deleteAct = new QAction(tr("删除"), this); // 创建删除动作
    openFolderAct = new QAction(tr("打开文件夹"), this); // 添加这一行

    // 连接动作和槽
    connect(newAct, &QAction::triggered, this, &MainWindow::newNote);
    connect(newFolderAct, &QAction::triggered, this, &MainWindow::newFolder);
    connect(openAct, &QAction::triggered, this, &MainWindow::openNote);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveNote);
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAsNote);
    connect(openFolderAct, &QAction::triggered, this, &MainWindow::openFolder);
    connect(deleteAct, &QAction::triggered, this, &MainWindow::deleteNote); // 连接删除动作到槽函数

    qDebug() << "Actions created successfully.";
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("文件"));
    QMenu *newMenu = fileMenu->addMenu(tr("新建"));
    newMenu->addAction(newAct);
    newMenu->addAction(newFolderAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(deleteAct); // 添加删除菜单项
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("编辑"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
}

void MainWindow::createToolBars() {
    fileToolBar = addToolBar(tr("文件"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(newFolderAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(saveAsAct);
    fileToolBar->addAction(deleteAct); // 添加删除按钮到工具栏

    formatToolBar = addToolBar(tr("格式"));
    fontCombo = new QFontComboBox(this);
    sizeCombo = new QComboBox(this);
    for (int i = 8; i <= 72; i += 2) {
        sizeCombo->addItem(QString::number(i));
    }
    colorBtn = new QToolButton(this);
    colorBtn->setPopupMode(QToolButton::MenuButtonPopup);
    colorBtn->setMenu(new QMenu(this));
    QAction *colorAct = new QAction(tr("文本颜色"), this);
    connect(colorAct, &QAction::triggered, this, [this]() {
        QColor color = QColorDialog::getColor(textEdit->textColor(), this, tr("选择文本颜色"));
        if (color.isValid()) {
            QTextCursor cursor = textEdit->textCursor();
            QTextCharFormat format = cursor.charFormat();
            format.setForeground(color);
            cursor.setCharFormat(format);
            textEdit->setTextCursor(cursor);
        }
    });
    colorBtn->setDefaultAction(colorAct);

    boldAct = new QAction(tr("加粗"), this);
    boldAct->setCheckable(true);
    connect(boldAct, &QAction::toggled, this, [this](bool checked) {
        QTextCursor cursor = textEdit->textCursor();
        QTextCharFormat format = cursor.charFormat();
        format.setFontWeight(checked ? QFont::Bold : QFont::Normal);
        cursor.setCharFormat(format);
        textEdit->setTextCursor(cursor);
    });

    italicAct = new QAction(tr("斜体"), this);
    italicAct->setCheckable(true);
    connect(italicAct, &QAction::toggled, this, [this](bool checked) {
        QTextCursor cursor = textEdit->textCursor();
        QTextCharFormat format = cursor.charFormat();
        format.setFontItalic(checked);
        cursor.setCharFormat(format);
        textEdit->setTextCursor(cursor);
    });

    underlineAct = new QAction(tr("下划线"), this);
    underlineAct->setCheckable(true);
    connect(underlineAct, &QAction::toggled, this, [this](bool checked) {
        QTextCursor cursor = textEdit->textCursor();
        QTextCharFormat format = cursor.charFormat();
        format.setFontUnderline(checked);
        cursor.setCharFormat(format);
        textEdit->setTextCursor(cursor);
    });

    formatToolBar->addWidget(fontCombo);
    formatToolBar->addWidget(sizeCombo);
    formatToolBar->addWidget(colorBtn);
    formatToolBar->addAction(boldAct);
    formatToolBar->addAction(italicAct);
    formatToolBar->addAction(underlineAct);

    tagToolBar = addToolBar(tr("标签"));
    addTagAct = new QAction(QIcon(":/tag_add.png"), tr("添加标签"), this);
    editTagAct = new QAction(QIcon(":/tag_edit.png"), tr("修改标签"), this);
    removeTagAct = new QAction(QIcon(":/tag_remove.png"), tr("删除标签"), this);
    tagToolBar->addActions({addTagAct, editTagAct, removeTagAct});
    tagToolBar->setAllowedAreas(Qt::TopToolBarArea); // 固定在顶部

    // 连接标签操作按钮到槽函数
    connect(addTagAct, &QAction::triggered, this, &MainWindow::showAddTagDialog);
    connect(editTagAct, &QAction::triggered, this, &MainWindow::showEditTagDialog);
    connect(removeTagAct, &QAction::triggered, this, &MainWindow::removeSelectedTag);

    // 连接字体选择信号到槽
    connect(fontCombo, &QFontComboBox::currentFontChanged, this, [this](const QFont &font) {
        textEdit->setFontFamily(font.family());
    });

    // 连接字体大小选择信号到槽
    connect(sizeCombo, &QComboBox::currentIndexChanged, this, [this](int index) {
        QString sizeText = sizeCombo->itemText(index);
        bool ok;
        int size = sizeText.toInt(&ok); // 将字符串转换为整数，并检查是否成功

        if (ok && size > 0) { // 确保转换成功且字体大小大于 0
            textEdit->setFontSize(size);
        } else {
            qDebug() << "Invalid font size selected:" << sizeText;
        }
    });

    // 确保工具栏不会覆盖菜单栏
    fileToolBar->setAllowedAreas(Qt::TopToolBarArea);
    formatToolBar->setAllowedAreas(Qt::TopToolBarArea);

    addToolBar(Qt::TopToolBarArea, fileToolBar);
    addToolBar(Qt::TopToolBarArea, formatToolBar);

    // MainWindow.cpp (createToolBars或createCentralWidget)
    // 添加标签工具栏
    tagToolBar = addToolBar(tr("标签"));
    addTagAct = new QAction(QIcon(":/add_tag.png"), tr("添加标签"), this);
    editTagAct = new QAction(QIcon(":/edit_tag.png"), tr("修改标签"), this);
    removeTagAct = new QAction(QIcon(":/remove_tag.png"), tr("删除标签"), this);
    tagToolBar->addActions({addTagAct, editTagAct, removeTagAct});

// 连接按钮到槽函数
    connect(addTagAct, &QAction::triggered, this, &MainWindow::showAddTagDialog);
    connect(editTagAct, &QAction::triggered, this, &MainWindow::showEditTagDialog);
    connect(removeTagAct, &QAction::triggered, this, &MainWindow::removeSelectedTag);
}

void MainWindow::createCentralWidget() {
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    textEdit = new NoteEditor(this);
    noteList = new QListWidget(this);
    categoryManager = new CategoryManager(this);
    historyList = new QListWidget(this);
    tagManager = new TagManager(this); // 新增标签管理器
    searchBar = new QLineEdit(this);    // 新增搜索栏

    // 左侧面板布局（类别、笔记列表、历史记录、标签管理器、搜索栏）
    QWidget *leftPanel = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(6); // 添加组件间距

    // 按顺序添加组件到左侧面板
    leftLayout->addWidget(categoryManager);
    leftLayout->addWidget(noteList);
    leftLayout->addWidget(historyList);
    leftLayout->addWidget(tagManager);   // 标签管理器
    leftLayout->addWidget(searchBar);     // 搜索栏

    // 设置左侧各组件的拉伸比例（类别和笔记占更多空间）
    leftLayout->setStretchFactor(categoryManager, 2);
    leftLayout->setStretchFactor(noteList, 2);
    leftLayout->setStretchFactor(historyList, 1);
    leftLayout->setStretchFactor(tagManager, 1);
    leftLayout->setStretchFactor(searchBar, 0); // 搜索栏不拉伸，固定高度

    splitter->addWidget(leftPanel);
    splitter->addWidget(textEdit);
    setCentralWidget(splitter);

    // 设置初始分割比例（左侧更宽，适应新增组件）
    splitter->setSizes({300, 900}); 

    // 连接信号和槽（新增标签和搜索相关连接）
    connect(noteList, &QListWidget::itemClicked, this, &MainWindow::loadNoteFromList);
    connect(textEdit, &NoteEditor::contentChanged, this, &MainWindow::updateCharCount);
    connect(categoryManager, &CategoryManager::noteSelected, this, &MainWindow::onNoteSelected);
    connect(categoryManager, &CategoryManager::folderSelected, this, &MainWindow::onFolderSelected);
    connect(categoryManager, &CategoryManager::historyUpdated, this, &MainWindow::updateHistoryList);
    connect(historyList, &QListWidget::itemClicked, this, &MainWindow::onHistorySelected);
    connect(tagManager, &TagManager::tagSelected, this, &MainWindow::filterNotesByTag); // 标签选择过滤
    connect(searchBar, &QLineEdit::textChanged, this, &MainWindow::filterNotesByTag);   // 搜索栏实时搜索

    connect(noteList, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {
        updateItemTagDisplay(noteList, item);
    });
    connect(historyList, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {
        updateItemTagDisplay(historyList, item);
    });
}

void MainWindow::newNote() {
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("选择文件夹"));
    if (folderPath.isEmpty()) {
        return;
    }

    QString noteName = QInputDialog::getText(this, tr("新建笔记"), tr("输入笔记名称"));
    if (noteName.isEmpty()) {
        return;
    }

    QString filePath = folderPath + "/" + noteName + ".txt";
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "";
        file.close();
        currentNotePath = filePath;
        textEdit->loadNote(filePath);

        // 更新文件夹显示
        categoryManager->setupFileSystem(folderPath);

        // 在 noteList 中添加新文件
        QListWidgetItem *item = new QListWidgetItem(noteName, noteList);
        item->setData(Qt::UserRole, filePath); // 设置文件路径为用户角色数据
    } else {
        QMessageBox::warning(this, tr("新建失败"), tr("无法创建文件"));
    }
}
void MainWindow::createStatusBar() {
    statusBar()->showMessage(tr("就绪"));
    charCountLabel = new QLabel(this);
    charCountLabel->setText(tr("字符数: 0"));
    statusBar()->addPermanentWidget(charCountLabel);
}

void MainWindow::newFolder() {
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("选择文件夹"));
    if (!folderPath.isEmpty()) {
        QString folderName = QInputDialog::getText(this, tr("新建文件夹"), tr("输入文件夹名称"));
        if (!folderName.isEmpty()) {
            QString newFolderPath = folderPath + "/" + folderName;
            if (QDir().mkdir(newFolderPath)) {
                categoryManager->setupFileSystem(folderPath);
                QMessageBox::information(this, tr("成功"), tr("文件夹已创建"));
            } else {
                QMessageBox::warning(this, tr("创建失败"), tr("无法创建文件夹"));
            }
        }
    }
}

void MainWindow::openNote() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("打开笔记"), "", tr("文本文件 (*.txt)"));
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            textEdit->setPlainText(in.readAll());
            file.close();
            currentNotePath = filePath;

            // 更新文件夹显示
            QString folderPath = QFileInfo(filePath).absolutePath();
            categoryManager->setupFileSystem(folderPath);

            QString noteName = QFileInfo(filePath).fileName();
            if (noteList->findItems(noteName, Qt::MatchExactly).isEmpty()) {
                QListWidgetItem *item = new QListWidgetItem(noteName, noteList);
                item->setData(Qt::UserRole, filePath);
            }

            // 添加到历史记录
            addFolderToHistory(filePath);
        } else {
            QMessageBox::warning(this, tr("打开失败"), tr("无法打开文件"));
        }
    }
}

void MainWindow::saveNote() {
    if (currentNotePath.isEmpty()) {
        saveAsNote();
    } else {
        QFile file(currentNotePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << textEdit->toPlainText();
            file.close();
            QMessageBox::information(this, tr("保存成功"), tr("笔记已保存"));
        } else {
            QMessageBox::warning(this, tr("保存失败"), tr("无法保存文件"));
        }
    }
}

void MainWindow::saveAsNote() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("另存为"), "", tr("文本文件 (*.txt)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << textEdit->toPlainText();
            file.close();
            QMessageBox::information(this, tr("保存成功"), tr("笔记已保存"));
            QString noteName = QFileInfo(fileName).fileName();
            if (noteList->findItems(noteName, Qt::MatchExactly).isEmpty()) {
                QListWidgetItem *item = new QListWidgetItem(noteName, noteList);
                item->setData(Qt::UserRole, fileName);
            }

            // 添加到历史记录
            addFolderToHistory(fileName);
        } else {
            QMessageBox::warning(this, tr("保存失败"), tr("无法保存文件"));
        }
    }
}

void MainWindow::deleteNote() {
    QListWidgetItem *item = noteList->currentItem();
    if (!item) {
        QMessageBox::warning(this, tr("警告"), tr("未选择任何项目"));
        return;
    }

    QString path = item->data(Qt::UserRole).toString();
    if (path.isEmpty()) {
        QMessageBox::warning(this, tr("错误"), tr("选中的项目没有关联的文件路径"));
        return;
    }

    QFileInfo fileInfo(path);
    if (fileInfo.isFile()) {
        // 删除文件
        if (QFile::remove(path)) {
            noteList->takeItem(noteList->row(item));
            delete item;
            currentNotePath.clear();
            textEdit->clear();
            QMessageBox::information(this, tr("删除成功"), tr("文件已被删除"));
        } else {
            QMessageBox::warning(this, tr("删除失败"), tr("无法删除文件"));
        }
    } else if (fileInfo.isDir()) {
        // 删除文件夹
        QString confirmMessage = tr("确定要删除文件夹及其所有内容吗？\n%1").arg(path);
        if (QMessageBox::question(this, tr("确认删除"), confirmMessage, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            if (QDir().rmdir(path)) {
                noteList->takeItem(noteList->row(item));
                delete item;
                QMessageBox::information(this, tr("删除成功"), tr("文件夹已被删除"));
            } else {
                QMessageBox::warning(this, tr("删除失败"), tr("无法删除文件夹"));
            }
        }
    } else {
        QMessageBox::warning(this, tr("错误"), tr("未知的项目类型"));
    }
}

void MainWindow::updateCharCount() {
    int count = textEdit->toPlainText().length();
    charCountLabel->setText(tr("字符数: %1").arg(count));
}

void MainWindow::loadNoteFromList(QListWidgetItem *item) {
    if (!item) return;
    QString filePath = item->data(Qt::UserRole).toString();
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        textEdit->setPlainText(in.readAll());
        file.close();
        currentNotePath = filePath;
    } else {
        QMessageBox::warning(this, tr("加载失败"), tr("无法加载文件"));
    }
}

void MainWindow::onFolderSelected(const QString &folderPath) {
    noteList->clear();
    QDir dir(folderPath);
    QStringList files = dir.entryList(QStringList() << "*.txt", QDir::Files);
    for (const QString &file : files) {
        QListWidgetItem *item = new QListWidgetItem(file, noteList);
        item->setData(Qt::UserRole, folderPath + "/" + file);
    }
}

void MainWindow::onNoteSelected(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        textEdit->setPlainText(in.readAll());
        file.close();
        currentNotePath = filePath;
        addFolderToHistory(filePath);
    } else {
        QMessageBox::warning(this, tr("加载失败"), tr("无法加载文件"));
    }
}

void MainWindow::addFolderToHistory(const QString &filePath) {
    if (!historyList->findItems(QFileInfo(filePath).fileName(), Qt::MatchExactly).isEmpty()) {
        return; // 如果文件已经在历史列表中，则不重复添加
    }
    QListWidgetItem *item = new QListWidgetItem(QFileInfo(filePath).fileName(), historyList);
    item->setData(Qt::UserRole, filePath);
}

void MainWindow::openFolder() {
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("打开文件夹"));
    if (!folderPath.isEmpty()) {
        categoryManager->setupFileSystem(folderPath, true);
        QDir dir(folderPath);
        QStringList files = dir.entryList(QStringList() << "*.txt", QDir::Files);
        for (const QString &file : files) {
            QListWidgetItem *item = new QListWidgetItem(file, noteList);
            item->setData(Qt::UserRole, folderPath + "/" + file);
        }
    }
}

void MainWindow::updateHistoryList(const QStringList &history) {
    historyList->clear();
    for (const QString &filePath : history) {
        QListWidgetItem *item = new QListWidgetItem(QFileInfo(filePath).fileName(), historyList);
        item->setData(Qt::UserRole, filePath);
    }
}

void MainWindow::onHistorySelected(QListWidgetItem *item) {
    if (!item) return;
    QString filePath = item->data(Qt::UserRole).toString();
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        textEdit->setPlainText(in.readAll());
        file.close();
        currentNotePath = filePath;
    } else {
        QMessageBox::warning(this, tr("加载失败"), tr("无法加载文件"));
    }
}
// MainWindow.cpp
void MainWindow::showAddTagDialog() {
    // 优先获取noteList或historyList的当前项
    QListWidgetItem *currentItem = noteList->currentItem() 
        ? noteList->currentItem() 
        : historyList->currentItem();
    
    if (!currentItem) {
        QMessageBox::warning(this, tr("警告"), tr("请先选择一个笔记"));
        return;
    }
    
    QString noteId = currentItem->data(Qt::UserRole).toString(); // 获取文件路径作为noteId
    bool ok;
    QString tag = QInputDialog::getText(this, tr("添加标签"), tr("输入标签"), QLineEdit::Normal, "", &ok);
    if (ok && !tag.isEmpty()) {
        tagManager->addTagToNote(noteId, tag); // 调用标签管理器添加标签
        updateItemTagDisplay(currentItem->listWidget(), currentItem); // 更新对应列表的标签显示
    }
}

// 更新笔记项的标签显示（在noteList中显示标签）
// MainWindow.cpp
void MainWindow::updateItemTagDisplay(QListWidget *listWidget, QListWidgetItem *item) {
    if (!listWidget || !item) return;
    
    QString noteId = item->data(Qt::UserRole).toString();
    QStringList tags = tagManager->getTagsForNote(noteId);
    QString tagText = tags.isEmpty() ? "" : tr(" [ ") + tags.join(", ") + tr("]");
    
    // 提取原始文件名（移除之前的标签文本）
    QString originalText = item->text();
    int tagStart = originalText.indexOf(" [ ");
    if (tagStart != -1) {
        originalText = originalText.left(tagStart);
    }
    
    // 设置新文本（文件名 + 标签）
    item->setText(originalText + tagText);
}
void MainWindow::showEditTagDialog() {
    QListWidgetItem *currentNoteItem = nullptr;
    QTreeWidgetItem *currentCategoryItem = nullptr;

    // 检查当前选中的项目是否在 noteList 或 historyList 中
    if (noteList->currentItem()) {
        currentNoteItem = noteList->currentItem();
    } else if (historyList->currentItem()) {
        currentNoteItem = historyList->currentItem();
    } else if (categoryManager->currentItem()) {
        currentCategoryItem = categoryManager->currentItem();
    }

    if (!currentNoteItem && !currentCategoryItem) {
        QMessageBox::warning(this, tr("警告"), tr("请先选择一个笔记或文件夹"));
        return;
    }

    QString noteId;
    if (currentNoteItem) {
        noteId = currentNoteItem->data(Qt::UserRole).toString();
    } else if (currentCategoryItem) {
        noteId = currentCategoryItem->data(0, Qt::UserRole).toString();
    }

    QStringList tags = tagManager->getTagsForNote(noteId);
    if (tags.isEmpty()) {
        QMessageBox::warning(this, tr("警告"), tr("该笔记或文件夹没有标签"));
        return;
    }

    // 选择要修改的旧标签
    QString oldTag = QInputDialog::getItem(this, tr("修改标签"), tr("选择要修改的标签"), tags, 0, false);
    if (oldTag.isEmpty()) return;

    // 输入新标签
    QString newTag = QInputDialog::getText(this, tr("修改标签"), tr("输入新标签"), QLineEdit::Normal, oldTag);
    if (newTag != oldTag && !newTag.isEmpty()) {
        tagManager->editTag(oldTag, newTag); // 调用标签管理器修改标签

        if (currentNoteItem) {
            updateItemTagDisplay(currentNoteItem->listWidget(), currentNoteItem); // 更新标签显示
        } else if (currentCategoryItem) {
            updateCategoryItemTagDisplay(currentCategoryItem); // 更新文件夹标签显示
        }
    }
}

void MainWindow::removeSelectedTag() {
    QListWidgetItem *currentItem = noteList->currentItem() 
        ? noteList->currentItem() 
        : historyList->currentItem();
    
    if (!currentItem) return;
    
    QString noteId = currentItem->data(Qt::UserRole).toString();
    QStringList tags = tagManager->getTagsForNote(noteId);
    if (tags.isEmpty()) return;
    
    // 选择要删除的标签
    QString tagToRemove = QInputDialog::getItem(this, tr("删除标签"), tr("选择要删除的标签"), tags, 0, false);
    if (!tagToRemove.isEmpty()) {
        tagManager->removeTagFromNote(noteId, tagToRemove); // 调用标签管理器删除标签
        updateItemTagDisplay(currentItem->listWidget(), currentItem); // 更新标签显示
    }
}
void MainWindow::filterNotesByTag(const QString &tag) {
    noteList->clear();
    if (tag.isEmpty()) {
        // 显示当前文件夹所有笔记（复用原有逻辑）
        onFolderSelected(categoryManager->currentNotePath().section('/', 0, -2)); // 获取当前文件夹路径
        return;
    }
    
    // 根据标签搜索笔记
    QStringList noteIds = tagManager->findNotesByTag(tag);
    foreach (const QString &noteId, noteIds) {
        QFileInfo info(noteId);
        QListWidgetItem *item = new QListWidgetItem(info.fileName(), noteList);
        item->setData(Qt::UserRole, noteId);
        updateItemTagDisplay(noteList, item); // 新调用方式，传入列表和项
    }
}
void MainWindow::updateCategoryItemTagDisplay(QTreeWidgetItem *item) {
    if (!item) return;

    QString noteId = item->data(0, Qt::UserRole).toString();
    QStringList tags = tagManager->getTagsForNote(noteId);
    QString tagText = tags.isEmpty() ? "" : tr(" [ ") + tags.join(", ") + tr("]");

    // 提取原始文件名（移除之前的标签文本）
    QString originalText = item->text(0);
    int tagStart = originalText.indexOf(" [ ");
    if (tagStart != -1) {
        originalText = originalText.left(tagStart);
    }

    // 设置新文本（文件名 + 标签）
    item->setText(0, originalText + tagText);
}