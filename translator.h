#pragma once
#include <QMap>
#include <QString>

class Translator
{
public:
    enum Language { English, Chinese };

    static Translator &instance() {
        static Translator t;
        return t;
    }

    void setLanguage(Language lang) { m_lang = lang; }
    Language language() const { return m_lang; }

    QString tr(const QString &key) const {
        if (m_lang == Chinese && m_zh.contains(key))
            return m_zh[key];
        return key;
    }

private:
    Translator() { initZh(); }

    void initZh() {
        // 菜单
        m_zh["File"]           = "文件";
        m_zh["Edit"]           = "编辑";
        m_zh["View"]           = "视图";
        m_zh["Language"]       = "语言";
        m_zh["English"]        = "English";
        m_zh["Chinese"]        = "中文";
        // File 菜单
        m_zh["New 16x16"]      = "新建 16x16";
        m_zh["New 32x32"]      = "新建 32x32";
        m_zh["New 48x48"]      = "新建 48x48";
        m_zh["Open..."]        = "打开...";
        m_zh["Save..."]        = "保存...";
        m_zh["Exit"]           = "退出";
        // Edit 菜单
        m_zh["Undo"]           = "撤销";
        m_zh["Redo"]           = "重做";
        // View 菜单
        m_zh["Zoom 4x"]        = "缩放 4x";
        m_zh["Zoom 8x"]        = "缩放 8x";
        m_zh["Zoom 12x"]       = "缩放 12x";
        m_zh["Zoom 16x"]       = "缩放 16x";
        // Dock
        m_zh["Colors"]         = "颜色";
        // 状态栏提示
        m_zh["Ready"]          = "就绪";
        m_zh["shortcut_hint"]  =
            "  |  Ctrl+N: 新建  Ctrl+O: 打开  Ctrl+S: 保存  "
            "Ctrl+Z: 撤销  Ctrl+Y: 重做  X: 互换颜色  F: 自定义颜色";
        m_zh["shortcut_hint_en"] =
            "  |  Ctrl+N: New  Ctrl+O: Open  Ctrl+S: Save  "
            "Ctrl+Z: Undo  Ctrl+Y: Redo  X: Swap  F: Custom Color";
        // 对话框
        m_zh["Open Image"]         = "打开图像";
        m_zh["Save Image"]         = "保存图像";
        m_zh["Icon Files (*.ico);;PNG Files (*.png);;All Files (*)"] =
            "图标文件 (*.ico);;PNG 文件 (*.png);;所有文件 (*)";
        m_zh["Error"]              = "错误";
        m_zh["Failed to open file."] = "文件打开失败。";
        m_zh["Failed to save file."] = "文件保存失败。";
        m_zh["Select Foreground Color"] = "选择前景色";
        // 窗口标题
        m_zh["IconCanvas"]         = "图标画布";
        // 颜色面板
        m_zh["X: Swap"] = "X: 互换";
        m_zh["Double-click: custom color"] = "双击: 自定义颜色";
        m_zh["Colors"]                     = "颜色";
    }

    Language m_lang = English;
    QMap<QString, QString> m_zh;
};

// 全局快捷调用
inline QString T(const QString &key) {
    return Translator::instance().tr(key);
}
