---
tags:
  - kalki/git
  - kalki/workflow
  - kalki/collaboration
status: complete
phase: foundation
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: beginner
---

# Version Control Guidelines

## Overview
This document defines version control practices, Git workflows, and collaboration standards for the Kalki project. Following these guidelines ensures smooth team collaboration, prevents conflicts, and maintains a clean project history.

---

## Purpose

### Why Version Control Guidelines Matter

**Problems without version control standards:**
1. **Merge Conflicts** - Constant conflicts, lost work
2. **Unclear History** - Can't understand what changed when
3. **Lost Work** - Accidentally overwritten changes
4. **Broken Builds** - Committed code that doesn't compile
5. **Difficult Collaboration** - Team members stepping on each other

**Benefits of good version control:**
- ✅ Clear project history
- ✅ Easy collaboration
- ✅ Protect against data loss
- ✅ Ability to revert changes
- ✅ Track who changed what and why
- ✅ Stable main branch

---

## Git Setup

### Repository Structure
```
Kalki/
├── .git/                   # Git metadata (don't touch)
├── .gitignore             # Files to ignore
├── .gitattributes         # Git attributes (LFS, line endings)
├── README.md              # Project overview
├── LICENSE                # Project license
├── CHANGELOG.md           # Version history
├── .github/
│   ├── workflows/         # CI/CD workflows
│   └── PULL_REQUEST_TEMPLATE.md
├── Source/                # C++ source code
├── Content/               # Unreal assets (LFS)
├── Config/                # Configuration files
├── Plugins/               # Project plugins
├── Saved/                 # Generated files (ignored)
├── Intermediate/          # Build files (ignored)
└── Kalki_GDD/            # Game design documentation
```

---

### .gitignore

**Essential .gitignore entries:**
```gitignore
# Unreal Engine
Binaries/
Build/
DerivedDataCache/
Intermediate/
Saved/
*.opensdf
*.sdf
*.sln
*.suo
*.VC.db
*.VC.opendb

# Visual Studio
.vs/
*.user
*.userosscache

# Rider
.idea/
*.DotSettings.user

# OS Files
.DS_Store
Thumbs.db
Desktop.ini

# Unreal specific
*.log
*.tmp

# Don't ignore
!Build/BatchFiles/
!Build/Scripts/
```

**Why these entries?**
- `Binaries/` - Generated on build, large
- `Intermediate/` - Temporary build files, large
- `Saved/` - Logs, screenshots, auto-saves
- `.vs/` - IDE-specific settings
- OS files - System-generated, not project files

---

### Git LFS (Large File Storage)

**What is Git LFS?**
- Handles large files (textures, meshes, audio)
- Stores large files separately
- Repository stays small
- Required for Unreal projects

**Setup:**
```bash
# Install Git LFS
git lfs install

# Track large file types
git lfs track "*.uasset"
git lfs track "*.umap"
git lfs track "*.upk"
git lfs track "*.png"
git lfs track "*.jpg"
git lfs track "*.tga"
git lfs track "*.fbx"
git lfs track "*.wav"
git lfs track "*.mp3"
git lfs track "*.3ds"
git lfs track "*.psd"
git lfs track "*.mb"
git lfs track "*.ma"

# Verify .gitattributes created
cat .gitattributes
```

**.gitattributes example:**
```gitattributes
# Unreal Engine
*.uasset filter=lfs diff=lfs merge=lfs -text
*.umap filter=lfs diff=lfs merge=lfs -text
*.upk filter=lfs diff=lfs merge=lfs -text

# Textures
*.png filter=lfs diff=lfs merge=lfs -text
*.jpg filter=lfs diff=lfs merge=lfs -text
*.tga filter=lfs diff=lfs merge=lfs -text
*.bmp filter=lfs diff=lfs merge=lfs -text
*.psd filter=lfs diff=lfs merge=lfs -text

# Audio
*.wav filter=lfs diff=lfs merge=lfs -text
*.mp3 filter=lfs diff=lfs merge=lfs -text
*.ogg filter=lfs diff=lfs merge=lfs -text

# Models
*.fbx filter=lfs diff=lfs merge=lfs -text
*.obj filter=lfs diff=lfs merge=lfs -text
*.3ds filter=lfs diff=lfs merge=lfs -text

# Maya/Blender
*.mb filter=lfs diff=lfs merge=lfs -text
*.ma filter=lfs diff=lfs merge=lfs -text
*.blend filter=lfs diff=lfs merge=lfs -text

# Line endings (important for cross-platform)
*.cpp text eol=lf
*.h text eol=lf
*.cs text eol=lf
*.ini text eol=lf
*.md text eol=lf
```

