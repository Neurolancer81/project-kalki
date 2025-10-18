---
tags:
  - kalki/assets
  - kalki/art
  - kalki/content
status: complete
phase: foundation
created: 2025-01-09
updated: 2025-01-09
author: Claude & V.S. Puranam
complexity: intermediate
---

# Asset Guidelines

## Overview
This document defines standards for creating, organizing, and optimizing art assets for the Kalki project. Following these guidelines ensures consistent quality, optimal performance, and maintainable content pipelines.

---

## Purpose

### Why Asset Guidelines Matter

**Problems without asset standards:**
1. **Inconsistent Quality** - Mixed art styles, varying detail levels
2. **Performance Issues** - Oversized textures, unoptimized meshes
3. **Difficult Maintenance** - Can't find assets, unclear naming
4. **Wasted Memory** - Duplicate assets, unnecessary detail
5. **Long Load Times** - Assets not optimized for streaming

**Benefits of good asset guidelines:**
- ✅ Consistent visual quality
- ✅ Optimal performance
- ✅ Easy to find and manage assets
- ✅ Efficient memory usage
- ✅ Fast load times
- ✅ Maintainable content pipeline

---

## Directory Structure

### Project Organization
```
Content/
├── Characters/
│   ├── Player/
│   │   ├── Meshes/
│   │   ├── Textures/
│   │   ├── Materials/
│   │   ├── Animations/
│   │   └── Blueprints/
│   ├── Enemies/
│   │   ├── Goblin/
│   │   ├── Orc/
│   │   └── Dragon/
│   └── NPCs/
│
├── Environment/
│   ├── Terrain/
│   │   ├── Meshes/
│   │   ├── Textures/
│   │   └── Materials/
│   ├── Props/
│   │   ├── Furniture/
│   │   ├── Vegetation/
│   │   └── Structures/
│   └── FX/
│
├── UI/
│   ├── Textures/
│   │   ├── Icons/
│   │   ├── Buttons/
│   │   └── Backgrounds/
│   ├── Fonts/
│   └── Widgets/
│
├── VFX/
│   ├── Particles/
│   ├── Materials/
│   └── Textures/
│
├── Audio/
│   ├── Music/
│   ├── SFX/
│   │   ├── Combat/
│   │   ├── UI/
│   │   └── Ambient/
│   └── Voice/
│
├── Weapons/
│   ├── Swords/
│   ├── Bows/
│   └── Staves/
│
├── Abilities/
│   ├── Fire/
│   ├── Ice/
│   └── Lightning/
│
└── Core/
    ├── Materials/
    │   ├── Master/
    │   └── Functions/
    ├── Blueprints/
    └── DataTables/
```

---

### Folder Conventions

**Rules:**
1. **Categorize by type** - Characters, Environment, UI, etc.
2. **Subcategorize by function** - Meshes, Textures, Materials
3. **Group related assets** - Keep character assets together
4. **Use clear names** - Descriptive folder names
5. **Avoid deep nesting** - Maximum 5 levels deep

**Example:**
```
✅ GOOD:
Content/Characters/Enemies/Goblin/Meshes/SM_Goblin_Warrior.uasset

❌ BAD:
Content/Assets/Models/Enemy/Types/Melee/Goblin/warrior_mesh.uasset
```

---

## Naming Conventions

### Asset Prefixes

**Standard Prefixes:**

| Asset Type | Prefix | Example |
|------------|--------|---------|
| Static Mesh | SM_ | SM_Rock_Large |
| Skeletal Mesh | SK_ | SK_Character_Warrior |
| Skeleton | SKEL_ | SKEL_Character_Warrior |
| Texture | T_ | T_Character_Diffuse |
| Material | M_ | M_Character_Skin |
| Material Instance | MI_ | MI_Character_Skin_Red |
| Material Function | MF_ | MF_ColorBlend |
| Animation | A_ | A_Character_Attack |
| Animation Blueprint | ABP_ | ABP_Character |
| Blueprint | BP_ | BP_Character_Warrior |
| Widget Blueprint | WBP_ | WBP_HealthBar |
| Particle System | PS_ | PS_Fire_Explosion |
| Niagara System | NS_ | NS_Magic_Sparkles |
| Sound Cue | SC_ | SC_Footstep |
| Sound Wave | SW_ | SW_Sword_Hit |
| Data Table | DT_ | DT_Characters |
| Curve Table | CT_ | CT_DamageFalloff |
| Level | L_ | L_Combat_Arena |
| Physics Material | PM_ | PM_Stone |

