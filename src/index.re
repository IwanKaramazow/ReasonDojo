open Reprocessing;

type player =
  | Waiting;

type pipe = {
  x: float,
  y: float,
};

type state = {
  image: imageT,
  xOffset: float,
  birdY: float,
  acceleration: float,
  player,
  pipes: list(pipe),
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
  let jumpAcceleration = (-500.);
};

module Images = {
  let birdSize = 20.;
  let birdX = 180.;

  let floorY = 500.;

  let pipeGap = 50.;
  let pipeHeight = 350.;
  let pipeGap = 100.;
  let pipeWidth = 50.;

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
      ~pos=(
        int_of_float(-. xOffset) mod width + width,
        int_of_float(floorY),
      ),
      ~width,
      ~height=140,
      ~texPos=(292, 0),
      ~texWidth=168,
      ~texHeight=56,
      env,
    );

    Draw.subImage(
      image,
      ~pos=(int_of_float(-. xOffset) mod width, int_of_float(floorY)),
      ~width,
      ~height=140,
      ~texPos=(292, 0),
      ~texWidth=168,
      ~texHeight=56,
      env,
    );
  };

  let drawPipe = (~x, ~y, ~image, env) => {
    let (topHeight, bottomHeight) = {
      let h = int_of_float(pipeHeight);
      (- h, h);
    };

    /* top pipe */
    Draw.subImage(
      image,
      ~pos=(int_of_float(x), int_of_float(y -. pipeGap)),
      ~width=int_of_float(pipeWidth),
      ~height=topHeight,
      ~texPos=(0, 323),
      ~texWidth=26,
      ~texHeight=160,
      env,
    );

    /* bottom pipe */
    Draw.subImage(
      image,
      ~pos=(int_of_float(x), int_of_float(y +. pipeGap)),
      ~width=int_of_float(pipeWidth),
      ~height=bottomHeight,
      ~texPos=(0, 323),
      ~texWidth=26,
      ~texHeight=160,
      env,
    );
  };
};

let generatePipe = x => {
  x: x +. Utils.randomf(~min=200., ~max=300.),
  y: Utils.randomf(~min=200., ~max=300.),
};

let initializePipes = () => [
  generatePipe(200.),
  generatePipe(400.),
  generatePipe(600.),
];

let updatePipes = (pipes, offset) =>
  List.map(
    pipe =>
      if (pipe.x +. Images.pipeWidth > 0.) {
        {...pipe, x: pipe.x -. offset /. 500.};
      } else {
        {...pipe, x: pipe.x +. 600.};
      },
    pipes,
  );

let birdHitsPipes = (birdY, xOffset, pipes) =>
  List.exists(
    ({x, y}) => {
      let topPipeCrash =
        birdY <= y -. Images.pipeGap && Images.birdX +. 40. >= x;

      let bottomPipeCrash = birdY >= y +. 70. && Images.birdX +. 40. >= x;

      topPipeCrash || bottomPipeCrash;
    },
    pipes,
  );

let updateXOffset = (xOffset, deltaTime) =>
  xOffset +. Physics.speed *. deltaTime;

let hitsFloor = birdY => false;

let setup = env => {
  Env.size(~width=400, ~height=640, env);

  {
    image: Draw.loadImage(~filename="assets/flappy.png", ~isPixel=true, env),
    xOffset: 0.,
    birdY: 150.,
    acceleration: 0.,
    player: Waiting,
    pipes: initializePipes(),
  };
};

let draw = (state, env) => {
  Images.drawBackground(~image=state.image, env);
  Images.drawBird(~x=Images.birdX, ~y=state.birdY, ~image=state.image, env);
  Images.drawGround(~xOffset=state.xOffset, ~image=state.image, env);

  List.iter(
    ({x, y}) => Images.drawPipe(~image=state.image, ~y, ~x, env),
    state.pipes,
  );

  /* let crash = birdHitsPipes(state.birdY, state.xOffset, state.pipes); */

  /* the time elapsed between two frame updates */
  let deltaTime = Env.deltaTime(env);

  switch (state.player) {
  | Waiting => {...state, xOffset: updateXOffset(state.xOffset, deltaTime)}
  };
};

run(~setup, ~draw, ());
