#!/bin/bash
# P3-WWW: append official-launcher HTTP startup into /root/native_boot.sh (idempotent)
set -e
F=/root/native_boot.sh
if grep -q "P3-WWW" "$F" 2>/dev/null; then
  echo "P3-WWW already present"; exit 0
fi
cat >> "$F" <<'EOF'

# --- P3-WWW: official launcher HTTP (port 80) ---
if ! ss -lnt 2>/dev/null | grep -q ':80 '; then
  (cd /root/zt/www && nohup python3 -m http.server 80 </dev/null > http.log 2>&1 &)
  sleep 1
fi
EOF
echo "appended P3-WWW block to $F"
tail -8 "$F"
