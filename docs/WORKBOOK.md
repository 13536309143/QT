# 逐步实操实验手册

这部分补充主教程的动手细节。请在同一个实验中按顺序修改、编译、观察，再解释结果。除“新增目标”实验外，均基于仓库现成示例，不需要从聊天中复制完整工程。

## 实验一：从空白文件增加第十四个示例

**目标**：不借助 IDE 向导，亲手理解一个 CMake target 的最小结构。

1. 新建 `examples/14_clock` 目录，添加 main.cpp。
2. 在根目录 CMakeLists 的 course_example 调用区追加 `course_example(14_clock)`。
3. main.cpp 使用以下代码：

```cpp
#include <QApplication>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QLabel clock;
    clock.setAlignment(Qt::AlignCenter);
    clock.resize(480, 240);
    QTimer timer;
    auto refresh = [&clock] {
        clock.setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    };
    QObject::connect(&timer, &QTimer::timeout, &clock, refresh);
    refresh();
    timer.start(1000);
    clock.show();
    return app.exec();
}
```

4. 再次执行 `scripts/build.cmd` 让 CMake 看到新目标；之后可只执行 `cmake --build build --config Release --target 14_clock`。
5. `scripts/run.cmd 14_clock` 会按根目录目标的通用路径运行；部署脚本采用白名单，若要部署，需明确将新目标加入白名单。

**解释关键细节**：refresh 在进入事件循环前调用一次，因此初始窗口不会空白一秒；定时器没有父对象，但作为局部变量会在 main 返回时正常析构；lambda 引用的 clock 比连接的使用时长更长；显示的是当前系统时间，定时器延迟不会累计成“时钟越走越慢”。

**故障实验**：把 timer 作为辅助函数的局部变量并让函数立即返回，观察时钟停止。不要以为 connect 会延长 sender 的寿命。验收：可以指出为什么定时器没活到下一次 timeout。

## 实验二：计数器的数据源必须唯一

**起点**：02_counter。先运行并按三次加一。

1. 在构造函数里创建 reset 按钮，加入已有布局。
2. 将 reset 的 clicked 连接到 `[this]` lambda。
3. lambda 先设置 `count_ = 0`，再 `emit countChanged(count_)`。
4. 测试加三次、复位、再加一次，显示应为 3、0、1。
5. 故意只把标签 setText 为 0 而不改 count_，再加一次，观察错误跳回 4。这说明界面文本不应成为另一个互不一致的状态来源。

**进一步要求**：新增 `setCount(int)`，所有改变都经由它。若传入值与现有值一致就返回，只在改变时发 countChanged。把状态修改从按钮逻辑中抽出来，以后读文件、网络更新也能重用。

**扩展题**：达到 10 时禁用加一按钮。连接 countChanged 到按钮 enabled 状态，复位后应自动启用。不要在复位和加一两处分别写一遍“是否禁用”的规则。若未来上限改变，只应改一个地方。

**验收表**：初始 0；十次后 10 且按钮禁用；复位显示 0 并启用；连续复位不产生没有必要的状态通知；窗口销毁后连接不会继续访问按钮。

## 实验三：表单校验的四个阶段

**起点**：03_form。将提交处理整理为“读取、规范化、验证、执行”。

| 输入 | 规范化 | 规则判断 | 界面结果 |
|---|---|---|---|
| 空字符串 | 空 | 姓名缺失 | 显示错误，聚焦姓名框 |
| 三个空格 | 空 | 姓名缺失 | 与空字符串相同 |
| ` 小明 ` | `小明` | 合法 | 结果不保留首尾空格 |
| 有姓名但未确认 | 保持姓名 | 确认缺失 | 提示勾选，保留姓名 |
| 姓名和确认都正确 | 保持 | 合法 | 展示最终值 |

1. 给失败分支补上 name->setFocus，但未确认时可以聚焦 agree。
2. 将错误文字设成可读的强调色，确保没有只靠颜色区分的提示。
3. 增加一个只读预览区域，展示规范化后的信息。
4. 增加重置按钮，明确重置所有状态还是只清空输入。保留默认年龄 18，清空结果并取消确认。

