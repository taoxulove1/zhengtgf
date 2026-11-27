Set sh = CreateObject("WScript.Shell")
Set fso = CreateObject("Scripting.FileSystemObject")
hostsPath = "C:\Windows\System32\drivers\etc\hosts"
If Not fso.FileExists(hostsPath & ".bak-zt") Then
    fso.CopyFile hostsPath, hostsPath & ".bak-zt"
End If
line = vbCrLf & "# --- zhengtu launcher test ---" & vbCrLf & _
       "127.0.0.1 srvlistall.ztgame.com.cn" & vbCrLf & _
       "127.0.0.1 zthj.ztgame.com" & vbCrLf & _
       "127.0.0.1 www.ztgame.com.cn" & vbCrLf & _
       "127.0.0.1 pay.ztgame.com" & vbCrLf & _
       "127.0.0.1 account.ztgame.com.cn" & vbCrLf & _
       "127.0.0.1 counter.ztgame.com"
fso.OpenTextFile(hostsPath, 8).Write line
WScript.Echo "hosts updated"