---

## Branching Strategy

### Branch Types

**Main Branches:**
```
main (or master)
└── Protected branch
    └── Always deployable
    └── Never commit directly
    └── Only merge from develop

develop
└── Integration branch
    └── Latest development
    └── Merge feature branches here
    └── Test before merging to main
```

**Supporting Branches:**
```
feature/feature-name
└── New features
└── Branched from: develop
└── Merge back to: develop

bugfix/bug-description
└── Bug fixes
└── Branched from: develop
└── Merge back to: develop

hotfix/critical-fix
└── Critical production bugs
└── Branched from: main
└── Merge to: main AND develop

release/version-number
└── Release preparation
└── Branched from: develop
└── Merge to: main AND develop
```

---

### Branch Naming

**Convention:**
```
[type]/[description]

Types:
- feature/   New feature
- bugfix/    Bug fix
- hotfix/    Critical fix
- release/   Release preparation
- docs/      Documentation only
- refactor/  Code refactoring
- test/      Testing/experimental
```

**Examples:**
```
✅ GOOD:
feature/grid-elevation-system
feature/combat-log-widget
bugfix/character-movement-desync
hotfix/crash-on-ability-cast
release/0.2.0
docs/update-api-reference
refactor/grid-manager-cleanup

❌ BAD:
new-feature
fix
johns-branch
test123
wip
```

**Rules:**
1. **Lowercase** - All lowercase letters
2. **Hyphens** - Separate words with `-`
3. **Descriptive** - Clear what branch does
4. **Type prefix** - Always include type
5. **No spaces** - Use hyphens instead
6. **Concise** - Keep reasonably short

---

## Commit Guidelines

### Commit Message Format

**Structure:**
```
<type>(<scope>): <subject>

[optional body]

[optional footer]
```

**Types:**

| Type | Description | Example |
|------|-------------|---------|
| feat | New feature | feat(grid): add elevation system |
| fix | Bug fix | fix(combat): resolve damage calculation |
| docs | Documentation | docs(api): update GridManager reference |
| style | Code style (no logic change) | style(grid): format code per standards |
| refactor | Code refactoring | refactor(grid): simplify tile lookup |
| perf | Performance improvement | perf(grid): optimize neighbor queries |
| test | Add/update tests | test(grid): add coordinate validation tests |
| chore | Maintenance | chore: update .gitignore |
| build | Build system | build: update Unreal to 5.3 |
| ci | CI/CD changes | ci: add automated tests |
| revert | Revert previous commit | revert: feat(grid): add elevation system |

---

### Commit Message Examples

**Good Commit Messages:**
```
feat(grid): implement elevation system

Add support for variable tile elevation:
- Add Elevation property to GridTile
- Update visualization to show elevation
- Add SetElevation() function to GridManager
- Update pathfinding to consider elevation

Closes #42

---

fix(combat): resolve turn order bug

Fixed issue where characters with same initiative
would sometimes be skipped.

- Sort by initiative then by spawn order
- Add test cases for tie-breaking
- Update CombatManager documentation

Fixes #87

---

docs(grid): update coordinate system guide

- Add visual examples of coordinate conventions
- Document distance calculation methods
- Include code examples for common operations

---

refactor(grid): simplify GetNeighbors implementation

Replace nested loops with cleaner direction array.
No functional changes, just cleaner code.

---

perf(grid): optimize tile queries

- Cache frequently accessed tiles
- Use TMap for O(1) lookup instead of O(n) array search
- Reduce memory allocations in hot paths

Performance improvement: ~30% faster GetTile() calls

---

test(grid): add comprehensive coordinate tests

- Test coordinate construction
- Test equality operators
- Test Manhattan distance calculation
- Test neighbor queries
```

