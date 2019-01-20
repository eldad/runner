/*
   App View
 */

let debug = false;

let canvas_width = 640;
let canvas_height = 320;

Random.self_init();

type action =
  | Start
  | WipeSave
  | Fullscreen(bool)
  | Tick
  | KeyUp
  | KeyDown;

type state = {
  start: bool,
  fullscreen: bool,
  last_tick: option(float),
  data: option(GameState.t),
};

let minimum_tick = 0.1;

let handleTick: (state, float) => state =
  (state, delta_t) =>
    switch (state.data) {
    | Some(data) =>
      let ret_state = ref(state);
      let residual_delta_t = ref(delta_t);
      while (residual_delta_t^ > 0.) {
        let current_delta_t = Js.Math.min_float(residual_delta_t^, minimum_tick);
        ret_state := {...ret_state^, data: Some(data->GameState.handleTick(current_delta_t))};
        residual_delta_t := residual_delta_t^ -. minimum_tick;
      };
      ret_state^;

    | _ => state
    };

let initialState = () => {start: false, fullscreen: false, last_tick: None, data: Some(GameState.localStorageState())};

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState,
  didMount: self => {
    let intervalId = Js.Global.setInterval(() => self.send(Tick), debug ? 1000 : 0);
    self.onUnmount(() => Js.Global.clearInterval(intervalId));

    /* Keyboard events */

    let keydown = e => {
      if (!e->Dom_html.keyboardEventIsRepeat) {
        let code = e->Dom_html.keyboardEventKey;
        switch (code) {
        | "w"
        | " " => self.send(KeyDown)
        | _ => ()
        };
      };
      true;
    };

    Dom_html.(addEventListener(document, "keydown", keydown, false));
    self.onUnmount(() => Dom_html.(removeEventListener(document, "keydown", keydown, false)));

    let keyup = e => {
      let code = e->Dom_html.keyboardEventKey;
      switch (code) {
      | "w"
      | " " => self.send(KeyUp)
      | _ => ()
      };

      true;
    };

    Dom_html.(addEventListener(document, "keyup", keyup, false));
    self.onUnmount(() => Dom_html.(removeEventListener(document, "keyup", keyup, false)));

    ();
  },
  reducer: (action, state) =>
    switch (action, state.data) {
    | (Start, _) => ReasonReact.Update({...state, start: true, data: Some(GameState.initialState())})
    | (Tick, _) =>
      let now = Js.Date.now() /. 1000.0;
      switch (state.last_tick) {
      | None => ReasonReact.Update({...state, last_tick: Some(now)})
      | Some(last_tick) =>
        let delta_t = now -. last_tick;
        ReasonReact.Update({...state->handleTick(delta_t), last_tick: Some(now)});
      };
    | (WipeSave, _) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, data: Some(GameState.initialState())},
        (_self => Dom.Storage.(localStorage |> clear)),
      )
    | (Fullscreen(fullscreen), _) => ReasonReact.Update({...state, fullscreen})
    | (KeyDown, Some(data)) when state.fullscreen => ReasonReact.Update({...state, data: Some(data->GameState.handleKeyDown)})
    | (KeyUp, Some(data)) when state.fullscreen => ReasonReact.Update({...state, data: Some(data->GameState.handleKeyUp)})
    | (KeyDown | KeyUp, _) => ReasonReact.NoUpdate
    },
  didUpdate: oldAndNewSelf => {
    let self = oldAndNewSelf.newSelf;
    switch (Dom_html.getCanvas2DContext("canvas"), self.state.data) {
    | (Some(context), Some(data)) =>
      Dom_html.requestAnimationFrame(_f => CanvasRender.render(~context, ~width=canvas_width, ~height=canvas_height, ~data, ()))
      |> ignore
    | _ => ()
    };
  },
  render: self =>
    <div className=Glamor.(css([display("flex"), alignItems("center"), flexDirection("column")]))>
      <canvas
        id="canvas"
        className=Glamor.(
          css([
            Selector(":not(:fullscreen)", [display("none")]),
            Selector(":not(:-webkit-full-screen)", [display("none")]) /* Chrome on Android */
          ])
        )
        width={canvas_width |> string_of_int}
        height={canvas_height |> string_of_int}
        onTouchStart={_e => self.send(KeyDown)}
        onTouchEnd={_e => self.send(KeyUp)}
        onMouseDown={_e => self.send(KeyDown)}
        onMouseUp={_e => self.send(KeyUp)}
      />
      <div className=Glamor.(css([display("flex"), alignItems("center"), flexDirection("column")]))>
        <div
          className=Glamor.(
            css([
              display("flex"),
              alignItems("center"),
              flexDirection("column"),
              textAlign("justify"),
              maxWidth("600px"),
              Selector("@media (max-width: 600px)", [maxWidth("80vw")]),
              padding("10px"),
            ])
          )>
          <p> {"Runner" |> ReasonReact.string} </p>
          {
            switch (self.state.data) {
            | Some(data) =>
              switch (data.highscore) {
              | Some(highscore) => <p> {Printf.sprintf("Highscore: %d", highscore) |> ReasonReact.string} </p>
              | _ => ReasonReact.null
              }
            | _ => ReasonReact.null
            }
          }
          <button
            className=Glamor.(css([padding("10px")]))
            onClick={
              _e => {
                Dom_html.requestFullscreenElement("canvas") |> ignore;
                self.send(Fullscreen(true));
              }
            }>
            {"Run! (Fullscreen)" |> ReasonReact.string}
          </button>
          {
            switch (self.state.data) {
            | Some(data) =>
              switch (data.highscore) {
              | Some(_) =>
                <button className=Glamor.(css([padding("10px")])) onClick=(_e => self.send(WipeSave))>
                  {Printf.sprintf("Wipe Save") |> ReasonReact.string}
                </button>
              | None => ReasonReact.null
              }
            | None => ReasonReact.null
            }
          }
          <p className=Glamor.(css([fontSize("0.7rem")]))>
            {
              "Warning! Persons (or cats) playing this game may experience severe side effects."
              ++ "You may or may not be excited. Your highscore is all that matters -- make it count."
              |> ReasonReact.string
            }
          </p>
          <div className=Glamor.(css([fontSize("0.5rem"), textAlign("center")]))>
            {
              [|
                "Copyright (c) 2019 Eldad Zack <eldad@fogrefinery.com>",
                "+++",
                "Pixel Art from OpenGameArt.org by <a href=https://opengameart.org/users/grafxkid>Jason Mercado (GrafixKid)</a>",
              |]
              |> Array.mapi((i, s) => <p key={i |> string_of_int} dangerouslySetInnerHTML={"__html": s} />)
              |> ReasonReact.array
            }
          </div>
        </div>
      </div>
    </div>,
};
