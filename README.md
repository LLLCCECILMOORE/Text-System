这是一个个人笔记本应用，所以我想的是做一个类似记事本的软件，但
是可以进行字体大小之类的改变，并且可以直接浏览文件，可以说是一个记
事本 plus 版本
MVC 模式简化：通过 UI 组件（View）与数据管理类（Model）分离，
如 MainWindow 负责界面布局，CategoryManager 和 TagManager 负责数据逻
辑。
信号槽机制：利用 Qt 的信号槽实现组件间通信，例如 CategoryManager
选中文件时发射 noteSelected 信号，触发 MainWindow 加载文件内容。
模块化设计：
将应用程序的功能划分为多个模块，每个模块负责特定的功能。例如：
NoteEditor 模块：负责笔记的文本编辑功能，包括加载、保存笔记以
及实时统计字符数等。
TagManager 模块：负责笔记的标签管理功能，包括添加、删除标签以
及根据标签查找笔记等。
CategoryManager 模块：负责笔记的分类管理功能，包括展示分类结构、
选择笔记等。
MainWindow 模块：作为应用程序的主窗口，负责整合各个模块的功能，
构建用户界面，处理用户交互以及协调模块之间的通信。
面向对象设计：
使用面向对象的编程思想，将每个模块封装为一个类。每个类都有明确
的职责和接口，通过继承、组合等方式实现功能的扩展和复用。例如：
NoteEditor 类继承自 QTextEdit，在其基础上扩展了加载、保存笔记
以及字符数统计等功能。
TagManager 和 CategoryManager 类 分 别 继 承 自 QListWidget 和
QTreeWidget，通过重写和扩展父类的功能来实现标签管理和分类管理的特
定需求。
MainWindow 类 通 过 组 合 的 方 式 将 NoteEditor 、 TagManager 、
CategoryManager 等模块整合在一起，构建出完整的应用程序界面，并通过
信号和槽机制实现模块之间的通信和交互。
