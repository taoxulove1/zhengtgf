#!/usr/bin/env bash
# protect-branch.sh — 一键下发仓库规则（幂等，可重复执行）
# 依赖: curl；不需要 gh CLI
# 用法:
#   export GITHUB_TOKEN=ghp_xxx        # 需 repo 管理员权限
#   export REPO=taoxulove1/zhengtugf
#   bash tools/protect-branch.sh
#
# 覆盖: 仓库合并策略 / 分支保护 / 标签体系
set -euo pipefail

: "${GITHUB_TOKEN:?请先 export GITHUB_TOKEN（需 admin:repo 权限）}"
REPO="${REPO:-taoxulove1/zhengtugf}"
BRANCH="${BRANCH:-main}"
API="https://api.github.com/repos/${REPO}"
HDR=(-H "Authorization: Bearer ${GITHUB_TOKEN}" -H "Accept: application/vnd.github+json" -H "X-GitHub-Api-Version: 2022-11-28")

say() { printf '\n==> %s\n' "$*"; }

# ---------- 1. 仓库级合并策略：只留 squash ----------
say "仓库合并策略"
curl -sS -X PATCH "${API}" "${HDR[@]}" -d '{
  "allow_squash_merge": true,
  "allow_merge_commit": false,
  "allow_rebase_merge": false,
  "allow_auto_merge": true,
  "delete_branch_on_merge": true,
  "squash_merge_commit_title": "PR_TITLE",
  "squash_merge_commit_message": "PR_BODY"
}' | python -c "import sys,json;d=json.load(sys.stdin);print('  squash=%s merge=%s rebase=%s auto=%s'%(d.get('allow_squash_merge'),d.get('allow_merge_commit'),d.get('allow_rebase_merge'),d.get('allow_auto_merge')))"

# ---------- 2. 分支保护 ----------
say "分支保护 ${BRANCH}"
curl -sS -X PUT "${API}/branches/${BRANCH}/protection" "${HDR[@]}" -d '{
  "required_status_checks": {
    "strict": true,
    "contexts": ["validate"]
  },
  "enforce_admins": true,
  "required_pull_request_reviews": {
    "dismiss_stale_reviews": true,
    "require_code_owner_reviews": true,
    "required_approving_review_count": 1,
    "require_last_push_approval": true
  },
  "restrictions": null,
  "required_linear_history": true,
  "allow_force_pushes": false,
  "allow_deletions": false,
  "required_conversation_resolution": true,
  "lock_branch": false,
  "allow_fork_syncing": false
}' | python -c "
import sys,json
d=json.load(sys.stdin)
if 'message' in d: print('  FAIL',d['message'])
else: print('  OK admins=%s linear=%s reviews=%s' % (d['enforce_admins']['enabled'], d['required_linear_history']['enabled'], d['required_pull_request_reviews']['required_approving_review_count']))
"

# ---------- 3. 标签体系 ----------
say "标签"
labels=(
  "task:run|1D76DB|已就绪待执行"
  "task:dispatch|FBCA04|请求下发任务"
  "data:change|0E8A16|资产或数据变更"
  "phase:0|5319E7|阶段 P0 收资"
  "phase:1|5319E7|阶段 P1 抓包解密"
  "phase:2|5319E7|阶段 P2 协议文档"
  "phase:3|5319E7|阶段 P3 服务端还原"
  "phase:4|5319E7|阶段 P4 数据与部署"
  "phase:5|5319E7|阶段 P5 联调"
  "phase:6|5319E7|阶段 P6 交付"
  "bug|D73A4A|可复现缺陷"
  "blocked|B60205|阻塞"
)
for item in "${labels[@]}"; do
  IFS='|' read -r name color desc <<< "$item"
  code=$(curl -sS -o /dev/null -w '%{http_code}' -X POST "${API}/labels" "${HDR[@]}" \
    -d "{\"name\":\"${name}\",\"color\":\"${color}\",\"description\":\"${desc}\"}")
  case "$code" in
    201) echo "  + ${name}" ;;
    422) echo "  = ${name}（已存在）" ;;
    *)   echo "  ! ${name} http=${code}" ;;
  esac
done

# ---------- 4. 验证 ----------
say "回读校验"
curl -sS "${API}/branches/${BRANCH}/protection" "${HDR[@]}" | python -c "
import sys,json
d=json.load(sys.stdin)
if 'message' in d:
    print('  未生效:',d['message']); raise SystemExit(1)
print('  状态检查:',[c['context'] for c in d['required_status_checks']['contexts']] if d.get('required_status_checks') else 'none')
print('  管理员强制:',d['enforce_admins']['enabled'])
print('  线性历史:',d['required_linear_history']['enabled'])
print('  强制推送:',d['allow_force_pushes']['enabled'])
"
echo
echo "完成。Actions 回写 state/queue.json 需把 github-actions[bot] 加入 bypass："
echo "  Settings → Rules → 该保护规则 → Bypass list → Add → GitHub Actions"
