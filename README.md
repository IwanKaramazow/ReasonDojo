Reason Dojo - Reprocessing - August 2018 - Ghent
---

No one ever learned to ride a bicycle, while watching other people bike.

## Need help?

1) Don't be afraid to ask help from a mentor at the Dojo
2) Join https://discord.gg/V7mUGq, look for the #reason-dojo channel
3) Tweet at https://twitter.com/_iwan_refmt

## Requirements

Make sure you have [node](https://nodejs.org/en/) installed.

## Editor setup

* [VSCode](https://github.com/reasonml-editor/vscode-reasonml) **recommended**

* Atom
```
apm install language-reason linter linter-refmt reason-refmt
```

* Others: https://reasonml.github.io/docs/en/editor-plugins#officially-supported-editors

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
`npm start` (from the previous step) automatically starts the app with hot reloading enabled.

* Open `index.re` and modify `speed` in the `Physics` module. (Hint: `float` numbers, always contain a `.`)
* Experiment with different speeds
* Reset the speed to `150.`

### 2. Birth of the bird

Let's start by taking a look at our bird.
The image of the bird gets drawn by the following code:
```reason
Images.drawBird(~x=180., ~y=state.birdY, ~image=state.image, env);
```

* Update the relevant part of the state in `draw` to give the bird a different position on the `y`-axis. For example give it a hard-coded position of `150.`

* **CONCEPT:** Take a look at the example below. Can you guess what `~` does for arguments?

  ```reason
  let add = (~a, ~b) => a + b;
  let result = add(~b=2, ~a=4);
  ```

* **CONCEPT:** Take a look at the example below. What happens if we don't provide `b` as an argument?
  ```reason
  let add = (~a, ~b=16) => a + b;
  let result = add(~a=4);
  ```

* Right now, the bird is floating aimlessly through the sky, without a care in the world. Let's change that by blowing some wind in its face!  
To do this, we want to increment the bird's Y-position with the result of our `Math.sineWave` function on every draw.

  Take a look at the `Math.sineWave` definition. You will see it requires several arguments but some of them have default values (see the concept above). These default values will be used if you omit them in the function invocation. So the only arguments we really need to provide is the offset and a `()` (unit) as last argument to finish the function application.

  As offset we can provide the `xOffset` value from the state.

* **CONCEPT:** Can you guess what the difference is between `18. +. 12.` and `18 + 12`?

**TIP:** Don't be afraid to ask a mentor for help. If you've never worked with named or default arguments before, you'll definitely have a lot of questions.

### 3. Let's play

* **CONCEPT:** Take a quick look at the documentation about [Variants and Constructors](https://reasonml.github.io/docs/en/variant) and [Pattern Matching](https://reasonml.github.io/docs/en/pattern-matching.html). What will the result be of the message function in the example below?

    ```reason
    type myFirstVariant =
    | Yes
    | PrettyMuch
    | No;

  let doYouUnderstandIt = Yes;

  let message =
    switch (doYouUnderstandIt) {
    | No => "No worries. Ask a mentor for help!"
    | Yes => "Great! Aren't Variants awesome?"
    | PrettyMuch => "Nice! Almost there!"
    };
    ```
Having a bird cruising on the wind is great but it's time to get some interaction going. When you press Space now, nothing is happening. And if you take a look at the player Variant, it's not difficult to see why. There is just one constructor and it indicates a waiting state.

```reason
type player =
  | Waiting;
```

* Add an extra constructor that indicates a player is "Playing". Note: constructors always start with an uppercased character.
* Modify the switch statement on `state.player`, to pattern match on your new constructor and return the next state.
* Whenever the user presses a `space` in the `Waiting` state, update the `player.state` to reflect that the game has started. Hint: to check whether a space was pressed you can use `Env.keyPressed(Space, env)`.

### 4. Fly

Now that we know the game started (a Space has been pressed), it's time to make the bird respond to our commands!

* Update the bird's `y`-axis position on every draw (when the user is playing) by incrementing its value with `state.acceleration *. deltaTime`
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
let crash = birdHitsPipes(state.birdY, state.pipes);
```
* The `crash` boolean indicates whether a bird hit a pipe. When there's a crash:
  - `state.player` should transition to the same state as when the birth hit the ground
  - the pipes should stop moving, e.g. no need to update them anymore