**常见错误**：把所有输入框都设为固定宽度，导致窗口缩放没有效果；在 textChanged 每次敲键时弹 QMessageBox；把 `toInt()` 失败的 0 当成用户真填了 0。优先通过 QSpinBox 保证输入范围，再在业务接口保护规则。

**验收**：用键盘 Tab 完成一次提交；显示错误后能继续修正；成功结果读的是提交时的数据，而不是另一个旧变量。

## 实验四：编辑器关闭确认是一张决策表

**起点**：04_editor。先准备一个可删除的 UTF-8 文本文件，不拿重要文档做实验。

| 文档状态/选择 | 保存结果 | 是否可关闭 |
|---|---|---|
| 无修改 | 不需要保存 | 是 |
| 有修改，点 Cancel | 无 | 否 |
| 有修改，点 Discard | 不保存 | 是 |
| 有修改，点 Save | 成功 | 是 |
| 有修改，点 Save | 取消路径选择 | 否 |
| 有修改，点 Save | 打开或写入失败 | 否 |

1. 在 confirm、saveFile 和 closeEvent 处打断点。
2. 新建后输入一句中文，关闭并选择 Save；在文件对话框取消，观察 saveFile 返回 false、confirm 返回 false、event.ignore。
3. 再次保存到正常路径，确认 modified 变为 false。
4. 修改后尝试保存到不存在的目录，检查错误与原文本保留。
5. 加入“另存为”：不要一进入对话框就覆盖 path_，应先存到局部 target，成功写入后再更新当前路径。

**为什么检查 write 长度**：写入调用成功打开文件，不代表全部字节已经写完。比较实际写入数与 bytes.size，失败不 commit。QSaveFile 的析构会放弃尚未提交的临时文件。编码采用 UTF-8，换行风格和原文件编码的完整保真是额外需求，本例不声称实现。

**验收**：完整走过决策表每一行，记录无法在当前机器模拟的故障；不通过“正常情况能保存”跳过取消与失败路径。

## 实验五：为表格添加行并理解通知

**起点**：06_model。在 TaskModel 的 public 区增加：

```cpp
void appendTask(const QString &title, int priority) {
    const int row = static_cast<int>(rows_.size());
    beginInsertRows(QModelIndex(), row, row);
    rows_.append(QStringList{title, QString::number(priority)});
    endInsertRows();
}
```

1. 在窗口加入标题输入框和“添加”按钮，按钮校验 trimmed 后调用模型方法。
2. 同时打开排序，再添加新行，观察新记录出现在排序后位置，而不是想当然的最后一行。
3. 加入过滤，再添加不符合关键词的任务，观察它暂时不可见；清空过滤后应该出现。
4. 在新行添加前后分别检查源模型 rowCount，增长应为 1。

**begin/end 为什么成对出现**：视图和选择模型依赖通知维护内部映射。beginInsertRows 在底层容器改变前发出，end 在改变后结束事务；通知区间必须是真正新增的源模型行。直接 append 后 emit dataChanged 不能替代新增行通知。

**进阶**：把 priority 从字符串改成 int。可定义 Task 结构体储存 title、priority，data 根据列返回 QString 或 int。排序时会按真实类型比较，不再按字典序把 10 排在 2 前。要保留格式化显示时，可另外定义 SortRole 给代理使用。

**验收**：排序、过滤、编辑和插入可组合使用，没有视图残影；不能通过重启窗口才看到新数据。

## 实验六：线程取消为什么不能只发一个排队信号

**起点**：08_worker，阅读 run 的循环和 requestInterruption。

1. 在后台循环打印 currentThreadId，在按钮处理打印同一函数，确认两个线程不同。
2. 点击开始后拖动窗口，观察仍能重绘；进度条通过排队信号在 GUI 线程更新。
3. 点击取消，后台在下一次检查标记时退出。
4. 再次开始，检查新 Worker 与旧 Worker 的清理，进度从 0 开始。
5. 开始后立即关闭窗口，确认析构中的 wait 不会无限等待。

**推理练习**：若把 cancel 设计为 Worker 槽，并用默认跨线程连接，run 一直占着后台线程执行长循环，那么 cancel 事件要等 run 返回才有机会执行。requestInterruption 则是线程安全标志，运行中的循环可以主动观察它。