---

### Naming Structure

**Pattern:**
```
[Prefix]_[AssetType]_[Descriptor]_[Variant]_[Suffix]

Examples:
SM_Prop_Barrel_Large_01
T_Character_Warrior_Diffuse_Blue
M_Environment_Stone_Rough
MI_Character_Skin_Pale
A_Character_Walk_Forward
```

**Rules:**
1. **Always use prefix** - Identifies asset type at a glance
2. **Use descriptive names** - Clear what asset represents
3. **Include variants** - Large/Small, Red/Blue, 01/02
4. **Use underscores** - Separate words with `_`
5. **No spaces** - Never use spaces in names
6. **PascalCase words** - Capitalize each word
7. **Be consistent** - Follow pattern throughout project

---

### Texture Suffixes

**Standard Suffixes:**

| Texture Type | Suffix | Example |
|--------------|--------|---------|
| Diffuse/Albedo | _D or _Diffuse | T_Rock_D |
| Normal Map | _N or _Normal | T_Rock_N |
| Roughness | _R or _Roughness | T_Rock_R |
| Metallic | _M or _Metallic | T_Rock_M |
| Ambient Occlusion | _AO | T_Rock_AO |
| Emissive | _E or _Emissive | T_Magic_E |
| Opacity/Alpha | _O or _Alpha | T_Glass_O |
| Height/Displacement | _H or _Height | T_Rock_H |
| Specular | _S or _Specular | T_Metal_S |
| Mask | _Mask | T_Character_Mask |

**Examples:**
```
T_Character_Warrior_Diffuse
T_Character_Warrior_Normal
T_Character_Warrior_Roughness
T_Character_Warrior_Metallic
T_Character_Warrior_AO
```

---

## Mesh Standards

### Static Meshes

**Specifications:**

| Detail Level | Triangle Count | Use Case |
|--------------|----------------|----------|
| Hero | 10,000 - 50,000 | Main characters, focal points |
| High | 5,000 - 10,000 | Important props, enemies |
| Medium | 1,000 - 5,000 | Standard environment |
| Low | 500 - 1,000 | Background props |
| Very Low | 100 - 500 | Distant objects |

**Best Practices:**

1. **Use appropriate detail** - Don't over-poly background objects
2. **Clean geometry** - Remove non-manifold edges, overlapping faces
3. **Optimize topology** - Even quad flow, minimal triangulation
4. **Set collision** - Use simple collision shapes (box, sphere, capsule)
5. **Set LODs** - 3-4 LOD levels for most meshes
6. **Pivot point** - At base/center of object
7. **Scale** - Import at correct scale (1 UU = 1cm)
8. **UV layout** - Non-overlapping, efficient space usage

---

### LOD Settings

**LOD Configuration:**
```
LOD0 (100% detail, 0-500 units):
- Full geometry
- All features visible
- Screen size: 1.0

LOD1 (60% detail, 500-1000 units):
- Reduced detail
- Minor features removed
- Screen size: 0.3

LOD2 (30% detail, 1000-2000 units):
- Simplified geometry
- Only major features
- Screen size: 0.15

LOD3 (10% detail, 2000+ units):
- Very simple shape
- Silhouette only
- Screen size: 0.05
```

**Example LOD Reduction:**
```
Character Model:
LOD0: 8,000 tris
LOD1: 4,000 tris (-50%)
LOD2: 1,500 tris (-81%)
LOD3: 500 tris (-94%)
```

---

### Skeletal Meshes

**Specifications:**

| Character Type | Triangle Count | Bone Count |
|----------------|----------------|------------|
| Hero Character | 15,000 - 30,000 | 70 - 100 |
| Enemy (High) | 8,000 - 15,000 | 50 - 70 |
| Enemy (Med) | 4,000 - 8,000 | 40 - 50 |
| Enemy (Low) | 2,000 - 4,000 | 30 - 40 |

**Requirements:**

1. **Skin weights** - Max 4 influences per vertex
2. **Bone hierarchy** - Logical parent-child relationships
3. **Bone naming** - Standard naming scheme
4. **Skeleton sharing** - Reuse skeletons when possible
5. **LODs** - 3-4 levels for characters
6. **Physics assets** - For ragdoll/hit reactions

