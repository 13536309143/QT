"""Check route coverage, code fences and offline links without network."""
from html.parser import HTMLParser
from pathlib import Path
from urllib.parse import unquote, urlsplit
import json
import re

ROOT = Path(__file__).resolve().parents[1]
errors = []
stages = json.loads((ROOT / 'tools/course.json').read_text(encoding='utf-8'))['stages']
ids = [c for s in stages for c in s['chapters']]
actual = {p.stem for p in (ROOT / 'docs').glob('[0-9][0-9].md')}
if len(ids) != len(set(ids)) or set(ids) != actual:
    errors.append('Route must list every chapter once')
if ids != [f'{i:02d}' for i in range(len(ids))]:
    errors.append('Chapter numbers must follow the learning route without gaps')
for chapter in actual:
    title = (ROOT / f'docs/{chapter}.md').read_text(encoding='utf-8').splitlines()[0]
    if not title.startswith(f'# {chapter} · '):
        errors.append(f'{chapter}.md: heading number differs from filename')

class Page(HTMLParser):
    def __init__(self, text):
        super().__init__()
        self.links, self.ids = [], set()
        self.feed(text)
    def handle_starttag(self, tag, attrs):
        attrs = dict(attrs)
        if 'id' in attrs: self.ids.add(attrs['id'])
        for key in ('href', 'src'):
            if key in attrs: self.links.append(attrs[key])

pages = {p: Page(p.read_text(encoding='utf-8')) for p in (ROOT / 'site').glob('*.html')}
for path, page in pages.items():
    for url in page.links:
        parsed = urlsplit(url)
        if parsed.scheme or parsed.netloc: continue
        target = (path.parent / unquote(parsed.path)).resolve() if parsed.path else path
        if not target.exists():
            errors.append(f'{path.relative_to(ROOT)}: missing {url}')
        elif parsed.fragment and target in pages and unquote(parsed.fragment) not in pages[target].ids:
            errors.append(f'{path.relative_to(ROOT)}: missing anchor {url}')
for path in (ROOT / 'docs').glob('*.md'):
    text = path.read_text(encoding='utf-8')
    if len(re.findall(r'^```', text, re.M)) % 2:
        errors.append(f'{path.name}: unclosed code fence')
    example = re.search(r'构建本章目标：`([^`]+)`', text)
    if example:
        for match in re.finditer(r'### ([^\n]+)\n\n```[^\n]*\n(.*?)\n```', text, re.S):
            source = ROOT / 'examples' / example[1] / match[1]
            if source.is_file() and source.read_text(encoding='utf-8').rstrip() != match[2].rstrip():
                errors.append(f'{path.name}: embedded source differs from {source.relative_to(ROOT)}')
    prose = re.sub(r'^```.*?^```', '', text, flags=re.M | re.S)
    for url in re.findall(r'\[[^\]]*\]\(([^\s)]+)\)', prose):
        parsed = urlsplit(url)
        if parsed.scheme or parsed.netloc or not parsed.path: continue
        if not (path.parent / unquote(parsed.path)).exists():
            errors.append(f'{path.name}: missing {url}')
for c in ids:
    if not (ROOT / f'site/{c}.html').exists(): errors.append(f'Missing rendered chapter {c}')
if errors: raise SystemExit('\n'.join(errors))
print(f'PASS {len(ids)} route chapters, {len(pages)} HTML pages, Markdown fences and local links')