---

### Commit Best Practices

**Do's:**
1. **Atomic commits** - One logical change per commit
2. **Descriptive messages** - Explain what and why
3. **Test before committing** - Ensure code compiles
4. **Commit often** - Small, frequent commits
5. **Reference issues** - Include issue numbers

**Don'ts:**
1. **Don't commit broken code** - Must compile
2. **Don't commit commented-out code** - Delete it
3. **Don't commit generated files** - Use .gitignore
4. **Don't commit secrets** - API keys, passwords
5. **Don't use vague messages** - "fix", "update", "wip"

---

### Commit Checklist

**Before Committing:**
- [ ] Code compiles without errors
- [ ] Code compiles without warnings
- [ ] Tests pass (if applicable)
- [ ] Code follows style guidelines
- [ ] No debug logging left in
- [ ] No commented-out code
- [ ] Files properly named
- [ ] Changes documented (if needed)
- [ ] Commit message is clear
- [ ] Related files included

---

## Workflow

### Feature Development Workflow

**1. Create Feature Branch:**
```bash
# Update develop branch
git checkout develop
git pull origin develop

# Create feature branch
git checkout -b feature/grid-elevation-system
```

---

**2. Develop Feature:**
```bash
# Make changes
# Test changes

# Check what changed
git status
git diff

# Stage changes
git add Source/Grid/GridManager.h
git add Source/Grid/GridManager.cpp
git add Content/Grid/Materials/

# Or stage all
git add .

# Commit
git commit -m "feat(grid): add elevation property to tiles"

# Continue developing...
git add .
git commit -m "feat(grid): implement SetElevation function"

git add .
git commit -m "feat(grid): update visualizer for elevation"

# Push to remote
git push origin feature/grid-elevation-system
```

---

**3. Keep Branch Updated:**
```bash
# Periodically sync with develop
git checkout develop
git pull origin develop

git checkout feature/grid-elevation-system
git merge develop

# Resolve any conflicts
# Test after merge
git add .
git commit -m "merge: sync with develop"
git push origin feature/grid-elevation-system
```

---

**4. Create Pull Request:**

On GitHub/GitLab:
1. Navigate to repository
2. Click "New Pull Request"
3. Select: `feature/grid-elevation-system` → `develop`
4. Fill out PR template
5. Assign reviewers
6. Submit

---

**5. Code Review:**

Reviewer checks:
- [ ] Code quality
- [ ] Follows conventions
- [ ] Tests included
- [ ] Documentation updated
- [ ] No breaking changes
- [ ] Performance acceptable

---

**6. Merge:**

After approval:
```bash
# Squash and merge on GitHub (recommended)
# Or locally:
git checkout develop
git merge --no-ff feature/grid-elevation-system
git push origin develop

# Delete feature branch
git branch -d feature/grid-elevation-system
git push origin --delete feature/grid-elevation-system
```

---

### Hotfix Workflow

**For Critical Bugs in Production:**
```bash
# Create hotfix from main
git checkout main
git pull origin main
git checkout -b hotfix/crash-on-startup

# Fix bug
git add .
git commit -m "fix: resolve crash on startup"

# Test thoroughly!

# Merge to main
git checkout main
git merge --no-ff hotfix/crash-on-startup
git tag v0.1.1
git push origin main --tags

# Also merge to develop
git checkout develop
git merge --no-ff hotfix/crash-on-startup
git push origin develop

# Delete hotfix branch
git branch -d hotfix/crash-on-startup
git push origin --delete hotfix/crash-on-startup
```

---

## Pull Request Guidelines

