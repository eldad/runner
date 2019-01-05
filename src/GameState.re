/*
     Game State
     Shared between the app and the canvas.
 */

Random.self_init();

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
  scroll_velocity: int,
  player_velocity: float,
  time: float,
  score: int,
  obstacles: array(float),
};

let player_jump_t = 0.4;
let player_jump_impulse = 10.;
let player_gravity_impulse = (-9.8);
let player_jump_t0_velocity = 125.;

let player_collision_x1 = 20.;
let player_collision_x2 = player_collision_x1 +. 12.; /* greenbob is 12px long */

let gameover_timeout = 10.;
let obstacle_horizon = 700.;

let initialState = () => {
  state: Idle,
  player_jumping: Ready,
  player_velocity: 0.,
  player_y: 0.,
  distance: 0.,
  scroll_velocity: 200,
  time: 0.,
  score: 0,
  obstacles: [||],
};

let updateObstacles: t => t =
  state => {
    let score = ref(state.score);
    if (state.obstacles |> Array.length > 0) {
      if (state.obstacles->Array.get(0) < state.distance -. (CanvasData.crate.width + 1 |> float_of_int)) {
        state.obstacles |> Js.Array.shift |> ignore;
        score := score^ + 1;
      };
    };

    {...state, score: score^};
  };

let generateObstacles: t => t =
  state => {
    if (state.obstacles |> Array.length < 1) {
      let min_space = 120.;
      let last_distance = ref(state.distance +. 700.);

      for (n in 0 to Random.int(9)) {
        let distance = last_distance^ +. min_space +. Random.float(150.);
        state.obstacles |> Js.Array.push(distance) |> ignore;
        last_distance := distance;
      };
    };

    state;
  };

let checkCollision: t => t =
  state => {
    let collision = ref(false);

    state.obstacles
    |> Array.iter(o =>
         if (o <= state.distance
             +. player_collision_x2
             && o
             +. (CanvasData.crate.width |> float_of_int) >= state.distance
             +. player_collision_x1) {
           if (state.player_y |> Js.Math.floor < CanvasData.crate.height) {
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
      | Exhausted when state.player_y == 0.0 => {...state, player_jumping: Ready, player_velocity: 0.}
      | On(t) when t == 0.0 && state.player_y == 0.0 => {...state, player_jumping: Ready, player_velocity: 0.}
      | On(t) =>
        let impulse = (t > 0. ? player_jump_impulse : 0.) +. player_gravity_impulse;
        let player_velocity = state.player_velocity +. impulse;
        let y_delta = delta_t *. player_velocity;
        let player_y = Js.Math.max_float(0., y_delta +. state.player_y);
        let player_jumping = On(Js.Math.max_float(0., t -. delta_t));

        {...state, player_y, player_jumping, player_velocity};
      | Exhausted =>
        let impulse = player_gravity_impulse;
        let player_velocity = state.player_velocity +. impulse;
        let y_delta = delta_t *. player_velocity;
        let player_y = Js.Math.max_float(0., y_delta +. state.player_y);
        {...state, player_y, player_velocity};
      | Ready => state
      };

    let distance = state.distance +. delta_t *. (state.scroll_velocity |> float_of_int);
    let time = state.time +. delta_t;

    {...state, distance, time};
  };

let handleTick: (t, float) => t =
  (state, delta_t) =>
    switch (state.state) {
    | GameOver(t) => {...state, state: GameOver(Js.Math.max_float(0.0, t -. delta_t))}
    | Run => state->updatePlayer(delta_t) |> checkCollision |> updateObstacles |> generateObstacles
    | _ => state
    };

let handleKeyDown: t => t =
  state =>
    switch (state.state, state.player_jumping) {
    | (Idle, _) => {...initialState(), state: Run}
    | (GameOver(_), _) => initialState()
    | (Run, Ready) when state.player_y == 0. => {
        ...state,
        player_jumping: On(player_jump_t),
        player_velocity: player_jump_t0_velocity,
      }
    | _ => state
    };

let handleKeyUp: t => t = state => {...state, player_jumping: Exhausted};
