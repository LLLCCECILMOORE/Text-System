#include "NoteEditor.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

NoteEditor::NoteEditor(QWidget *parent) 
    : QTextEdit(parent) 
{
    connect(this, &QTextEdit::textChanged, this, &NoteEditor::contentChanged);
}

void NoteEditor::loadNote(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, tr("错误"), tr("无法打开文件"));
        return;
    }
    
    QTextStream in(&file);
    setPlainText(in.readAll());
    file.close();
}

void NoteEditor::saveNote(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, tr("错误"), tr("无法保存文件"));
        return;
    }
    
    QTextStream out(&file);
    out << toPlainText();
    file.close();
}

void NoteEditor::setFontFamily(const QString &fontFamily) {
    QTextCursor cursor = textCursor(); // 获取当前光标
    QTextCharFormat format = cursor.charFormat(); // 获取当前字符格式
    format.setFontFamilies({fontFamily}); // 使用 setFontFamilies 设置字体族
    cursor.setCharFormat(format); // 应用格式
    setTextCursor(cursor); // 更新光标
}
void NoteEditor::setFontSize(int size) {
    QTextCursor cursor = textCursor(); // 获取当前光标
    QTextCharFormat format = cursor.charFormat(); // 获取当前字符格式
    format.setFontPointSize(size); // 设置字体大小
    cursor.setCharFormat(format); // 应用格式
    setTextCursor(cursor); // 更新光标
}