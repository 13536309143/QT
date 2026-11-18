"""Generate file://-compatible offline lessons in recommended learning order."""
from pathlib import Path
import html, json, re
import markdown
root = Path(__file__).resolve().parents[1]
site = root / 'site'
stages = json.loads((root/'tools/course.json').read_text(encoding='utf-8'))['stages']
ids = [c for s in stages for c in s['chapters']]
assert len(ids) == len(set(ids)) and set(ids) == {p.stem for p in (root/'docs').glob('[0-9][0-9].md')}
examples = [p for p in (root/'examples').iterdir() if (p/'main.cpp').is_file()]
labs = len(re.findall(r'^## 实验', (root/'docs/WORKBOOK.md').read_text(encoding='utf-8'), re.M))
extras = {'WORKBOOK':'workbook', 'API':'api', 'REFERENCES':'references', 'VALIDATION':'validation'}
page_map = dict(zip(ids,ids)) | extras | {'00-目录':'index'}
items = {}
for c in ids:
    md = (root/f'docs/{c}.md').read_text(encoding='utf-8')
    example = re.search(r'构建本章目标：`([^`]+)`', md)
    if example:
        folder = root/'examples'/example[1]
        def sync(m):
            source = folder/m[1]
            return ('### '+m[1]+'\n\n```'+m[2]+'\n'+source.read_text(encoding='utf-8').rstrip()+'\n```') if source.is_file() else m[0]
        md = re.sub(r'### ([^\n]+)\n\n```([^\n]*)\n.*?\n```', sync, md, flags=re.S)
    items[c] = (md.splitlines()[0].lstrip('# '), md)
def render(md):
    engine = markdown.Markdown(extensions=['fenced_code','tables','toc'])
    body = engine.convert(md)
    if len(re.findall(r'^### ', md, re.M)) >= 8:
        toc = '<details class="page-toc"><summary>本页函数与小节目录（展开定位）</summary>' + engine.toc + '</details>'
        body = body.replace('</h1>', '</h1>' + toc, 1)
    def link(m):
        url = html.unescape(m[1])
        path, sep, fragment = url.partition('#')
        if path.endswith('.md') and '/' not in path and Path(path).stem in page_map:
            url = page_map[Path(path).stem]+'.html'+(sep+fragment if sep else '')
        return 'href="'+html.escape(url,quote=True)+'"'
    return re.sub(r'href="([^"]*)"', link, body)
def nav(active):
    out = '<input id="search" type="search" placeholder="搜索章节与正文" aria-label="搜索章节与正文"><p><a href="index.html">课程首页与推荐路线</a>'
    out += f'<a href="workbook.html">{labs} 个逐步实操实验</a><a href="api.html">函数详解手册</a><a href="references.html">官方参考资料</a></p>'
    for stage in stages:
        out += '<section class="nav-stage"><h2>'+html.escape(stage['title'])+'</h2>'
        for c in stage['chapters']:
            css = 'chapter active' if c == active else 'chapter'
            current = ' aria-current="page"' if c == active else ''
            out += f'<a class="{css}" data-id="{c}" href="{c}.html"{current}>{html.escape(items[c][0])}</a>'
        out += '</section>'
    return out+'<p id="search-status" role="status" aria-live="polite"></p>'
def wrap(title,body,active=''):
    return '<!doctype html><html lang="zh-CN"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>'+html.escape(title)+' · Qt 6 中文教程</title><link rel="stylesheet" href="style.css"></head><body><a class="skip-link" href="#content">跳到正文</a><header><div class="tag">C++ · QT 6 · 从入门到项目</div><h1>Qt 6 中文学习手册</h1>'+f'<p>{len(ids)} 章分阶段学习 / {len(examples)} 个源码示例 / {labs} 个逐步实验 / 离线可读</p></header><div class="shell"><nav aria-label="课程导航">'+nav(active)+'</nav><main id="content">'+body+'</main></div><script src="search.js"></script></body></html>'
