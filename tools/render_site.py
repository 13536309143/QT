"""Regenerate offline HTML from Markdown. Run from any directory."""
from pathlib import Path
import re, json, html
import markdown
root=Path(__file__).resolve().parents[1]
site=root/'site'
pages=sorted((root/'docs').glob('[0-9][0-9].md'))
items=[]
for p in pages:
    md=p.read_text(encoding='utf-8')
    title=md.splitlines()[0].lstrip('# ')
    example=re.search(r'构建本章目标：`([^`]+)`',md)
    if example:
        folder=root/'examples'/example.group(1)
        def sync(match):
            source=folder/match.group(1)
            if not source.is_file(): return match.group(0)
            return '### '+match.group(1)+'\n\n```'+match.group(2)+'\n'+source.read_text(encoding='utf-8').rstrip()+'\n```'
        md=re.sub(r'### ([^\n]+)\n\n```([^\n]*)\n.*?\n```',sync,md,flags=re.S)
        p.write_text(md,encoding='utf-8')
    items.append((p,title,md))
nav='<input id="search" type="search" placeholder="搜索章节与正文" aria-label="搜索章节"><p><a href="index.html">课程首页</a></p>'
nav+='<a href="workbook.html">12 个逐步实操实验</a>'
nav+=''.join(f'<a class="chapter" data-i="{i}" href="{p.stem}.html">{html.escape(t)}</a>' for i,(p,t,_) in enumerate(items))
def wrap(title,body):
    return '<!doctype html><html lang="zh-CN"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>'+html.escape(title)+' · Qt 6 中文教程</title><link rel="stylesheet" href="style.css"></head><body><header><div class="tag">C++ · QT 6 · 从入门到项目</div><h1>Qt 6 中文学习手册</h1><p>离线阅读 / 配套源码 / 练习验收 / 固定版本可重现</p></header><div class="shell"><nav>'+nav+'</nav><main>'+body+'</main></div><script src="search.js"></script></body></html>'
for i,(p,t,md) in enumerate(items):
    md=md.replace('(00-目录.md)','(index.html)')
    pager='<div class="pager">'+(f'<a href="{items[i-1][0].stem}.html">← 上一章</a>' if i else '<span></span>')+(f'<a href="{items[i+1][0].stem}.html">下一章 →</a>' if i+1<len(items) else '<a href="index.html">返回首页</a>')+'</div>'
    (site/(p.stem+'.html')).write_text(wrap(t,markdown.markdown(md,extensions=['fenced_code','tables'])+pager),encoding='utf-8')
intro='# 欢迎开始 Qt 6 学习\n\n33 章中文讲解、13 个配套示例、1 个综合项目。环境：Qt 6.8.3 / MSVC 2022 / C++17。先阅读项目 README，再运行 build.cmd。\n\n'+ '\n'.join(f'- [{t}]({p.stem}.html)' for p,t,_ in items)
(site/'index.html').write_text(wrap('首页',markdown.markdown(intro)),encoding='utf-8')
(site/'search.js').write_text('const texts='+json.dumps([(t,md) for _,t,md in items],ensure_ascii=False)+';document.getElementById("search").addEventListener("input",e=>{const q=e.target.value.toLowerCase();document.querySelectorAll("a.chapter").forEach(a=>{a.style.display=texts[Number(a.dataset.i)].join(" ").toLowerCase().includes(q)?"block":"none"})});',encoding='utf-8')
print('Rendered',len(items),'chapters')

workbook=(root/'docs/WORKBOOK.md').read_text(encoding='utf-8').replace('(00-目录.md)','(index.html)')
(site/'workbook.html').write_text(wrap('实操实验手册',markdown.markdown(workbook,extensions=['fenced_code','tables'])),encoding='utf-8')
