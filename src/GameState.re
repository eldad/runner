/*
     Game State
     Shared between the app and the canvas.
 */

type exhaustable =
  | Ready
  | On(float)
  | Exhausted;

type state =
  | Idle
  | Run
  | GameOver(float);

type t = {
  state,
  player_jumping: exhaustable,
  player_y: float,
  distance: float,
  velocity: int,
  time: float,
  score: int,
  obstacles: array(float),
};

let player_jump_t = 0.5;
let player_jump_impulse = 400.0;
let player_gravity_impulse = (-200.0);

let player_collision_x1 = 42.;
let player_collision_x2 = player_collision_x1 +. 12.; /* greenbob is 12px long */

let gameover_timeout = 10.;
let obstacle_horizon = 700.;

let initialState = () => {
  state: Idle,
  player_jumping: Ready,
  player_y: 0.,
  distance: 0.,
  velocity: 200,
  time: 0.,
  score: 0,
  obstacles: [||],
};

let updateObstacles: t => t =
  state => {
    let score = ref(state.score);
    if (state.obstacles |> Array.length > 0) {
      if (state.obstacles->Array.get(0) < state.distance) {
        state.obstacles |> Js.Array.shift |> ignore;
        score := score^ + 1;
      };
    };

    {...state, score: score^};
  };

let generateObstacles: t => t =
  state => {
    if (state.obstacles |> Array.length < 1) {
      state.obstacles |> Js.Array.push(state.distance +. 700.) |> ignore;
    };

    state;
  };

let checkCollision: t => t =
  state => {
    let collision = ref(false);

    state.obstacles
    |> Array.iter(o =>
         if (o > state.distance +. player_collision_x1 && o < state.distance +. player_collision_x2) {
           if (state.player_y |> Js.Math.floor < CanvasData.crate_height) {
             collision := true;
           };
         }
       );

    collision^ ? {...state, state: GameOver(gameover_timeout)} : state;
  };

let updatePlayer: (t, float) => t =
  (state, delta_t) => {
    let state =
      switch (state.player_jumping) {
      | Exhausted when state.player_y == 0.0 => {...state, player_jumping: Ready}
      | On(t) when t == 0.0 && state.player_y == 0.0 => {...state, player_jumping: Ready}
      | On(t) =>
        let impulse = (t > 0. ? player_jump_impulse : 0.) +. player_gravity_impulse;
        let y_delta = delta_t *. impulse;
        let player_y = Js.Math.max_float(0., y_delta +. state.player_y);
        let player_jumping = On(Js.Math.max_float(0., t -. delta_t));

        {...state, player_y, player_jumping};
      | Exhausted =>
        let impulse = player_gravity_impulse;
        let y_delta = delta_t *. impulse;
        let player_y = Js.Math.max_float(0., y_delta +. state.player_y);
        {...state, player_y};
      | Ready => state
      };

    let distance = state.distance +. delta_t *. (state.velocity |> float_of_int);
    let time = state.time +. delta_t;

    {...state, distance, time};
  };

let handleTick: (t, float) => t =
  (state, delta_t) =>
    switch (state.state) {
    | GameOver(t) =>
      let t = t -. delta_t;
      t < 0. ? initialState() : {...state, state: GameOver(t)};
    | Run => state->updatePlayer(delta_t) |> checkCollision |> updateObstacles |> generateObstacles;
    | _ => state
    };

let handleKeyDown: t => t =
  state =>
    switch (state.state, state.player_jumping) {
    | (Idle, _) => {...initialState(), state: Run}
    | (GameOver(_), _) => initialState()
    | (Run, Ready) when state.player_y == 0. => {...state, player_jumping: On(player_jump_t)}
    | _ => state
    };

let handleKeyUp: t => t = state => {...state, player_jumping: Exhausted};
