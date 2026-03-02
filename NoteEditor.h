#pragma once
#include <QTextEdit>

class NoteEditor : public QTextEdit {
    Q_OBJECT
public:
    explicit NoteEditor(QWidget *parent = nullptr);
    
    void loadNote(const QString &filePath);
    void saveNote(const QString &filePath);
    void setFontFamily(const QString &fontFamily); // 设置字体族
    void setFontSize(int size); // 添加字体大小设置方法声明

signals:
    void contentChanged();
};