**边界讨论**：如果一轮工作需要十分钟阻塞且无法中断，这种循环每轮检查一次仍然不够。必须把阻塞调用改为超时、分块、库自身取消 API 或独立进程方案。`quit()` 仅要求事件循环退出，不会强行终止当前正在运行的函数。

**改进作业**：区分“完成”“取消”“失败”三种结果，不要仅凭进度是否到 100 判断。结果包含状态枚举与错误字符串，GUI 根据状态更新说明文字。验收：取消不显示“任务完成”，异常不留下永久禁用的开始按钮。

## 实验七：用本地 HTTP 服务验证四层错误

**起点**：09_http，先按主教程启动只监听 127.0.0.1 的 Python 服务。

1. 请求 sample.json，预期 HTTP 200。
2. 改请求为 missing.json，预期 404，按钮恢复可点击。
3. 停掉服务再请求，预期连接错误，而不是 JSON 解析错误。
4. 新建临时 bad.json，内容写成 `{broken`，请求能成功但 JSON 解析失败。
5. JSON 合法但没有 message 字段时，展示“字段缺失”，不要把它归类网络失败。

解析部分可放在请求成功分支：

```cpp
QJsonParseError parseError;
QJsonDocument document = QJsonDocument::fromJson(bytes, &parseError);
if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
    // 提示响应格式错误
} else {
    const auto message = document.object().value("message");
    if (!message.isString()) {
        // 提示字段类型不符合约定
    }
}
```

**注意**：reply->readAll 会消费当前缓冲区。不要先调用它展示原文，再调用一次期待得到同样字节；先保存到 bytes，多个后续步骤都读取这份值。请求关闭与超时时，仍要统一恢复按钮、清理 reply。

**扩展**：快速更换搜索词时，为请求附加序号，响应返回时仅处理最新序号；中止旧请求也仍可能触发 finished，因此完成处理不能假定一定是“当前请求”。验收：最后输入的关键词决定最终展示，旧响应不覆盖新结果。

## 实验八：用事务证明“要么全做，要么全不做”

**起点**：10_sql，在建表后添加一段实验。

1. 表中已经有 id=1，或手动插入 id=1。
2. db.transaction，先插入 id=2，随后尝试再次插入 id=1 触发主键冲突。
3. 检查第二次 exec 的返回值并 rollback。
4. SELECT COUNT(*) WHERE id=2，应为 0。
5. 换成两个不冲突的 id，再运行并 commit，两个记录应同时存在。

```cpp
if (!db.transaction()) return false;
bool ok = firstQuery.exec() && secondQuery.exec();
if (!ok) {
    db.rollback();
    return false;
}
if (!db.commit()) {
    db.rollback();
    return false;
}
return true;
```

代码片段中的 query 应提前 prepare/bind；实际函数也要记录 rollback 失败的情况和原始错误，不要让清理错误覆盖首个业务错误。事务保持尽量短，不要开着事务等用户点对话框，也不要在事务中执行长网络请求。

**验收**：不是只检查函数返回 false，还要查询数据库证明确实没有部分数据。扩展至任务导入时，先验证全部数据，再在一个事务中执行插入。对数量极大的导入再设计批次与进度，不牺牲可恢复性。

## 实验九：QML 绑定与命令赋值的不同

**起点**：13_quick。

1. 保持 Label 的 `text: "点击次数：" + backend.count`，点击按钮，标签自动更新。
2. 给 Label 一个 id，并在按钮处理中故意写 `label.text = "手工文字"`，观察绑定被替换。
3. 恢复绑定，点击处理只调用 backend.increment。
4. 给 C++ 添加 reset，并在 QML 增加复位按钮。
5. 为标签加入 `color: backend.count >= 10 ? "tomato" : "black"`，观察状态推导外观。

**NOTIFY 实验**：去掉 increment 中的 emit 后重新构建，count 的内部值仍变化，但界面不会按预期得知改变。NOTIFY 不是存储数据，它通知绑定该重新读取。把 emit 加回，避免用定时器每秒“强行刷新界面”掩盖属性设计错误。

