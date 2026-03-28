# IconCanvas

A pixel-level icon editor built with Qt6, inspired by the VS2010 icon resource editor.

一款基于 Qt6 开发的像素级图标编辑器，功能对标 VS2010 图标资源编辑器。

![Platform](https://img.shields.io/badge/platform-Windows-blue)
![Qt](https://img.shields.io/badge/Qt-6.7-green)
![License](https://img.shields.io/badge/license-MIT-orange)

---

## Screenshots / 截图

> Launch the app, open or create a new icon, and start drawing pixel by pixel.
>
> 启动程序后，新建或打开图标文件，即可开始像素级绘制。

---

## Features / 功能特性

- **Pixel canvas** — Draw on 16×16, 32×32, or 48×48 grids with zoom levels from 4× to 16×
- **Color palette** — 16 preset colors; left-click sets foreground, right-click sets background; double-click to open custom color picker
- **Real-time preview** — Live 1:1 size preview panel below the color palette
- **Undo / Redo** — Full stroke-level undo history (up to 64 steps) via Ctrl+Z / Ctrl+Y
- **File I/O** — Open and save `.ico` and `.png` files
- **Bilingual UI** — Switch between English and Chinese from the Language menu
- **Optimized rendering** — Dirty-rect partial repaint + grid line pixmap cache for smooth performance

---

- **像素画布** — 支持 16×16、32×32、48×48 三种尺寸，缩放比例 4×～16×
- **颜色面板** — 16 种预设颜色；左键设前景色，右键设背景色；双击打开自定义颜色对话框
- **实时预览** — 颜色面板下方实时显示图标 1:1 真实尺寸
- **撤销/重做** — 基于笔划的撤销历史（最多 64 步），支持 Ctrl+Z / Ctrl+Y
- **文件读写** — 支持打开和保存 `.ico`、`.png` 格式
- **中英双语** — 通过 Language 菜单一键切换中文/英文界面
- **性能优化** — 局部脏区刷新 + 网格线 Pixmap 缓存，绘制流畅

---

## Keyboard Shortcuts / 快捷键

| Shortcut | Function                           | 功能                 |
| -------- | ---------------------------------- | -------------------- |
| `Ctrl+N` | New 32×32 canvas                   | 新建 32×32 画布      |
| `Ctrl+O` | Open file                          | 打开文件             |
| `Ctrl+S` | Save file                          | 保存文件             |
| `Ctrl+Z` | Undo                               | 撤销                 |
| `Ctrl+Y` | Redo                               | 重做                 |
| `X`      | Swap foreground / background color | 互换前景色与背景色   |
| `F`      | Open custom color picker           | 打开自定义颜色对话框 |

---

## Project Structure / 项目结构

```
IconCanvas/
├── main.cpp              # Entry point
├── mainwindow.h/.cpp     # Main window, menu bar, dock layout
├── canvaswidget.h/.cpp   # Pixel canvas with zoom, undo, partial repaint
├── colorpalette.h/.cpp   # Color palette panel
├── previewwidget.h/.cpp  # Real-time 1:1 preview panel
├── translator.h          # Bilingual string pool (EN / ZH)
├── app.rc                # Windows resource file for app icon
├── app.qrc               # Qt resource file
├── CMakeLists.txt        # CMake build configuration
└── icon.ico              # Application icon
```

---

## Build Environment / 构建环境

| Requirement  | Version                 |
| ------------ | ----------------------- |
| Qt           | 6.4 or above            |
| CMake        | 3.16 or above           |
| C++ Standard | C++17                   |
| Compiler     | MSVC 2019+ or MinGW 11+ |
| OS           | Windows 10 / 11         |

---

## Build Instructions / 构建方法

### Using Qt Creator / 使用 Qt Creator

1. Open Qt Creator
2. **File → Open File or Project** → select `CMakeLists.txt`
3. Choose a Qt 6 kit
4. Click **Run ▶**

### Using Command Line / 使用命令行

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=C:/Qt/6.7.0/msvc2019_64
cmake --build . --config Release
```

---

## Git Workflow / 版本管理

This project uses Git for version control with the following commit convention:

```
feat:   new feature
fix:    bug fix
chore:  build / config changes
docs:   documentation
```

View the full commit history:

```bash
git log --oneline
```

---

## Roadmap / 后续计划

- [ ] Toolbox: line, rectangle, ellipse, fill tools
- [ ] Multiple image sizes in one `.ico` file
- [ ] Eraser tool
- [ ] Export as multi-size `.ico`
- [ ] macOS support

---

## Author / 作者

**Tang0314** — [github.com/Tang0314](https://github.com/Tang0314)

Project link / 项目链接: [https://github.com/Tang0314/IconCanvas](https://github.com/Tang0314/IconCanvas)
