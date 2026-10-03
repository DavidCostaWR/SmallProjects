# RPG Project: Session Kickoff

Paste or attach this at the start of a clean session. It holds only the decisions already made.

## Collaboration preferences
- Be concise. Prioritise clean code, organisation and best practices.
- If I share code with an error, point it out but do not fix it until I acknowledge.
- When modifying code, show only the modified sections, not whole files.
- Split large tasks into chunks and give a roadmap before implementing.
- Treat me as an experienced embedded developer who wants to deepen C/C++ skills.
- Do not create PRs or push unless I ask.

## Goal
A turn-based RPG in the style of Final Fantasy 1-3 (rounds-based, party vs enemies). Later flair: ATB (FF4-9 style), equipment, overworld, polish.
The project is for learning and practice in C++ (and some C). It must actually get finished, so scope discipline matters more than ambition.

## Decisions made
- **Dropped:** Unity (too much work), and any embedded-style restrictions (no-heap, no-exceptions, etc.). Normal modern C++ is fine.
- **Language:** C++20.
- **Architecture:** Entity Component System. Components are plain data, systems are free functions over registry views.
  Headless core library with no graphics or I/O. Frontends are thin layers on top.
- **Core contract:** frontends send Commands in (Attack, Skill, Item, Defend, Run). The core returns Events out (DamageDealt, ActorDied, StatusApplied, ...).
- **ECS library:** EnTT (header-only, via CMake FetchContent). A hand-rolled ECS is an optional later exercise.
- **Method:** Test-driven development. Failing test first, minimum code, refactor. The RNG is injected and seeded so tests are deterministic.
- **Build:** CMake. Tests with doctest. Data as JSON (nlohmann/json) loaded into components.
- **Frontends:** console first, raylib later.
- **Environment:** Windows, CLion (MinGW or MSVC). VS and VSCode also available.
- **Task tracking:** Codecks. No connector exists in Claude. Options: draft cards as markdown for import, or use the Codecks REST API with a token kept in an env var (never in the repo).
- **Repo:** SmallProjects is a collection of C++/Visual Studio projects. The RPG could live in an `RPG/` subfolder here or in its own repo (undecided).

## Minimal-version scope (initial proposal, to be confirmed in the GDD)
- 4 party members, rounds-based turns ordered by speed.
- Commands: Attack, Skill (2-3 spells), Item, Defend, Run.
- 3 enemy types plus 1 boss, with simple weighted AI.
- Status effects: poison and sleep.
- XP and level-ups, basic inventory.

## Planned ECS shape (draft)
- Components: Stats, Health, Mana, Speed, Team, SkillList, StatusEffects, AIControlled, Inventory, Experience, Name.
- Systems: TurnOrder, Command, Damage, Status, AI, Death, Experience, BattleOutcome.

## Roadmap
0. **GDD (current phase).** Written with me, section by section.
1. Skeleton: CMake, EnTT, doctest, console target, warnings, clang-tidy.
2. Components and test helpers.
3. Combat resolution (damage, hit/miss, crit, defend, death), test-first.
4. Turn order and battle loop, with win/lose.
5. Skills, items and status effects, data-driven.
6. Enemy AI.
7. Console frontend with a fully playable battle.
8. XP/levelling and JSON data loading.
9. World layer: party, inventory, equipment, encounters, map, game-state machine.
10. Save/load, then raylib frontend, then flair.

## First task for the new session
Create the Game Design Document with me, in chunks, asking me questions rather than assuming. Suggested sections:
1. Vision, pillars and scope (what "finished" means).
2. Core loop and game flow.
3. Party and characters (classes, stats, progression).
4. Combat rules (turn order, formulas, commands, statuses).
5. Enemies and AI.
6. Items, equipment and economy.
7. World, exploration and encounters.
8. Story and dialogue (minimal).
9. UI and controls.
10. Technical design (ECS layout, data formats, testing strategy).
11. Milestones, with a minimal-version cut line and risks.

Then convert the GDD milestones into Codecks-ready card drafts.
Do not write any code until I approve the GDD.
