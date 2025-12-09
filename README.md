# Forge - ARPG Prototype

An Action RPG project built in Unreal Engine with C++, inspired by Path of Exile. It is composed of 4 core systems that were developed as separate projects and then integrated together.

## Core Systems

### 🌍 Procedural Map Generation

Tiled-based map generator that generates an abstract graph layout:

1. **Graph Generation** - Creates a graph with an abstract layout composed of a main path and branches with shapes (L, U, Stairs, Square, Straight, etc.)
2. **Tile Selection** - Weighted random selection of tiles matching graph cells using connector compatibility
3. **World Placement** - Spawns tiles with proper rotation

### 📦 Item & Inventory System

Path of Exile-inspired item system. The inventory uses a grid system with equipment slots that apply item attributes to the character through GAS.

- Generation: Weighted random selection for attributes and item bases from data tables.
- Architecture: Inventory manager connecting inventory and equipment components with UI. Separation of inventory logic and visuals.
- GAS Integration: Equipment slots automatically apply attribute modifiers to the player
- Crafting: Logic for modifying existing item attribute values.

Items are represented with the following structures:

- Item base, which contains all the data needed to generate an item, like its name, type, and possible attributes that can be rolled.
- Item instance, which is the generated item with rolled attributes
- Inventory entry contains item instance data needed for the inventory, like their quantity
- World items that are actors representing an item in the world with their location

### 🧠 AI System

It contains ranged and melee AI characters using behavior tree and Unreal Engine environment query system (EQS). The AI uses Pawn Sensing to detect a target and then move into attack range of the target. It uses an EQS query to find a location, generating points in an arc of a circle around the target.

If none of the points are valid, the AI character uses another EQS query in a cone shape in front of him to move foward

### 🗡️ Combat System

Abilities and attributes using Unreal's Gameplay Ability System (GAS).  It includes:

- Character attributes with damage modifiers automatically applied when added to their corresponding data table
- Abilities containing tags (e.g. fire, projectile, physical) that define which attributes are applied
- Input management assigning abilities at runtime
- Specific mechanics associated with attributes such as attack speed or projectile count.
- Chaining projectiles ability bouncing on ennemies

The damage calculation follows Path of Exile's model with flat, additive, and multiplicative modifiers.

```Damage = (BaseDamage + Flat) × (1 + Additive%) × Multiplicative%```

## Technical Challenges & Architecture

I focused on keeping the code simple and readable. I devoleped system as seperate projects so they could be independant which makes the architecture easier to understand and debug. Each system has data assets for configuration making it easier to customize.

I learned about project architecture and some Unreal specific systems like GAS, behavior tree and EQS. The biggest challenge was the procedural map generation, creating a nice graph layout is challenging and can be hard to debug, especially retracing the tile generation.

## Links

- [Demo Video](https://www.youtube.com/watch?v=DX2ztxTcXCM)
- [Portfolio Website](https://paulbeneteau.com)
