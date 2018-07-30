Reason Dojo - Reprocessing - August 2018 - Ghent
---

No one ever learned to ride a bicycle, while watching other people bike.

## Need help?

1) Don't be afraid to ask help from a mentor at the Dojo
2) Join https://discord.gg/V7mUGq, look for the #reason-dojo channel
3) Tweet at https://twitter.com/_iwan_refmt

## Requirements

Make sure you have [node](https://nodejs.org/en/) installed.

## Install
```
git clone https://github.com/IwanKaramazow/ReasonDojo.git
cd ReasonDojo

npm install
# or with yarn
yarn
```

## Docs

[Reprocessing](https://schmavery.github.io/reprocessing/)

[Reason](https://reasonml.github.io/docs/en/quickstart-javascript.html)

## Dojo

### 0. Hello world

```
npm run build
npm start
```
A window with a floating static bird should appear.

### 1. Baby steps - Hot reloading

Reprocessing is able to live reload code while developing, opening up all possibilities for a tremendous dev experience. Ever heard of live reloading code in native environments? Me neither, until Reason ;)

* Open `index.re` and modify `speed` in the `Physics` module.
* Experiment with different speeds

### 2. Birth of the bird

The bird seems to hang frozen in the air. Let's bring him alive.
The image of the bird gets drawn by the following code:
```reason
Images.drawBird(~x=180., ~y=state.birdY, ~image=state.image, env);
```
* Update the relevant part of the state to give the bird a different position on the `y`-axis. For example give it a hard-coded position of `150.`
* Increment the bird position (`y`-axis) on every draw with the `Math.sineWave` function. Hint: part of the named arguments contain default values in the function definition. If you omit them when calling the function the default values are used. You can use the `xOffset` value in the state as `offset`. Don't forget to pass a `()` (unit) as last argument to finish the function application.

### 3. Let's play

The player seems to be constantly waiting:
```reason
type player =
  | Waiting;
```
* Add an extra constructor that indicates a player is "playing". Note: constructors always start with an uppercased character.
* Modify the switch statement on `state.player`, to pattern match on your new constructor and return the next state.
* Whenever the user presses a `space` in the `Waiting` state, update the `player.state` to reflect that the game has started. Hint: to check whether a space was pressed you can use `Env.keyPressed(Space, env)`.

### 4. Fly

Now that we know the game started (a space has been pressed), it's time to make the bird fly high in the sky.

* Update the bird's `y`-axis position on every draw (when the user is playing) by incrementing with the `state.acceleration *. deltaTime`
* Gravity is a thing in the real world. Increment the `acceleration ` on every draw with the `gravity` from the `Physics` module.
* Whenever the user presses space, the bird needs to fly higher and higher. If a space is pressed, the `acceleration` is equal to the force contained by `jumpAcceleration`  in the `Physics` module.

### 5. Ground zero

Amazing, the bird is able to fly through the air whenever the space-key is pressed. Due to weird laws governing the FlappyBird-world, a bird (unfortunately) dies when it hits the ground. It's time to implement ground collision detection.

* Add an extra constructor to the `player`-type indicating the player has lost, because his bird hit the ground.
* Modify the `hitsFloor` function. It should return `true` whenever the bird hit the floor.
* Use the `hitsFloor` function to update the `state.player`. (Indicates death of the bird when it hit the ground)
* Make sure that the bird stays dead on the ground, whenever it hit the ground. (A bird falling through the earth violates certain laws of physics).
* If the user presses `space` when his bird is dead, reset the game to the `Waiting` state.

### 6. Recalibrating the vision

Great, you wrote safe, approachable code, that runs seamlessly everywhere as fast as it possibly can in each possible environment. The code you just wrote doesn't care about where it runs, you can use it in old browsers, new browsers, native apps, compiled bytecode - anywhere. (The main point being that the backend is treated as an implementation detail.)

1. Compile your game to a native app and play it
```
npm run build:native
# play the game
npm run start:native
```

2. Compile your game to bytecode and play it
```
npm run build:bytecode
# play the game
npm run start:bytecode
```

3. Compile your game to javascript and play it
```
npm run build:web
open `index.html` in a browser, e.g. safari, firefox... (there's a Reprocessing bug in Chrome atm, will be fixed)
```

### 7. Beware of the pipes

* When the user is playing, use the `updatePipes` function to update the `pipes` state on every draw
* Uncomment following part of the code in the `draw`  function:
```reason
let crash = birdHitsPipes(state.birdY, state.xOffset, state.pipes);
```
* The `crash` boolean indicates whether a bird hit a pipe. When there's a crash:
  - `state.player` should transition to the same state as when the birth hit the ground
  - the pipes should stop moving, e.g. no need to update them anymore