### PR Template
```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update

## Related Issues
Closes #42
Relates to #38

## Changes Made
- Added elevation system to grid
- Updated GridManager API
- Added tests for elevation
- Updated documentation

## Testing
- [ ] Compiled successfully
- [ ] Existing tests pass
- [ ] New tests added
- [ ] Manually tested in editor

## Screenshots (if applicable)
[Add screenshots here]

## Checklist
- [ ] Code follows style guidelines
- [ ] Self-review completed
- [ ] Comments added for complex code
- [ ] Documentation updated
- [ ] No warnings generated
- [ ] Dependent changes merged

## Additional Notes
Any extra information for reviewers
```

---

### Code Review Process

**Reviewer Responsibilities:**

1. **Check Code Quality:**
   - Follows coding standards
   - No code smells
   - Proper error handling
   - Appropriate comments

2. **Check Functionality:**
   - Does what it claims
   - No regressions
   - Edge cases handled
   - Performance acceptable

3. **Check Tests:**
   - Adequate test coverage
   - Tests actually test the feature
   - Tests pass

4. **Provide Feedback:**
   - Be constructive
   - Explain reasoning
   - Suggest alternatives
   - Approve or request changes

**Feedback Examples:**
```
✅ GOOD:
"Consider using a TMap here instead of TArray for O(1) lookup
instead of O(n). See GridManager::GetTile() for an example."

"Great implementation! One suggestion: extract this logic
into a separate function for reusability."

❌ BAD:
"This is wrong."
"Fix this."
"Bad code."
```

---

## Merge Conflicts

### Preventing Conflicts

**Best Practices:**
1. **Sync frequently** - Pull develop regularly
2. **Small features** - Keep branches short-lived
3. **Communicate** - Tell team what you're working on
4. **Review .gitignore** - Don't commit generated files

---

### Resolving Conflicts

**When Conflict Occurs:**
```bash
# Pull latest changes
git checkout develop
git pull origin develop

# Try to merge
git checkout feature/my-feature
git merge develop

# If conflicts:
Auto-merging Source/Grid/GridManager.cpp
CONFLICT (content): Merge conflict in Source/Grid/GridManager.cpp
Automatic merge failed; fix conflicts and then commit the result.
```

**Resolve Manually:**
```cpp
// GridManager.cpp
<<<<<<< HEAD
// Your changes
void UKalkiGridManager::SetElevation(const FKalkiGridCoord& Coord, float Elevation)
{
    // Your implementation
}
=======
// Their changes
void UKalkiGridManager::UpdateElevation(const FKalkiGridCoord& Coord, float NewElevation)
{
    // Their implementation
}
>>>>>>> develop
```

**Steps:**
1. Open conflicted file
2. Look for conflict markers (`<<<<<<<`, `=======`, `>>>>>>>`)
3. Decide what to keep:
   - Keep yours
   - Keep theirs
   - Combine both
   - Write new solution
4. Remove conflict markers
5. Test changes
6. Stage and commit
```bash
# After resolving
git add Source/Grid/GridManager.cpp
git commit -m "merge: resolve conflicts with develop"
git push origin feature/my-feature
```

---

### Conflict Resolution Tools

**Visual Studio Code:**
- Click "Accept Current" / "Accept Incoming" / "Accept Both"
- Use 3-way merge view

**Visual Studio:**
- Right-click → "Resolve Conflicts"
- Choose resolution method

**Merge Tools:**
```bash
# Configure merge tool
git config --global merge.tool vscode
git config --global mergetool.vscode.cmd 'code --wait $MERGED'

# Use merge tool
git mergetool
```

---

## Collaboration Guidelines

### Working with Team

**Communication:**
1. **Announce work** - Tell team what you're working on
2. **Ask questions** - Don't struggle alone
3. **Review code** - Help others with reviews
4. **Share knowledge** - Document learnings

**Branch Coordination:**
```
❌ BAD:
Two people working on feature/combat-system simultaneously

✅ GOOD:
Person A: feature/combat-damage-calculation
Person B: feature/combat-turn-order
```

---

### Handling Others' Branches

**Checking Out Others' Branches:**
```bash
# Fetch all branches
git fetch origin

# See all branches
git branch -r

# Checkout teammate's branch
git checkout -b feature/their-feature origin/feature/their-feature

# Make changes (if helping)
git add .
git commit -m "fix: resolve issue in feature"
git push origin feature/their-feature
```

