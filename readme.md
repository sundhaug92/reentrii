# REENTRii

An April fools joke game for the Wii based on DevKitPPC (DevKitPro), GRRLib.

## Development

```
git clone https://github.com/Sundhaug92/REENTRii Sundhaug92/REENTRii
cd Sundhaug92/REENTRii
git submodule init && git submodule update
```

`.devcontainer/devcontainer.json` automagically configures a development-environment for you in Visual Studio Code. 

- data/ - asset-files converted to .h files and used during the build.
- deps/ - dependencies, currently just the GRRLib submodule.
- Other/ - contains some reference-files used during development but not included in the build.
- source/ - contains the "actual" source-files.


The initial entrypoint of the game is the game-manager, which initializes global call-backs and manages switching between the screens.
Each screen has a screens ID, and a function that takes a reference to the global state and returns a GameModeExit struct.
The GameModeExit struct tells the game-manager which screen it should switch to next unless the game shutdown callbacks have been called.
If the game-manager sees an invalid screen being requested, it will switch to the error-screen and display an error-message.

Screen-transitions:

```mermaid
graph TD;
    "Splash screen"-->"Credits screen";
    "Credits screen"-->"Splash screen";
    "Splash screen"-->"Story screen";
    "Story screen"-->"Game screen";
    "Game screen"-->"Victory screen";
    "Victory screen"-->"Credits screen";
    "Game screen"-->"Game Over";
    "Game Over screen"-->"Credits screen";
```

## Funny little bits

1. The doctor is named Lentokentta, which in Finnish means "airfield" and is the name of the district of Vantaa that houses Helsinki-Vantaa airport. This is a reference to Petri Wilhelmsen, developer of Reentry, being part Finnish, and inspired by [The Technical Difficulties](<https://www.youtube.com/watch?v=3UAOs9B9UH8>). The gender of the evil doctor is intentionally left ambigious.
2. Entering the [Konami Code](<https://en.wikipedia.org/wiki/Konami_Code>) on the splash-screen works.
3. Not only does cheating get called out by the splash-screen, but the victory and game-over screens will call you out for it.
4. Most of the graphics were added late at night, past midnight, only a few hours before the game was announced. Before then, the player texture was a white "C", the projectiles were -> and <- (depending on if they were friendly or not), and enemies were the letter "E" colored depending on their level.