**Bone Naming Convention:**
```
Root
├── Pelvis
│   ├── Spine_01
│   │   ├── Spine_02
│   │   │   ├── Spine_03
│   │   │   │   ├── Neck_01
│   │   │   │   │   └── Head
│   │   │   │   ├── Clavicle_L
│   │   │   │   │   └── Shoulder_L
│   │   │   │   │       └── Elbow_L
│   │   │   │   │           └── Wrist_L
│   │   │   │   │               └── Hand_L
│   │   │   │   └── Clavicle_R
│   │   │   │       └── [Mirror of left arm]
│   ├── Thigh_L
│   │   └── Calf_L
│   │       └── Foot_L
│   └── Thigh_R
│       └── [Mirror of left leg]
```

---

## Texture Standards

### Texture Resolutions

**Recommended Sizes:**

| Asset Type | Resolution | Notes |
|------------|------------|-------|
| Character (Body) | 2048×2048 | Main character |
| Character (Face) | 1024×1024 | Detailed faces |
| Character (Hands) | 512×512 | Less visible |
| Environment (Large) | 2048×2048 | Walls, floors |
| Environment (Medium) | 1024×1024 | Props |
| Environment (Small) | 512×512 | Details |
| UI Icons | 256×256 | Sharp at UI size |
| UI Buttons | 512×256 | Common size |
| VFX | 512×512 or 1024×1024 | Depends on effect |

**Rules:**
1. **Power of two** - Always use 256, 512, 1024, 2048, 4096
2. **Square when possible** - Easier to manage, better compression
3. **Match usage** - Don't use 4K for tiny props
4. **Consider texture atlasing** - Combine small textures

---

### Texture Compression

**Compression Settings:**

| Texture Type | Compression | sRGB | Notes |
|--------------|-------------|------|-------|
| Diffuse/Albedo | BC1 or BC7 | Yes | Color textures |
| Diffuse with Alpha | BC3 | Yes | Transparent areas |
| Normal Map | BC5 | No | Best for normals |
| Roughness | BC4 | No | Single channel |
| Metallic | BC4 | No | Single channel |
| Packed (RGB) | BC1 | No | Multiple masks |
| HDR | BC6H | No | Emissive, HDR |
| UI | BC7 | Yes | High quality |

**Texture Settings Template:**
```cpp
// In Texture Editor

// Diffuse
Compression: BC7 (or BC1 for no alpha)
sRGB: Checked
Mip Gen Settings: FromTextureGroup
LOD Group: TEXTUREGROUP_Character (or appropriate)

// Normal Map
Compression: BC5
sRGB: Unchecked
Flip Green Channel: Check if needed
LOD Group: TEXTUREGROUP_CharacterNormalMap

// Masks (R/M/AO)
Compression: BC4
sRGB: Unchecked
```

---

### Texture Packing

**Common Packing Schemes:**

**ORM (Occlusion, Roughness, Metallic):**
```
Red Channel:   Ambient Occlusion
Green Channel: Roughness
Blue Channel:  Metallic
Alpha Channel: (Unused or custom mask)
```

**Benefits:**
- Saves memory (3 textures → 1 texture)
- Fewer texture samples in shader
- Better performance

**Example Material Setup:**
```cpp
// In Material Editor
ORM_Texture (Texture Sample)
├── R → AO pin
├── G → Roughness pin
├── B → Metallic pin
```

---

## Material Standards

### Material Complexity

**Instruction Budget:**

| Material Type | Max Instructions | Example |
|--------------|------------------|---------|
| Character | 200 | Skin, clothing |
| Environment | 150 | Walls, floors |
| Props | 100 | Barrels, crates |
| VFX | 150 | Particles |
| UI | 50 | Simple, fast |

**Optimization Tips:**
1. **Minimize texture samples** - Each sample is expensive
2. **Use LODs** - Simpler materials at distance
3. **Avoid per-pixel calculations** - Do in vertex shader when possible
4. **Use static switches** - Remove unused features
5. **Minimize branching** - If statements are expensive

---

### Master Materials

**Create Master Materials for:**
- Character materials (skin, cloth, metal)
- Environment materials (stone, wood, dirt)
- VFX materials (particles, ribbons)
- UI materials (icons, text)

