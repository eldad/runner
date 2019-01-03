/*
     Game State
     Shared between the app and the canvas.
 */

type exhaustable =
  | Ready
  | On(float)
  | Exhausted;

type t = {
  player_jumping: exhaustable,
  player_y: float,
  bgscroll: float,
  velocity: int,
};

let player_jump_t = 5.0;
let player_jump_impulse = 200.0;
let player_gravity_impulse = (-100.0);

let initialState = () => {player_jumping: Ready, player_y: 0., bgscroll: 0., velocity: 10};

let handleTick: (t, float) => t =
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

    let bgscroll = state.bgscroll +. delta_t *. (state.velocity |> float_of_int);

    {...state, bgscroll};
  };

let handleKeyDown: t => t =
  state =>
    switch (state.player_jumping) {
    | Ready when state.player_y == 0. => {...state, player_jumping: On(player_jump_t)}
    | _ => state
    };

let handleKeyUp: t => t = state => {...state, player_jumping: Exhausted};
