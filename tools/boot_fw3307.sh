# --- ZT: 3307 只允许本机（MySQL 不对外） ---
iptables -D INPUT -i lo -p tcp --dport 3307 -j ACCEPT 2>/dev/null
iptables -D INPUT -p tcp --dport 3307 -j DROP 2>/dev/null
iptables -A INPUT -i lo -p tcp --dport 3307 -j ACCEPT
iptables -A INPUT -p tcp --dport 3307 -j DROP
