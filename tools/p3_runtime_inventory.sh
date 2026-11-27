#!/usr/bin/env bash
# Check the existing P3 build and record the target host's runtime capabilities.
set -euo pipefail
export LC_ALL=C PYTHONIOENCODING=utf-8

fail() {
    printf '[FAIL][INVENTORY] %s\n' "$*" >&2
    exit 1
}

[[ $(uname -s) == Linux ]] || fail 'Requires the Linux build host'
[[ $# == 1 && -r "$1" ]] || fail 'Usage: bash p3_runtime_inventory.sh P3_BUILD_LOG'
for tool in ldd ss sha256sum python3; do
    command -v "$tool" >/dev/null || fail "Missing runtime inventory tool: $tool"
done

log=$1
build_root=$(sed -n 's/^\[P3_BUILD_READY\] //p' "$log" | tail -n 1)
if [[ -z "$build_root" ]]; then
    # A repeated build can overwrite the success log after finding the output directory.
    work=$(sed -n 's/^\[WORKDIR\] //p' "$log" | tail -n 1)
    [[ -n "$work" ]] || fail "No build directory found in $log"
    build_root="$work/zhengtugf-p3-934bf0d/build/p3-native"
fi
[[ "$build_root" == /* && "$build_root" == */build/p3-native && -d "$build_root" ]] ||
    fail "Invalid P3 artifact directory: $build_root"
printf '[ARTIFACT_DIR] %s\n' "$build_root"

repo=${build_root%/build/p3-native}
python3 "$repo/tools/validate_repo.py" --phase P3 || fail 'P3 ELF artifact gate failed'
services=(SuperServer FLServer RecordServer BillServer MiniServer SessionServer ScenesServer GatewayServer)
missing_libraries=0
for service in "${services[@]}"; do
    binary="$build_root/$service"
    digest=$(sha256sum -- "$binary")
    printf '[ELF] %s sha256=%s\n' "$service" "${digest%% *}"
    if ! libraries=$(ldd "$binary" 2>&1); then
        printf '[FAIL][LIBS] %s\n%s\n' "$service" "$libraries"
        missing_libraries=1
    elif grep -E 'not found' <<< "$libraries"; then
        printf '[FAIL][LIBS] %s\n' "$service"
        missing_libraries=1
    fi
done
(( missing_libraries == 0 )) || fail 'One or more services have unresolved runtime libraries'
printf '[OK][ELF_AND_LIBS] 8 x86 Linux executables and their runtime libraries\n'

printf '[HOST][PORTS] Existing TCP listeners on game and database ports\n'
listeners=$(ss -lntp) || fail 'Could not read TCP listeners'
awk '$4 ~ /:(3306|3307|7000|7001|7002|8000|8001|8002)$/ {print}' <<< "$listeners"

if command -v unshare >/dev/null && command -v ip >/dev/null && unshare -n true 2>/dev/null; then
    printf '[OK][NETNS] Can create a private network namespace\n'
else
    printf '[P3_PENDING][NETNS] Private network namespace is unavailable\n'
fi

db_tools=0
for tool in mysqld mariadbd mysql_install_db mariadb-install-db mysql mariadb; do
    if path=$(command -v "$tool"); then
        printf '[HOST][DB_TOOL] %s %s\n' "$tool" "$path"
        db_tools=$((db_tools + 1))
    fi
done
if (( db_tools == 0 )); then printf '[HOST][DB_TOOL] none on PATH\n'; fi
printf '[P3_PENDING][RUNTIME] Listener identity, protocol comparison, and 100 exchanges\n'