**Master Material Structure:**
```
M_Master_Character
├── Material Instances:
│   ├── MI_Character_Warrior
│   ├── MI_Character_Mage
│   └── MI_Character_Rogue
│
├── Parameters:
│   ├── Textures
│   │   ├── Diffuse
│   │   ├── Normal
│   │   ├── ORM (Occlusion/Rough/Metal)
│   ├── Colors
│   │   ├── Tint Color
│   │   ├── Detail Color
│   ├── Scalars
│   │   ├── Roughness Multiplier
│   │   ├── Metallic Multiplier
│   │   ├── Normal Strength
│   └── Switches
│       ├── Use Emissive
│       ├── Use Detail Normal
│       └── Use Vertex Color
```

**Benefits:**
- Consistent look across assets
- Easy iteration (change master, all instances update)
- Material instances are cheap
- Performance benefits (same base material = better batching)

---

### Material Functions

**Common Material Functions:**
```
MF_ColorBlend
- Blend two colors with mask

MF_NormalBlend
- Blend two normal maps

MF_Parallax
- Parallax occlusion mapping

MF_Fresnel
- Fresnel effect (rim lighting)

MF_WorldAlignedTexture
- Project texture in world space

MF_DistanceFade
- Fade based on camera distance
```

**Usage:**
```cpp
// In Material Graph
MF_ColorBlend (Material Function)
├── Input: Color A (Base Color)
├── Input: Color B (Detail Color)
├── Input: Mask (Vertex Color R)
└── Output → Base Color
```

---

## Animation Standards

### Animation Specifications

**Frame Rate:**
```
Standard: 30 FPS
Gameplay: 30 FPS (matches logic tick)
Cinematic: 30 FPS (or 60 FPS for smoothness)
```

**Animation Lengths:**

| Animation Type | Duration | Notes |
|----------------|----------|-------|
| Idle | 2-4 seconds | Loop seamlessly |
| Walk Cycle | 1-1.5 seconds | 1 full cycle |
| Run Cycle | 0.8-1 second | 1 full cycle |
| Attack | 0.5-1.5 seconds | Depends on weapon |
| Hit React | 0.3-0.5 seconds | Quick response |
| Death | 2-3 seconds | Dramatic fall |

---

### Animation Compression

**Settings:**
```
Compression Scheme: Automatic
Translation Compression: 0.01 (cm)
Rotation Compression: 0.0003 (degrees)
Scale Compression: 0.0001
```

**Root Motion:**
```
Enable Root Motion: Only for movement animations
Root Bone: Pelvis or Root
Lock Root Position: No (allow movement)
Lock Root Rotation: Yes (rotation handled by code)
```

---

### Animation Blueprint Organization

**Structure:**
```
ABP_Character
├── State Machines
│   ├── Locomotion
│   │   ├── Idle
│   │   ├── Walk
│   │   ├── Run
│   │   └── Jump
│   ├── Combat
│   │   ├── Attack
│   │   ├── Block
│   │   └── Dodge
│   └── Death
│
├── Blend Spaces
│   ├── BS_Locomotion (Walk/Run directions)
│   └── BS_Aim (Aim offsets)
│
└── Variables
    ├── Speed
    ├── Direction
    ├── IsInCombat
    └── IsDead
```

---

## Audio Standards

### Audio Format and Settings

**File Formats:**
```
Source: WAV (uncompressed)
Import: WAV or OGG
In-Engine: Platform-specific compression
```

**Audio Settings:**

| Type | Sample Rate | Bit Depth | Channels |
|------|-------------|-----------|----------|
| Music | 44.1 kHz | 16-bit | Stereo |
| SFX | 44.1 kHz | 16-bit | Mono/Stereo |
| Voice | 22 kHz | 16-bit | Mono |
| Ambient | 44.1 kHz | 16-bit | Stereo |

---

### Sound Cue Organization

**Sound Cue Structure:**
```
SC_Footstep
├── Random Node
│   ├── SW_Footstep_01 (80% probability)
│   ├── SW_Footstep_02 (80%)
│   ├── SW_Footstep_03 (80%)
│   └── SW_Footstep_04 (80%)
├── Volume (0.8 - 1.2 random)
├── Pitch (0.9 - 1.1 random)
└── Output
```