---

### Stashing Changes

**When Switching Branches:**
```bash
# You have uncommitted changes
# Need to switch branches

# Stash changes
git stash save "WIP: grid elevation work"

# Switch branches
git checkout other-branch

# Do work on other branch
# ...

# Return to original branch
git checkout feature/grid-elevation

# Restore stashed changes
git stash pop

# Or list stashes
git stash list
git stash apply stash@{0}
```

---

## Tags and Releases

### Semantic Versioning

**Version Format:** `MAJOR.MINOR.PATCH`
```
v1.0.0 = Major.Minor.Patch

MAJOR: Breaking changes (v1.0.0 → v2.0.0)
MINOR: New features, backwards compatible (v1.0.0 → v1.1.0)
PATCH: Bug fixes (v1.0.0 → v1.0.1)

Examples:
v0.1.0 - Initial development
v0.2.0 - Grid system added
v0.2.1 - Grid bug fixes
v1.0.0 - First release
v1.1.0 - Combat system added
v1.1.1 - Combat bug fix
v2.0.0 - Complete overhaul
```

---

### Creating Tags
```bash
# Create annotated tag
git tag -a v0.2.0 -m "Release 0.2.0: Grid System"

# Push tag
git push origin v0.2.0

# Push all tags
git push origin --tags

# List tags
git tag

# Checkout specific tag
git checkout v0.2.0

# Delete tag (if mistake)
git tag -d v0.2.0
git push origin --delete v0.2.0
```

---

### Release Workflow

**Creating Release:**

1. **Finish all features** for release
2. **Create release branch:**
```bash
   git checkout develop
   git checkout -b release/0.2.0
```

3. **Final testing** and bug fixes
4. **Update version** numbers in code
5. **Update CHANGELOG.md**
6. **Merge to main:**
```bash
   git checkout main
   git merge --no-ff release/0.2.0
   git tag -a v0.2.0 -m "Release 0.2.0"
   git push origin main --tags
```

7. **Merge back to develop:**
```bash
   git checkout develop
   git merge --no-ff release/0.2.0
   git push origin develop
```

8. **Delete release branch:**
```bash
   git branch -d release/0.2.0
```

9. **Create GitHub Release:**
   - Go to Releases page
   - Draft new release
   - Select tag v0.2.0
   - Add release notes
   - Attach binaries (if applicable)
   - Publish

---

## CHANGELOG.md

### Format
```markdown
# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Added
- New features in develop but not yet released

### Changed
- Changes to existing functionality

### Deprecated
- Features that will be removed in future

### Removed
- Features that were removed

### Fixed
- Bug fixes

### Security
- Security fixes

## [0.2.0] - 2025-01-15
### Added
- Grid elevation system
- Tile elevation visualization
- Pathfinding considers elevation
- SetElevation API function

### Changed
- GridManager API updated with elevation support
- GridTile structure includes Elevation property

### Fixed
- Grid coordinate conversion edge case
- Visualizer Z-fighting issue

## [0.1.0] - 2025-01-01
### Added
- Initial grid system
- Grid visualization
- Basic coordinate system
- Grid manager subsystem

[Unreleased]: https://github.com/user/kalki/compare/v0.2.0...HEAD
[0.2.0]: https://github.com/user/kalki/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/user/kalki/releases/tag/v0.1.0
```

---

## Common Git Commands

