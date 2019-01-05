/*
   App View
 */

let debug = false;

let canvas_width = 640;
let canvas_height = 320;

Random.self_init();

type action =
  | Start
  | Fullscreen
  | Tick
  | CanvasClick(int, int)
  | KeyUp
  | KeyDown;

type state = {
  start: bool,
  x: int,
  y: int,
  last_tick: option(float),
  data: option(GameState.t),
};

let handleTick: (state, float) => state =
  (state, delta_t) =>
    switch (state.data) {
    | Some(data) when delta_t < 1.0 => {...state, data: Some(data->GameState.handleTick(delta_t))}
    | _ => state
    };

let initialState = () => {start: false, x: 0, y: 0, last_tick: None, data: Some(GameState.initialState())};

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
        | "w" => self.send(KeyDown)
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
      | "w" => self.send(KeyUp)
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
    | (Fullscreen, _) => ReasonReact.SideEffects((_self => Dom_html.requestFullscreenElement("canvas") |> ignore))
    | (CanvasClick(x, y), _) =>
      /* velocity -100..100 */
      /* let velocity = 200 * (x - canvas_width / 2) / canvas_width; */
      ReasonReact.Update({...state, x, y})
    | (KeyDown, Some(data)) => ReasonReact.Update({...state, data: Some(data->GameState.handleKeyDown)})
    | (KeyUp, Some(data)) => ReasonReact.Update({...state, data: Some(data->GameState.handleKeyUp)})
    | _ => ReasonReact.NoUpdate
    },
  didUpdate: oldAndNewSelf => {
    let self = oldAndNewSelf.newSelf;
    switch (Dom_html.getCanvas2DContext("canvas"), self.state.data) {
    | (Some(context), Some(data)) =>
      Dom_html.requestAnimationFrame(_f => CanvasRender.render(~context, ~width=canvas_width, ~height=canvas_height, ~data, ()))
      |> ignore
    /*
     | (Some(context), None) => attraction screen
     */
    | _ => ()
    };
  },
  render: self =>
    <div className=Glamor.(css([display("flex"), alignItems("center"), flexDirection("column")]))>
      <canvas
        id="canvas"
        width={canvas_width |> string_of_int}
        height={canvas_height |> string_of_int}
        onTouchStart={_e => self.send(KeyDown)}
        onTouchEnd={_e => self.send(KeyUp)}
        onMouseDown={_e => self.send(KeyDown)}
        onMouseUp={_e => self.send(KeyUp)}
        onClick={
          e => {
            let ne = e |> ReactEvent.Mouse.nativeEvent;
            self.send(CanvasClick(ne##offsetX, ne##offsetY));
          }
        }
      />
      <div> <button onClick={_e => self.send(Fullscreen)}> {"Fullscreen" |> ReasonReact.string} </button> </div>
    </div>,
  /*
   <div className=Glamor.(css([backgroundColor("#333")]))>
     {Printf.sprintf("x: %d, y: %d", self.state.x, self.state.y) |> ReasonReact.string}
   </div>
   */
};
