# GitHub 发布前准备

本页介绍如何将课程仓库发布到 GitHub，以及发布前应完成的检查。

## 先检查内容

1. 阅读 README、LICENSE 和 THIRD_PARTY_NOTICES，确认所选 MIT 许可符合你的发布意图。
2. 运行 scripts/build.cmd 和 scripts/check.ps1。
3. 检查暂存列表不含 sdk、.tools、build、dist、数据库、日志、令牌或私人文件。
4. 可将 README 的展示名称调整为你最终仓库名。没有添加虚假的 CI 徽章或不存在的远程链接。
5. 确认 GitHub Actions 在你的仓库允许执行，下载 Qt 需要访问外部网络。

## 当你决定上传时再执行

在 GitHub 新建空仓库，不勾选自动生成 README。执行以下命令前，将 YOUR_NAME 和 YOUR_REPO 替换为实际账号与仓库名。

```powershell
git init -b main
git add .
git status --short
git diff --cached --stat
git commit -m "Add Qt 6 Chinese course and runnable examples"
git remote add origin https://github.com/YOUR_NAME/YOUR_REPO.git
git push -u origin main
```

如果当前项目已经是独立 Git 仓库，跳过 init；若只是外层目录属于其他仓库，先确认你希望的独立仓库边界，避免把父目录无关内容提交。作者身份按你自己的 Git 配置处理。

## 网页课程更新

教程的 Markdown 是可编辑内容，site 是已生成静态阅读版。修改 Markdown 或示例后，执行 `python -m pip install -r tools/requirements-docs.txt`，再执行 `python tools/render_site.py` 同步网页，然后检查链接与版式。不会运行自动发布或上传。

可选 GitHub Pages：将 site 内容作为静态站点发布。SDK 和构建目录不属于网页站点内容。
