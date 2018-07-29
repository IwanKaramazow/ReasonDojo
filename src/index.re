open Reprocessing;

type player =
  | Waiting;

type state = {
  image: imageT,
  xOffset: float,
  birdY: float,
  velocity: float,
  player,
};

module Math = {
  /* A sinus wave represents the following motion: ∿∿∿∿∿∿∿∿∿∿∿∿
       -amplitude: peak deviation of the function from zero
       -freq: number of oscillations that occur each second of time
       -offset: where in its cycle the oscillation is at
     */
  let sineWave = (~amplitude=2., ~freq=10., ~offset, ()) =>
    amplitude *. sin(freq *. offset);
};

module Physics = {
  let speed = 150.;
  let gravity = 16.66;
  let jumpForce = (-500.);
};

module Images = {
  let birdSize = 20.;

  let drawBird = (~x, ~y, ~image, env) =>
    Draw.subImage(
      image,
      ~pos=(int_of_float(x), int_of_float(y)),
      ~width=int_of_float(birdSize) * 2 + 4,
      ~height=int_of_float(birdSize) * 2 + 4,
      ~texPos=(115, 328),
      ~texWidth=17,
      ~texHeight=17,
      env,
    );

  let drawBackground = (~night=false, ~image, env) => {
    let width = Env.width(env);
    let height = Env.height(env);

    Draw.subImage(
      image,
      ~pos=(0, 0),
      ~width,
      ~height,
      ~texPos=(night ? 146 : 0, 0),
      ~texWidth=144,
      ~texHeight=256,
      env,
    );
  };

  let drawGround = (~image, ~xOffset, env) => {
    let width = Env.width(env);

    Draw.subImage(
      image,
      ~pos=(int_of_float(-. xOffset) mod width + width, 500),
      ~width,
      ~height=140,
      ~texPos=(292, 0),
      ~texWidth=168,
      ~texHeight=56,
      env,
    );

    Draw.subImage(
      image,
      ~pos=(int_of_float(-. xOffset) mod width, 500),
      ~width,
      ~height=140,
      ~texPos=(292, 0),
      ~texWidth=168,
      ~texHeight=56,
      env,
    );
  };

  let drawPipe = (~x, ~y, ~image, env) => {
    let pipeHeight = 350.;
    let pipeGap = 100.;
    let pipeWidth = 50.;
    let height = - int_of_float(pipeHeight);

    Draw.subImage(
      image,
      ~pos=(int_of_float(x), int_of_float(y -. pipeGap)),
      ~width=int_of_float(pipeWidth),
      ~height,
      ~texPos=(0, 323),
      ~texWidth=26,
      ~texHeight=160,
      env,
    );

    Draw.subImage(
      image,
      ~pos=(int_of_float(x), int_of_float(y +. pipeGap)),
      ~width=int_of_float(pipeWidth),
      ~height=int_of_float(pipeHeight),
      ~texPos=(0, 323),
      ~texWidth=26,
      ~texHeight=160,
      env,
    );
  };
};

let setup = env => {
  Env.size(~width=400, ~height=640, env);

  {
    image: Draw.loadImage(~filename="assets/flappy.png", ~isPixel=true, env),
    xOffset: 0.,
    birdY: 200.,
    velocity: 0.,
    player: Waiting,
  };
};

let nextXOffset = (xOffset, deltaTime) =>
  xOffset +. Physics.speed *. deltaTime;

let draw = (state, env) => {
  Images.drawBackground(~image=state.image, env);
  Images.drawBird(~x=180., ~y=state.birdY, ~image=state.image, env);
  Images.drawGround(~xOffset=state.xOffset, ~image=state.image, env);

  /* the time elapsed between two frame updates */
  let deltaTime = Env.deltaTime(env);

  switch (state.player) {
  | Waiting => {...state, xOffset: nextXOffset(state.xOffset, deltaTime)}
  };
};

run(~setup, ~draw, ());