**Benefits:**
- Variation (not repetitive)
- Randomized volume/pitch
- Easy to add/remove sounds
- Reusable

---

### Audio Compression

**Compression Settings:**

| Type | Quality | Load Type | Notes |
|------|---------|-----------|-------|
| Music | 40-60 | Streaming | Large files |
| SFX (Long) | 60-80 | Streaming | > 3 seconds |
| SFX (Short) | 80-100 | Load on Demand | < 3 seconds |
| UI | 100 | Load on Demand | Must be instant |
```cpp
// In Sound Wave settings
Compression Quality: 60 (for SFX)
Loading Behavior: 
  - Streaming: Large files
  - Load on Demand: Small files
```

---

## VFX Standards

### Particle System Guidelines

**Performance Budget:**

| Effect Type | Max Particles | Max Draw Calls |
|-------------|---------------|----------------|
| Continuous | 50-100 | 2-3 |
| Burst | 200-500 | 3-5 |
| Ultimate Ability | 500-1000 | 5-8 |

**Best Practices:**

1. **Use sprite particles** - Cheaper than mesh particles
2. **Limit material complexity** - Simple shaders
3. **Use GPU particles** - For large particle counts
4. **Pool particle systems** - Reuse, don't spawn/destroy
5. **Set bounds** - Fixed bounds for better culling
6. **Use LODs** - Reduce particles at distance

---

### VFX Textures

**Specifications:**
```
Resolution: 512×512 or 1024×1024
Format: PNG with alpha
Channels:
  RGB: Color/shape
  Alpha: Opacity
  
Common Atlases:
  4×4 grid = 16 frames (flipbook)
  8×8 grid = 64 frames (smooth animation)
```

**Flipbook Animation:**
```cpp
// In Particle System Module
SubUV Movie: T_VFX_Explosion_Flipbook
SubUV Method: Linear Blend
Columns: 4
Rows: 4
```

---

## UI Asset Standards

### UI Texture Guidelines

**Specifications:**

| UI Element | Size | Format | Notes |
|------------|------|--------|-------|
| Icons | 256×256 | PNG | Sharp, clear |
| Buttons | 512×256 | PNG | 9-slice capable |
| Backgrounds | 1024×1024 | PNG/JPG | Tiled |
| Portraits | 512×512 | PNG | Character faces |
| Cursor | 64×64 | PNG | Small, distinct |

**9-Slice Setup:**
```
Button Texture (512×256):

Border Margins:
- Left: 32px
- Right: 32px
- Top: 32px
- Bottom: 32px

Center: Stretches
Corners: Fixed size
Edges: Stretch in one direction
```

---

### Font Guidelines

**Font Specifications:**
```
Body Text: 14-18pt
Headers: 24-32pt
Titles: 36-48pt

Fonts:
- UI Text: Sans-serif, clear at small sizes
- Titles: Decorative, fantasy-themed
- Numbers: Monospace (for alignment)

Import Settings:
- TTF or OTF format
- Include character set needed
- Anti-aliasing: Enabled
```

---

## Content Validation

### Asset Checklist

**Before Committing Asset:**

**Mesh:**
- [ ] Correct scale (matches project units)
- [ ] Pivot at base/center
- [ ] Clean geometry (no errors)
- [ ] Collision set up
- [ ] LODs generated
- [ ] Named correctly with prefix
- [ ] In correct folder

**Texture:**
- [ ] Power-of-two resolution
- [ ] Correct compression
- [ ] sRGB set correctly
- [ ] Mip maps generated
- [ ] Named with suffix
- [ ] Appropriate file size

**Material:**
- [ ] Instruction count reasonable
- [ ] Parameters exposed
- [ ] Textures assigned
- [ ] Preview looks correct
- [ ] Instance of master (not unique)

**Animation:**
- [ ] Loops seamlessly (if loop)
- [ ] Root motion set correctly
- [ ] Compressed appropriately
- [ ] Named clearly
- [ ] Correct frame rate

**Audio:**
- [ ] Correct sample rate
- [ ] Normalized volume
- [ ] No clipping/distortion
- [ ] Fades in/out if needed
- [ ] Compression appropriate

---

### Automated Validation