### Essential Commands
```bash
# Status
git status                    # See what changed
git log                       # View commit history
git log --oneline            # Compact history
git log --graph --oneline    # Visual branch history

# Branches
git branch                    # List local branches
git branch -r                 # List remote branches
git branch -a                 # List all branches
git checkout branch-name      # Switch branch
git checkout -b new-branch    # Create and switch
git branch -d branch-name     # Delete local branch

# Staging
git add file.cpp             # Stage specific file
git add .                    # Stage all changes
git add -p                   # Stage interactively

# Committing
git commit -m "message"      # Commit with message
git commit --amend           # Modify last commit
git commit --no-verify       # Skip pre-commit hooks

# Pulling/Pushing
git pull origin develop      # Pull latest changes
git push origin branch-name  # Push branch
git push origin --tags       # Push tags

# Merging
git merge branch-name        # Merge branch
git merge --no-ff branch     # Merge with merge commit
git merge --abort            # Abort merge

# Resetting
git reset HEAD file.cpp      # Unstage file
git reset --soft HEAD~1      # Undo last commit, keep changes
git reset --hard HEAD~1      # Undo last commit, discard changes

# Stashing
git stash                    # Stash changes
git stash pop                # Apply and remove stash
git stash list               # List stashes
git stash drop               # Delete stash

# Viewing
git diff                     # See unstaged changes
git diff --staged            # See staged changes
git show commit-hash         # Show commit details

# Cleaning
git clean -n                 # Show what would be deleted
git clean -fd                # Delete untracked files

# Remote
git remote -v                # List remotes
git fetch origin             # Fetch from remote
git fetch --prune            # Clean up deleted branches
```

---

## Troubleshooting

### Accidentally Committed to Wrong Branch
```bash
# You committed to develop instead of feature branch

# Create feature branch (includes commit)
git branch feature/my-feature

# Reset develop to before commit
git reset --hard HEAD~1

# Switch to feature branch
git checkout feature/my-feature
```

---

### Need to Undo Last Commit
```bash
# Undo commit but keep changes
git reset --soft HEAD~1

# Undo commit and discard changes
git reset --hard HEAD~1

# Already pushed? Revert instead
git revert HEAD
git push origin branch-name
```

---

### Accidentally Deleted Branch
```bash
# Find commit hash
git reflog

# Recreate branch
git checkout -b recovered-branch commit-hash
```

---

### Large Files in History
```bash
# If accidentally committed large files
# Use BFG Repo-Cleaner or git filter-branch

# Better: Use Git LFS from the start!
```

---

## Best Practices Summary

### ✅ Do This:

1. **Commit often** - Small, logical commits
2. **Write clear messages** - Explain what and why
3. **Test before committing** - Code must compile
4. **Pull before pushing** - Stay synchronized
5. **Use feature branches** - Isolate work
6. **Review code** - Help teammates
7. **Use Git LFS** - For large files
8. **Tag releases** - Mark important versions
9. **Keep history clean** - Use meaningful commits
10. **Communicate** - Tell team what you're doing

---

### ❌ Don't Do This:

1. **Don't commit broken code** - Must compile
2. **Don't commit directly to main** - Use branches
3. **Don't force push** - Unless you know why
4. **Don't commit generated files** - Use .gitignore
5. **Don't commit secrets** - API keys, passwords
6. **Don't use vague messages** - Be descriptive
7. **Don't work alone** - Collaborate and communicate
8. **Don't ignore conflicts** - Resolve properly
9. **Don't delete others' work** - Communicate first
10. **Don't push without pulling** - Sync first

---

## Related Documents

### Foundation
- [[02_Conventions/Naming_Conventions]] - File naming
- [[02_Conventions/Coding_Standards]] - Code quality
- [[04_Implementation/Testing_Procedures]] - Test before commit

### Collaboration
- [[01_Project_Overview]] - Project structure
- [[02_Conventions/Asset_Guidelines]] - Asset organization

---

## Quick Reference

### Common Workflow
```bash
# Start new feature
git checkout develop
git pull origin develop
git checkout -b feature/my-feature

# Work on feature
git add .
git commit -m "feat: add feature"
git push origin feature/my-feature

# Create pull request on GitHub

# After approval, merge on GitHub

# Clean up
git checkout develop
git pull origin develop
git branch -d feature/my-feature
```

### Commit Message Template
```
<type>(<scope>): <subject>

[body]

[footer]

Types: feat, fix, docs, style, refactor, perf, test, chore
```

---

## Document Info
**File Path:** `Kalki_GDD/02_Conventions/Version_Control_Guidelines.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** Foundation  
**Complexity:** Beginner  
**Note:** Comprehensive Git workflow and collaboration guidelines