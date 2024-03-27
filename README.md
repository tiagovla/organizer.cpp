# Organizer.cpp

# Installation

```bash
cmake -B build .
cmake --build build
cmake --install build
```

# Usage

```bash
organizer
# organizer path (defaults to ~/Downloads)
```

# Config

```toml
# ~/.config/organizer.toml
[folders]
documents = [".pdf", ".mobi", ".epub", ".docx", ".txt", ".djvu"]
music = [".mp3"]
videos = [".mp4", ".mkv", ".avi", ".mov", ".flv", ".wmv", ".webm", ".webp"]
images = [".jpg", ".jpeg", ".svg"]
compressed = [".zip", ".rar", ".gz", ".bz2", ".7z"]
```
