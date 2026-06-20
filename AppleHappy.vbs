Option Explicit

Dim fso, shell, htaFile, f

Set fso = CreateObject("Scripting.FileSystemObject")
Set shell = CreateObject("WScript.Shell")

htaFile = fso.GetSpecialFolder(2) & "\fireworks.hta"

Set f = fso.CreateTextFile(htaFile, True)

' ======================
' HTA HEADER
' ======================
f.WriteLine "<html>"
f.WriteLine "<head>"
f.WriteLine "<title>APPLE HAPPY 20TH ANNIVERSARY OF THE iPHONE!</title>"

f.WriteLine "<meta http-equiv='X-UA-Compatible' content='IE=11' />"

f.WriteLine "<style>"
f.WriteLine "html,body{margin:0;padding:0;overflow:hidden;width:100%;height:100%;background:black;}"
f.WriteLine "canvas{display:block;}"
f.WriteLine "</style>"

f.WriteLine "</head>"
f.WriteLine "<body>"

f.WriteLine "<canvas id='c'></canvas>"

' ======================
' JAVASCRIPT (IE SAFE)
' ======================
f.WriteLine "<script language='javascript'>"

f.WriteLine "window.onload = function () {"

f.WriteLine "  var c = document.getElementById('c');"
f.WriteLine "  if (!c) return;"

f.WriteLine "  var ctx = c.getContext('2d');"
f.WriteLine "  if (!ctx) return;"

' ---- FULLSCREEN FIX ----
f.WriteLine "  function resize(){"
f.WriteLine "    c.width = window.innerWidth;"
f.WriteLine "    c.height = window.innerHeight;"
f.WriteLine "  }"

f.WriteLine "  window.onresize = resize;"
f.WriteLine "  resize();"
f.WriteLine "  setTimeout(resize,50);"
f.WriteLine "  setTimeout(resize,200);"

f.WriteLine "  var particles = [];"

' ---- SPAWN ----
f.WriteLine "  function spawn(){"
f.WriteLine "    particles.push({"
f.WriteLine "      x: Math.random()*c.width,"
f.WriteLine "      y: Math.random()*c.height,"
f.WriteLine "      vx: (Math.random()-0.5)*4,"
f.WriteLine "      vy: (Math.random()-0.5)*4,"
f.WriteLine "      alpha: 1,"
f.WriteLine "      color: 'rgb(' + Math.floor(Math.random()*255) + ',' + Math.floor(Math.random()*255) + ',' + Math.floor(Math.random()*255) + ')'"
f.WriteLine "    });"
f.WriteLine "  }"

' ---- UPDATE ----
f.WriteLine "  function update(){"
f.WriteLine "    ctx.fillStyle = 'rgba(0,0,0,0.25)';"
f.WriteLine "    ctx.fillRect(0,0,c.width,c.height);"

f.WriteLine "    var alive = [];"

f.WriteLine "    for (var i=0;i<particles.length;i++){"
f.WriteLine "      var p = particles[i];"

f.WriteLine "      p.x += p.vx;"
f.WriteLine "      p.y += p.vy;"
f.WriteLine "      p.alpha -= 0.01;"

f.WriteLine "      if (p.alpha > 0){"
f.WriteLine "        ctx.globalAlpha = p.alpha;"
f.WriteLine "        ctx.fillStyle = p.color;"
f.WriteLine "        ctx.fillRect(p.x,p.y,3,3);"
f.WriteLine "        alive.push(p);"
f.WriteLine "      }"
f.WriteLine "    }"

f.WriteLine "    particles = alive;"
f.WriteLine "    ctx.globalAlpha = 1;"
f.WriteLine "  }"

' ---- LOOP ----
f.WriteLine "  setInterval(spawn, 120);"
f.WriteLine "  setInterval(update, 30);"

f.WriteLine "};"

f.WriteLine "</script>"

f.WriteLine "</body>"
f.WriteLine "</html>"

f.Close

' ======================
' RUN HTA
' ======================
shell.Run "mshta.exe """ & htaFile & """", 1, False

' ======================
' WAIT FOR CLOSE THEN RICKROLL
' ======================
Do While IsProcessRunning("mshta.exe")
    WScript.Sleep 300
Loop

shell.Run "cmd /c start """" https://www.youtube.com/watch?v=dQw4w9WgXcQ", 1, False

' ======================
' PROCESS CHECK
' ======================
Function IsProcessRunning(procName)
    Dim wmi, list
    Set wmi = GetObject("winmgmts:\\.\root\cimv2")
    Set list = wmi.ExecQuery("Select * from Win32_Process Where Name='" & procName & "'")
    IsProcessRunning = (list.Count > 0)
End Function