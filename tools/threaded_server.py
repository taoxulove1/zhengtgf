#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""并发 HTTP 静态服务器（替代 python3 -m http.server 单线程版）。

针对 patchupdate.exe（WININET 多连接并发下载）的需求：
  1. ThreadingMixIn 多线程，每连接一线程；
  2. request_queue_size=128（默认 5，patchupdate 并发连接直接被拒/超时）；
  3. 支持 Range 请求（206/Content-Range，断点续传必需）；
  4. HTTP/1.1 keep-alive，Content-Length 全量回传；
  5. query string 剥离（?CRC= / ?time= / ?versionsign=）。
兼容 Python 3.6，无第三方依赖。用法：threaded_server.py <port> <rootdir>
"""
import os
import re
import socketserver
import sys
from http.server import SimpleHTTPRequestHandler
from socketserver import ThreadingMixIn

try:
    from http.server import ThreadingHTTPServer  # py3.7+
    BASE = ThreadingHTTPServer
except ImportError:
    class BASE(ThreadingMixIn, socketserver.TCPServer):
        daemon_threads = True
        allow_reuse_address = True
        request_queue_size = 128


class PatchHandler(SimpleHTTPRequestHandler):
    protocol_version = "HTTP/1.1"

    # 剥离 query string
    def translate_path(self, path):
        path = path.split("?", 1)[0].split("#", 1)[0]
        return SimpleHTTPRequestHandler.translate_path(self, path)

    # Range 支持：单区间 bytes=a-b / a- / -suffix
    def send_head(self):
        rng = self.headers.get("Range")
        if not rng:
            return SimpleHTTPRequestHandler.send_head(self)
        path = self.translate_path(self.path)
        if os.path.isdir(path):
            return SimpleHTTPRequestHandler.send_head(self)
        try:
            f = open(path, "rb")
        except OSError:
            self.send_error(404, "File not found")
            return None
        try:
            fs = os.fstat(f.fileno())
            m = re.match(r"bytes=(\d*)-(\d*)", rng.strip())
            if not m:
                f.close()
                self.send_error(416, "Bad Range")
                return None
            a, b = m.group(1), m.group(2)
            size = fs.st_size
            if a == "" and b == "":
                f.close(); self.send_error(416); return None
            if a == "":
                start = max(0, size - int(b)); end = size - 1
            else:
                start = int(a); end = int(b) if b != "" else size - 1
            if start > end or start >= size:
                f.close()
                self.send_error(416, "Range Not Satisfiable")
                return None
            end = min(end, size - 1)
            self.send_response(206)
            self.send_header("Content-Type", self.guess_type(path))
            self.send_header("Content-Range", "bytes %d-%d/%d" % (start, end, size))
            self.send_header("Content-Length", str(end - start + 1))
            self.send_header("Accept-Ranges", "bytes")
            self.send_header("Last-Modified", self.date_time_string(int(fs.st_mtime)))
            self.end_headers()
            f.seek(start)
            self._range_left = end - start + 1
            self._range_file = f
            return _RangeFile(f, end - start + 1)
        except Exception:
            f.close()
            raise

    def copyfile(self, src, dst):
        if isinstance(src, _RangeFile):
            left = src.left
            f = src.f
            while left > 0:
                chunk = f.read(min(65536, left))
                if not chunk:
                    break
                dst.write(chunk)
                left -= len(chunk)
            f.close()
            return
        return SimpleHTTPRequestHandler.copyfile(self, src, dst)

    def log_message(self, fmt, *args):
        sys.stderr.write("%s - [%s] %s\n" % (self.address_string(),
                         self.log_date_time_string(), fmt % args))


class _RangeFile(object):
    def __init__(self, f, left):
        self.f = f
        self.left = left


def main():
    port = int(sys.argv[1]) if len(sys.argv) > 1 else 80
    root = sys.argv[2] if len(sys.argv) > 2 else "/root/zt/www"
    os.chdir(root)
    srv = BASE(("", port), PatchHandler)
    print("threaded http server on %d root=%s" % (port, root))
    srv.serve_forever()


if __name__ == "__main__":
    main()