**扩展**：把窗口布局与业务类型分离到不同 QML 文件，在 CMake 的 QML_FILES 中声明新文件。URI 保持 Course，必要时查 qmldir 和构建日志，不能只把文件放到目录就认为已进入模块。

**验收**：新旧绑定行为能解释；没有循环绑定；属性读取函数快速、不做 I/O；C++ 负责业务状态，QML 负责展示。

## 实验十：TaskDesk 中最重要的误删测试

1. 添加“苹果”“香蕉”“草莓”三条，记下数据库 id。
2. 按任务名排序，视图第一行不一定对应源查询第一行。
3. 搜索“香蕉”，现在只有一条可见，视图 currentIndex.row 可能为 0。
4. 选择它，删除确认先点 No，确认任务还在。
5. 再删除并点 Yes，清空搜索，应该只少“香蕉”。

**源码定位**：sourceIndex 调用 proxy.mapToSource，按源模型索引取第 0 列主键，store.remove 用 WHERE id=?。若错误使用视图行号作为数据库 id，最可能在简单默认顺序里“看起来成功”，一排序就暴露问题。

**扩展验收**：切换完成状态使用同样映射；重启确认更改保留。不存在 id 的 remove 返回 false，不应宣称操作成功。此时给用户的错误可继续完善为“任务可能已被删除，请刷新”，区别于数据库故障。

**架构作业**：把 queryModel 全量查询也封装到独立读取接口；为分页提供总数查询。添加 JSON 导入时不要绕过 TaskStore 的验证规则。数据库导出/备份另用 SQLite 兼容方案，不要在正在写入时随意复制一个可能依赖 WAL 的文件。

## 实验十一：有效的测试应能发现错误

1. 找到 tests/tst_course.cpp 的 validation_data，观察空白、引号与长度边界。
2. 临时把 TaskStore 的 `t.size()>200` 改为 `t.size()>=200`。
3. 重建并运行测试，limit 这个用例应失败，因为恰好 200 应被接受。
4. 恢复生产代码，确认测试重新通过。
5. 添加 trim 用例：store.add("  task  ") 后查询，标题应为 "task"。

测试要验证外部行为，不应仅为了通过而复制实现中的判断表达式。持久化测试先关闭一个连接，再创建新仓储重开文件；如果只用同一个对象查刚插入的数据，不能充分验证重启保留。

**GUI 检查**：点击和键盘测试涉及焦点、平台以及真实事件，不能被 --smoke 完全代替。启动检查只证明构造和退出过程没有立即报错。线程示例的 --smoke 默认不会点击开始，所以要另行执行实验六。

**验收**：能证明测试曾因真实错误失败，并说明覆盖不到的场景。CI 工作流提交后仍需在 GitHub 实际执行一次，不能将 YAML 存在等同于远程验证成功。

## 实验十二：从源码仓库到可用分发包

1. 执行 scripts/deploy.cmd taskdesk，检查 dist/taskdesk 目录。
2. 确认包含 TaskDesk exe、Qt DLL、platforms/qwindows.dll 与 sqldrivers/qsqlite.dll。
3. 在普通终端中从 dist 启动，确保没有使用开发环境运行脚本。
4. 拷到干净 Windows 测试环境，测试创建任务、关闭、重开和删除。
5. 为 13_quick 生成分发包，观察 QML 模块等附加内容，检查首次启动日志。

**版本记录**：发布包记录 Qt 版本、编译器、架构、配置与源码版本。可将构建元信息编入“关于”窗口，但不放私人绝对路径和凭据。对外上传前核对依赖许可与运行库分发要求。

**Git 暂存演练**：先 git status，再 git add，最后 git diff --cached --stat。若看到 sdk、build 或用户数据库，立即停止发布并修正忽略规则；已经被跟踪的文件不会因新增 .gitignore 自动消失，需要显式停止跟踪且确认保留本地文件。不要对不确定目录执行递归删除。

**最终验收**：另一个人只凭 README 能安装、编译、运行；普通用户只拿分发包能启动；仓库无 SDK 和私人数据；未验证平台与已知限制均清楚说明。

[返回主教程目录](00-目录.md)