**Editor Utility Widget:**
```cpp
// Content validation checks
void ValidateAsset(UObject* Asset)
{
    if (UStaticMesh* Mesh = Cast<UStaticMesh>(Asset))
    {
        // Check triangle count
        int32 TriCount = Mesh->GetNumTriangles(0);
        if (TriCount > 50000)
        {
            LogWarning("High triangle count: " + FString::FromInt(TriCount));
        }

        // Check LODs
        if (Mesh->GetNumLODs() < 3)
        {
            LogWarning("Missing LODs");
        }

        // Check collision
        if (!Mesh->GetBodySetup())
        {
            LogWarning("No collision");
        }
    }

    if (UTexture2D* Texture = Cast<UTexture2D>(Asset))
    {
        // Check resolution
        int32 Width = Texture->GetSizeX();
        int32 Height = Texture->GetSizeY();

        if (!FMath::IsPowerOfTwo(Width) || !FMath::IsPowerOfTwo(Height))
        {
            LogError("Non-power-of-two texture");
        }

        // Check size
        if (Width > 2048 || Height > 2048)
        {
            LogWarning("Large texture resolution");
        }

        // Check compression
        if (Texture->CompressionSettings == TC_Default)
        {
            LogWarning("Default compression (should be specific)");
        }
    }
}
```

---

## Common Mistakes

### Mistake 1: Oversized Textures
```
❌ BAD:
T_SmallProp_Diffuse: 4096×4096 (16 MB)
// Tiny prop doesn't need 4K texture!

✅ GOOD:
T_SmallProp_Diffuse: 512×512 (0.25 MB)
// Appropriate for size/importance
```

---

### Mistake 2: No LODs
```
❌ BAD:
SM_Character:
  LOD0: 15,000 tris
  // No other LODs - renders 15K tris even at distance!

✅ GOOD:
SM_Character:
  LOD0: 15,000 tris (0-500 units)
  LOD1: 7,500 tris (500-1000 units)
  LOD2: 3,000 tris (1000-2000 units)
  LOD3: 1,000 tris (2000+ units)
```

---

### Mistake 3: Incorrect Naming
```
❌ BAD:
mesh1.uasset
char_tex.uasset
my_material.uasset

✅ GOOD:
SM_Prop_Barrel_Large.uasset
T_Character_Warrior_Diffuse.uasset
M_Environment_Stone.uasset
```

---

### Mistake 4: Wrong Compression
```
❌ BAD:
Normal Map: BC1 (wrong!)
// BC1 doesn't preserve normal data well

✅ GOOD:
Normal Map: BC5
// BC5 is designed for normal maps
```

---

### Mistake 5: Unoptimized Materials
```
❌ BAD:
Material Instructions: 500+
- 10 texture samples
- Complex math every pixel
- Multiple dynamic branches

✅ GOOD:
Material Instructions: < 200
- 3-4 texture samples
- Simple operations
- Static switches for variants
```

---

## Related Documents

### Implementation
- [[04_Implementation/Performance_Guidelines]] - Performance targets
- [[02_Conventions/Naming_Conventions]] - Naming standards
- [[02_Conventions/Coding_Standards]] - Code organization

### Systems
- [[03_Systems/Grid_Visualizer]] - Visual asset usage
- [[03_Systems/Combat_Log_System]] - UI asset usage

---

## Quick Reference

### Asset Prefixes
```
SM_   Static Mesh
SK_   Skeletal Mesh
T_    Texture
M_    Material
MI_   Material Instance
A_    Animation
BP_   Blueprint
WBP_  Widget Blueprint
PS_   Particle System
SC_   Sound Cue
SW_   Sound Wave
```

### Texture Suffixes
```
_D or _Diffuse    Base color
_N or _Normal     Normal map
_R or _Roughness  Roughness
_M or _Metallic   Metallic
_AO               Ambient Occlusion
_E or _Emissive   Emissive
```

### Common Resolutions
```
Characters: 2048×2048
Props (Large): 1024×1024
Props (Small): 512×512
UI Icons: 256×256
```

### Triangle Budgets
```
Hero Character: 15K-30K
Enemy (High): 8K-15K
Props (Medium): 1K-5K
Props (Low): 500-1K
```

---

## Document Info
**File Path:** `Kalki_GDD/02_Conventions/Asset_Guidelines.md`  
**Last Updated:** 2025-01-09  
**Status:** Complete  
**Phase:** Foundation  
**Complexity:** Intermediate  
**Note:** Comprehensive asset creation and organization standards