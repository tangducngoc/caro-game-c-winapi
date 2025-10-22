# Caro Game – WinAPI C  
> 🇻🇳 Trò chơi cờ Caro (Gomoku) viết bằng ngôn ngữ C sử dụng Win32 API.  
> 🇬🇧 A simple Gomoku (Caro) game written in C using the Win32 API.

---

## 🧩 Tính năng | Features
- 🇻🇳 Giao diện đồ họa đơn giản, dễ nhìn.  
  🇬🇧 Simple and clear graphical interface.  
- 🇻🇳 X (đỏ) đi trước, O (xanh) là người hoặc máy.  
  🇬🇧 X (red) goes first, O (blue) is player or AI.  
- 🇻🇳 Có menu chọn: Chơi 2 người, Chơi với máy, Chơi lại, Thoát.  
  🇬🇧 Menu: 2 Players, Play with AI, Restart, Exit.  
- 🇻🇳 AI chọn ô trống ngẫu nhiên (phiên bản cơ bản).  
  🇬🇧 AI randomly picks empty cells (basic version).  

---

## ⚙️ Cách build & chạy | Build & Run
```bash
gcc main.c -o caro.exe -lgdi32 -luser32 -mwindows
.\caro.exe
# caro-game-c-winapi

---
