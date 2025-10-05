---
tags: [technical, build]
---

# Build and Packaging

## Project Settings
| Setting | Value |
|----------|--------|
| Engine Version | 5.6 |
| Target Platforms | Windows, Linux |
| Build Configuration | Development / Shipping |
| Default Map | Hub_Main.umap |

---

## Automation
- `Kalkibuild.py` script calls `RunUAT BuildCookRun`.  
- Output builds into `/Builds/<version>/`.

---

## Packaging Tips
- Use Pak chunking per mode (Combat, Hub, Overworld).  
- Exclude debug logs from Shipping.  
- Include tag dictionary and rule packs as cooked DataAssets.

---

## Versioning
- Semantic version: `MAJOR.MINOR.PATCH`.  
- Journal schema versions follow same sequence.

---

## Distribution
- Itch.io or Epic Private Release.  
- Upload `.pak` + `.exe` bundle with versioned folder.

See also → [[Production/Release Pipeline]].