site.mkdir(exist_ok=True)
for i,c in enumerate(ids):
    title,md = items[c]
    stage = next(s['title'] for s in stages if c in s['chapters'])
    position = f'<p class="reading-position">{html.escape(stage)} · 推荐路线 {i+1} / {len(ids)}</p>'
    before = f'<a href="{ids[i-1]}.html">← 上一课</a>' if i else '<a href="index.html">← 学习路线</a>'
    after = f'<a href="{ids[i+1]}.html">下一课 →</a>' if i+1 < len(ids) else '<a href="index.html">返回首页</a>'
    (site/f'{c}.html').write_text(wrap(title,position+render(md)+'<div class="pager" aria-label="推荐学习顺序">'+before+after+'</div>',c),encoding='utf-8')
intro = f'''# 从第一个窗口开始，逐步学会独立开发

本课程面向刚接触 Qt 的学习者，环境为 Qt 6.8.3 / MSVC 2022 / C++17。
共 **{len(ids)} 章、{len(examples)} 个可编译示例、1 个 TaskDesk 综合项目、{labs} 个逐步实验**。

## 选择你的起点

- **第一次写 C++**：[先读零基础预备课](01.md)，再学习工具、环境和首个窗口。
- **学过 C++，第一次用 Qt**：[从学习地图开始](00.md)，按下方各阶段推进。
- **想用 Qt Creator 图形操作**：[从 Creator 工作区入门](59.md)，跟着完成 Kit、Designer、信号槽和调试练习；第 02 章后即可提前学习。
- **已有入门经验**：选择 [异步任务](28.md)、[模型进阶](23.md)、[测试进阶](37.md) 补齐薄弱环节。
- **学习图形集成**：从 [图形预备课](49.md) 开始，再学习 OpenGL 与 Vulkan；图形自检需要正常桌面与驱动，Vulkan 为可选构建。

每课先理解输入、输出和对象寿命，再运行原例，修改一个功能，验证正常、边界、失败三条路径。
全书从 00 到 {len(ids)-1:02d} 连续编号，章节文件名、侧栏与“上一课 / 下一课”均采用同一顺序。每个主要函数的参数、结果与时机可查 [函数详解手册](API.md)。

## 完成第一个小目标

1. 阅读项目 README 的环境要求，运行根目录 build.cmd。
2. 在项目根目录运行 `scripts\\run.cmd 01_hello`。
3. 修改 examples/01_hello/main.cpp 的标题，重新构建并确认变化。

遇到问题看 [新手答疑](48.md)，想跟着操作看 [逐步实操实验](WORKBOOK.md)。

'''
for stage in stages:
    intro += '## '+stage['title']+'\n\n'+'\n'.join(f'- [{items[c][0]}]({c}.md)' for c in stage['chapters'])+'\n\n'
(site/'index.html').write_text(wrap('首页',render(intro)),encoding='utf-8')
for source,output in extras.items():
    md = (root/f'docs/{source}.md').read_text(encoding='utf-8')
    (site/f'{output}.html').write_text(wrap(md.splitlines()[0].lstrip('# '),render(md)),encoding='utf-8')
script = 'const texts='+json.dumps({c:'\n'.join(items[c]).lower() for c in ids},ensure_ascii=False)+';\n'+'''
document.getElementById('search').addEventListener('input',event=>{
    const query=event.target.value.trim().toLowerCase();let count=0;
    document.querySelectorAll('a.chapter').forEach(link=>{link.hidden=!texts[link.dataset.id].includes(query);if(!link.hidden)count++;});
    document.querySelectorAll('.nav-stage').forEach(stage=>{stage.hidden=!Array.from(stage.querySelectorAll('a.chapter')).some(link=>!link.hidden);});
    document.getElementById('search-status').textContent=query?`找到 ${count} 个章节`:'';
});
'''
(site/'search.js').write_text(script,encoding='utf-8')
print(f'Rendered {len(ids)} chapters, {len(examples)} examples, {labs} labs')
